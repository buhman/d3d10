#include "collada_types.hpp"

namespace curve_interpolation {

using namespace collada;

float const array_node_cube_translation_x_input_array[] = {
  0.0f,
  1.666667f,
  3.333333f,
  5.0f,
};

float const array_node_cube_translation_x_output_array[] = {
  10.0f,
  -10.0f,
  10.0f,
  -10.0f,
};

float2 const array_node_cube_translation_x_intangent_array[] = {
  { -0.3332306f, 10.0f },
  { 1.111167f, -10.0f },
  { 2.778333f, 10.0f },
  { 4.4445f, -9.219337f },
};

float2 const array_node_cube_translation_x_outtangent_array[] = {
  { 0.5555f, 10.0f },
  { 2.222167f, -10.0f },
  { 3.888333f, 10.0f },
  { 4.000208f, -8.594958f },
};

enum interpolation const array_node_cube_translation_x_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_cube_translation_x_sampler = {
  // node_cube_translation_x_input
  .input = {
    .float_array = array_node_cube_translation_x_input_array,
    .count = 4,
  },
  // node_cube_translation_x_output
  .output = {
    .float_array = array_node_cube_translation_x_output_array,
    .count = 4,
  },
  // node_cube_translation_x_intangent
  .in_tangent = {
    .float2_array = array_node_cube_translation_x_intangent_array,
    .count = 4,
  },
  // node_cube_translation_x_outtangent
  .out_tangent = {
    .float2_array = array_node_cube_translation_x_outtangent_array,
    .count = 4,
  },
  // node_cube_translation_x_interpolation
  .interpolation = {
    .interpolation_array = array_node_cube_translation_x_interpolation_array,
    .count = 4,
  },
};

float const array_node_cube_translation_y_input_array[] = {
  -0.8333334f,
  0.8333334f,
  2.5f,
  4.166667f,
};

float const array_node_cube_translation_y_output_array[] = {
  -10.05776f,
  10.05852f,
  -9.941484f,
  10.05852f,
};

float2 const array_node_cube_translation_y_intangent_array[] = {
  { -1.166264f, -10.05776f },
  { 0.2778334f, 10.05852f },
  { 1.9445f, -9.941484f },
  { 3.611667f, 10.05852f },
};

float2 const array_node_cube_translation_y_outtangent_array[] = {
  { -0.2783333f, -10.05776f },
  { 1.388833f, 10.05852f },
  { 3.0555f, -9.941484f },
  { 4.499598f, 10.05852f },
};

enum interpolation const array_node_cube_translation_y_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_cube_translation_y_sampler = {
  // node_cube_translation_y_input
  .input = {
    .float_array = array_node_cube_translation_y_input_array,
    .count = 4,
  },
  // node_cube_translation_y_output
  .output = {
    .float_array = array_node_cube_translation_y_output_array,
    .count = 4,
  },
  // node_cube_translation_y_intangent
  .in_tangent = {
    .float2_array = array_node_cube_translation_y_intangent_array,
    .count = 4,
  },
  // node_cube_translation_y_outtangent
  .out_tangent = {
    .float2_array = array_node_cube_translation_y_outtangent_array,
    .count = 4,
  },
  // node_cube_translation_y_interpolation
  .interpolation = {
    .interpolation_array = array_node_cube_translation_y_interpolation_array,
    .count = 4,
  },
};

channel const node_channel_node_cube_translation_x = {
  .source_sampler = &sampler_node_cube_translation_x_sampler,
  .target_transform_index = 0,
  .target_attribute = target_attribute::X,
};

channel const node_channel_node_cube_translation_y = {
  .source_sampler = &sampler_node_cube_translation_y_sampler,
  .target_transform_index = 0,
  .target_attribute = target_attribute::Y,
};

effect const effect_material__15 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .color = {0.6705883f, 0.5843138f, 1.0f, 1.0f},
    },
    .diffuse = {
      .color = {0.6705883f, 0.5843138f, 1.0f, 1.0f},
    },
    .specular = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_material__16 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .color = {0.5803922f, 1.0f, 0.9647059f, 1.0f},
    },
    .diffuse = {
      .color = {0.5803922f, 1.0f, 0.9647059f, 1.0f},
    },
    .specular = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_material__17 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .color = {0.6509804f, 1.0f, 0.5803922f, 1.0f},
    },
    .diffuse = {
      .color = {0.6509804f, 1.0f, 0.5803922f, 1.0f},
    },
    .specular = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_material__18 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .color = {0.9333334f, 1.0f, 0.5647059f, 1.0f},
    },
    .diffuse = {
      .color = {0.9333334f, 1.0f, 0.5647059f, 1.0f},
    },
    .specular = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_material__19 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .color = {1.0f, 0.7686275f, 0.5803922f, 1.0f},
    },
    .diffuse = {
      .color = {1.0f, 0.7686275f, 0.5803922f, 1.0f},
    },
    .specular = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_material__20 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .color = {1.0f, 0.5803922f, 0.5803922f, 1.0f},
    },
    .diffuse = {
      .color = {1.0f, 0.5803922f, 0.5803922f, 1.0f},
    },
    .specular = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_coloreffectr229g154b215 = {
  .type = effect_type::PHONG,
  .phong = {
    .emission = {
      .color = {0.0f, 0.0f, 0.0f, 0.0f},
    },
    .ambient = {
      .color = {0.8980392f, 0.6039216f, 0.8431373f, 1.0f},
    },
    .diffuse = {
      .color = {0.8980392f, 0.6039216f, 0.8431373f, 1.0f},
    },
    .specular = {
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_coloreffectr28g149b177 = {
  .type = effect_type::PHONG,
  .phong = {
    .emission = {
      .color = {0.0f, 0.0f, 0.0f, 0.0f},
    },
    .ambient = {
      .color = {0.1098039f, 0.5843137f, 0.6941176f, 1.0f},
    },
    .diffuse = {
      .color = {0.1098039f, 0.5843137f, 0.6941176f, 1.0f},
    },
    .specular = {
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

material const material_coloreffectr229g154b215_material = {
  .effect = &effect_coloreffectr229g154b215,
};

material const material_coloreffectr28g149b177_material = {
  .effect = &effect_coloreffectr28g149b177,
};

material const material_material__15_material = {
  .effect = &effect_material__15,
};

material const material_material__16_material = {
  .effect = &effect_material__16,
};

material const material_material__17_material = {
  .effect = &effect_material__17,
};

material const material_material__18_material = {
  .effect = &effect_material__18,
};

material const material_material__19_material = {
  .effect = &effect_material__19,
};

material const material_material__20_material = {
  .effect = &effect_material__20,
};

input_element const input_elements_position_0_3_normal_0_3_texcoord_0_3[] = {
  {
    .semantic = "POSITION",
    .semantic_index = 0,
    .format = input_format::FLOAT3,
  },
  {
    .semantic = "NORMAL",
    .semantic_index = 0,
    .format = input_format::FLOAT3,
  },
  {
    .semantic = "TEXCOORD",
    .semantic_index = 0,
    .format = input_format::FLOAT3,
  },
};

triangles const triangles_geom_cube[] = {
  {
    .count = 2, // triangles
    .index_offset = 0, // indices
    .inputs_index = 0, // index into inputs_list
  },
  {
    .count = 2, // triangles
    .index_offset = 6, // indices
    .inputs_index = 0, // index into inputs_list
  },
  {
    .count = 2, // triangles
    .index_offset = 12, // indices
    .inputs_index = 0, // index into inputs_list
  },
  {
    .count = 2, // triangles
    .index_offset = 18, // indices
    .inputs_index = 0, // index into inputs_list
  },
  {
    .count = 2, // triangles
    .index_offset = 24, // indices
    .inputs_index = 0, // index into inputs_list
  },
  {
    .count = 2, // triangles
    .index_offset = 30, // indices
    .inputs_index = 0, // index into inputs_list
  },
};

geometry const geometry_geom_cube = {
  .mesh = {
    .triangles = triangles_geom_cube,
    .triangles_count = 6,

    .vertex_buffer_offset = 0,
    .vertex_buffer_size = 864,

    .index_buffer_offset = 0,
    .index_buffer_size = 144,
  }
};

triangles const triangles_geom_cylinder001[] = {
  {
    .count = 30, // triangles
    .index_offset = 0, // indices
    .inputs_index = 0, // index into inputs_list
  },
};

geometry const geometry_geom_cylinder001 = {
  .mesh = {
    .triangles = triangles_geom_cylinder001,
    .triangles_count = 1,

    .vertex_buffer_offset = 864,
    .vertex_buffer_size = 1152,

    .index_buffer_offset = 144,
    .index_buffer_size = 360,
  }
};

triangles const triangles_geom_plane001[] = {
  {
    .count = 2, // triangles
    .index_offset = 0, // indices
    .inputs_index = 0, // index into inputs_list
  },
};

geometry const geometry_geom_plane001 = {
  .mesh = {
    .triangles = triangles_geom_plane001,
    .triangles_count = 1,

    .vertex_buffer_offset = 2016,
    .vertex_buffer_size = 144,

    .index_buffer_offset = 504,
    .index_buffer_size = 24,
  }
};

geometry const * const geometries[] = {
  &geometry_geom_cube,
  &geometry_geom_cylinder001,
  &geometry_geom_plane001,
};

transform const transforms_node_environmentambientlight[] = {
};

instance_geometry const instance_geometries_node_environmentambientlight[] = {
};

channel const * const node_channels_node_environmentambientlight[] = {};

node const node_node_environmentambientlight = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_environmentambientlight,
  .transforms_count = 0,

  .instance_geometries = instance_geometries_node_environmentambientlight,
  .instance_geometries_count = 0,

  .channels = node_channels_node_environmentambientlight,
  .channels_count = 0,
};

transform const transforms_node_cube[] = {
  {
    .type = transform_type::TRANSLATE,
    .translate = {10.0f, -1.14258e-07f, 0.0f},
  },
};

instance_material const instance_materials_node_cube_0[] = {
  {
    .element_index = 1, // an index into mesh.triangles
    .material = &material_material__15_material,
  },
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_material__16_material,
  },
  {
    .element_index = 5, // an index into mesh.triangles
    .material = &material_material__17_material,
  },
  {
    .element_index = 3, // an index into mesh.triangles
    .material = &material_material__18_material,
  },
  {
    .element_index = 2, // an index into mesh.triangles
    .material = &material_material__19_material,
  },
  {
    .element_index = 4, // an index into mesh.triangles
    .material = &material_material__20_material,
  },
};

instance_geometry const instance_geometries_node_cube[] = {
  {
    .geometry = &geometry_geom_cube,
    .instance_materials = instance_materials_node_cube_0,
    .instance_materials_count = 6,
  },
};

channel const * const node_channels_node_cube[] = {
  &node_channel_node_cube_translation_y,
  &node_channel_node_cube_translation_x,
};

node const node_node_cube = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_cube,
  .transforms_count = 1,

  .instance_geometries = instance_geometries_node_cube,
  .instance_geometries_count = 1,

  .channels = node_channels_node_cube,
  .channels_count = 2,
};

transform const transforms_node_cylinder001[] = {
};

instance_material const instance_materials_node_cylinder001_0[] = {
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_coloreffectr229g154b215_material,
  },
};

instance_geometry const instance_geometries_node_cylinder001[] = {
  {
    .geometry = &geometry_geom_cylinder001,
    .instance_materials = instance_materials_node_cylinder001_0,
    .instance_materials_count = 1,
  },
};

channel const * const node_channels_node_cylinder001[] = {
};

node const node_node_cylinder001 = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_cylinder001,
  .transforms_count = 0,

  .instance_geometries = instance_geometries_node_cylinder001,
  .instance_geometries_count = 1,

  .channels = node_channels_node_cylinder001,
  .channels_count = 0,
};

transform const transforms_node_plane001[] = {
  {
    .type = transform_type::TRANSLATE,
    .translate = {0.0f, 0.0f, 0.01f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {0.0f, 0.0f, -1.0f, -44.99999f},
  },
};

instance_material const instance_materials_node_plane001_0[] = {
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_coloreffectr28g149b177_material,
  },
};

instance_geometry const instance_geometries_node_plane001[] = {
  {
    .geometry = &geometry_geom_plane001,
    .instance_materials = instance_materials_node_plane001_0,
    .instance_materials_count = 1,
  },
};

channel const * const node_channels_node_plane001[] = {
};

node const node_node_plane001 = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_plane001,
  .transforms_count = 2,

  .instance_geometries = instance_geometries_node_plane001,
  .instance_geometries_count = 1,

  .channels = node_channels_node_plane001,
  .channels_count = 0,
};

node const * const nodes[] = {
  &node_node_environmentambientlight,
  &node_node_cube,
  &node_node_cylinder001,
  &node_node_plane001,
};

inputs const inputs_list[] = {
  {
    .elements = input_elements_position_0_3_normal_0_3_texcoord_0_3,
    .elements_count = 3,
  },
};

descriptor const descriptor = {
  .nodes = nodes,
  .nodes_count = (sizeof (nodes)) / (sizeof (nodes[0])),

  .inputs_list = inputs_list,
  .inputs_list_count = (sizeof (inputs_list)) / (sizeof (inputs_list[0])),
};

}
