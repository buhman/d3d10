from typing import Dict, List, Any

from collections import defaultdict
from dataclasses import dataclass
from itertools import islice, chain
from io import BytesIO
import struct

from collada.util import matrix_transpose, find_semantics
from collada import parse
from collada import types
from collada.generate import renderer
from collada import buffer

from prettyprinter import pprint, install_extras
install_extras(include=["dataclasses"])

@dataclass
class State:
    # arbitrary binary data, including vertex buffers and index
    # buffers
    vertex_buffer: BytesIO
    index_buffer: BytesIO

    # geometry__indices:
    #   keys: collada <geometry> id
    #   values: the index the geometry was placed at in the C++ geometries[] array
    geometry__indices: Dict[str, int]

    # geometry__vertex_index_tables:
    #   keys: collada <geometry> id
    #   values: vertex_index_table (see buffer.py)
    geometry__vertex_index_tables: Dict[str, List[int]]

    # symbol_names: C++ symbols/names already emitted
    symbol_names: Dict[str, Any]

    # emitted_input_elements_arrays
    emitted_input_elements_arrays: Dict[str, tuple]

    # channel nodes: node_id to list of sanitized target names
    node_animation_channels: Dict[str, set]

    # linearized_nodes
    linearized_nodes: List[types.Node]
    node_parents: Dict[int, int]

    def __init__(self):
        self.vertex_buffer = BytesIO()
        self.index_buffer = BytesIO()
        self.geometry__indices = {}
        self.geometry__vertex_index_tables = {}
        self.node_names = {}
        self.symbol_names = {}
        self.emitted_input_elements_arrays = {}
        self.node_animation_channels = defaultdict(set)
        self.linearized_nodes = []
        self.node_parents = {}

def sanitize_name(state, name, value, *, allow_slash=False):
    assert name is not None, value
    assert type(name) is str
    if not allow_slash:
        assert '/' not in name, name
    c_id = name.lower().replace('-', '_').replace('.', '_').replace('/', '_')

    assert c_id not in state.node_names or state.node_names[c_id] is value
    state.symbol_names[c_id] = value

    return c_id

def render_matrix(fs):
    fsi = iter(fs)
    for i in range(4):
        s = ", ".join(f"{f}f" for f in islice(fsi, 4))
        yield f"{s},"

def render_inverse_bind_matrix(collada, skin):
    inverse_bind_matrix_input, = find_semantics(skin.joints.inputs, "INV_BIND_MATRIX")
    inverse_bind_matrix_source = collada.lookup(inverse_bind_matrix_input.source, types.SourceCore)
    stride = inverse_bind_matrix_source.technique_common.accessor.stride
    count = inverse_bind_matrix_source.technique_common.accessor.count
    array = inverse_bind_matrix_source.array_element
    assert type(inverse_bind_matrix_source.array_element) == types.FloatArray
    assert stride == 16
    assert array.count == count * stride

    inverse_bind_matrices = []
    yield "static const float inverse_bind_matrices[] = {"
    for i in range(count):
        offset = stride * i
        matrix = matrix_transpose(array.floats[offset:offset+stride])
        yield from render_matrix(matrix)
    yield "};"

def renderbin(f, elems, t):
    fmt = {
        float: '<f',
        int: '<i',
    }[t]
    for e in elems:
        assert type(e) is t, (e, elems)
        f.write(struct.pack(fmt, e))

def offset_table_key(offset_table):
    for input, source in offset_table:
        semantic_index = 0 if input.set is None else input.set
        assert all(param.type == 'float' for param in source.technique_common.accessor.params)
        assert type(source.technique_common.accessor.stride) is int
        stride = source.technique_common.accessor.stride
        semantic = "POSITION" if input.semantic == "VERTEX" else input.semantic
        yield semantic, semantic_index, stride

def input_elements_key_name(key):
    return "_".join(map(str, chain.from_iterable(key))).lower()

def render_input_elements(state, collada, geometry_name, offset_tables):
    for i, offset_table in enumerate(offset_tables):
        key = tuple(offset_table_key(offset_table))
        key_name = input_elements_key_name(key)
        if key_name in state.emitted_input_elements_arrays:
            assert state.emitted_input_elements_arrays[key_name][1] == key
            continue
        state.emitted_input_elements_arrays[key_name] = (i, key)

        yield f"input_element const input_elements_{key_name}[] = {{"
        for semantic, semantic_index, stride in key:
            yield "{"
            yield f'.semantic = "{semantic}",'
            yield f".semantic_index = {semantic_index},"
            yield f".format = input_format::FLOAT{stride},"
            yield "},"
        yield "};"

def render_triangles(state, collada, geometry_name, primitive_elements, mesh_buffer_state):
    yield from render_input_elements(state, collada, geometry_name, mesh_buffer_state.offset_tables)

    yield f"triangles const triangles_{geometry_name}[] = {{"
    for i, triangles in enumerate(primitive_elements):
        assert type(triangles) is types.Triangles, type(triangles)

        key = tuple(offset_table_key(mesh_buffer_state.offset_tables[i]))
        key_name = input_elements_key_name(key)
        index, _ = state.emitted_input_elements_arrays[key_name]

        yield "{"
        yield f".count = {triangles.count}, // triangles"
        yield f".index_offset = {mesh_buffer_state.index_buffer_offsets[i]}, // indices"
        yield f".inputs_index = {index}, // index into inputs_list"
        yield "},"
    yield "};"

def render_geometry(state, collada, geometry):
    geometry_name = sanitize_name(state, geometry.id, geometry)

    mesh = geometry.geometric_element
    assert type(mesh) is types.Mesh
    mesh_buffer_state = buffer.mesh_vertex_index_buffer(collada, mesh)

    vertex_buffer_offset = state.vertex_buffer.tell()
    renderbin(state.vertex_buffer, mesh_buffer_state.vertex_buffer, float)
    vertex_buffer_size = state.vertex_buffer.tell() - vertex_buffer_offset

    index_buffer_offset = state.index_buffer.tell()
    renderbin(state.index_buffer, mesh_buffer_state.index_buffer, int)
    index_buffer_size = state.index_buffer.tell() - index_buffer_offset

    yield from render_triangles(state, collada, geometry_name, mesh.primitive_elements, mesh_buffer_state)

    # used by skin_vertex_buffer
    state.geometry__vertex_index_tables[geometry.id] = mesh_buffer_state.vertex_index_table

    yield f"geometry const geometry_{geometry_name} = {{"
    yield ".mesh = {"
    yield f".triangles = triangles_{geometry_name},"
    yield f".triangles_count = {len(mesh.primitive_elements)},"
    yield ""
    yield f".vertex_buffer_offset = {vertex_buffer_offset},"
    yield f".vertex_buffer_size = {vertex_buffer_size},"
    yield ""
    yield f".index_buffer_offset = {index_buffer_offset},"
    yield f".index_buffer_size = {index_buffer_size},"
    yield "}"
    yield "};"

def render_library_geometries(state, collada):
    next_index = 0
    for library_geometries in collada.library_geometries:
        for geometry in library_geometries.geometries:
            assert geometry.id is not None
            state.geometry__indices[geometry.id] = next_index
            next_index += 1
            yield from render_geometry(state, collada, geometry)

    yield "geometry const * const geometries[] = {"
    for library_geometries in collada.library_geometries:
        for geometry in library_geometries.geometries:
            geometry_name = sanitize_name(state, geometry.id, geometry)
            yield f"&geometry_{geometry_name},"
    yield "};"

def render_float_tuple(t):
    s = ", ".join((f"{float(f)}f" for f in t))
    return f"{{{s}}}"

def render_node_transforms(state, collada, node_name, transformation_elements):
    yield f"transform const transforms_{node_name}[] = {{"
    for transform in transformation_elements:
        yield "{"
        if type(transform) is types.Lookat:
            yield ".type = transform_type::LOOKAT,"
            yield ".lookat = {"
            yield f".eye = {render_float_tuple(transform.eye)}",
            yield f".at = {render_float_tuple(transform.at)}",
            yield f".up = {render_float_tuple(transform.up)}",
            yield "},"
        elif type(transform) is types.Matrix:
            yield ".type = transform_type::MATRIX,"
            yield f".matrix = {render_float_tuple(matrix_transpose(transform.matrix))},"
        elif type(transform) is types.Rotate:
            yield ".type = transform_type::ROTATE,"
            yield f".rotate = {render_float_tuple(transform.rotate)},"
        elif type(transform) is types.Scale:
            yield ".type = transform_type::SCALE,"
            yield f".scale = {render_float_tuple(transform.scale)},"
        elif type(transform) is types.Skew:
            yield ".type = transform_type::SKEW,"
            yield f".skew = {render_float_tuple(transform.skew)},"
        elif type(transform) is types.Translate:
            yield ".type = transform_type::TRANSLATE,"
            yield f".translate = {render_float_tuple(transform.translate)},"
        else:
            assert False, type(transform)
        yield "},"
    yield "};"

def find_material_symbol(geometry, material_symbol):
    assert material_symbol is not None
    mesh = geometry.geometric_element
    assert type(mesh) is types.Mesh
    for i, element in enumerate(mesh.primitive_elements):
        if element.material == material_symbol:
            return i # element index
    assert False, material_symbol

def render_node_instance_geometry_instance_materials(state, collada, node_name, i, geometry, instance_materials):
    yield f"instance_material const instance_materials_{node_name}_{i}[] = {{"
    for instance_material in instance_materials:
        # bind <triangles> with `symbol` to <effect> with `target`

        # symbol is not an XML id
        # symbol is the `.material` attribute of <triangles> in geometry.mesh
        element_index = find_material_symbol(geometry, instance_material.symbol)

        # target is an XML id
        material = collada.lookup(instance_material.target, types.Material)
        material_name = sanitize_name(state, material.id, material)

        yield "{"
        yield f".element_index = {element_index}, // an index into mesh.triangles"
        yield f".material = &material_{material_name},"
        yield "},"
    yield "};"

def get_instance_materials(instance_geometry):
    return instance_geometry.bind_material.technique_common.materials if instance_geometry.bind_material is not None else []

def render_node_instance_geometries(state, collada, node_name, instance_geometries):
    for i, instance_geometry in enumerate(instance_geometries):
        geometry = collada.lookup(instance_geometry.url, types.Geometry)
        instance_materials = get_instance_materials(instance_geometry)
        yield from render_node_instance_geometry_instance_materials(state, collada, node_name, i, geometry, instance_materials)

    yield f"instance_geometry const instance_geometries_{node_name}[] = {{"
    for i, instance_geometry in enumerate(instance_geometries):
        geometry = collada.lookup(instance_geometry.url, types.Geometry)
        geometry_name = sanitize_name(state, geometry.id, geometry)

        instance_materials = get_instance_materials(instance_geometry)

        yield "{"
        yield f".geometry = &geometry_{geometry_name},"
        yield f".instance_materials = instance_materials_{node_name}_{i},"
        yield f".instance_materials_count = {len(instance_materials)},"
        yield "},"
    yield "};"

def get_node_name_id(node):
    name = node.id if node.id is not None else f"node-{node.name}"
    assert name is not None, node
    return name

#def render_node_children(state, collada, node_name, nodes):
#    yield f"node const * const node_children_{node_name}[] = {{"
#    for node in nodes:
#        node_name_id = get_node_name_id(node)
#        node_name = sanitize_name(state, node_name_id, node)
#        yield "&node_{node_name},"
#    yield "};"

def render_node_channels(state, collada, node, node_name):
    if node.id is None:
        # nodes with no ID can't have channels
        yield f"channel const * const node_channels_{node_name}[] = {{}};"
        return

    target_names = state.node_animation_channels[node.id]
    yield f"channel const * const node_channels_{node_name}[] = {{"
    for target_name in target_names:
        yield f"&node_channel_{target_name},"
    yield "};"

def render_node_instance_lights(state, collada, node_name, instance_lights):
    yield f"instance_light const instance_lights_{node_name}[] = {{"
    for instance_light in instance_lights:
        light = collada.lookup(instance_light.url, types.Light)
        light_name = sanitize_name(state, light.id, light)
        yield "{"
        yield f".light = &light_{light_name},"
        yield "}"
    yield "};"

def render_node(state, collada, node, node_index):
    node_name_id = get_node_name_id(node)
    node_name = sanitize_name(state, node_name_id, node)
    #yield from render_node_children(state, collada, node_name, node.nodes)
    yield from render_node_transforms(state, collada, node_name, node.transformation_elements)
    yield from render_node_instance_geometries(state, collada, node_name, node.instance_geometries)
    yield from render_node_instance_lights(state, collada, node_name, node.instance_lights)
    yield from render_node_channels(state, collada, node, node_name)

    type = {
        types.NodeType.JOINT: "JOINT",
        types.NodeType.NODE: "NODE",
    }[node.type]

    yield f"node const node_{node_name} = {{"
    yield f".parent_index = {state.node_parents[node_index]},"
    yield ""
    yield f".type = node_type::{type},"
    yield ""
    yield f".transforms = transforms_{node_name},"
    yield f".transforms_count = {len(node.transformation_elements)},"
    yield ""
    yield f".instance_geometries = instance_geometries_{node_name},"
    yield f".instance_geometries_count = {len(node.instance_geometries)},"
    yield ""
    yield f".instance_lights = instance_lights_{node_name},"
    yield f".instance_lights_count = {len(node.instance_lights)},"
    yield ""
    yield f".channels = node_channels_{node_name},"
    yield f".channels_count = {len(state.node_animation_channels[node.id])},"
    #yield ""
    #yield f".nodes = node_children_{node_name},"
    #yield f".nodes_count = {len(node.nodes)},"
    yield "};"

def traverse_node(state, parent_node_index, node):
    assert parent_node_index < len(state.linearized_nodes)
    node_index = len(state.linearized_nodes)
    state.linearized_nodes.append(node)
    assert node_index not in state.node_parents
    state.node_parents[node_index] = parent_node_index
    for child_node in node.nodes:
        traverse_node(state, node_index, child_node)

def linearize_nodes(state, collada):
    for library_visual_scenes in collada.library_visual_scenes:
        for visual_scene in library_visual_scenes.visual_scenes:
            for node in visual_scene.nodes:
                traverse_node(state, -1, node)

def render_library_visual_scenes(state, collada):
    linearize_nodes(state, collada)

    for node_index, node in enumerate(state.linearized_nodes):
        yield from render_node(state, collada, node, node_index)

    yield "node const * const nodes[] = {"
    for node_index, node in enumerate(state.linearized_nodes):
        node_name_id = get_node_name_id(node)
        node_name = sanitize_name(state, node_name_id, node)
        yield f"&node_{node_name},"
    yield "};"

def render_header(namespace):
    yield '#include "collada_types.hpp"'
    yield ''
    yield f'namespace {namespace} {{'
    yield ''
    yield 'using namespace collada;'

def render_opt_color(field_name, color):
    yield f".color = {render_float_tuple(color.value)},"

def render_opt_color_or_texture(field_name, color_or_texture):
    if color_or_texture is None:
        color_or_texture = types.Color(value=(0.0, 0.0, 0.0, 0.0))
    assert type(color_or_texture) is types.Color, color_or_texture
    yield f".{field_name} = {{"
    yield from render_opt_color("color", color_or_texture)
    yield "},"

def render_opt_float(field_name, f):
    if f is None:
        f = types.Float(value=0.0)
    yield f".{field_name} = {float(f.value)}f,"

def render_effect(state, collada, effect):
    profile_common, = effect.profile_common
    assert profile_common.newparam == []
    shader = profile_common.technique.shader
    effect_name = sanitize_name(state, effect.id, effect)
    yield f"effect const effect_{effect_name} = {{"

    if type(shader) is types.Blinn:
        yield ".type = effect_type::BLINN,"
        yield ".blinn = {"
        yield from render_opt_color_or_texture("emission", shader.emission)
        yield from render_opt_color_or_texture("ambient", shader.ambient)
        yield from render_opt_color_or_texture("diffuse", shader.diffuse)
        yield from render_opt_color_or_texture("specular", shader.specular)
        yield from render_opt_float("shininess", shader.shininess)
        yield from render_opt_color_or_texture("reflective", shader.reflective)
        yield from render_opt_float("reflectivity", shader.reflectivity)
        yield from render_opt_color_or_texture("transparent", shader.transparent)
        yield from render_opt_float("transparency", shader.transparency)
        yield from render_opt_float("index_of_refraction", shader.index_of_refraction)
        yield "}"
    elif type(shader) is types.Lambert:
        yield ".type = effect_type::LAMBERT,"
        yield ".lambert = {"
        yield from render_opt_color_or_texture("emission", shader.emission)
        yield from render_opt_color_or_texture("ambient", shader.ambient)
        yield from render_opt_color_or_texture("diffuse", shader.diffuse)
        yield from render_opt_color_or_texture("reflective", shader.reflective)
        yield from render_opt_float("reflectivity", shader.reflectivity)
        yield from render_opt_color_or_texture("transparent", shader.transparent)
        yield from render_opt_float("transparency", shader.transparency)
        yield from render_opt_float("index_of_refraction", shader.index_of_refraction)
        yield "}"
    elif type(shader) is types.Phong:
        yield ".type = effect_type::PHONG,"
        yield ".phong = {"
        yield from render_opt_color_or_texture("emission", shader.emission)
        yield from render_opt_color_or_texture("ambient", shader.ambient)
        yield from render_opt_color_or_texture("diffuse", shader.diffuse)
        yield from render_opt_color_or_texture("specular", shader.specular)
        yield from render_opt_float("shininess", shader.shininess)
        yield from render_opt_color_or_texture("reflective", shader.reflective)
        yield from render_opt_float("reflectivity", shader.reflectivity)
        yield from render_opt_color_or_texture("transparent", shader.transparent)
        yield from render_opt_float("transparency", shader.transparency)
        yield from render_opt_float("index_of_refraction", shader.index_of_refraction)
        yield "}"
    elif type(shader) is types.Constant:
        yield ".type = effect_type::CONSTANT,"
        yield ".constant = {"
        yield from render_opt_color("color", shader.color)
        yield from render_opt_color_or_texture("reflective", shader.reflective)
        yield from render_opt_float("reflectivity", shader.reflectivity)
        yield from render_opt_color_or_texture("transparent", shader.transparent)
        yield from render_opt_float("transparency", shader.transparency)
        yield from render_opt_float("index_of_refraction", shader.index_of_refraction)
        yield "}"
    else:
        assert False, type(shader)

    yield "};"

def render_library_effects(state, collada):
    for library_effects in collada.library_effects:
        for effect in library_effects.effects:
            yield from render_effect(state, collada, effect)

def render_library_materials(state, collada):
    for library_materials in collada.library_materials:
        for material in library_materials.materials:
            effect = collada.lookup(material.instance_effect.url, types.Effect)
            material_name = sanitize_name(state, material.id, material)
            effect_name = sanitize_name(state, effect.id, effect)
            yield f"material const material_{material_name} = {{"
            yield f".effect = &effect_{effect_name},"
            yield "};"

def render_input_elements_list(state):
    yield "inputs const inputs_list[] = {"
    for key_name, (index, key) in state.emitted_input_elements_arrays.items():
        yield "{"
        yield f".elements = input_elements_{key_name},"
        yield f".elements_count = {len(key)},"
        yield "},"
    yield "};"

def render_descriptor(namespace):
    yield f"extern collada::descriptor const descriptor;"
    yield ""
    yield "collada::descriptor const descriptor = {"
    yield ".nodes = nodes,"
    yield ".nodes_count = (sizeof (nodes)) / (sizeof (nodes[0])),"
    yield ""
    yield ".inputs_list = inputs_list,"
    yield ".inputs_list_count = (sizeof (inputs_list)) / (sizeof (inputs_list[0])),"
    yield "};"

def render_end_of_namespace():
    yield "}"

def render_animation_children(state, collada, animation_name, animations):
    yield f"node const * const animation_children_{animation_name} = {{"
    for animation in animations:
        animation_name = sanitize_name(state, animation.id, animation)
        yield "&animation_{animation_name},"
    yield "};"

def render_array(state, collada, accessor, array):
    array_name = sanitize_name(state, array.id, array)
    # render the array
    if type(array) is types.NameArray:
        assert accessor.stride == 1
        assert accessor.params[0].name == "INTERPOLATION"
        assert len(array.names) == accessor.count
        yield f"enum interpolation const array_{array_name}[] = {{"
        for name in array.names:
            assert name in {"BEZIER", "LINEAR"}, name
            yield f"interpolation::{name},"
        yield "};"
    elif type(array) is types.FloatArray:
        yield f"float const array_{array_name}[] = {{"
        it = iter(array.floats)
        for i in range(accessor.count):
            vector = ", ".join(f"{float(f)}f" for f in islice(it, accessor.stride))
            yield f"{vector},"
        yield "};"
    else:
        assert False, type(array)

def render_source(state, collada, field_name, source):
    array_name = sanitize_name(state, source.array_element.id, source.array_element)
    c_type = "interpolation" if type(source.array_element) is types.NameArray else "float"
    source_name = sanitize_name(state, source.id, source)
    #yield f"source const source_{source_name} = {{"
    yield f"// {source_name}"
    yield f".{field_name} = {{"
    yield f".{c_type}_array = array_{array_name},"
    yield f".count = {source.technique_common.accessor.count},"
    yield f".stride = {source.technique_common.accessor.stride},"
    yield "},"

def render_sampler(state, collada, sampler):
    order = dict((s, i) for i, s in
                 enumerate(["INPUT", "OUTPUT", "IN_TANGENT", "OUT_TANGENT", "INTERPOLATION"]))
    inputs = sorted((input for input in sampler.inputs if input.semantic in order),
                    key=lambda input: order[input.semantic])
    inputs_semantics = [input.semantic for input in inputs]
    assert len(inputs_semantics) == len(set(inputs_semantics))
    assert "INPUT" in inputs_semantics
    assert "OUTPUT" in inputs_semantics
    assert "INTERPOLATION" in inputs_semantics

    # sampler validation
    counts = set()
    for input in inputs:
        source = collada.lookup(input.source, types.SourceCore)
        if input.semantic == "INTERPOLATION":
            nonlinear_interpolation_params = [e for e in source.array_element.names if e != "LINEAR"]
            if nonlinear_interpolation_params:
                assert "IN_TANGENT" in inputs_semantics
                assert "OUT_TANGENT" in inputs_semantics
        counts.add(source.technique_common.accessor.count)
    assert len(counts) == 1

    # render the source arrays first
    for input in inputs:
        assert type(input) is types.InputUnshared
        source = collada.lookup(input.source, types.SourceCore)

        yield from render_array(state, collada, source.technique_common.accessor, source.array_element)

    # render the sampler
    sampler_name = sanitize_name(state, sampler.id, sampler)
    yield f"sampler const sampler_{sampler_name} = {{"
    for input in inputs:
        source = collada.lookup(input.source, types.SourceCore)
        field_name = input.semantic.lower()
        yield from render_source(state, collada, field_name, source)
    yield "};"

target_attributes = {
    "A", "ANGLE", "B", "G", "P", "Q", "R", "S", "T", "TIME", "U", "V", "W", "X", "Y", "Z", "ALL"
}

def find_transform_index_in_node(node, transform):
    for i, node_transform in enumerate(node.transformation_elements):
        if node_transform is transform:
            return i
    assert False, (node, transform)

def transform_sid_lookup(node, sid):
    transform_types = {
        types.Lookat, types.Matrix, types.Rotate,
        types.Scale, types.Skew, types.Translate
    }
    transform = node.sid_lookup[sid]
    assert type(transform) in transform_types, transform
    return transform

def render_channel(state, collada, channel):
    sampler = collada.lookup(channel.source, types.Sampler)
    sampler_name = sanitize_name(state, sampler.id, sampler)

    assert '/' in channel.target, channel.target
    assert "(" not in channel.target, channel.target

    node_id, rest = channel.target.split("/")
    if '.' in rest:
        node_transform_sid, target_attribute = rest.split(".")
    else:
        node_transform_sid = rest
        target_attribute = 'ALL'
    assert target_attribute in target_attributes

    node = collada.lookup(f"#{node_id}", types.Node)
    node_name_id = get_node_name_id(node)
    node_name = sanitize_name(state, node_name_id, node)

    transform = transform_sid_lookup(node, node_transform_sid)
    transform_index = find_transform_index_in_node(node, transform)

    target_name = sanitize_name(state, channel.target, channel, allow_slash=True)
    assert target_name not in state.node_animation_channels[node.id]
    state.node_animation_channels[node.id].add(target_name)

    yield f"channel const node_channel_{target_name} = {{"
    yield f".source_sampler = &sampler_{sampler_name},"
    yield f".target_transform_index = {transform_index},"
    yield f".target_attribute = target_attribute::{target_attribute},"
    yield "};"

def render_animation(state, collada, animation_name, animation):
    # render children first
    for i, child_animation in enumerate(animation.animations):
        child_animation_name = f"{animation_name}_{i}"
        yield from render_animation(state, collada, child_animation_name, child_animation)

    # samplers (includes sources)
    for sampler in animation.samplers:
        yield from render_sampler(state, collada, sampler)

    for channel in animation.channels:
        yield from render_channel(state, collada, channel)

    # all animations channels are referenced from the node (inverse
    # the relationship in collada)
    #
    # I haven't considered how nested or layered animations are
    # affected by this inversion.

    #yield f"animation const animation_{animation_name} = {{"
    #yield f".animations = animation_children_{animation_name},"
    #yield f".animations_count = {len(animation.animations)},"
    #yield ""
    #yield f".channels = animation_channels_{animation_name}"
    #yield f".channels_count = {len(animation.channels)}"
    #yield "};"

def render_library_animations(state, collada):
    animation_ix = 0
    for library_animations in collada.library_animations:
        for animation in library_animations.animations:
            animation_name = f"{animation_ix}"
            yield from render_animation(state, collada, animation_name, animation)
            animation_ix += 1

def render_light_type(technique_common: types.TechniqueCommon_Light):
    return {
        types.Ambient: "AMBIENT",
        types.Directional: "DIRECTIONAL",
        types.Point: "POINT",
        types.Spot: "SPOT",
    }[type(technique_common.light)]

def render_light(state, collada, light):
    technique_common = light.technique_common
    light_name = sanitize_name(state, light.id, light)
    color = ", ".join(f"{float(f)}f" for f in technique_common.light.color)
    light_type = render_light_type(technique_common)
    yield f"light const light_{light_name} = {{"
    yield f".type = light_type::{light_type},"
    yield f".color = {{ {color} }},"
    yield "};"

def render_library_lights(state, collada):
    for library_lights in collada.library_lights:
        for light in library_lights.lights:
            yield from render_light(state, collada, light)

def render_all(collada, namespace):
    state = State()
    render, out = renderer()
    render(render_header(namespace))
    render(render_library_lights(state, collada))
    render(render_library_animations(state, collada))
    render(render_library_effects(state, collada))
    render(render_library_materials(state, collada))
    render(render_library_geometries(state, collada))

    # root elements
    render(render_library_visual_scenes(state, collada))
    render(render_input_elements_list(state))
    render(render_descriptor(namespace))
    render(render_end_of_namespace())
    return state, out

def render_hpp(namespace):
    yield f"namespace {namespace} {{"
    yield "extern collada::descriptor const descriptor;"
    yield "}"

def render_all_hpp(namespace):
    render, out = renderer()
    render(render_hpp(namespace))
    return out

if __name__ == "__main__":
    import sys
    collada = parse.parse_collada_file(sys.argv[1])

    #skin = collada.library_controllers[0].controllers[0].control_element
    #assert type(skin) is types.Skin
    #foo = render_inverse_bind_matrix(collada, skin)

    state, out = render_all(collada, "test")
    print(out.getvalue())
