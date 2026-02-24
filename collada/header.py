from typing import Dict, List, Any, Tuple

from operator import attrgetter
from collections import defaultdict
from dataclasses import dataclass
from itertools import islice, chain
from urllib.parse import unquote
from io import BytesIO
import os.path
import struct

from collada.util import matrix_transpose, find_semantics
from collada import parse
from collada import types
from collada.generate import renderer
from collada import buffer

lang_header = None

@dataclass
class State:
    # arbitrary binary data, including vertex buffers and index
    # buffers
    vertex_buffer: BytesIO
    index_buffer: BytesIO
    joint_weight_vertex_buffer: BytesIO

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

    # image_paths: filename: path
    image_paths: Dict[str, str]

    # image_indices: image_id: image_index
    image_indices: Dict[str, str]

    # resource_names: resource compiler names already emitted
    resource_names: Dict[str, str]

    # effect_textures_by_texcoord: (effect_id, channel): [sampler_sid]
    effect_textures_by_texcoord: Dict[Tuple[str, str], list]

    # input_filename
    input_filename: str
    relative_path: bool

    def __init__(self, input_filename):
        self.vertex_buffer = BytesIO()
        self.index_buffer = BytesIO()
        self.joint_weight_vertex_buffer = BytesIO()
        self.geometry__indices = {}
        self.geometry__vertex_index_tables = {}
        self.symbol_names = {}
        self.emitted_input_elements_arrays = {}
        self.node_animation_channels = defaultdict(set)
        self.linearized_nodes = []
        self.node_parents = {}
        self.image_paths = {}
        self.image_indices = {}
        self.resource_names = {}
        self.effect_textures_by_texcoord = defaultdict(list)
        self.input_filename = input_filename
        self.relative_path = False

def _sanitize(name):
    return name.replace(' ', '_').replace('-', '_').replace('.', '_').replace('/', '_')

def _validate_name_value(name, value):
    assert name is not None, name
    assert value is not None, value
    assert type(name) is str, name
    assert '\\' not in name, name

def sanitize_resource_name(state, name, value):
    _validate_name_value(name, value)
    assert '/' not in name, name
    resource_name = f'_{_sanitize(name).upper()}'

    assert resource_name not in state.resource_names or state.resource_names[resource_name] is value
    state.resource_names[resource_name] = value
    return resource_name

def sanitize_name(state, name, value, *, allow_slash=False):
    _validate_name_value(name, value)
    assert ' ' not in name, name
    if not allow_slash:
        assert '/' not in name, name
    c_id = _sanitize(name).lower()
    assert c_id not in state.symbol_names or state.symbol_names[c_id] is value
    state.symbol_names[c_id] = value
    return c_id

def renderbin(f, elems, t):
    fmt = {
        float: '<f',
        int: '<I',
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

        yield from lang_header.render_input_elements(key_name, key)

def render_triangles(state, collada, geometry_name, primitive_elements, mesh_buffer_state):
    yield from render_input_elements(state, collada, geometry_name, mesh_buffer_state.offset_tables)

    def items():
        for i, triangles in enumerate(primitive_elements):
            assert type(triangles) is types.Triangles, type(triangles)

            key = tuple(offset_table_key(mesh_buffer_state.offset_tables[i]))
            key_name = input_elements_key_name(key)
            index, _ = state.emitted_input_elements_arrays[key_name]

            yield triangles.count, mesh_buffer_state.index_buffer_offsets[i], index

    yield from lang_header.render_triangles(geometry_name, items())

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

    triangles_count = len(mesh.primitive_elements)

    yield from lang_header.render_geometry(geometry_name,
                                          triangles_count,
                                          vertex_buffer_offset,
                                          vertex_buffer_size,
                                          index_buffer_offset,
                                          index_buffer_size)

def render_library_geometries(state, collada):
    next_index = 0
    for library_geometries in collada.library_geometries:
        for geometry in library_geometries.geometries:
            assert geometry.id is not None
            state.geometry__indices[geometry.id] = next_index
            next_index += 1
            yield from render_geometry(state, collada, geometry)

    def geometry_names():
        for library_geometries in collada.library_geometries:
            for geometry in library_geometries.geometries:
                geometry_name = sanitize_name(state, geometry.id, geometry)
                yield geometry_name

    yield from lang_header.render_library_geometries(geometry_names())

def render_node_transforms(state, collada, node_name, transformation_elements):
    def render_transform(transform):
        if type(transform) is types.Lookat:
            yield from lang_header.render_transform_lookat(transform.eye, transform.at, transform.up)
        elif type(transform) is types.Matrix:
            yield from lang_header.render_transform_matrix(matrix_transpose(transform.values))
        elif type(transform) is types.Rotate:
            yield from lang_header.render_transform_rotate(transform.rotate)
        elif type(transform) is types.Scale:
            yield from lang_header.render_transform_scale(transform.scale)
        elif type(transform) is types.Skew:
            yield from lang_header.render_transform_skew(transform.skew)
        elif type(transform) is types.Translate:
            yield from lang_header.render_transform_translate(transform.translate)
        else:
            assert False, type(transform)

    yield from lang_header.render_node_transforms(node_name, transformation_elements, render_transform)

def find_material_symbol(geometry, material_symbol):
    assert material_symbol is not None
    mesh = geometry.geometric_element
    assert type(mesh) is types.Mesh
    for i, element in enumerate(mesh.primitive_elements):
        if element.material == material_symbol:
            return i # element index
    assert False, material_symbol

def shader_to_input_set(channel_to_input_set, shader, op):
    if type(shader) is types.Constant:
        return -1

    shader_op = op(shader)

    if type(shader_op) is types.Color:
        return -1
    if shader_op is None:
        return -1

    assert type(shader_op) is types.Texture
    texture = shader_op
    assert texture.texcoord in channel_to_input_set

    return channel_to_input_set[texture.texcoord]

def render_node_geometry_instance_materials(state, collada, prefix, node_name, i, geometry, instance_materials):
    def items():
        for instance_material in instance_materials:
            # bind <triangles> with `symbol` to <effect> with `target`

            # symbol is not an XML id
            # symbol is the `.material` attribute of <triangles> in geometry.mesh
            element_index = find_material_symbol(geometry, instance_material.symbol)

            # target is an XML id
            material = collada.lookup(instance_material.target, types.Material)
            material_name = sanitize_name(state, material.id, material)

            channel_to_input_set = {}
            for bind_vertex_input in instance_material.bind_vertex_inputs:
                assert bind_vertex_input.input_semantic == "TEXCOORD", bind_vertex_input
                if bind_vertex_input.semantic in channel_to_input_set:
                    assert channel_to_input_set[bind_vertex_input.semantic] == bind_vertex_input.input_set
                else:
                    channel_to_input_set[bind_vertex_input.semantic] = bind_vertex_input.input_set


            effect = collada.lookup(material.instance_effect.url, types.Effect)
            profile_common, = effect.profile_common
            shader = profile_common.technique.shader

            emission_input_set = shader_to_input_set(channel_to_input_set, shader, attrgetter("emission"))
            ambient_input_set = shader_to_input_set(channel_to_input_set, shader, attrgetter("ambient"))
            diffuse_input_set = shader_to_input_set(channel_to_input_set, shader, attrgetter("diffuse"))
            specular_input_set = shader_to_input_set(channel_to_input_set, shader, attrgetter("specular"))
            yield element_index, material_name, emission_input_set, ambient_input_set, diffuse_input_set, specular_input_set

    yield from lang_header.render_node_geometry_instance_materials(prefix, node_name, i, items())

def get_instance_materials(instance_geometry):
    return instance_geometry.bind_material.technique_common.materials if instance_geometry.bind_material is not None else []

def render_node_instance_geometries(state, collada, node_name, instance_geometries):
    for i, instance_geometry in enumerate(instance_geometries):
        geometry = collada.lookup(instance_geometry.url, types.Geometry)
        instance_materials = get_instance_materials(instance_geometry)
        yield from render_node_geometry_instance_materials(state, collada, "instance_geometry", node_name, i, geometry, instance_materials)

    def items():
        for i, instance_geometry in enumerate(instance_geometries):
            geometry = collada.lookup(instance_geometry.url, types.Geometry)
            geometry_name = sanitize_name(state, geometry.id, geometry)

            instance_materials = get_instance_materials(instance_geometry)
            instance_materials_count = len(instance_materials)

            yield geometry_name, i, instance_materials_count

    yield from lang_header.render_node_instance_geometries(node_name, items())

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
        yield from lang_header.render_node_channels(node_name, [])
    else:
        target_names = state.node_animation_channels[node.id]
        yield from lang_header.render_node_channels(node_name, target_names)

def render_node_instance_lights(state, collada, node_name, instance_lights):
    def light_names():
        for instance_light in instance_lights:
            light = collada.lookup(instance_light.url, types.Light)
            light_name = sanitize_name(state, light.id, light)
            yield light_name

    yield from lang_header.render_node_instance_lights(node_name, light_names())

def find_node_by_sid(root_node, sid):
    if sid == root_node.sid:
        return root_node
    for child_node in root_node.nodes:
        node = find_node_by_sid(child_node, sid)
        if node is not None:
            return node
    return None

def find_node_index(state, node):
    for other_index, other in enumerate(state.linearized_nodes):
        if other is node:
            return other_index

def render_joint_node_indices(state, collada, skin, node_name, controller_name, skeleton_node):
    joint_input, = find_semantics(skin.joints.inputs, "JOINT")
    joint_source = collada.lookup(joint_input.source, types.SourceCore)
    stride = joint_source.technique_common.accessor.stride
    count = joint_source.technique_common.accessor.count
    array = joint_source.array_element
    assert type(joint_source.array_element) == types.NameArray, array
    assert stride == 1
    assert array.count == count * stride

    def items():
        for node_sid in array.names:
            joint_node = find_node_by_sid(skeleton_node, node_sid);
            assert joint_node is not None, (node_sid, skeleton_node.sid_lookup)
            joint_node_index = find_node_index(state, joint_node)
            joint_node_name_id = get_node_name_id(joint_node)
            joint_node_name = sanitize_name(state, joint_node_name_id, joint_node)
            yield joint_node_index, node_sid, joint_node_name

    yield from lang_header.render_joint_node_indices(node_name, controller_name, items())

def render_node_instance_controller_joint_node_indices(state, collada, node_name, instance_controller):
    controller = collada.lookup(instance_controller.url, types.Controller)
    controller_name = sanitize_name(state, controller.id, controller)
    assert type(controller.control_element) is types.Skin
    skin = controller.control_element
    skeleton_node = collada.lookup(instance_controller.skeleton, types.Node)

    yield from render_joint_node_indices(state, collada, skin, node_name, controller_name, skeleton_node)

def render_node_instance_controllers(state, collada, node_name, instance_controllers):
    for i, instance_controller in enumerate(instance_controllers):
        yield from render_node_instance_controller_joint_node_indices(state, collada, node_name, instance_controller)
        controller = collada.lookup(instance_controller.url, types.Controller)
        assert type(controller.control_element) is types.Skin
        geometry = collada.lookup(controller.control_element.source, types.Geometry)
        instance_materials = get_instance_materials(instance_controller)
        yield from render_node_geometry_instance_materials(state, collada, "instance_controller", node_name, i, geometry, instance_materials)

    def items():
        for i, instance_controller in enumerate(instance_controllers):
            controller = collada.lookup(instance_controller.url, types.Controller)
            controller_name = sanitize_name(state, controller.id, controller)

            instance_materials = get_instance_materials(instance_controller)
            instance_materials_count = len(instance_materials)

            yield controller_name, i, instance_materials_count

    yield from lang_header.render_node_instance_controllers(node_name, items())

def render_node(state, collada, node, node_index):
    node_name_id = get_node_name_id(node)
    node_name = sanitize_name(state, node_name_id, node)
    #yield from render_node_children(state, collada, node_name, node.nodes)
    yield from render_node_transforms(state, collada, node_name, node.transformation_elements)
    yield from render_node_instance_geometries(state, collada, node_name, node.instance_geometries)
    yield from render_node_instance_controllers(state, collada, node_name, node.instance_controllers)
    yield from render_node_instance_lights(state, collada, node_name, node.instance_lights)
    yield from render_node_channels(state, collada, node, node_name)

    type = {
        types.NodeType.JOINT: "JOINT",
        types.NodeType.NODE: "NODE",
    }[node.type]

    parent_index = state.node_parents[node_index]
    transforms_count = len(node.transformation_elements)
    instance_geometries_count = len(node.instance_geometries)
    instance_controllers_count = len(node.instance_controllers)
    instance_lights_count = len(node.instance_lights)
    channels_count = len(state.node_animation_channels[node.id])

    yield from lang_header.render_node(node_name, parent_index, type,
                                      transforms_count,
                                      instance_geometries_count,
                                      instance_controllers_count,
                                      instance_lights_count,
                                      channels_count)

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

    def items():
        for node_index, node in enumerate(state.linearized_nodes):
            node_name_id = get_node_name_id(node)
            node_name = sanitize_name(state, node_name_id, node)
            yield node_name, node_index

    yield from lang_header.render_library_visual_scenes(items())

def render_opt_texture(state, profile_common, field_name, texture):
    sampler_sid = texture.texture
    sampler = profile_common.sid_lookup[sampler_sid]
    assert type(sampler) is types.Newparam, sampler
    assert type(sampler.parameter_type) is types.Sampler2D, sampler

    surface_sid = sampler.parameter_type.source.sid
    surface = profile_common.sid_lookup[surface_sid]
    assert type(surface) is types.Newparam, surface
    assert type(surface.parameter_type) is types.Surface, surface
    assert surface.parameter_type.type is types.FxSurfaceType._2D
    image_id = surface.parameter_type.init_from.uri
    image_index = state.image_indices[image_id]

    yield from lang_header.render_opt_texture(field_name, image_index, image_id)

def render_opt_color(field_name, color):
    yield from lang_header.render_opt_color(field_name, color)

def render_opt_color_or_texture(state, profile_common, field_name, color_or_texture):
    if color_or_texture is None:
        color_or_texture = types.Color(value=(0.0, 0.0, 0.0, 0.0))
    opt_type = {
        types.Color: "COLOR",
        types.Texture: "TEXTURE",
    }[type(color_or_texture)]

    def render_body():
        if type(color_or_texture) is types.Color:
            yield from render_opt_color("color", color_or_texture)
        elif type(color_or_texture) is types.Texture:
            yield from render_opt_texture(state, profile_common, "texture", color_or_texture)
        else:
            assert False, color_or_texture

    yield from lang_header.render_opt_color_or_texture(field_name, opt_type, render_body)

def render_opt_float(field_name, f):
    if f is None:
        f = types.Float(value=0.0)
    yield from lang_header.render_opt_float(field_name, float(f.value))

def render_effect(state, collada, effect):
    profile_common, = effect.profile_common

    shader = profile_common.technique.shader
    effect_name = sanitize_name(state, effect.id, effect)

    if type(shader) is types.Blinn:
        type_name = "BLINN"
        field_name = "blinn"
        def render_body():
            yield from render_opt_color_or_texture(state, profile_common, "emission", shader.emission)
            yield from render_opt_color_or_texture(state, profile_common, "ambient", shader.ambient)
            yield from render_opt_color_or_texture(state, profile_common, "diffuse", shader.diffuse)
            yield from render_opt_color_or_texture(state, profile_common, "specular", shader.specular)
            yield from render_opt_float("shininess", shader.shininess)
            yield from render_opt_color_or_texture(state, profile_common, "reflective", shader.reflective)
            yield from render_opt_float("reflectivity", shader.reflectivity)
            yield from render_opt_color_or_texture(state, profile_common, "transparent", shader.transparent)
            yield from render_opt_float("transparency", shader.transparency)
            yield from render_opt_float("index_of_refraction", shader.index_of_refraction)
    elif type(shader) is types.Lambert:
        type_name = "LAMBERT"
        field_name = "lambert"
        def render_body():
            yield from render_opt_color_or_texture(state, profile_common, "emission", shader.emission)
            yield from render_opt_color_or_texture(state, profile_common, "ambient", shader.ambient)
            yield from render_opt_color_or_texture(state, profile_common, "diffuse", shader.diffuse)
            yield from render_opt_color_or_texture(state, profile_common, "reflective", shader.reflective)
            yield from render_opt_float("reflectivity", shader.reflectivity)
            yield from render_opt_color_or_texture(state, profile_common, "transparent", shader.transparent)
            yield from render_opt_float("transparency", shader.transparency)
            yield from render_opt_float("index_of_refraction", shader.index_of_refraction)
    elif type(shader) is types.Phong:
        type_name = "PHONG"
        field_name = "phong"
        def render_body():
            yield from render_opt_color_or_texture(state, profile_common, "emission", shader.emission)
            yield from render_opt_color_or_texture(state, profile_common, "ambient", shader.ambient)
            yield from render_opt_color_or_texture(state, profile_common, "diffuse", shader.diffuse)
            yield from render_opt_color_or_texture(state, profile_common, "specular", shader.specular)
            yield from render_opt_float("shininess", shader.shininess)
            yield from render_opt_color_or_texture(state, profile_common, "reflective", shader.reflective)
            yield from render_opt_float("reflectivity", shader.reflectivity)
            yield from render_opt_color_or_texture(state, profile_common, "transparent", shader.transparent)
            yield from render_opt_float("transparency", shader.transparency)
            yield from render_opt_float("index_of_refraction", shader.index_of_refraction)
    elif type(shader) is types.Constant:
        type_name = "CONSTANT"
        field_name = "constant"
        def render_body():
            yield from render_opt_color("color", shader.color)
            yield from render_opt_color_or_texture(state, profile_common, "reflective", shader.reflective)
            yield from render_opt_float("reflectivity", shader.reflectivity)
            yield from render_opt_color_or_texture(state, profile_common, "transparent", shader.transparent)
            yield from render_opt_float("transparency", shader.transparency)
            yield from render_opt_float("index_of_refraction", shader.index_of_refraction)
    else:
        assert False, type(shader)

    yield from lang_header.render_effect(effect_name, type_name, field_name, render_body)

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
            yield from lang_header.render_library_material(material_name, effect_name)

def render_input_elements_list(state):
    def items():
        for key_name, (index, key) in state.emitted_input_elements_arrays.items():
            elements_count = len(key)
            yield key_name, elements_count
    yield from lang_header.render_input_elements_list(items())

def render_animation_children(state, collada, animation_name, animations):
    def items():
        for animation in animations:
            animation_name = sanitize_name(state, animation.id, animation)
            yield animation_name
    yield from lang_header.render_animation_children(animation_name, items())

def render_array(state, collada, accessor, array):
    array_name = sanitize_name(state, array.id, array)
    # render the array
    if type(array) is types.NameArray:
        assert accessor.stride == 1
        assert accessor.params[0].name == "INTERPOLATION"
        assert len(array.names) == accessor.count
        def names():
            for name in array.names:
                assert name in {"BEZIER", "LINEAR"}, name
                yield name
        yield from lang_header.render_interpolation_array(array_name, names())
    elif type(array) is types.FloatArray:
        def vectors():
            it = iter(array.floats)
            for i in range(accessor.count):
                vector = list(islice(it, accessor.stride))
                yield vector
        yield from lang_header.render_float_array(array_name, vectors())
    else:
        assert False, type(array)

def render_source(state, collada, field_name, source):
    array_name = sanitize_name(state, source.array_element.id, source.array_element)
    c_type = "interpolation" if type(source.array_element) is types.NameArray else "float"
    source_name = sanitize_name(state, source.id, source)

    yield from lang_header.render_source(source_name,
                                        field_name,
                                        c_type,
                                        array_name,
                                        source.technique_common.accessor.count,
                                        source.technique_common.accessor.stride)

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
    def render_body():
        for input in inputs:
            source = collada.lookup(input.source, types.SourceCore)
            field_name = input.semantic.lower()
            yield from render_source(state, collada, field_name, source)
    yield from lang_header.render_sampler(sampler_name, render_body)

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

    yield from lang_header.render_channel(target_name, sampler_name, transform_index, target_attribute)

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
    color = technique_common.light.color
    light_type = render_light_type(technique_common)
    yield from lang_header.render_light(light_name, light_type, color)

def render_library_lights(state, collada):
    for library_lights in collada.library_lights:
        for light in library_lights.lights:
            yield from render_light(state, collada, light)

def image_resource_name(state, uri):
    uri = unquote(uri)
    file_prefix = "file:///"
    path = uri[len(file_prefix):] if uri.startswith(file_prefix) else uri
    if not os.path.isabs(path) and not os.path.exists(path):
        path = os.path.join(os.path.dirname(state.input_filename), path)
        state.relative_path = True
    assert os.path.exists(path), path
    image_extensions = {".png", ".jpg", ".bmp", ".jpeg", ".tiff"}
    assert os.path.splitext(path)[1].lower() in image_extensions, path

    filename = os.path.split(path)[1]
    assert filename not in state.image_paths, filename
    state.image_paths[filename] = path
    return sanitize_resource_name(state, filename, path)

def render_image(state, collada, image, image_index):
    assert image.id is not None
    assert image.id not in state.image_indices
    state.image_indices[image.id] = image_index

    assert type(image.image_source) is types.InitFrom
    resource_name = image_resource_name(state, image.image_source.uri)
    image_name = sanitize_name(state, image.id, image)

    yield from lang_header.render_image(image.id, image_name, resource_name, image.image_source.uri)

def render_library_images(state, collada):
    image_index = 0
    for library_images in collada.library_images:
        for image in library_images.images:
            yield from render_image(state, collada, image, image_index)
            image_index += 1

    def image_names():
        for library_images in collada.library_images:
            for image in library_images.images:
                image_name = sanitize_name(state, image.id, image)
                yield image_name
    yield from lang_header.render_library_images(image_names())

def render_inverse_bind_matrices(collada, skin, controller_name):
    inverse_bind_matrix_input, = find_semantics(skin.joints.inputs, "INV_BIND_MATRIX")
    inverse_bind_matrix_source = collada.lookup(inverse_bind_matrix_input.source, types.SourceCore)
    stride = inverse_bind_matrix_source.technique_common.accessor.stride
    count = inverse_bind_matrix_source.technique_common.accessor.count
    array = inverse_bind_matrix_source.array_element
    assert type(inverse_bind_matrix_source.array_element) == types.FloatArray
    assert stride == 16
    assert array.count == count * stride

    # emitted in joint order
    def matrices():
        for i in range(count):
            offset = stride * i
            matrix = matrix_transpose(array.floats[offset:offset+stride])
            yield matrix
    yield from lang_header.render_inverse_bind_matrices(controller_name, matrices())

def renderbin_any(f, elems):
    fmt = {
        float: '<f',
        int: '<I',
    }
    for e in elems:
        f.write(struct.pack(fmt[type(e)], e))

def render_controller(state, collada, controller):
    controller_name = sanitize_name(state, controller.id, controller)

    assert type(controller.control_element) == types.Skin
    skin = controller.control_element
    geometry = collada.lookup(skin.source, types.Geometry)
    geometry_name = sanitize_name(state, geometry.id, geometry)
    vertex_index_table = state.geometry__vertex_index_tables[geometry.id]
    # this is a special index buffer that contains mixed floats/ints,
    # and needs to be packed accordingly

    # fixme: skin_vertex_buffer should multiply vertices by the bind shape matrix
    vertex_buffer = buffer.skin_vertex_buffer(collada, skin, vertex_index_table)
    # skin.vertex_weights and skin.source are entirely dealt with
    vertex_buffer_offset = state.joint_weight_vertex_buffer.tell()
    renderbin_any(state.joint_weight_vertex_buffer, vertex_buffer)
    vertex_buffer_size = state.joint_weight_vertex_buffer.tell() - vertex_buffer_offset

    yield from render_inverse_bind_matrices(collada, skin, controller_name)

    yield from lang_header.render_controller(controller_name, geometry_name, vertex_buffer_offset, vertex_buffer_size)

def render_library_controllers(state, collada):
    for library_controllers in collada.library_controllers:
        for controller in library_controllers.controllers:
            yield from render_controller(state, collada, controller)

def render_camera(state, collada, camera):
    camera_name = sanitize_name(state, camera.id, camera)
    perspective = camera.optics.technique_common.projection_type
    assert type(perspective) is types.Perspective
    def nf(f):
        if f is None:
            return float(0)
        else:
            return f

    yield from lang_header.render_camera(camera_name,
                                        nf(perspective.xfov),
                                        nf(perspective.yfov),
                                        nf(perspective.znear),
                                        nf(perspective.zfar),
                                        nf(perspective.aspect_ratio))

def render_library_cameras(state, collada):
    for library_cameras in collada.library_cameras:
        for camera in library_cameras.cameras:
            yield from render_camera(state, collada, camera)

def render_all(collada, namespace, input_filename):
    state = State(input_filename)
    render, out = renderer()
    render(lang_header.render_prelude(namespace))
    render(render_library_cameras(state, collada))
    render(render_library_lights(state, collada))
    render(render_library_animations(state, collada))
    render(render_library_images(state, collada))
    render(render_library_effects(state, collada))
    render(render_library_materials(state, collada))
    render(render_library_geometries(state, collada))
    render(render_library_controllers(state, collada))

    # root elements
    render(render_library_visual_scenes(state, collada))
    render(render_input_elements_list(state))
    render(lang_header.render_descriptor(namespace))
    render(lang_header.render_prologue())
    return state, out

def render_all_hpp(namespace):
    render, out = renderer()
    render(lang_header.render_hpp(namespace))
    return out

if __name__ == "__main__":
    import sys
    collada = parse.parse_collada_file(sys.argv[1])

    state, out = render_all(collada, "test", "./test.DAE")
    print(out.getvalue())
