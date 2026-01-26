from lxml import etree
from collada import types
from functools import partial

xml_namespace = "http://www.collada.org/2005/11/COLLADASchema"

def tag(s):
    return f"{{{xml_namespace}}}{s}"

def lookup_add(lookup, name, value):
    if name is None:
        return
    assert name not in lookup
    lookup[name] = value

def parse_init_from(lookup, root):
    return types.InitFrom(root.text.strip())

def parse_surface_type(s):
    if s == "UNTYPED":
        return types.FxSurfaceType.UNTYPED
    if s == "1D":
        return types.FxSurfaceType._1D
    if s == "2D":
        return types.FxSurfaceType._2D
    if s == "3D":
        return types.FxSurfaceType._3D
    if s == "CUBE":
        return types.FxSurfaceType.CUBE
    if s == "DEPTH":
        return types.FxSurfaceType.DEPTH
    if s == "RECT":
        return types.FxSurfaceType.RECT
    assert False, s

def parse_surface(lookup, root):
    type = parse_surface_type(root.attrib["type"])

    init_from = None
    for child in root.getchildren():
        if child.tag == tag("format"):
            assert False, child.tag
        if child.tag == tag("format_hint"):
            assert False, child.tag
        if child.tag == tag("size"):
            assert False, child.tag
        if child.tag == tag("viewport_ratio"):
            assert False, child.tag
        if child.tag == tag("mip_levels"):
            assert False, child.tag
        if child.tag == tag("mipmap_generate"):
            assert False, child.tag
        if child.tag == tag("init_from"):
            assert init_from is None
            init_from = parse_init_from(lookup, child)
        if child.tag == tag("generator"):
            assert False, child.tag

    surface = types.Surface(type, init_from)
    return surface

def parse_source_fx(lookup, root):
    return types.SourceFX(root.text.strip())

def parse_sampler2d(lookup, root):
    source_fx = None
    for child in root.getchildren():
        if child.tag == tag("source"):
            assert source_fx is None
            source_fx = parse_source_fx(lookup, child)

    assert source_fx is not None
    sampler2d = types.Sampler2D(source_fx)
    return sampler2d

def parse_newparam(lookup, sid_lookup, root):
    sid = root.attrib["sid"]

    parameter_type = None
    for child in root.getchildren():
        if child.tag == tag("float"):
            assert False, child.tag
        if child.tag == tag("float2"):
            assert False, child.tag
        if child.tag == tag("float3"):
            assert False, child.tag
        if child.tag == tag("float4"):
            assert False, child.tag
        if child.tag == tag("surface"):
            assert parameter_type is None
            parameter_type = parse_surface(lookup, child)
        if child.tag == tag("sampler2D"):
            assert parameter_type is None
            parameter_type = parse_sampler2d(lookup, child)

    assert parameter_type is not None
    newparam = types.Newparam(sid, parameter_type)
    lookup_add(sid_lookup, sid, newparam)
    return newparam

def parse_fields(lookup, root, *, cls, fields):
    field_kv = {}
    for child in root.getchildren():
        for tag_name, parser in fields:
            if child.tag == tag(tag_name):
                assert tag_name not in field_kv
                grandchildren = child.getchildren()
                assert len(grandchildren) == 1
                field_kv[tag_name] = parser(lookup, grandchildren[0])
    for tag_name, _ in fields:
        if tag_name not in field_kv:
            field_kv[tag_name] = None
    return cls(**field_kv)

def parse_color_or_texture(lookup, root):
    if root.tag == tag("color"):
        return types.Color(tuple(map(float, root.text.strip().split())))
    if root.tag == tag("texture"):
        return types.Texture(root.attrib["texture"], root.attrib["texcoord"])
    assert False, (root, root.tag)

def parse_float(lookup, root):
    assert root.tag == tag("float")
    return types.Float(float(root.text.strip()))

blinn_phong_fields = [
    ("emission", parse_color_or_texture),
    ("ambient", parse_color_or_texture),
    ("diffuse", parse_color_or_texture),
    ("specular", parse_color_or_texture),
    ("shininess", parse_float),
    ("reflective", parse_color_or_texture),
    ("reflectivity", parse_float),
    ("transparent", parse_color_or_texture),
    ("transparency", parse_float),
    ("index_of_refraction", parse_float),
]

lambert_fields = [
    ("emission", parse_color_or_texture),
    ("ambient", parse_color_or_texture),
    ("diffuse", parse_color_or_texture),
    ("reflective", parse_color_or_texture),
    ("reflectivity", parse_float),
    ("transparent", parse_color_or_texture),
    ("transparency", parse_float),
    ("index_of_refraction", parse_float),
]

constant_fields = [
    ("emission", parse_color_or_texture),
    ("reflective", parse_color_or_texture),
    ("reflectivity", parse_float),
    ("transparent", parse_color_or_texture),
    ("transparency", parse_float),
    ("index_of_refraction", parse_float),
]

parse_blinn = partial(parse_fields, cls=types.Blinn, fields=blinn_phong_fields)
parse_phong = partial(parse_fields, cls=types.Phong, fields=blinn_phong_fields)
parse_lambert = partial(parse_fields, cls=types.Lambert, fields=lambert_fields)
parse_constant = partial(parse_fields, cls=types.Constant, fields=constant_fields)

def parse_technique_fx(lookup, sid_lookup, root):
    id = root.attrib.get("id")
    sid = root.attrib["sid"]

    shader = None
    for child in root.getchildren():
        if child.tag == tag("blinn"):
            assert shader is None
            shader = parse_blinn(lookup, child)
        if child.tag == tag("constant"):
            assert shader is None
            shader = parse_constant(lookup, child)
        if child.tag == tag("lambert"):
            assert shader is None
            shader = parse_lambert(lookup, child)
        if child.tag == tag("phong"):
            assert shader is None
            shader = parse_phong(lookup, child)
    assert shader is not None

    technique = types.TechniqueFX(id, sid, shader)
    lookup_add(lookup, id, technique)
    lookup_add(sid_lookup, sid, technique)
    return technique

def parse_profile_common(lookup, root):
    id = root.attrib.get("id")

    newparam = []
    technique_fx = None
    sid_lookup = {}

    for child in root.getchildren():
        if child.tag == tag("image"):
            assert False, child.tag
        if child.tag == tag("newparam"):
            newparam.append(parse_newparam(lookup, sid_lookup, child))
        if child.tag == tag("technique"):
            assert technique_fx is None
            technique_fx = parse_technique_fx(lookup, sid_lookup, child)

    assert technique_fx is not None
    profile_common = types.ProfileCommon(id, newparam, technique_fx, sid_lookup)
    lookup_add(lookup, id, profile_common)
    return profile_common

def parse_effect(lookup, root):
    id = root.attrib["id"]
    name = root.attrib.get("name")

    profile_common = []

    for child in root.getchildren():
        if child.tag == tag("profile_COMMON"):
            profile_common.append(parse_profile_common(lookup, child))

    effect = types.Effect(id, name, profile_common)
    lookup_add(lookup, id, effect)
    return effect

def parse_library_effects(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")
    effects = []

    for child in root.getchildren():
        if child.tag == tag("effect"):
            effects.append(parse_effect(lookup, child))

    library_effects = types.LibraryEffects(id, name, effects)
    lookup_add(lookup, id, library_effects)
    return library_effects

def parse_instance_effect(lookup, sid_lookup, root):
    sid = root.attrib.get("sid")
    name = root.attrib.get("name")
    url = root.attrib["url"]

    instance_effect = types.InstanceEffect(sid, name, url)
    lookup_add(sid_lookup, sid, instance_effect)
    return instance_effect

def parse_material(lookup, root):
    sid_lookup = {}

    id = root.attrib.get("id")
    name = root.attrib.get("name")
    instance_effect = None

    for child in root.getchildren():
        if child.tag == tag("instance_effect"):
            assert instance_effect is None
            instance_effect = parse_instance_effect(lookup, sid_lookup, child)
    assert instance_effect is not None

    material = types.Material(id, name, instance_effect, sid_lookup)
    lookup_add(lookup, id, material)
    return material

def parse_library_materials(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")
    materials = []

    for child in root.getchildren():
        if child.tag == tag("material"):
            materials.append(parse_material(lookup, child))

    library_materials = types.LibraryMaterials(id, name, materials)
    lookup_add(lookup, id, library_materials)
    return library_materials

def parse_input_unshared(lookup, root):
    semantic = root.attrib["semantic"]
    source = root.attrib["source"]
    return types.InputUnshared(semantic, source)

def parse_vertices(lookup, root):
    id = root.attrib["id"]
    name = root.attrib.get("name")
    input = []

    for child in root.getchildren():
        if child.tag == tag("input"):
            input.append(parse_input_unshared(lookup, child))

    assert len(input) >= 1
    vertices = types.Vertices(id, name, input)
    lookup_add(lookup, id, vertices)
    return vertices

def parse_name_array(lookup, root):
    count = int(root.attrib["count"])
    id = root.attrib.get("id")
    name = root.attrib.get("name")

    assert len(root.getchildren()) == 0
    names = root.text.strip().split()

    name_array = types.NameArray(count, id, name, names)
    lookup_add(lookup, id, name_array)
    return name_array

def parse_bool(s):
    if s == "false":
        return False
    if s == "true":
        return True
    assert False, s

def parse_bool_array(lookup, root):
    count = int(root.attrib["count"])
    id = root.attrib.get("id")
    name = root.attrib.get("name")

    assert len(root.getchildren()) == 0
    bools = [parse_bool(s) for s in root.text.strip().split()]

    bool_array = types.BoolArray(count, id, name, bools)
    lookup_add(lookup, id, bool_array)
    return bool_array

def parse_float_array(lookup, root):
    count = int(root.attrib["count"])
    id = root.attrib.get("id")
    name = root.attrib.get("name")
    digits = int(root.attrib.get("digits", 6))
    magnitude = int(root.attrib.get("digits", 38))

    assert len(root.getchildren()) == 0
    floats = [float(s) for s in root.text.strip().split()]

    float_array = types.FloatArray(count, id, name, digits, magnitude, floats)
    lookup_add(lookup, id, float_array)
    return float_array

def parse_int_array(lookup, root):
    count = int(root.attrib["count"])
    id = root.attrib.get("id")
    name = root.attrib.get("name")
    min_inclusive = int(root.attrib.get("minInclusive", -2147483648))
    max_inclusive = int(root.attrib.get("maxInclusive", 2147483647))

    assert len(root.getchildren()) == 0
    ints = [int(s) for s in root.text.strip().split()]

    int_array = types.IntArray(count, id, name, minInclusive, maxInclusive, ints)
    lookup_add(lookup, id, int_array)
    return int_array

def parse_param(lookup, sid_lookup, root):
    name = root.attrib.get("name")
    sid = root.attrib.get("sid")
    type = root.attrib["type"]
    semantic = root.attrib.get("semantic")

    param = types.Param(name, sid, type, semantic)
    lookup_add(sid_lookup, sid, param)
    return param

def parse_accessor(lookup, root):
    count = int(root.attrib["count"])
    offset = int(root.attrib.get("offset", 0))
    source = root.attrib["source"]
    stride = int(root.attrib.get("stride", 1))

    sid_lookup = {}

    params = []
    for child in root.getchildren():
        if child.tag == tag("param"):
            params.append(parse_param(lookup, sid_lookup, child))

    return types.Accessor(count, offset, source, stride, params, sid_lookup)

def parse_technique_common_source_core(lookup, root):
    accessor = None

    for child in root.getchildren():
        if child.tag == tag("accessor"):
            assert accessor is None
            accessor = parse_accessor(lookup, child)

    assert accessor is not None
    return types.TechniqueCommon_SourceCore(accessor)

def parse_source_core(lookup, root):
    id = root.attrib["id"]
    name = root.attrib.get("name")
    array_element = None # 0 or 1
    technique_common = None # 0 or 1

    for child in root.getchildren():
        if child.tag == tag("IDREF_array"):
            assert array_element is None
            assert False, child.tag # not implemented
        if child.tag == tag("Name_array"):
            assert array_element is None
            array_element = parse_name_array(lookup, child)
        if child.tag == tag("bool_array"):
            assert array_element is None
            array_element = parse_bool_array(lookup, child)
        if child.tag == tag("float_array"):
            assert array_element is None
            array_element = parse_float_array(lookup, child)
        if child.tag == tag("int_array"):
            assert array_element is None
            array_element = parse_int_array(lookup, child)
        if child.tag == tag("technique_common"):
            assert technique_common is None
            technique_common = parse_technique_common_source_core(lookup, child)
        if child.tag == tag("technique"):
            assert False, child.tag # not implemented

    source_core = types.SourceCore(id, name, array_element, technique_common)
    lookup_add(lookup, id, source_core)
    return source_core

def parse_input_shared(lookup, root):
    offset = int(root.attrib["offset"])
    semantic = root.attrib["semantic"]
    source = root.attrib["source"]
    set = int(root.attrib["set"]) if "set" in root.attrib else None
    return types.InputShared(offset, semantic, source, set)

def parse_p(lookup, root):
    assert len(root.getchildren()) == 0
    return [int(i) for i in root.text.strip().split()]

def parse_triangles(lookup, root):
    name = root.attrib.get("name")
    count = int(root.attrib["count"])
    material = root.attrib.get("material")

    inputs = []
    p = None

    for child in root.getchildren():
        if child.tag == tag("input"):
            inputs.append(parse_input_shared(lookup, child))
        if child.tag == tag("p"):
            assert p is None
            p = parse_p(lookup, child)

    return types.Triangles(name, count, material, inputs, p)

def parse_mesh(lookup, root):
    sources = []
    vertices = None
    primitive_elements = []

    for child in root.getchildren():
        if child.tag == tag("source"):
            sources.append(parse_source_core(lookup, child))
        if child.tag == tag("vertices"):
            assert vertices is None
            vertices = parse_vertices(lookup, child)
        if child.tag == tag("lines"):
            assert False, child.tag
        if child.tag == tag("linestrips"):
            assert False, child.tag
        if child.tag == tag("polygons"):
            assert False, child.tag
        if child.tag == tag("polylist"):
            assert False, child.tag
        if child.tag == tag("triangles"):
            primitive_elements.append(parse_triangles(lookup, child))
        if child.tag == tag("trifans"):
            assert False, child.tag
        if child.tag == tag("tristrips"):
            assert False, child.tag
    assert vertices is not None
    assert len(sources) >= 1

    mesh = types.Mesh(sources, vertices, primitive_elements)
    return mesh

def parse_geometry(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")

    geometric_element = None

    for child in root.getchildren():
        if child.tag == tag("convex_mesh"):
            assert child.tag, False
        if child.tag == tag("mesh"):
            assert geometric_element is None
            geometric_element = parse_mesh(lookup, child)
        if child.tag == tag("spline"):
            assert child.tag, False
    assert geometric_element is not None

    geometry = types.Geometry(id, name, geometric_element)
    lookup_add(lookup, id, geometry)
    return geometry

def parse_library_geometries(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")
    geometries = []

    for child in root.getchildren():
        if child.tag == tag("geometry"):
            geometries.append(parse_geometry(lookup, child))

    assert len(geometries) >= 1
    library_geometries = types.LibraryGeometries(id, name, geometries)
    lookup_add(lookup, id, library_geometries)
    return library_geometries

def parse_light_color(lookup, root, *, cls):
    color = None
    for child in root.getchildren():
        if child.tag == tag("color"):
            assert color is None
            color = tuple([float(i) for i in child.text.strip().split()])
    assert color is not None
    return cls(color)

parse_ambient = partial(parse_light_color, cls=types.Ambient)
parse_directional = partial(parse_light_color, cls=types.Directional)
parse_point = partial(parse_light_color, cls=types.Point)
parse_spot = partial(parse_light_color, cls=types.Spot)

def parse_technique_common_light(lookup, root):
    light = None

    for child in root.getchildren():
        if child.tag == tag("ambient"):
            assert light is None
            light = parse_ambient(lookup, child)
        if child.tag == tag("directional"):
            assert light is None
            light = parse_directional(lookup, child)
        if child.tag == tag("point"):
            assert light is None
            light = parse_point(lookup, child)
        if child.tag == tag("spot"):
            assert light is None
            light = parse_spot(lookup, child)

    assert light is not None

    return types.TechniqueCommon_Light(light)

def parse_light(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")

    technique_common = None

    for child in root.getchildren():
        if child.tag == tag("technique_common"):
            assert technique_common is None
            technique_common = parse_technique_common_light(lookup, child)

    assert technique_common is not None

    light = types.Light(id, name, technique_common)
    lookup_add(lookup, id, light)
    return light

def parse_library_lights(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")
    lights = []

    for child in root.getchildren():
        if child.tag == tag("light"):
            lights.append(parse_light(lookup, child))

    assert len(lights) >= 1
    library_lights = types.LibraryLights(id, name, lights)
    lookup_add(lookup, id, library_lights)
    return library_lights

def parse_init_from(lookup, root):
    assert len(root.getchildren()) == 0
    return types.InitFrom(root.text.strip())

def parse_image(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")
    format = root.attrib.get("format")
    height = root.attrib.get("height")
    width = root.attrib.get("width")
    depth = root.attrib.get("depth")

    image_source = None

    for child in root.getchildren():
        if child.tag == tag("data"):
            assert False, child.tag
        if child.tag == tag("init_from"):
            assert image_source is None
            image_source = parse_init_from(lookup, child)

    assert image_source is not None
    image = types.Image(id, name, format, height, width, depth, image_source)
    lookup_add(lookup, id, image)
    return image

def parse_library_images(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")

    images = []

    for child in root.getchildren():
        if child.tag == tag("image"):
            images.append(parse_image(lookup, child))

    assert len(images) >= 1

    library_images = types.LibraryImages(id, name, images)
    lookup_add(lookup, id, library_images)
    return library_images

def parse_node_type(s):
    if s == "JOINT":
        return types.NodeType.JOINT
    if s == "NODE":
        return types.NodeType.NODE
    assert False, s

def parse_lookat(lookup, sid_lookup, root):
    sid = root.attrib.get("sid")

    assert len(root.getchildren()) == 0
    values = [float(i) for i in root.text.strip().split()]
    assert len(values) == 9

    eye = tuple(values[0:3])
    at = tuple(values[3:6])
    up = tuple(values[6:9])

    lookat = types.Lookat(sid, eye, at, up)
    lookup_add(sid_lookup, sid, lookat)
    return lookat

def parse_matrix(lookup, sid_lookup, root):
    sid = root.attrib.get("sid")

    assert len(root.getchildren()) == 0
    values = [float(i) for i in root.text.strip().split()]
    assert len(values) == 16

    r0 = tuple(values[0:4])
    r1 = tuple(values[4:8])
    r2 = tuple(values[8:12])
    r3 = tuple(values[12:16])

    matrix = types.Matrix(sid, tuple([r0, r1, r2, r3]))
    lookup_add(sid_lookup, sid, matrix)
    return matrix

def parse_simple_transform(lookup, sid_lookup, root, *, cls, count):
    sid = root.attrib.get("sid")

    assert len(root.getchildren()) == 0
    values = [float(i) for i in root.text.strip().split()]
    assert len(values) == count

    inst = cls(sid, values)
    lookup_add(sid_lookup, sid, inst)
    return inst

parse_rotate = partial(parse_simple_transform, cls=types.Rotate, count=4)
parse_scale = partial(parse_simple_transform, cls=types.Scale, count=3)
parse_skew = partial(parse_simple_transform, cls=types.Skew, count=7)
parse_translate = partial(parse_simple_transform, cls=types.Translate, count=3)

def parse_bind_vertex_input(lookup, root):
    semantic = root.attrib["semantic"]
    input_semantic = root.attrib["input_semantic"]
    input_set = root.attrib.get("input_set")

    return types.BindVertexInput(semantic, input_semantic, input_set)

def parse_instance_material(lookup, sid_lookup, root):
    sid = root.attrib.get("sid")
    name = root.attrib.get("name")
    target = root.attrib["target"]
    symbol = root.attrib["symbol"]

    bind_vertex_inputs = []
    for child in root.getchildren():
        if child.tag == tag("bind_vertex_input"):
            bind_vertex_inputs.append(parse_bind_vertex_input(lookup, child))

    inst = types.InstanceMaterial(sid, name, target, symbol, bind_vertex_inputs)
    lookup_add(sid_lookup, sid, inst)
    return inst

def parse_technique_common_bind_material(lookup, root):
    materials = []
    sid_lookup = {}

    for child in root.getchildren():
        if child.tag == tag("instance_material"):
            materials.append(parse_instance_material(lookup, sid_lookup, child))

    assert len(materials) >= 1
    return types.TechniqueCommon_BindMaterial(materials, sid_lookup)

def parse_bind_material(lookup, root):
    technique_common = None
    for child in root.getchildren():
        if child.tag == tag("technique_common"):
            assert technique_common is None
            technique_common = parse_technique_common_bind_material(lookup, child)

    assert technique_common is not None
    return types.BindMaterial(technique_common)

def parse_instance_geometry(lookup, sid_lookup, root):
    sid = root.attrib.get("sid")
    name = root.attrib.get("name")
    url = root.attrib["url"]

    bind_material = None # optional

    for child in root.getchildren():
        if child.tag == tag("bind_material"):
            assert bind_material is None
            bind_material = parse_bind_material(lookup, child)

    # bind_material may be none
    instance_geometry = types.InstanceGeometry(sid, name, url, bind_material)
    lookup_add(sid_lookup, sid, instance_geometry)
    return instance_geometry

def parse_instance_light(lookup, sid_lookup, root):
    sid = root.attrib.get("sid")
    name = root.attrib.get("name")
    url = root.attrib["url"]

    instance_light = types.InstanceLight(sid, name, url)
    lookup_add(sid_lookup, sid, instance_light)
    return instance_light

def parse_node(lookup, sid_lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")
    sid = root.attrib.get("sid")
    type = parse_node_type(root.attrib["type"]) if "type" in root.attrib else types.NodeType.NODE
    layer = root.attrib.get("layer", "").strip().split()

    transformation_elements = []
    instance_geometries = []
    instance_lights = []
    nodes = []

    child_sid_lookup = {}

    for child in root.getchildren():
        if child.tag == tag("lookat"):
            transformation_elements.append(parse_lookat(lookup, child_sid_lookup, child))
        if child.tag == tag("matrix"):
            transformation_elements.append(parse_matrix(lookup, child_sid_lookup, child))
        if child.tag == tag("rotate"):
            transformation_elements.append(parse_rotate(lookup, child_sid_lookup, child))
        if child.tag == tag("scale"):
            transformation_elements.append(parse_scale(lookup, child_sid_lookup, child))
        if child.tag == tag("skew"):
            transformation_elements.append(parse_skew(lookup, child_sid_lookup, child))
        if child.tag == tag("translate"):
            transformation_elements.append(parse_translate(lookup, child_sid_lookup, child))
        if child.tag == tag("instance_geometry"):
            instance_geometries.append(parse_instance_geometry(lookup, child_sid_lookup, child))
        if child.tag == tag("instance_light"):
            instance_lights.append(parse_instance_light(lookup, child_sid_lookup, child))
        if child.tag == tag("node"):
            nodes.append(parse_node(lookup, child_sid_lookup, child))

    node = types.Node(id, name, sid, type, layer,
                      transformation_elements,
                      instance_geometries,
                      instance_lights,
                      nodes,
                      child_sid_lookup)

    lookup_add(lookup, id, node)
    lookup_add(sid_lookup, sid, node)
    return node

def parse_visual_scene(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")

    sid_lookup = {}

    nodes = []
    for child in root.getchildren():
        if child.tag == tag("node"):
            nodes.append(parse_node(lookup, sid_lookup, child))

    assert len(nodes) >= 1

    visual_scenes = types.VisualScene(id, name, nodes, sid_lookup)
    lookup_add(lookup, id, visual_scenes)
    return visual_scenes

def parse_library_visual_scenes(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")

    visual_scenes = []

    for child in root.getchildren():
        if child.tag == tag("visual_scene"):
            visual_scenes.append(parse_visual_scene(lookup, child))

    assert len(visual_scenes) >= 1

    library_visual_scenes = types.LibraryVisualScenes(id, name, visual_scenes)
    lookup_add(lookup, id, library_visual_scenes)
    return library_visual_scenes

def parse_instance_visual_scene(lookup, sid_lookup, root):
    sid = root.attrib.get("sid")
    name = root.attrib.get("name")
    url = root.attrib["url"]

    instance_visual_scene = types.InstanceVisualScene(sid, name, url)
    lookup_add(sid_lookup, sid, instance_visual_scene)
    return instance_visual_scene

def parse_scene(lookup, root):
    instance_visual_scene = None
    sid_lookup = {}

    for child in root.getchildren():
        if child.tag == tag("instance_visual_scene"):
            instance_visual_scene = parse_instance_visual_scene(lookup, sid_lookup, child)

    # instance_visual_scene may be none
    return types.Scene(instance_visual_scene, sid_lookup)

def parse_sampler(lookup, root):
    id = root.attrib.get("id")

    inputs = []
    for child in root.getchildren():
        if child.tag == tag("input"):
            inputs.append(parse_input_unshared(lookup, child))

    sampler = types.Sampler(id, inputs)
    lookup_add(lookup, id, sampler)
    return sampler

def parse_channel(lookup, root):
    source = root.attrib["source"]
    target = root.attrib["target"]

    return types.Channel(source, target)

def parse_animation(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")

    animations = [] # nested animation
    sources = []
    samplers = []
    channels = []
    for child in root.getchildren():
        if child.tag == tag("animation"):
            animations.append(parse_animation(lookup, child))
        if child.tag == tag("source"):
            sources.append(parse_source_core(lookup, child))
        if child.tag == tag("sampler"):
            samplers.append(parse_sampler(lookup, child))
        if child.tag == tag("channels"):
            channels.append(parse_channel(lookup, child))

    animation = types.Animation(id, name, animations, sources, samplers, channels)
    lookup_add(lookup, id, animation)
    return animation

def parse_library_animations(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")

    animations = []
    for child in root.getchildren():
        if child.tag == tag("animation"):
            animations.append(parse_animation(lookup, child))

    assert len(animations) >= 1

    library_animations = types.LibraryAnimations(id, name, animations)
    lookup_add(lookup, id, library_animations)
    return library_animations

def parse_bind_shape_matrix(lookup, root):
    assert len(root.getchildren()) == 0
    values = [float(i) for i in root.text.strip().split()]
    assert len(values) == 16

    r0 = tuple(values[0:4])
    r1 = tuple(values[4:8])
    r2 = tuple(values[8:12])
    r3 = tuple(values[12:16])
    values = tuple([r0, r1, r2, r3])
    return types.BindShapeMatrix(values)

def parse_joints(lookup, root):
    inputs = []

    for child in root.getchildren():
        if child.tag == tag("input"):
            inputs.append(parse_input_unshared(lookup, child))

    assert len(inputs) >= 2

    return types.Joints(inputs)

def parse_vertex_weights(lookup, root):
    inputs = []
    vcount = None
    v = None

    for child in root.getchildren():
        if child.tag == tag("input"):
            inputs.append(parse_input_shared(lookup, child))
        if child.tag == tag("vcount"):
            assert vcount is None
            vcount = parse_p(lookup, child)
        if child.tag == tag("v"):
            assert v is None
            v = parse_p(lookup, child)

    assert len(inputs) >= 2
    assert vcount is not None
    assert v is not None
    return types.VertexWeights(inputs, vcount, v)

def parse_skin(lookup, root):
    source = root.attrib["source"]

    bind_shape_matrix = None
    sources = []
    joints = None
    vertex_weights = None

    for child in root.getchildren():
        if child.tag == tag("bind_shape_matrix"):
            bind_shape_matrix = parse_bind_shape_matrix(lookup, child)
        if child.tag == tag("source"):
            sources.append(parse_source_core(lookup, child))
        if child.tag == tag("joints"):
            assert joints is None
            joints = parse_joints(lookup, child)
        if child.tag == tag("vertex_weights"):
            assert vertex_weights is None
            vertex_weights = parse_vertex_weights(lookup, child)

    assert joints is not None
    assert vertex_weights is not None

    return types.Skin(source, bind_shape_matrix, sources, joints, vertex_weights)

def parse_controller(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")

    control_element = None
    for child in root.getchildren():
        if child.tag == tag("skin"):
            assert control_element is None
            control_element = parse_skin(lookup, child)
        if child.tag == tag("morph"):
            assert control_element is None
            assert False, child.tag # not implemented

    assert control_element is not None
    controller = types.Controller(id, name, control_element)
    lookup_add(lookup, id, controller)
    return controller

def parse_library_controllers(lookup, root):
    id = root.attrib.get("id")
    name = root.attrib.get("name")

    controllers = []
    for child in root.getchildren():
        if child.tag == tag("controller"):
            controllers.append(parse_controller(lookup, child))

    assert len(controllers) >= 1

    library_controllers = types.LibraryControllers(id, name, controllers)
    lookup_add(lookup, id, library_controllers)
    return library_controllers

def parse_collada(tree):
    root = tree.getroot()
    assert root.tag == tag("COLLADA")

    collada = types.Collada()
    lookup = {}

    for child in root.getchildren():
        if child.tag == tag("library_animations"):
            collada.library_animations.append(parse_library_animations(lookup, child))
        if child.tag == tag("library_controllers"):
            collada.library_controllers.append(parse_library_controllers(lookup, child))
        if child.tag == tag("library_effects"):
            collada.library_effects.append(parse_library_effects(lookup, child))
        if child.tag == tag("library_materials"):
            collada.library_materials.append(parse_library_materials(lookup, child))
        if child.tag == tag("library_geometries"):
            collada.library_geometries.append(parse_library_geometries(lookup, child))
        if child.tag == tag("library_lights"):
            collada.library_lights.append(parse_library_lights(lookup, child))
        if child.tag == tag("library_images"):
            collada.library_images.append(parse_library_images(lookup, child))
        if child.tag == tag("library_visual_scenes"):
            collada.library_visual_scenes.append(parse_library_visual_scenes(lookup, child))
        if child.tag == tag("scene"):
            collada.scenes.append(parse_scene(lookup, child))

    collada._lookup = lookup
    return collada

def parse_collada_file(filename):
    with open(filename) as f:
        tree = etree.parse(f)
    collada = parse_collada(tree)
    return collada

if __name__ == "__main__":
    import sys
    from prettyprinter import pprint, install_extras
    install_extras(include=["dataclasses"])
    collada = parse_collada_file(sys.argv[1])
    pprint(collada, width=120)
