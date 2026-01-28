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

float const array_node_cube_translation_x_intangent_array[] = {
  -0.3332306f, 10.0f,
  1.111167f, -10.0f,
  2.778333f, 10.0f,
  4.4445f, -9.219337f,
};

float const array_node_cube_translation_x_outtangent_array[] = {
  0.5555f, 10.0f,
  2.222167f, -10.0f,
  3.888333f, 10.0f,
  4.000208f, -8.594958f,
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
    .stride = 1,
  },
  // node_cube_translation_x_output
  .output = {
    .float_array = array_node_cube_translation_x_output_array,
    .count = 4,
    .stride = 1,
  },
  // node_cube_translation_x_intangent
  .in_tangent = {
    .float_array = array_node_cube_translation_x_intangent_array,
    .count = 4,
    .stride = 2,
  },
  // node_cube_translation_x_outtangent
  .out_tangent = {
    .float_array = array_node_cube_translation_x_outtangent_array,
    .count = 4,
    .stride = 2,
  },
  // node_cube_translation_x_interpolation
  .interpolation = {
    .interpolation_array = array_node_cube_translation_x_interpolation_array,
    .count = 4,
    .stride = 1,
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

float const array_node_cube_translation_y_intangent_array[] = {
  -1.166264f, -10.05776f,
  0.2778334f, 10.05852f,
  1.9445f, -9.941484f,
  3.611667f, 10.05852f,
};

float const array_node_cube_translation_y_outtangent_array[] = {
  -0.2783333f, -10.05776f,
  1.388833f, 10.05852f,
  3.0555f, -9.941484f,
  4.499598f, 10.05852f,
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
    .stride = 1,
  },
  // node_cube_translation_y_output
  .output = {
    .float_array = array_node_cube_translation_y_output_array,
    .count = 4,
    .stride = 1,
  },
  // node_cube_translation_y_intangent
  .in_tangent = {
    .float_array = array_node_cube_translation_y_intangent_array,
    .count = 4,
    .stride = 2,
  },
  // node_cube_translation_y_outtangent
  .out_tangent = {
    .float_array = array_node_cube_translation_y_outtangent_array,
    .count = 4,
    .stride = 2,
  },
  // node_cube_translation_y_interpolation
  .interpolation = {
    .interpolation_array = array_node_cube_translation_y_interpolation_array,
    .count = 4,
    .stride = 1,
  },
};

float const array_node_torus001_rotationx_angle_input_array[] = {
  0.0f,
  0.8333334f,
  1.666667f,
  2.5f,
  3.333333f,
};

float const array_node_torus001_rotationx_angle_output_array[] = {
  0.0f,
  -90.0f,
  -180.0f,
  -270.0f,
  -360.0f,
};

float const array_node_torus001_rotationx_angle_intangent_array[] = {
  0.9997917f, 0.0f,
  0.3270486f, -89.70257f,
  0.9961964f, -179.1937f,
  1.990403f, -269.8432f,
  2.6534f, -360.0f,
};

float const array_node_torus001_rotationx_angle_outtangent_array[] = {
  0.6602973f, 0.0f,
  1.324714f, -90.28867f,
  2.332406f, -180.8006f,
  3.004865f, -270.1553f,
  2.333542f, -360.0f,
};

enum interpolation const array_node_torus001_rotationx_angle_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_torus001_rotationx_angle_sampler = {
  // node_torus001_rotationx_angle_input
  .input = {
    .float_array = array_node_torus001_rotationx_angle_input_array,
    .count = 5,
    .stride = 1,
  },
  // node_torus001_rotationx_angle_output
  .output = {
    .float_array = array_node_torus001_rotationx_angle_output_array,
    .count = 5,
    .stride = 1,
  },
  // node_torus001_rotationx_angle_intangent
  .in_tangent = {
    .float_array = array_node_torus001_rotationx_angle_intangent_array,
    .count = 5,
    .stride = 2,
  },
  // node_torus001_rotationx_angle_outtangent
  .out_tangent = {
    .float_array = array_node_torus001_rotationx_angle_outtangent_array,
    .count = 5,
    .stride = 2,
  },
  // node_torus001_rotationx_angle_interpolation
  .interpolation = {
    .interpolation_array = array_node_torus001_rotationx_angle_interpolation_array,
    .count = 5,
    .stride = 1,
  },
};

float const array_node_geosphere001_scale_input_array[] = {
  0.0f,
  1.666667f,
  3.333333f,
};

float const array_node_geosphere001_scale_output_array[] = {
  1.0f, 1.0f, 1.0f,
  1.996525f, 1.996525f, 1.996525f,
  1.0f, 1.0f, 1.0f,
};

float const array_node_geosphere001_scale_intangent_array[] = {
  0.9997917f, 1.0f, 0.9997917f, 1.0f, 0.9997917f, 1.0f,
  1.111167f, 1.996525f, 1.111167f, 1.996525f, 1.111167f, 1.996525f,
  2.777833f, 1.0f, 2.777833f, 1.0f, 2.777833f, 1.0f,
};

float const array_node_geosphere001_scale_outtangent_array[] = {
  0.5555f, 1.0f, 0.5555f, 1.0f, 0.5555f, 1.0f,
  2.222167f, 1.996525f, 2.222167f, 1.996525f, 2.222167f, 1.996525f,
  2.333542f, 1.0f, 2.333542f, 1.0f, 2.333542f, 1.0f,
};

enum interpolation const array_node_geosphere001_scale_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_geosphere001_scale_sampler = {
  // node_geosphere001_scale_input
  .input = {
    .float_array = array_node_geosphere001_scale_input_array,
    .count = 3,
    .stride = 1,
  },
  // node_geosphere001_scale_output
  .output = {
    .float_array = array_node_geosphere001_scale_output_array,
    .count = 3,
    .stride = 3,
  },
  // node_geosphere001_scale_intangent
  .in_tangent = {
    .float_array = array_node_geosphere001_scale_intangent_array,
    .count = 3,
    .stride = 6,
  },
  // node_geosphere001_scale_outtangent
  .out_tangent = {
    .float_array = array_node_geosphere001_scale_outtangent_array,
    .count = 3,
    .stride = 6,
  },
  // node_geosphere001_scale_interpolation
  .interpolation = {
    .interpolation_array = array_node_geosphere001_scale_interpolation_array,
    .count = 3,
    .stride = 1,
  },
};

float const array_node_geosphere001_inversescaleaxisrotation_input_array[] = {
  0.0f,
  1.666667f,
  3.333333f,
};

float const array_node_geosphere001_inversescaleaxisrotation_output_array[] = {
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
};

enum interpolation const array_node_geosphere001_inversescaleaxisrotation_interpolation_array[] = {
  interpolation::LINEAR,
  interpolation::LINEAR,
  interpolation::LINEAR,
};

sampler const sampler_node_geosphere001_inversescaleaxisrotation_sampler = {
  // node_geosphere001_inversescaleaxisrotation_input
  .input = {
    .float_array = array_node_geosphere001_inversescaleaxisrotation_input_array,
    .count = 3,
    .stride = 1,
  },
  // node_geosphere001_inversescaleaxisrotation_output
  .output = {
    .float_array = array_node_geosphere001_inversescaleaxisrotation_output_array,
    .count = 3,
    .stride = 4,
  },
  // node_geosphere001_inversescaleaxisrotation_interpolation
  .interpolation = {
    .interpolation_array = array_node_geosphere001_inversescaleaxisrotation_interpolation_array,
    .count = 3,
    .stride = 1,
  },
};

float const array_node_geosphere001_scaleaxisrotation_input_array[] = {
  0.0f,
  1.666667f,
  3.333333f,
};

float const array_node_geosphere001_scaleaxisrotation_output_array[] = {
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
};

enum interpolation const array_node_geosphere001_scaleaxisrotation_interpolation_array[] = {
  interpolation::LINEAR,
  interpolation::LINEAR,
  interpolation::LINEAR,
};

sampler const sampler_node_geosphere001_scaleaxisrotation_sampler = {
  // node_geosphere001_scaleaxisrotation_input
  .input = {
    .float_array = array_node_geosphere001_scaleaxisrotation_input_array,
    .count = 3,
    .stride = 1,
  },
  // node_geosphere001_scaleaxisrotation_output
  .output = {
    .float_array = array_node_geosphere001_scaleaxisrotation_output_array,
    .count = 3,
    .stride = 4,
  },
  // node_geosphere001_scaleaxisrotation_interpolation
  .interpolation = {
    .interpolation_array = array_node_geosphere001_scaleaxisrotation_interpolation_array,
    .count = 3,
    .stride = 1,
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

channel const node_channel_node_torus001_rotationx_angle = {
  .source_sampler = &sampler_node_torus001_rotationx_angle_sampler,
  .target_transform_index = 3,
  .target_attribute = target_attribute::ANGLE,
};

channel const node_channel_node_geosphere001_scale = {
  .source_sampler = &sampler_node_geosphere001_scale_sampler,
  .target_transform_index = 2,
  .target_attribute = target_attribute::ALL,
};

channel const node_channel_node_geosphere001_inversescaleaxisrotation = {
  .source_sampler = &sampler_node_geosphere001_inversescaleaxisrotation_sampler,
  .target_transform_index = 1,
  .target_attribute = target_attribute::ALL,
};

channel const node_channel_node_geosphere001_scaleaxisrotation = {
  .source_sampler = &sampler_node_geosphere001_scaleaxisrotation_sampler,
  .target_transform_index = 3,
  .target_attribute = target_attribute::ALL,
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

effect const effect_coloreffectr26g177b26 = {
  .type = effect_type::PHONG,
  .phong = {
    .emission = {
      .color = {0.0f, 0.0f, 0.0f, 0.0f},
    },
    .ambient = {
      .color = {0.1019608f, 0.6941176f, 0.1019608f, 1.0f},
    },
    .diffuse = {
      .color = {0.1019608f, 0.6941176f, 0.1019608f, 1.0f},
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

effect const effect_coloreffectr198g224b87 = {
  .type = effect_type::PHONG,
  .phong = {
    .emission = {
      .color = {0.0f, 0.0f, 0.0f, 0.0f},
    },
    .ambient = {
      .color = {0.7764706f, 0.8784314f, 0.3411765f, 1.0f},
    },
    .diffuse = {
      .color = {0.7764706f, 0.8784314f, 0.3411765f, 1.0f},
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

material const material_coloreffectr26g177b26_material = {
  .effect = &effect_coloreffectr26g177b26,
};

material const material_coloreffectr229g154b215_material = {
  .effect = &effect_coloreffectr229g154b215,
};

material const material_coloreffectr28g149b177_material = {
  .effect = &effect_coloreffectr28g149b177,
};

material const material_coloreffectr198g224b87_material = {
  .effect = &effect_coloreffectr198g224b87,
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

triangles const triangles_geom_torus001[] = {
  {
    .count = 240, // triangles
    .index_offset = 0, // indices
    .inputs_index = 0, // index into inputs_list
  },
};

geometry const geometry_geom_torus001 = {
  .mesh = {
    .triangles = triangles_geom_torus001,
    .triangles_count = 1,

    .vertex_buffer_offset = 864,
    .vertex_buffer_size = 5148,

    .index_buffer_offset = 144,
    .index_buffer_size = 2880,
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

    .vertex_buffer_offset = 6012,
    .vertex_buffer_size = 1152,

    .index_buffer_offset = 3024,
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

    .vertex_buffer_offset = 7164,
    .vertex_buffer_size = 144,

    .index_buffer_offset = 3384,
    .index_buffer_size = 24,
  }
};

triangles const triangles_geom_geosphere001[] = {
  {
    .count = 40, // triangles
    .index_offset = 0, // indices
    .inputs_index = 0, // index into inputs_list
  },
};

geometry const geometry_geom_geosphere001 = {
  .mesh = {
    .triangles = triangles_geom_geosphere001,
    .triangles_count = 1,

    .vertex_buffer_offset = 7308,
    .vertex_buffer_size = 4320,

    .index_buffer_offset = 3408,
    .index_buffer_size = 480,
  }
};

geometry const * const geometries[] = {
  &geometry_geom_cube,
  &geometry_geom_torus001,
  &geometry_geom_cylinder001,
  &geometry_geom_plane001,
  &geometry_geom_geosphere001,
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
    .element_index = 4, // an index into mesh.triangles
    .material = &material_material__20_material,
  },
  {
    .element_index = 3, // an index into mesh.triangles
    .material = &material_material__18_material,
  },
  {
    .element_index = 2, // an index into mesh.triangles
    .material = &material_material__19_material,
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

transform const transforms_node_torus001[] = {
  {
    .type = transform_type::TRANSLATE,
    .translate = {5.0f, 1.14258e-07f, 2.0f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {0.0f, 0.0f, 1.0f, 0.0f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {0.0f, 1.0f, 0.0f, 2.00358e-05f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {1.0f, 0.0f, 0.0f, 0.0f},
  },
  {
    .type = transform_type::SCALE,
    .scale = {0.5f, 0.5f, 0.5f},
  },
};

instance_material const instance_materials_node_torus001_0[] = {
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_coloreffectr26g177b26_material,
  },
};

instance_geometry const instance_geometries_node_torus001[] = {
  {
    .geometry = &geometry_geom_torus001,
    .instance_materials = instance_materials_node_torus001_0,
    .instance_materials_count = 1,
  },
};

channel const * const node_channels_node_torus001[] = {
  &node_channel_node_torus001_rotationx_angle,
};

node const node_node_torus001 = {
  .parent_index = 1,

  .type = node_type::NODE,

  .transforms = transforms_node_torus001,
  .transforms_count = 5,

  .instance_geometries = instance_geometries_node_torus001,
  .instance_geometries_count = 1,

  .channels = node_channels_node_torus001,
  .channels_count = 1,
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

transform const transforms_node_geosphere001[] = {
  {
    .type = transform_type::TRANSLATE,
    .translate = {0.03164387f, -0.03838623f, 0.0f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {0.0f, 0.0f, 0.0f, 0.0f},
  },
  {
    .type = transform_type::SCALE,
    .scale = {1.0f, 1.0f, 1.0f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {0.0f, 0.0f, 0.0f, 0.0f},
  },
};

instance_material const instance_materials_node_geosphere001_0[] = {
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_coloreffectr198g224b87_material,
  },
};

instance_geometry const instance_geometries_node_geosphere001[] = {
  {
    .geometry = &geometry_geom_geosphere001,
    .instance_materials = instance_materials_node_geosphere001_0,
    .instance_materials_count = 1,
  },
};

channel const * const node_channels_node_geosphere001[] = {
  &node_channel_node_geosphere001_inversescaleaxisrotation,
  &node_channel_node_geosphere001_scale,
  &node_channel_node_geosphere001_scaleaxisrotation,
};

node const node_node_geosphere001 = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_geosphere001,
  .transforms_count = 4,

  .instance_geometries = instance_geometries_node_geosphere001,
  .instance_geometries_count = 1,

  .channels = node_channels_node_geosphere001,
  .channels_count = 3,
};

node const * const nodes[] = {
  &node_node_environmentambientlight,
  &node_node_cube,
  &node_node_torus001,
  &node_node_cylinder001,
  &node_node_plane001,
  &node_node_geosphere001,
};

inputs const inputs_list[] = {
  {
    .elements = input_elements_position_0_3_normal_0_3_texcoord_0_3,
    .elements_count = 3,
  },
};

extern collada::descriptor const descriptor;

collada::descriptor const descriptor = {
  .nodes = nodes,
  .nodes_count = (sizeof (nodes)) / (sizeof (nodes[0])),

  .inputs_list = inputs_list,
  .inputs_list_count = (sizeof (inputs_list)) / (sizeof (inputs_list[0])),
};

}
