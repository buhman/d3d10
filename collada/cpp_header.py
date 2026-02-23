from itertools import islice

from collada import types

def render_input_elements(key_name, semantic__semantic_index__stride):
    yield f"input_element const input_elements_{key_name}[] = {{"
    for semantic, semantic_index, stride in semantic__semantic_index__stride:
        yield "{"
        yield f'.semantic = "{semantic}",'
        yield f".semantic_index = {semantic_index},"
        yield f".format = input_format::FLOAT{stride},"
        yield "},"
    yield "};"

def render_triangles(geometry_name, items):
    yield f"triangles const triangles_{geometry_name}[] = {{"
    for count, index_offset, inputs_index in items:
        yield "{"
        yield f".count = {count}, // triangles"
        yield f".index_offset = {index_offset}, // indices"
        yield f".inputs_index = {inputs_index}, // index into inputs_list"
        yield "},"
    yield "};"

def render_geometry(geometry_name,
                    triangles_count,
                    vertex_buffer_offset, vertex_buffer_size,
                    index_buffer_offset, index_buffer_size):
    yield f"geometry const geometry_{geometry_name} = {{"
    yield ".mesh = {"
    yield f".triangles = triangles_{geometry_name},"
    yield f".triangles_count = {triangles_count},"
    yield ""
    yield f".vertex_buffer_offset = {vertex_buffer_offset},"
    yield f".vertex_buffer_size = {vertex_buffer_size},"
    yield ""
    yield f".index_buffer_offset = {index_buffer_offset},"
    yield f".index_buffer_size = {index_buffer_size},"
    yield "}"
    yield "};"

def render_library_geometries(geometry_names):
    yield "geometry const * const geometries[] = {"
    for geometry_name in geometry_names:
        yield f"&geometry_{geometry_name},"
    yield "};"

def render_float_tuple(t):
    s = ", ".join((f"{float(f)}f" for f in t))
    return f"{{{s}}}"

def render_lookat(eye, at, up):
    yield ".type = transform_type::LOOKAT,"
    yield ".lookat = {"
    yield f".eye = {render_float_tuple(eye)}",
    yield f".at = {render_float_tuple(at)}",
    yield f".up = {render_float_tuple(up)}",
    yield "},"

def render_matrix(fs):
    fsi = iter(fs)
    for i in range(4):
        s = ", ".join(f"{f}f" for f in islice(fsi, 4))
        yield f"{s},"

def render_transform_matrix(matrix):
    yield ".type = transform_type::MATRIX,"
    yield f".matrix = {render_float_tuple(matrix)},"

def render_transform_rotate(rotate):
    yield ".type = transform_type::ROTATE,"
    yield f".rotate = {render_float_tuple(rotate)},"

def render_transform_scale(scale):
    yield ".type = transform_type::SCALE,"
    yield f".scale = {render_float_tuple(scale)},"

def render_transform_skew(skew):
    yield ".type = transform_type::SKEW,"
    yield f".skew = {render_float_tuple(skew)},"

def render_transform_translate(translate):
    yield ".type = transform_type::TRANSLATE,"
    yield f".translate = {render_float_tuple(translate)},"

def render_node_transforms(node_name, transformation_elements, render_transform):
    yield f"transform const transforms_{node_name}[] = {{"
    for transform in transformation_elements:
        yield "{"
        yield from render_transform(transform)
        yield "},"
    yield "};"

def render_node_geometry_instance_materials(prefix, node_name, i,
                                            items):
    yield f"instance_material const {prefix}_instance_materials_{node_name}_{i}[] = {{"
    for element_index, material_name, emission_input_set, ambient_input_set, diffuse_input_set, specular_input_set in items:
        yield "{"
        yield f".element_index = {element_index}, // an index into mesh.triangles"
        yield f".material = &material_{material_name},"
        yield ""
        yield f".emission = {{ .input_set = {emission_input_set} }},"
        yield f".ambient = {{ .input_set = {ambient_input_set} }},"
        yield f".diffuse = {{ .input_set = {diffuse_input_set} }},"
        yield f".specular = {{ .input_set = {specular_input_set} }},"
        yield "},"
    yield "};"

def render_node_instance_geometries(node_name, items):
    yield f"instance_geometry const instance_geometries_{node_name}[] = {{"
    for geometry_name, i, instance_materials_count in items:
        yield "{"
        yield f".geometry = &geometry_{geometry_name},"
        yield ""
        yield f".instance_materials = instance_geometry_instance_materials_{node_name}_{i},"
        yield f".instance_materials_count = {instance_materials_count},"
        yield "},"
    yield "};"

def render_node_channels(node_name, target_names):
    yield f"channel const * const node_channels_{node_name}[] = {{"
    for target_name in target_names:
        yield f"&node_channel_{target_name},"
    yield "};"

def render_node_instance_lights(node_name, light_names):
    yield f"instance_light const instance_lights_{node_name}[] = {{"
    for light_name in light_names:
        yield "{"
        yield f".light = &light_{light_name},"
        yield "}"
    yield "};"

def render_joint_node_indices(node_name, controller_name, items):
    yield f"int const joint_node_indices_{node_name}_{controller_name}[] = {{"
    for joint_node_index, node_sid, joint_node_name in items:
        yield f"{joint_node_index}, // {node_sid} {joint_node_name}"
    yield "};"

def render_node_instance_controllers(node_name, items):
    yield f"instance_controller const instance_controllers_{node_name}[] = {{"
    for controller_name, i, instance_materials_count in items:
        yield "{"
        yield f".controller = &controller_{controller_name},"
        yield ""
        yield f".joint_node_indices = joint_node_indices_{node_name}_{controller_name},"
        yield f".joint_count = (sizeof (joint_node_indices_{node_name}_{controller_name})) / (sizeof (int)),"
        yield ""
        yield f".instance_materials = instance_controller_instance_materials_{node_name}_{i},"
        yield f".instance_materials_count = {instance_materials_count},"
        yield "},"
    yield "};"

def render_node(node_name, parent_index, type,
                transforms_count,
                instance_geometries_count,
                instance_controllers_count,
                instance_lights_count,
                channels_count):
    yield f"node const node_{node_name} = {{"
    yield f".parent_index = {parent_index},"
    yield ""
    yield f".type = node_type::{type},"
    yield ""
    yield f".transforms = transforms_{node_name},"
    yield f".transforms_count = {transforms_count},"
    yield ""
    yield f".instance_geometries = instance_geometries_{node_name},"
    yield f".instance_geometries_count = {instance_geometries_count},"
    yield ""
    yield f".instance_controllers = instance_controllers_{node_name},"
    yield f".instance_controllers_count = {instance_controllers_count},"
    yield ""
    yield f".instance_lights = instance_lights_{node_name},"
    yield f".instance_lights_count = {instance_lights_count},"
    yield ""
    yield f".channels = node_channels_{node_name},"
    yield f".channels_count = {channels_count},"
    #yield ""
    #yield f".nodes = node_children_{node_name},"
    #yield f".nodes_count = {len(node.nodes)},"
    yield "};"

def render_library_visual_scenes(items):
    yield "node const * const nodes[] = {"
    for node_name, node_index in items:
        yield f"&node_{node_name}, // {node_index}"
    yield "};"

def render_opt_color(field_name, color):
    yield f".{field_name} = {render_float_tuple(color.value)},"

def render_opt_texture(field_name, image_index, image_id):
    yield f".{field_name} = {{ .image_index = {image_index} }}, // {image_id}"

def render_opt_color_or_texture(field_name, opt_type, render_body):
    yield f".{field_name} = {{"
    yield f".type = color_or_texture_type::{opt_type},"
    yield from render_body()
    yield "},"

def render_opt_float(field_name, value):
    yield f".{field_name} = {value}f,"

def render_effect(effect_name, type_name, field_name, render_body):
    yield f"effect const effect_{effect_name} = {{"

    yield f".type = effect_type::{type_name},"
    yield f".{field_name} = {{"
    yield from render_body()
    yield "}"

    yield "};"

def render_library_material(material_name, effect_name):
    yield f"material const material_{material_name} = {{"
    yield f".effect = &effect_{effect_name},"
    yield "};"

def render_input_elements_list(items):
    yield "inputs const inputs_list[] = {"
    for key_name, elements_count in items:
        yield "{"
        yield f".elements = input_elements_{key_name},"
        yield f".elements_count = {elements_count},"
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
    yield ""
    yield ".images = images,"
    yield ".images_count = (sizeof (images)) / (sizeof (images[0])),"
    yield ""
    yield f'.position_normal_texture_buffer = L"RES_SCENES_{namespace.upper()}_VTX",'
    yield f'.joint_weight_buffer = L"RES_SCENES_{namespace.upper()}_VJW",'
    yield f'.index_buffer = L"RES_SCENES_{namespace.upper()}_IDX",'
    yield "};"

def render_prelude(namespace):
    yield '#include "collada_types.hpp"'
    yield ''
    yield f'namespace {namespace} {{'
    yield ''
    yield 'using namespace collada;'

def render_prologue():
    yield "}"

def render_animation_children(anomation_name, items):
    yield f"node const * const animation_children_{animation_name} = {{"
    for animation_name in items:
        yield "&animation_{animation_name},"
    yield "};"

def render_interpolation_array(array_name, names):
    yield f"enum interpolation const array_{array_name}[] = {{"
    for name in names:
        yield f"interpolation::{name},"
    yield "};"

def render_float_array(array_name, vectors):
    yield f"float const array_{array_name}[] = {{"
    for vector in vectors:
        yield f"{render_float_tuple(vector)},"
    yield "};"

def render_source(source_name, field_name, c_type, array_name, count, stride):
    yield f"// {source_name}"
    yield f".{field_name} = {{"
    yield f".{c_type}_array = array_{array_name},"
    yield f".count = {count},"
    yield f".stride = {stride},"
    yield "},"

def render_sampler(sampler_name, render_body):
    yield f"sampler const sampler_{sampler_name} = {{"
    yield from render_body()
    yield "};"

def render_channel(target_name, sampler_name, transform_index, target_attribute):
    yield f"channel const node_channel_{target_name} = {{"
    yield f".source_sampler = &sampler_{sampler_name},"
    yield f".target_transform_index = {transform_index},"
    yield f".target_attribute = target_attribute::{target_attribute},"
    yield "};"

def render_light(light_name, light_type, color):
    yield f"light const light_{light_name} = {{"
    yield f".type = light_type::{light_type},"
    yield f".color = {{ {render_float_tuple(color)} }},"
    yield "};"

def render_image(image_id, image_name, resource_name):
    yield f"// {image.id}"
    yield f"image const image_{image_name} = {{"
    yield f'.resource_name = L"{resource_name}",'
    yield "};"

def render_library_images(image_names):
    yield "image const * const images[] = {"
    for image_name in image_names:
        yield f"&image_{image_name},"
    yield "};"

def render_inverse_bind_matrices(controller_name, matrices):
    yield f"matrix const inverse_bind_matrices_{controller_name}[] = {{"
    for matrix in matrices:
        yield "{"
        yield from render_matrix(matrix)
        yield "},"
    yield "};"

def render_controller(controller_name, geometry_name, vertex_buffer_offset, vertex_buffer_size):
    yield f"controller const controller_{controller_name} = {{"
    yield ".skin = {"
    yield f".geometry = &geometry_{geometry_name},"
    yield ""
    yield f".inverse_bind_matrices = inverse_bind_matrices_{controller_name},"
    yield ""
    yield f".vertex_buffer_offset = {vertex_buffer_offset},"
    yield f".vertex_buffer_size = {vertex_buffer_size},"
    yield "}"
    yield "};"

def render_camera(camera_name, xfov, yfov, znear, zfar, aspect_ratio):
    yield f"camera const camera_{camera_name} = {{"
    yield f".xfov = {xfov}f,"
    yield f".yfov = {yfov}f,"
    yield f".znear = {znear}f,"
    yield f".zfar = {zfar}f,"
    yield f".aspect_ratio = {aspect_ratio}f,"
    yield "};"

def render_hpp(namespace):
    yield f"namespace {namespace} {{"
    yield "extern collada::descriptor const descriptor;"
    yield "}"
