from dataclasses import dataclass, field
from enum import Enum, auto
from typing import Optional, Union, List, Tuple

URI = str
ID = str

Float3 = Tuple[float, float, float]
Float4 = Tuple[float, float, float, float]
Float7 = Tuple[float, float, float, float, float, float, float]

@dataclass(frozen=True)
class Lookat:
    sid: Optional[str]
    eye: Float3
    at: Float3
    up: Float3

@dataclass(frozen=True)
class Matrix:
    # collada is the transpose of a directx matrix
    # 1.0  0.0  0.0  0.0
    # 0.0  1.0  0.0  0.0
    # 0.0  0.0  1.0  0.0
    # 2.0  3.0  4.0  1.0
    sid: Optional[str]
    values: Tuple[Float4, Float4, Float4, Float4]

@dataclass(frozen=True)
class Rotate:
    # x y z w ; quaternion
    sid: Optional[str]
    rotate: Float4

@dataclass(frozen=True)
class Scale:
    sid: Optional[str]
    scale: Float3

@dataclass(frozen=True)
class Skew:
    sid: Optional[str]
    skew: Float7

@dataclass(frozen=True)
class Translate:
    sid: Optional[str]
    translate: Float3

@dataclass(frozen=True)
class Param:
    name: Optional[str]
    sid: Optional[str]
    type: str
    semantic: Optional[str]

@dataclass(frozen=True)
class Accessor:
    count: int
    offset: Optional[int]
    source: URI
    stride: Optional[int]

    params: List[Param]

    sid_lookup: dict = field(repr=False)

@dataclass(frozen=True)
class TechniqueCommon_SourceCore:
    accessor: Accessor

@dataclass(frozen=True)
class Ambient:
    color: Float3

@dataclass(frozen=True)
class Directional:
    color: Float3

@dataclass(frozen=True)
class Point:
    color: Float3

@dataclass(frozen=True)
class Spot:
    color: Float3

@dataclass(frozen=True)
class BindVertexInput:
    semantic: str
    input_semantic: str
    input_set: int

@dataclass(frozen=True)
class InstanceMaterial:
    sid: Optional[str]
    name: Optional[str]
    target: URI
    symbol: str

    bind_vertex_inputs: List[BindVertexInput]

@dataclass(frozen=True)
class TechniqueCommon_BindMaterial:
    materials: List[InstanceMaterial] # one or more

    sid_lookup: dict = field(repr=False)

@dataclass(frozen=True)
class BindMaterial:
    technique_common: TechniqueCommon_BindMaterial

@dataclass(frozen=True)
class InstanceGeometry:
    sid: Optional[str]
    name: Optional[str]
    url: URI

    # child element
    bind_material: Optional[BindMaterial]

@dataclass(frozen=True)
class InstanceLight:
    sid: Optional[str]
    name: Optional[str]
    url: URI

@dataclass(frozen=True)
class InstanceVisualScene:
    sid: Optional[str]
    name: Optional[str]
    url: URI

@dataclass(frozen=True)
class Scene:
    instance_visual_scene: InstanceVisualScene

    sid_lookup: dict = field(repr=False)

class FxSurfaceType(Enum):
    UNTYPED = auto()
    _1D = auto()
    _2D = auto()
    _3D = auto()
    CUBE = auto()
    DEPTH = auto()
    RECT = auto()

@dataclass(frozen=True)
class InitFrom:
    uri: str

@dataclass(frozen=True)
class Surface:
    type: FxSurfaceType

    init_from: InitFrom

@dataclass(frozen=True)
class SourceFX:
    sid: ID

@dataclass(frozen=True)
class Sampler2D:
    source: SourceFX

@dataclass(frozen=True)
class Newparam:
    sid: str # required
    parameter_type: Union[Surface, Sampler2D]

@dataclass(frozen=True)
class Color:
    value: Float4

@dataclass(frozen=True)
class Float:
    value: float

@dataclass(frozen=True)
class Texture:
    texture: str
    texcoord:  str

@dataclass(frozen=True)
class Blinn:
    emission: Optional[Union[Color, Texture]]
    ambient: Optional[Union[Color, Texture]]
    diffuse: Optional[Union[Color, Texture]]
    specular: Optional[Union[Color, Texture]]
    shininess: Optional[Float]
    reflective: Optional[Union[Color, Texture]]
    reflectivity: Optional[Float]
    transparent: Optional[Union[Color, Texture]]
    transparency: Optional[Float]
    index_of_refraction: Optional[Float]

@dataclass(frozen=True)
class Lambert:
    emission: Optional[Union[Color, Texture]]
    ambient: Optional[Union[Color, Texture]]
    diffuse: Optional[Union[Color, Texture]]
    reflective: Optional[Union[Color, Texture]]
    reflectivity: Optional[Float]
    transparent: Optional[Union[Color, Texture]]
    transparency: Optional[Float]
    index_of_refraction: Optional[Float]

@dataclass(frozen=True)
class Phong:
    emission: Optional[Union[Color, Texture]]
    ambient: Optional[Union[Color, Texture]]
    diffuse: Optional[Union[Color, Texture]]
    specular: Optional[Union[Color, Texture]]
    shininess: Optional[Float]
    reflective: Optional[Union[Color, Texture]]
    reflectivity: Optional[Float]
    transparent: Optional[Union[Color, Texture]]
    transparency: Optional[Float]
    index_of_refraction: Optional[Float]

@dataclass(frozen=True)
class Constant:
    emission: Optional[Color]
    reflective: Optional[Union[Color, Texture]]
    reflectivity: Optional[Float]
    transparent: Optional[Union[Color, Texture]]
    transparency: Optional[Float]
    index_of_refraction: Optional[Float]

@dataclass(frozen=True)
class TechniqueFX:
    id: Optional[ID]
    sid: str # required
    shader: Union[Blinn, Lambert, Phong, Constant]

@dataclass(frozen=True)
class ProfileCommon:
    id: Optional[ID]

    newparam: List[Newparam]
    technique: TechniqueFX

    sid_lookup: dict = field(repr=False)

@dataclass(frozen=True)
class Effect:
    id: str
    name: Optional[str]

    profile_common: List[ProfileCommon]

@dataclass(frozen=True)
class LibraryEffects:
    id: Optional[ID]
    name: Optional[str]

    effects: List[Effect]

@dataclass(frozen=True)
class InstanceEffect:
    sid: Optional[str]
    name: Optional[str]
    url: URI

@dataclass(frozen=True)
class Material:
    id: Optional[ID]
    name: Optional[str]

    instance_effect: InstanceEffect

    sid_lookup: dict = field(repr=False)

@dataclass(frozen=True)
class LibraryMaterials:
    id: Optional[ID]
    name: Optional[str]

    materials: List[Material]

@dataclass(frozen=True)
class NameArray:
    count: int
    id: Optional[ID]
    name: Optional[str]

    names: List[str]

@dataclass(frozen=True)
class BoolArray:
    count: int
    id: Optional[ID]
    name: Optional[str]

    bools: List[bool]

@dataclass(frozen=True)
class FloatArray:
    count: int
    id: Optional[ID]
    name: Optional[str]
    digits: Optional[int]
    magnitude: Optional[int]

    floats: List[float]

@dataclass(frozen=True)
class IntArray:
    count: int
    id: Optional[ID]
    name: Optional[str]
    minInclusive: Optional[int]
    maxInclusive: Optional[int]

    ints: List[int]

@dataclass(frozen=True)
class SourceCore:
    id: ID
    name: Optional[str]

    array_element: Union[NameArray, BoolArray, FloatArray, IntArray]
    technique_common: TechniqueCommon_SourceCore

@dataclass(frozen=True)
class InputShared:
    offset: int
    semantic: str
    source: URI
    set: Optional[int]

@dataclass(frozen=True)
class Triangles:
    name: Optional[str]
    count: int
    material: Optional[str]
    inputs: List[InputShared]
    p: List[int]

@dataclass(frozen=True)
class InputUnshared:
    semantic: str
    source: URI

@dataclass(frozen=True)
class Vertices:
    id: ID
    name: Optional[str]

    inputs: List[InputUnshared] # 1 or more

@dataclass(frozen=True)
class Mesh:
    sources: List[SourceCore]
    vertices: Vertices
    primitive_elements: List[Union[Triangles]]

@dataclass(frozen=True)
class Geometry:
    id: Optional[ID]
    name: Optional[str]

    geometric_element: Union[Mesh]

@dataclass(frozen=True)
class LibraryGeometries:
    id: Optional[ID]
    name: Optional[str]

    geometries: List[Geometry]

@dataclass(frozen=True)
class TechniqueCommon_Light:
    light: Union[Ambient, Directional, Point, Spot]

@dataclass(frozen=True)
class Light:
    id: Optional[ID]
    name: Optional[str]

    technique_common: TechniqueCommon_Light

@dataclass(frozen=True)
class LibraryLights:
    id: Optional[ID]
    name: Optional[str]

    lights: List[Light]

@dataclass(frozen=True)
class Image:
    id: Optional[ID]
    name: Optional[str]
    format: Optional[str]
    height: Optional[int]
    width: Optional[int]
    depth: Optional[int]

    image_source: Union[InitFrom]

@dataclass(frozen=True)
class LibraryImages:
    id: Optional[ID]
    name: Optional[str]

    images: List[Image]

class NodeType(Enum):
    JOINT = auto()
    NODE = auto()

TransformationElements = Union[Lookat, Matrix, Rotate, Scale, Skew, Translate]

@dataclass(frozen=True)
class Node:
    id: Optional[ID]
    name: Optional[str]
    sid: Optional[str]
    type: NodeType
    layer: List[str]

    transformation_elements: List[TransformationElements]
    instance_geometries: List[InstanceGeometry]
    instance_lights: List[InstanceLight]
    nodes: List['Node']

    sid_lookup: dict = field(repr=False)

@dataclass(frozen=True)
class VisualScene:
    id: Optional[ID]
    name: Optional[str]

    nodes: List[Node]

    sid_lookup: dict = field(repr=False)

@dataclass(frozen=True)
class LibraryVisualScenes:
    id: Optional[ID]
    name: Optional[str]

    visual_scenes: List[VisualScene]

@dataclass(frozen=True)
class BindShapeMatrix:
    # it is written in row-major order in the COLLADA document for
    # human readability.
    values: Tuple[Float4, Float4, Float4, Float4]

@dataclass(frozen=True)
class Joints:
    inputs: List[InputUnshared] # 2 or more

@dataclass(frozen=True)
class VertexWeights:
    inputs: List[InputShared] # 2 or more
    vcount: List[int]
    v: List[int]

@dataclass(frozen=True)
class Skin:
    source: URI # required

    bind_shape_matrix: Optional[BindShapeMatrix]
    sources: List[SourceCore] # 3 or more
    joints: Joints # 1
    vertex_weights: VertexWeights # 1

@dataclass(frozen=True)
class Controller:
    id: Optional[ID]
    name: Optional[str]

    control_element: Union[Skin]

@dataclass(frozen=True)
class LibraryControllers:
    id: Optional[ID]
    name: Optional[str]

    controllers: List[Controller]

@dataclass(frozen=True)
class Sampler:
    id: Optional[ID]
    inputs: List[InputUnshared] # 1 or more

@dataclass(frozen=True)
class Channel:
    source: URI
    target: URI

@dataclass(frozen=True)
class Animation:
    id: Optional[ID]
    name: Optional[str]

    animations: List['Animation']
    sources: List[SourceCore]
    samplers: List[Sampler]
    channels: List[Channel]

@dataclass(frozen=True)
class LibraryAnimations:
    id: Optional[ID]
    name: Optional[str]

    animations: List[Animation] # 1 or more

@dataclass
class Collada:
    library_animations: List[LibraryAnimations]
    library_controllers: List[LibraryControllers]
    library_effects: List[LibraryEffects]
    library_materials: List[LibraryMaterials]
    library_geometries: List[LibraryGeometries]
    library_lights: List[LibraryLights]
    library_images: List[LibraryImages]
    library_visual_scenes: List[LibraryVisualScenes]
    scenes: List[Scene]
    _lookup: dict = field(repr=False)

    def __init__(self):
        self.library_animations = []
        self.library_controllers = []
        self.library_effects = []
        self.library_materials = []
        self.library_geometries = []
        self.library_lights = []
        self.library_images = []
        self.library_visual_scenes = []
        self.scenes = []
        self._lookup = None

    def lookup(self, s, t):
        assert '/' not in s
        assert s.startswith("#")
        id = s[1:]
        value = self._lookup[id]
        assert type(value) is t, (s, type(value))
        return value
