#include "collada_types.hpp"

namespace curve_interpolation {

using namespace collada;

light const light_environmentambientlight = {
  .type = light_type::AMBIENT,
  .color = { 0.0f, 0.0f, 0.0f },
};

light const light_light_light = {
  .type = light_type::POINT,
  .color = { 1.0f, 1.0f, 1.0f },
};

float const array_node_cube_translation_x_input_array[] = {
  0.0f,
  1.666667f,
  3.333333f,
  5.0f,
};

float const array_node_cube_translation_x_output_array[] = {
  34.88703f,
  -34.88703f,
  34.88703f,
  -34.88703f,
};

float const array_node_cube_translation_x_intangent_array[] = {
  -0.3332306f, 34.88703f,
  1.111167f, -34.88703f,
  2.778333f, 34.88703f,
  4.4323f, -34.88086f,
};

float const array_node_cube_translation_x_outtangent_array[] = {
  0.5555f, 34.88703f,
  2.222167f, -34.88703f,
  3.888333f, 34.88703f,
  4.000208f, -29.96252f,
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
  -35.08854f,
  35.09117f,
  -34.68288f,
  35.09117f,
};

float const array_node_cube_translation_y_intangent_array[] = {
  -1.166264f, -35.08854f,
  0.2778334f, 35.09117f,
  1.9445f, -34.68288f,
  3.611667f, 35.09117f,
};

float const array_node_cube_translation_y_outtangent_array[] = {
  -0.2783333f, -35.08854f,
  1.388833f, 35.09117f,
  3.0555f, -34.68288f,
  4.499598f, 35.09117f,
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

float const array_node_torus_rotationx_angle_input_array[] = {
  0.0f,
  0.8333334f,
  1.666667f,
  2.5f,
  3.333333f,
};

float const array_node_torus_rotationx_angle_output_array[] = {
  0.0f,
  -90.0f,
  -180.0f,
  -270.0f,
  -360.0f,
};

float const array_node_torus_rotationx_angle_intangent_array[] = {
  0.9997917f, 0.0f,
  0.3270486f, -89.70257f,
  0.9961964f, -179.1937f,
  1.990403f, -269.8432f,
  2.6534f, -360.0f,
};

float const array_node_torus_rotationx_angle_outtangent_array[] = {
  0.6602973f, 0.0f,
  1.324714f, -90.28867f,
  2.332406f, -180.8006f,
  3.004865f, -270.1553f,
  2.333542f, -360.0f,
};

enum interpolation const array_node_torus_rotationx_angle_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_torus_rotationx_angle_sampler = {
  // node_torus_rotationx_angle_input
  .input = {
    .float_array = array_node_torus_rotationx_angle_input_array,
    .count = 5,
    .stride = 1,
  },
  // node_torus_rotationx_angle_output
  .output = {
    .float_array = array_node_torus_rotationx_angle_output_array,
    .count = 5,
    .stride = 1,
  },
  // node_torus_rotationx_angle_intangent
  .in_tangent = {
    .float_array = array_node_torus_rotationx_angle_intangent_array,
    .count = 5,
    .stride = 2,
  },
  // node_torus_rotationx_angle_outtangent
  .out_tangent = {
    .float_array = array_node_torus_rotationx_angle_outtangent_array,
    .count = 5,
    .stride = 2,
  },
  // node_torus_rotationx_angle_interpolation
  .interpolation = {
    .interpolation_array = array_node_torus_rotationx_angle_interpolation_array,
    .count = 5,
    .stride = 1,
  },
};

float const array_node_geosphere_scale_input_array[] = {
  0.0f,
  1.666667f,
  3.333333f,
};

float const array_node_geosphere_scale_output_array[] = {
  3.523671f, 3.523671f, 3.523671f,
  7.035098f, 7.035098f, 7.035098f,
  3.523671f, 3.523671f, 3.523671f,
};

float const array_node_geosphere_scale_intangent_array[] = {
  0.9997917f, 3.523671f, 0.9997917f, 3.523671f, 0.9997917f, 3.523671f,
  1.111167f, 7.035098f, 1.111167f, 7.035098f, 1.111167f, 7.035098f,
  2.777833f, 3.523671f, 2.777833f, 3.523671f, 2.777833f, 3.523671f,
};

float const array_node_geosphere_scale_outtangent_array[] = {
  0.5555f, 3.523671f, 0.5555f, 3.523671f, 0.5555f, 3.523671f,
  2.222167f, 7.035098f, 2.222167f, 7.035098f, 2.222167f, 7.035098f,
  2.333542f, 3.523671f, 2.333542f, 3.523671f, 2.333542f, 3.523671f,
};

enum interpolation const array_node_geosphere_scale_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_geosphere_scale_sampler = {
  // node_geosphere_scale_input
  .input = {
    .float_array = array_node_geosphere_scale_input_array,
    .count = 3,
    .stride = 1,
  },
  // node_geosphere_scale_output
  .output = {
    .float_array = array_node_geosphere_scale_output_array,
    .count = 3,
    .stride = 3,
  },
  // node_geosphere_scale_intangent
  .in_tangent = {
    .float_array = array_node_geosphere_scale_intangent_array,
    .count = 3,
    .stride = 6,
  },
  // node_geosphere_scale_outtangent
  .out_tangent = {
    .float_array = array_node_geosphere_scale_outtangent_array,
    .count = 3,
    .stride = 6,
  },
  // node_geosphere_scale_interpolation
  .interpolation = {
    .interpolation_array = array_node_geosphere_scale_interpolation_array,
    .count = 3,
    .stride = 1,
  },
};

float const array_node_geosphere_inversescaleaxisrotation_input_array[] = {
  0.0f,
  1.666667f,
  3.333333f,
};

float const array_node_geosphere_inversescaleaxisrotation_output_array[] = {
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
};

enum interpolation const array_node_geosphere_inversescaleaxisrotation_interpolation_array[] = {
  interpolation::LINEAR,
  interpolation::LINEAR,
  interpolation::LINEAR,
};

sampler const sampler_node_geosphere_inversescaleaxisrotation_sampler = {
  // node_geosphere_inversescaleaxisrotation_input
  .input = {
    .float_array = array_node_geosphere_inversescaleaxisrotation_input_array,
    .count = 3,
    .stride = 1,
  },
  // node_geosphere_inversescaleaxisrotation_output
  .output = {
    .float_array = array_node_geosphere_inversescaleaxisrotation_output_array,
    .count = 3,
    .stride = 4,
  },
  // node_geosphere_inversescaleaxisrotation_interpolation
  .interpolation = {
    .interpolation_array = array_node_geosphere_inversescaleaxisrotation_interpolation_array,
    .count = 3,
    .stride = 1,
  },
};

float const array_node_geosphere_scaleaxisrotation_input_array[] = {
  0.0f,
  1.666667f,
  3.333333f,
};

float const array_node_geosphere_scaleaxisrotation_output_array[] = {
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
};

enum interpolation const array_node_geosphere_scaleaxisrotation_interpolation_array[] = {
  interpolation::LINEAR,
  interpolation::LINEAR,
  interpolation::LINEAR,
};

sampler const sampler_node_geosphere_scaleaxisrotation_sampler = {
  // node_geosphere_scaleaxisrotation_input
  .input = {
    .float_array = array_node_geosphere_scaleaxisrotation_input_array,
    .count = 3,
    .stride = 1,
  },
  // node_geosphere_scaleaxisrotation_output
  .output = {
    .float_array = array_node_geosphere_scaleaxisrotation_output_array,
    .count = 3,
    .stride = 4,
  },
  // node_geosphere_scaleaxisrotation_interpolation
  .interpolation = {
    .interpolation_array = array_node_geosphere_scaleaxisrotation_interpolation_array,
    .count = 3,
    .stride = 1,
  },
};

float const array_node_light_translation_x_input_array[] = {
  0.0f,
  0.5f,
  1.0f,
  1.5f,
  2.0f,
  2.833333f,
  3.333333f,
};

float const array_node_light_translation_x_output_array[] = {
  24.0f,
  -35.0f,
  -35.0f,
  5.0f,
  5.0f,
  24.0f,
  24.0f,
};

float const array_node_light_translation_x_intangent_array[] = {
  0.9997917f, 24.0f,
  0.33335f, -35.0f,
  0.83335f, -35.0f,
  1.33335f, 5.0f,
  1.83335f, 5.0f,
  2.555583f, 24.0f,
  3.166683f, 24.0f,
};

float const array_node_light_translation_x_outtangent_array[] = {
  0.16665f, 24.0f,
  0.66665f, -35.0f,
  1.16665f, -35.0f,
  1.66665f, 5.0f,
  2.27775f, 5.0f,
  2.999983f, 24.0f,
  3.666564f, 24.0f,
};

enum interpolation const array_node_light_translation_x_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_light_translation_x_sampler = {
  // node_light_translation_x_input
  .input = {
    .float_array = array_node_light_translation_x_input_array,
    .count = 7,
    .stride = 1,
  },
  // node_light_translation_x_output
  .output = {
    .float_array = array_node_light_translation_x_output_array,
    .count = 7,
    .stride = 1,
  },
  // node_light_translation_x_intangent
  .in_tangent = {
    .float_array = array_node_light_translation_x_intangent_array,
    .count = 7,
    .stride = 2,
  },
  // node_light_translation_x_outtangent
  .out_tangent = {
    .float_array = array_node_light_translation_x_outtangent_array,
    .count = 7,
    .stride = 2,
  },
  // node_light_translation_x_interpolation
  .interpolation = {
    .interpolation_array = array_node_light_translation_x_interpolation_array,
    .count = 7,
    .stride = 1,
  },
};

float const array_node_light_translation_y_input_array[] = {
  0.0f,
  0.5f,
  1.0f,
  1.5f,
  2.0f,
  2.833333f,
  3.333333f,
};

float const array_node_light_translation_y_output_array[] = {
  25.52443f,
  2.0f,
  2.0f,
  -20.0f,
  -20.0f,
  25.52443f,
  25.52443f,
};

float const array_node_light_translation_y_intangent_array[] = {
  0.9997917f, 25.52443f,
  0.33335f, 2.0f,
  0.83335f, 2.0f,
  1.33335f, -20.0f,
  1.83335f, -20.0f,
  2.555583f, 25.52443f,
  3.166683f, 25.52443f,
};

float const array_node_light_translation_y_outtangent_array[] = {
  0.16665f, 25.52443f,
  0.66665f, 2.0f,
  1.16665f, 2.0f,
  1.66665f, -20.0f,
  2.27775f, -20.0f,
  2.999983f, 25.52443f,
  3.666564f, 25.52443f,
};

enum interpolation const array_node_light_translation_y_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_light_translation_y_sampler = {
  // node_light_translation_y_input
  .input = {
    .float_array = array_node_light_translation_y_input_array,
    .count = 7,
    .stride = 1,
  },
  // node_light_translation_y_output
  .output = {
    .float_array = array_node_light_translation_y_output_array,
    .count = 7,
    .stride = 1,
  },
  // node_light_translation_y_intangent
  .in_tangent = {
    .float_array = array_node_light_translation_y_intangent_array,
    .count = 7,
    .stride = 2,
  },
  // node_light_translation_y_outtangent
  .out_tangent = {
    .float_array = array_node_light_translation_y_outtangent_array,
    .count = 7,
    .stride = 2,
  },
  // node_light_translation_y_interpolation
  .interpolation = {
    .interpolation_array = array_node_light_translation_y_interpolation_array,
    .count = 7,
    .stride = 1,
  },
};

float const array_node_bone002_rotationz_angle_input_array[] = {
  0.0f,
  0.4333333f,
  0.8333334f,
  1.266667f,
  1.666667f,
  2.1f,
  2.5f,
  2.933333f,
  3.333333f,
};

float const array_node_bone002_rotationz_angle_output_array[] = {
  180.0f,
  230.0f,
  180.0f,
  130.0f,
  180.0f,
  230.0f,
  180.0f,
  130.0f,
  180.0f,
};

float const array_node_bone002_rotationz_angle_intangent_array[] = {
  -0.3332639f, 180.0f,
  0.2888889f, 213.3333f,
  0.7f, 196.6667f,
  1.122222f, 146.6667f,
  1.533333f, 163.3333f,
  1.955556f, 213.3333f,
  2.366667f, 196.6667f,
  2.788889f, 146.6667f,
  3.2f, 163.3333f,
};

float const array_node_bone002_rotationz_angle_outtangent_array[] = {
  0.1444445f, 196.6667f,
  0.5666667f, 213.3333f,
  0.9777778f, 163.3333f,
  1.4f, 146.6667f,
  1.811111f, 196.6667f,
  2.233333f, 213.3333f,
  2.644444f, 163.3333f,
  3.066667f, 146.6667f,
  3.666597f, 180.0f,
};

enum interpolation const array_node_bone002_rotationz_angle_interpolation_array[] = {
  interpolation::LINEAR,
  interpolation::LINEAR,
  interpolation::LINEAR,
  interpolation::LINEAR,
  interpolation::LINEAR,
  interpolation::LINEAR,
  interpolation::LINEAR,
  interpolation::LINEAR,
  interpolation::LINEAR,
};

sampler const sampler_node_bone002_rotationz_angle_sampler = {
  // node_bone002_rotationz_angle_input
  .input = {
    .float_array = array_node_bone002_rotationz_angle_input_array,
    .count = 9,
    .stride = 1,
  },
  // node_bone002_rotationz_angle_output
  .output = {
    .float_array = array_node_bone002_rotationz_angle_output_array,
    .count = 9,
    .stride = 1,
  },
  // node_bone002_rotationz_angle_intangent
  .in_tangent = {
    .float_array = array_node_bone002_rotationz_angle_intangent_array,
    .count = 9,
    .stride = 2,
  },
  // node_bone002_rotationz_angle_outtangent
  .out_tangent = {
    .float_array = array_node_bone002_rotationz_angle_outtangent_array,
    .count = 9,
    .stride = 2,
  },
  // node_bone002_rotationz_angle_interpolation
  .interpolation = {
    .interpolation_array = array_node_bone002_rotationz_angle_interpolation_array,
    .count = 9,
    .stride = 1,
  },
};

float const array_node_pyramid001_translation_x_input_array[] = {
  0.0f,
  0.6333333f,
  1.466667f,
  3.333333f,
};

float const array_node_pyramid001_translation_x_output_array[] = {
  10.76371f,
  10.76371f,
  10.76371f,
  10.76371f,
};

float const array_node_pyramid001_translation_x_intangent_array[] = {
  0.9997917f, 10.76371f,
  0.4222434f, 10.76371f,
  1.188917f, 10.76371f,
  2.711174f, 10.76371f,
};

float const array_node_pyramid001_translation_x_outtangent_array[] = {
  0.21109f, 10.76371f,
  0.9110833f, 10.76371f,
  2.088827f, 10.76371f,
  3.666564f, 10.76371f,
};

enum interpolation const array_node_pyramid001_translation_x_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_pyramid001_translation_x_sampler = {
  // node_pyramid001_translation_x_input
  .input = {
    .float_array = array_node_pyramid001_translation_x_input_array,
    .count = 4,
    .stride = 1,
  },
  // node_pyramid001_translation_x_output
  .output = {
    .float_array = array_node_pyramid001_translation_x_output_array,
    .count = 4,
    .stride = 1,
  },
  // node_pyramid001_translation_x_intangent
  .in_tangent = {
    .float_array = array_node_pyramid001_translation_x_intangent_array,
    .count = 4,
    .stride = 2,
  },
  // node_pyramid001_translation_x_outtangent
  .out_tangent = {
    .float_array = array_node_pyramid001_translation_x_outtangent_array,
    .count = 4,
    .stride = 2,
  },
  // node_pyramid001_translation_x_interpolation
  .interpolation = {
    .interpolation_array = array_node_pyramid001_translation_x_interpolation_array,
    .count = 4,
    .stride = 1,
  },
};

float const array_node_pyramid001_translation_y_input_array[] = {
  0.0f,
  0.6333333f,
  1.466667f,
  3.333333f,
};

float const array_node_pyramid001_translation_y_output_array[] = {
  -13.71933f,
  -13.71933f,
  11.03402f,
  -13.71933f,
};

float const array_node_pyramid001_translation_y_intangent_array[] = {
  0.9997917f, -13.71933f,
  0.4222434f, -13.71933f,
  1.188917f, 11.03402f,
  2.711174f, -13.71933f,
};

float const array_node_pyramid001_translation_y_outtangent_array[] = {
  0.21109f, -13.71933f,
  0.9110833f, -13.71933f,
  2.088827f, 11.03402f,
  3.666564f, -13.71933f,
};

enum interpolation const array_node_pyramid001_translation_y_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_pyramid001_translation_y_sampler = {
  // node_pyramid001_translation_y_input
  .input = {
    .float_array = array_node_pyramid001_translation_y_input_array,
    .count = 4,
    .stride = 1,
  },
  // node_pyramid001_translation_y_output
  .output = {
    .float_array = array_node_pyramid001_translation_y_output_array,
    .count = 4,
    .stride = 1,
  },
  // node_pyramid001_translation_y_intangent
  .in_tangent = {
    .float_array = array_node_pyramid001_translation_y_intangent_array,
    .count = 4,
    .stride = 2,
  },
  // node_pyramid001_translation_y_outtangent
  .out_tangent = {
    .float_array = array_node_pyramid001_translation_y_outtangent_array,
    .count = 4,
    .stride = 2,
  },
  // node_pyramid001_translation_y_interpolation
  .interpolation = {
    .interpolation_array = array_node_pyramid001_translation_y_interpolation_array,
    .count = 4,
    .stride = 1,
  },
};

float const array_node_pyramid001_translation_z_input_array[] = {
  0.0f,
  0.6333333f,
  1.466667f,
  3.333333f,
};

float const array_node_pyramid001_translation_z_output_array[] = {
  0.01108826f,
  17.80861f,
  17.80861f,
  0.01108826f,
};

float const array_node_pyramid001_translation_z_intangent_array[] = {
  0.9997917f, 0.01108826f,
  0.4222434f, 17.80861f,
  1.188917f, 17.80861f,
  2.711174f, 0.01108826f,
};

float const array_node_pyramid001_translation_z_outtangent_array[] = {
  0.21109f, 0.01108826f,
  0.9110833f, 17.80861f,
  2.088827f, 17.80861f,
  3.666564f, 0.01108826f,
};

enum interpolation const array_node_pyramid001_translation_z_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_pyramid001_translation_z_sampler = {
  // node_pyramid001_translation_z_input
  .input = {
    .float_array = array_node_pyramid001_translation_z_input_array,
    .count = 4,
    .stride = 1,
  },
  // node_pyramid001_translation_z_output
  .output = {
    .float_array = array_node_pyramid001_translation_z_output_array,
    .count = 4,
    .stride = 1,
  },
  // node_pyramid001_translation_z_intangent
  .in_tangent = {
    .float_array = array_node_pyramid001_translation_z_intangent_array,
    .count = 4,
    .stride = 2,
  },
  // node_pyramid001_translation_z_outtangent
  .out_tangent = {
    .float_array = array_node_pyramid001_translation_z_outtangent_array,
    .count = 4,
    .stride = 2,
  },
  // node_pyramid001_translation_z_interpolation
  .interpolation = {
    .interpolation_array = array_node_pyramid001_translation_z_interpolation_array,
    .count = 4,
    .stride = 1,
  },
};

float const array_node_pyramid001_rotationz_angle_input_array[] = {
  0.0f,
  1.466667f,
  2.366667f,
  2.8f,
  3.333333f,
};

float const array_node_pyramid001_rotationz_angle_output_array[] = {
  0.0f,
  0.0f,
  0.0f,
  0.0f,
  0.0f,
};

float const array_node_pyramid001_rotationz_angle_intangent_array[] = {
  0.9997917f, 0.0f,
  0.9778267f, 0.0f,
  2.066697f, 0.0f,
  2.65557f, 0.0f,
  3.155573f, 0.0f,
};

float const array_node_pyramid001_rotationz_angle_outtangent_array[] = {
  0.48884f, 0.0f,
  1.766637f, 0.0f,
  2.511097f, 0.0f,
  2.97776f, 0.0f,
  3.666564f, 0.0f,
};

enum interpolation const array_node_pyramid001_rotationz_angle_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_pyramid001_rotationz_angle_sampler = {
  // node_pyramid001_rotationz_angle_input
  .input = {
    .float_array = array_node_pyramid001_rotationz_angle_input_array,
    .count = 5,
    .stride = 1,
  },
  // node_pyramid001_rotationz_angle_output
  .output = {
    .float_array = array_node_pyramid001_rotationz_angle_output_array,
    .count = 5,
    .stride = 1,
  },
  // node_pyramid001_rotationz_angle_intangent
  .in_tangent = {
    .float_array = array_node_pyramid001_rotationz_angle_intangent_array,
    .count = 5,
    .stride = 2,
  },
  // node_pyramid001_rotationz_angle_outtangent
  .out_tangent = {
    .float_array = array_node_pyramid001_rotationz_angle_outtangent_array,
    .count = 5,
    .stride = 2,
  },
  // node_pyramid001_rotationz_angle_interpolation
  .interpolation = {
    .interpolation_array = array_node_pyramid001_rotationz_angle_interpolation_array,
    .count = 5,
    .stride = 1,
  },
};

float const array_node_pyramid001_rotationy_angle_input_array[] = {
  0.0f,
  1.466667f,
  2.366667f,
  2.8f,
  3.333333f,
};

float const array_node_pyramid001_rotationy_angle_output_array[] = {
  0.0f,
  0.0f,
  0.0f,
  0.0f,
  0.0f,
};

float const array_node_pyramid001_rotationy_angle_intangent_array[] = {
  0.9997917f, 0.0f,
  0.9778267f, 0.0f,
  2.066697f, 0.0f,
  2.65557f, 0.0f,
  3.155573f, 0.0f,
};

float const array_node_pyramid001_rotationy_angle_outtangent_array[] = {
  0.48884f, 0.0f,
  1.766637f, 0.0f,
  2.511097f, 0.0f,
  2.97776f, 0.0f,
  3.666564f, 0.0f,
};

enum interpolation const array_node_pyramid001_rotationy_angle_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_pyramid001_rotationy_angle_sampler = {
  // node_pyramid001_rotationy_angle_input
  .input = {
    .float_array = array_node_pyramid001_rotationy_angle_input_array,
    .count = 5,
    .stride = 1,
  },
  // node_pyramid001_rotationy_angle_output
  .output = {
    .float_array = array_node_pyramid001_rotationy_angle_output_array,
    .count = 5,
    .stride = 1,
  },
  // node_pyramid001_rotationy_angle_intangent
  .in_tangent = {
    .float_array = array_node_pyramid001_rotationy_angle_intangent_array,
    .count = 5,
    .stride = 2,
  },
  // node_pyramid001_rotationy_angle_outtangent
  .out_tangent = {
    .float_array = array_node_pyramid001_rotationy_angle_outtangent_array,
    .count = 5,
    .stride = 2,
  },
  // node_pyramid001_rotationy_angle_interpolation
  .interpolation = {
    .interpolation_array = array_node_pyramid001_rotationy_angle_interpolation_array,
    .count = 5,
    .stride = 1,
  },
};

float const array_node_pyramid001_rotationx_angle_input_array[] = {
  0.0f,
  1.466667f,
  2.366667f,
  2.8f,
  3.333333f,
};

float const array_node_pyramid001_rotationx_angle_output_array[] = {
  0.0f,
  -69.99999f,
  -175.0f,
  -235.0f,
  0.0f,
};

float const array_node_pyramid001_rotationx_angle_intangent_array[] = {
  0.9997917f, 0.0f,
  0.9778267f, -69.99999f,
  2.066697f, -175.0f,
  2.65557f, -235.0f,
  3.155573f, 0.0f,
};

float const array_node_pyramid001_rotationx_angle_outtangent_array[] = {
  0.48884f, 0.0f,
  1.766637f, -69.99999f,
  2.511097f, -175.0f,
  2.97776f, -235.0f,
  3.666564f, 0.0f,
};

enum interpolation const array_node_pyramid001_rotationx_angle_interpolation_array[] = {
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
  interpolation::BEZIER,
};

sampler const sampler_node_pyramid001_rotationx_angle_sampler = {
  // node_pyramid001_rotationx_angle_input
  .input = {
    .float_array = array_node_pyramid001_rotationx_angle_input_array,
    .count = 5,
    .stride = 1,
  },
  // node_pyramid001_rotationx_angle_output
  .output = {
    .float_array = array_node_pyramid001_rotationx_angle_output_array,
    .count = 5,
    .stride = 1,
  },
  // node_pyramid001_rotationx_angle_intangent
  .in_tangent = {
    .float_array = array_node_pyramid001_rotationx_angle_intangent_array,
    .count = 5,
    .stride = 2,
  },
  // node_pyramid001_rotationx_angle_outtangent
  .out_tangent = {
    .float_array = array_node_pyramid001_rotationx_angle_outtangent_array,
    .count = 5,
    .stride = 2,
  },
  // node_pyramid001_rotationx_angle_interpolation
  .interpolation = {
    .interpolation_array = array_node_pyramid001_rotationx_angle_interpolation_array,
    .count = 5,
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

channel const node_channel_node_torus_rotationx_angle = {
  .source_sampler = &sampler_node_torus_rotationx_angle_sampler,
  .target_transform_index = 3,
  .target_attribute = target_attribute::ANGLE,
};

channel const node_channel_node_geosphere_scale = {
  .source_sampler = &sampler_node_geosphere_scale_sampler,
  .target_transform_index = 2,
  .target_attribute = target_attribute::ALL,
};

channel const node_channel_node_geosphere_inversescaleaxisrotation = {
  .source_sampler = &sampler_node_geosphere_inversescaleaxisrotation_sampler,
  .target_transform_index = 1,
  .target_attribute = target_attribute::ALL,
};

channel const node_channel_node_geosphere_scaleaxisrotation = {
  .source_sampler = &sampler_node_geosphere_scaleaxisrotation_sampler,
  .target_transform_index = 3,
  .target_attribute = target_attribute::ALL,
};

channel const node_channel_node_light_translation_x = {
  .source_sampler = &sampler_node_light_translation_x_sampler,
  .target_transform_index = 0,
  .target_attribute = target_attribute::X,
};

channel const node_channel_node_light_translation_y = {
  .source_sampler = &sampler_node_light_translation_y_sampler,
  .target_transform_index = 0,
  .target_attribute = target_attribute::Y,
};

channel const node_channel_node_bone002_rotationz_angle = {
  .source_sampler = &sampler_node_bone002_rotationz_angle_sampler,
  .target_transform_index = 1,
  .target_attribute = target_attribute::ANGLE,
};

channel const node_channel_node_pyramid001_translation_x = {
  .source_sampler = &sampler_node_pyramid001_translation_x_sampler,
  .target_transform_index = 0,
  .target_attribute = target_attribute::X,
};

channel const node_channel_node_pyramid001_translation_y = {
  .source_sampler = &sampler_node_pyramid001_translation_y_sampler,
  .target_transform_index = 0,
  .target_attribute = target_attribute::Y,
};

channel const node_channel_node_pyramid001_translation_z = {
  .source_sampler = &sampler_node_pyramid001_translation_z_sampler,
  .target_transform_index = 0,
  .target_attribute = target_attribute::Z,
};

channel const node_channel_node_pyramid001_rotationz_angle = {
  .source_sampler = &sampler_node_pyramid001_rotationz_angle_sampler,
  .target_transform_index = 1,
  .target_attribute = target_attribute::ANGLE,
};

channel const node_channel_node_pyramid001_rotationy_angle = {
  .source_sampler = &sampler_node_pyramid001_rotationy_angle_sampler,
  .target_transform_index = 2,
  .target_attribute = target_attribute::ANGLE,
};

channel const node_channel_node_pyramid001_rotationx_angle = {
  .source_sampler = &sampler_node_pyramid001_rotationx_angle_sampler,
  .target_transform_index = 3,
  .target_attribute = target_attribute::ANGLE,
};

// american_cherry_png
image const image_american_cherry_png = {
  .resource_name = L"_AMERICAN_CHERRY_PNG",
};

// Masonry_Unit_Masonry_Glass_Block_Square_Stack_jpg
image const image_masonry_unit_masonry_glass_block_square_stack_jpg = {
  .resource_name = L"_MASONRY_UNIT_MASONRY_GLASS_BLOCK_SQUARE_STACK_JPG",
};

// _02_png
image const image__02_png = {
  .resource_name = L"_102_PNG",
};

// Finishes_Flooring_Tile_Square_Medium_Blue_png
image const image_finishes_flooring_tile_square_medium_blue_png = {
  .resource_name = L"_FINISHES_FLOORING_TILE_SQUARE_MEDIUM_BLUE_PNG",
};

// SiteWork_Planting_Grass_Bermuda1_jpg
image const image_sitework_planting_grass_bermuda1_jpg = {
  .resource_name = L"_SITEWORK_PLANTING_GRASS_BERMUDA1_JPG",
};

image const * const images[] = {
  &image_american_cherry_png,
  &image_masonry_unit_masonry_glass_block_square_stack_jpg,
  &image__02_png,
  &image_finishes_flooring_tile_square_medium_blue_png,
  &image_sitework_planting_grass_bermuda1_jpg,
};

effect const effect_material__15 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.6705883f, 0.5843138f, 1.0f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {0.6705883f, 0.5843138f, 1.0f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
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
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.5803922f, 1.0f, 0.9647059f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {0.5803922f, 1.0f, 0.9647059f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
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
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.6509804f, 1.0f, 0.5803922f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {0.6509804f, 1.0f, 0.5803922f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
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
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.9333334f, 1.0f, 0.5647059f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {0.9333334f, 1.0f, 0.5647059f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
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
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 0.7686275f, 0.5803922f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 0.7686275f, 0.5803922f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
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
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 0.5803922f, 0.5803922f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 0.5803922f, 0.5803922f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
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
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 0.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.1019608f, 0.6941176f, 0.1019608f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {0.1019608f, 0.6941176f, 0.1019608f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_grass = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.588f, 0.588f, 0.588f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::TEXTURE,
      .texture = { .image_index = 4 }, // SiteWork_Planting_Grass_Bermuda1_jpg
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_wood = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.588f, 0.588f, 0.588f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::TEXTURE,
      .texture = { .image_index = 0 }, // american_cherry_png
    },
    .specular = {
      .type = color_or_texture_type::TEXTURE,
      .texture = { .image_index = 3 }, // Finishes_Flooring_Tile_Square_Medium_Blue_png
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
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
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 0.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.7764706f, 0.8784314f, 0.3411765f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {0.7764706f, 0.8784314f, 0.3411765f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_lightemit = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_material__13 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.5882353f, 0.5882353f, 1.0f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {0.5882353f, 0.5882353f, 1.0f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_material__14 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.5882353f, 0.9450981f, 1.0f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {0.5882353f, 0.9450981f, 1.0f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_material__15_1 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.5882353f, 1.0f, 0.6156863f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {0.5882353f, 1.0f, 0.6156863f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_material__16_1 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.9960785f, 1.0f, 0.5882353f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {0.9960785f, 1.0f, 0.5882353f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_material__17_1 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.9960785f, 0.8196079f, 0.5882353f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {0.9960785f, 0.8196079f, 0.5882353f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_material__18_1 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 0.5882353f, 0.5882353f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 0.5882353f, 0.5882353f, 1.0f},
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

effect const effect_material__21 = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.588f, 0.588f, 0.588f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::TEXTURE,
      .texture = { .image_index = 1 }, // Masonry_Unit_Masonry_Glass_Block_Square_Stack_jpg
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .shininess = 10.0f,
    .reflective = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.0f, 0.0f, 1.0f},
    },
    .reflectivity = 0.0f,
    .transparent = {
      .type = color_or_texture_type::COLOR,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
    },
    .transparency = 1.0f,
    .index_of_refraction = 0.0f,
  }
};

material const material_coloreffectr26g177b26_material = {
  .effect = &effect_coloreffectr26g177b26,
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

material const material_grass_material = {
  .effect = &effect_grass,
};

material const material_wood_material = {
  .effect = &effect_wood,
};

material const material_lightemit_material = {
  .effect = &effect_lightemit,
};

material const material_material__13_material = {
  .effect = &effect_material__13,
};

material const material_material__14_material = {
  .effect = &effect_material__14,
};

material const material_material__15_1_material = {
  .effect = &effect_material__15_1,
};

material const material_material__16_1_material = {
  .effect = &effect_material__16_1,
};

material const material_material__17_1_material = {
  .effect = &effect_material__17_1,
};

material const material_material__18_1_material = {
  .effect = &effect_material__18_1,
};

material const material_material__21_material = {
  .effect = &effect_material__21,
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

triangles const triangles_geom_torus[] = {
  {
    .count = 240, // triangles
    .index_offset = 0, // indices
    .inputs_index = 0, // index into inputs_list
  },
};

geometry const geometry_geom_torus = {
  .mesh = {
    .triangles = triangles_geom_torus,
    .triangles_count = 1,

    .vertex_buffer_offset = 864,
    .vertex_buffer_size = 5148,

    .index_buffer_offset = 144,
    .index_buffer_size = 2880,
  }
};

triangles const triangles_geom_cylinder[] = {
  {
    .count = 30, // triangles
    .index_offset = 0, // indices
    .inputs_index = 0, // index into inputs_list
  },
};

geometry const geometry_geom_cylinder = {
  .mesh = {
    .triangles = triangles_geom_cylinder,
    .triangles_count = 1,

    .vertex_buffer_offset = 6012,
    .vertex_buffer_size = 1152,

    .index_buffer_offset = 3024,
    .index_buffer_size = 360,
  }
};

triangles const triangles_geom_plane[] = {
  {
    .count = 2, // triangles
    .index_offset = 0, // indices
    .inputs_index = 0, // index into inputs_list
  },
};

geometry const geometry_geom_plane = {
  .mesh = {
    .triangles = triangles_geom_plane,
    .triangles_count = 1,

    .vertex_buffer_offset = 7164,
    .vertex_buffer_size = 144,

    .index_buffer_offset = 3384,
    .index_buffer_size = 24,
  }
};

triangles const triangles_geom_geosphere[] = {
  {
    .count = 40, // triangles
    .index_offset = 0, // indices
    .inputs_index = 0, // index into inputs_list
  },
};

geometry const geometry_geom_geosphere = {
  .mesh = {
    .triangles = triangles_geom_geosphere,
    .triangles_count = 1,

    .vertex_buffer_offset = 7308,
    .vertex_buffer_size = 4320,

    .index_buffer_offset = 3408,
    .index_buffer_size = 480,
  }
};

triangles const triangles_geom_lightindicator[] = {
  {
    .count = 8, // triangles
    .index_offset = 0, // indices
    .inputs_index = 0, // index into inputs_list
  },
};

geometry const geometry_geom_lightindicator = {
  .mesh = {
    .triangles = triangles_geom_lightindicator,
    .triangles_count = 1,

    .vertex_buffer_offset = 11628,
    .vertex_buffer_size = 864,

    .index_buffer_offset = 3888,
    .index_buffer_size = 96,
  }
};

triangles const triangles_geom_box001[] = {
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
    .count = 10, // triangles
    .index_offset = 12, // indices
    .inputs_index = 0, // index into inputs_list
  },
  {
    .count = 10, // triangles
    .index_offset = 42, // indices
    .inputs_index = 0, // index into inputs_list
  },
  {
    .count = 10, // triangles
    .index_offset = 72, // indices
    .inputs_index = 0, // index into inputs_list
  },
  {
    .count = 10, // triangles
    .index_offset = 102, // indices
    .inputs_index = 0, // index into inputs_list
  },
};

geometry const geometry_geom_box001 = {
  .mesh = {
    .triangles = triangles_geom_box001,
    .triangles_count = 6,

    .vertex_buffer_offset = 12492,
    .vertex_buffer_size = 2016,

    .index_buffer_offset = 3984,
    .index_buffer_size = 528,
  }
};

triangles const triangles_geom_pyramid001[] = {
  {
    .count = 8, // triangles
    .index_offset = 0, // indices
    .inputs_index = 0, // index into inputs_list
  },
};

geometry const geometry_geom_pyramid001 = {
  .mesh = {
    .triangles = triangles_geom_pyramid001,
    .triangles_count = 1,

    .vertex_buffer_offset = 14508,
    .vertex_buffer_size = 612,

    .index_buffer_offset = 4512,
    .index_buffer_size = 96,
  }
};

geometry const * const geometries[] = {
  &geometry_geom_cube,
  &geometry_geom_torus,
  &geometry_geom_cylinder,
  &geometry_geom_plane,
  &geometry_geom_geosphere,
  &geometry_geom_lightindicator,
  &geometry_geom_box001,
  &geometry_geom_pyramid001,
};

matrix const inverse_bind_matrices_geom_box001_skin1[] = {
  {
    -5.11435e-14f, -1.74846e-07f, -1.0f, 0.0f,
    3.13916e-07f, 1.0f, -1.74846e-07f, 0.0f,
    1.0f, -3.13916e-07f, 3.74342e-15f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
  },
  {
    -8.74227e-08f, -8.74228e-08f, -1.0f, 0.0f,
    -5.52335e-07f, 1.0f, -8.74227e-08f, 0.0f,
    1.0f, 5.52335e-07f, -8.74227e-08f, 0.0f,
    -10.0f, -9.85461e-06f, 8.74227e-07f, 1.0f,
  },
};

controller const controller_geom_box001_skin1 = {
  .skin = {
    .geometry = &geometry_geom_box001,

    .inverse_bind_matrices = inverse_bind_matrices_geom_box001_skin1,

    .vertex_buffer_offset = 0,
    .vertex_buffer_size = 1792,
  }
};

transform const transforms_node_environmentambientlight[] = {
};

instance_geometry const instance_geometries_node_environmentambientlight[] = {
};

instance_controller const instance_controllers_node_environmentambientlight[] = {
};

instance_light const instance_lights_node_environmentambientlight[] = {
  {
    .light = &light_environmentambientlight,
  }
};

channel const * const node_channels_node_environmentambientlight[] = {};

node const node_node_environmentambientlight = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_environmentambientlight,
  .transforms_count = 0,

  .instance_geometries = instance_geometries_node_environmentambientlight,
  .instance_geometries_count = 0,

  .instance_controllers = instance_controllers_node_environmentambientlight,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_environmentambientlight,
  .instance_lights_count = 1,

  .channels = node_channels_node_environmentambientlight,
  .channels_count = 0,
};

transform const transforms_node_cube[] = {
  {
    .type = transform_type::TRANSLATE,
    .translate = {34.88703f, -1.85427e-06f, 0.0f},
  },
  {
    .type = transform_type::SCALE,
    .scale = {3.523671f, 3.523671f, 3.523671f},
  },
};

instance_material const instance_geometry_instance_materials_node_cube_0[] = {
  {
    .element_index = 1, // an index into mesh.triangles
    .material = &material_material__15_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_material__16_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
  {
    .element_index = 3, // an index into mesh.triangles
    .material = &material_material__18_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
  {
    .element_index = 5, // an index into mesh.triangles
    .material = &material_material__17_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
  {
    .element_index = 2, // an index into mesh.triangles
    .material = &material_material__19_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
  {
    .element_index = 4, // an index into mesh.triangles
    .material = &material_material__20_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
};

instance_geometry const instance_geometries_node_cube[] = {
  {
    .geometry = &geometry_geom_cube,

    .instance_materials = instance_geometry_instance_materials_node_cube_0,
    .instance_materials_count = 6,
  },
};

instance_controller const instance_controllers_node_cube[] = {
};

instance_light const instance_lights_node_cube[] = {
};

channel const * const node_channels_node_cube[] = {
  &node_channel_node_cube_translation_x,
  &node_channel_node_cube_translation_y,
};

node const node_node_cube = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_cube,
  .transforms_count = 2,

  .instance_geometries = instance_geometries_node_cube,
  .instance_geometries_count = 1,

  .instance_controllers = instance_controllers_node_cube,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_cube,
  .instance_lights_count = 0,

  .channels = node_channels_node_cube,
  .channels_count = 2,
};

transform const transforms_node_torus[] = {
  {
    .type = transform_type::TRANSLATE,
    .translate = {5.000001f, 1.14258e-07f, 2.0f},
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

instance_material const instance_geometry_instance_materials_node_torus_0[] = {
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_coloreffectr26g177b26_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
};

instance_geometry const instance_geometries_node_torus[] = {
  {
    .geometry = &geometry_geom_torus,

    .instance_materials = instance_geometry_instance_materials_node_torus_0,
    .instance_materials_count = 1,
  },
};

instance_controller const instance_controllers_node_torus[] = {
};

instance_light const instance_lights_node_torus[] = {
};

channel const * const node_channels_node_torus[] = {
  &node_channel_node_torus_rotationx_angle,
};

node const node_node_torus = {
  .parent_index = 1,

  .type = node_type::NODE,

  .transforms = transforms_node_torus,
  .transforms_count = 5,

  .instance_geometries = instance_geometries_node_torus,
  .instance_geometries_count = 1,

  .instance_controllers = instance_controllers_node_torus,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_torus,
  .instance_lights_count = 0,

  .channels = node_channels_node_torus,
  .channels_count = 1,
};

transform const transforms_node_cylinder[] = {
  {
    .type = transform_type::SCALE,
    .scale = {3.523671f, 3.523671f, 3.523671f},
  },
};

instance_material const instance_geometry_instance_materials_node_cylinder_0[] = {
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_grass_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = 0 },
    .specular = { .input_set = -1 },
  },
};

instance_geometry const instance_geometries_node_cylinder[] = {
  {
    .geometry = &geometry_geom_cylinder,

    .instance_materials = instance_geometry_instance_materials_node_cylinder_0,
    .instance_materials_count = 1,
  },
};

instance_controller const instance_controllers_node_cylinder[] = {
};

instance_light const instance_lights_node_cylinder[] = {
};

channel const * const node_channels_node_cylinder[] = {
};

node const node_node_cylinder = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_cylinder,
  .transforms_count = 1,

  .instance_geometries = instance_geometries_node_cylinder,
  .instance_geometries_count = 1,

  .instance_controllers = instance_controllers_node_cylinder,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_cylinder,
  .instance_lights_count = 0,

  .channels = node_channels_node_cylinder,
  .channels_count = 0,
};

transform const transforms_node_plane[] = {
  {
    .type = transform_type::TRANSLATE,
    .translate = {0.0f, 0.0f, 0.01f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {0.0f, 0.0f, -1.0f, -44.99999f},
  },
  {
    .type = transform_type::SCALE,
    .scale = {3.523672f, 3.523672f, 3.523671f},
  },
};

instance_material const instance_geometry_instance_materials_node_plane_0[] = {
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_wood_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = 0 },
    .specular = { .input_set = 0 },
  },
};

instance_geometry const instance_geometries_node_plane[] = {
  {
    .geometry = &geometry_geom_plane,

    .instance_materials = instance_geometry_instance_materials_node_plane_0,
    .instance_materials_count = 1,
  },
};

instance_controller const instance_controllers_node_plane[] = {
};

instance_light const instance_lights_node_plane[] = {
};

channel const * const node_channels_node_plane[] = {
};

node const node_node_plane = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_plane,
  .transforms_count = 3,

  .instance_geometries = instance_geometries_node_plane,
  .instance_geometries_count = 1,

  .instance_controllers = instance_controllers_node_plane,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_plane,
  .instance_lights_count = 0,

  .channels = node_channels_node_plane,
  .channels_count = 0,
};

transform const transforms_node_geosphere[] = {
  {
    .type = transform_type::TRANSLATE,
    .translate = {-18.87136f, 14.31975f, 0.0f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {0.0f, 0.0f, 0.0f, 0.0f},
  },
  {
    .type = transform_type::SCALE,
    .scale = {3.523671f, 3.523671f, 3.523671f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {0.0f, 0.0f, 0.0f, 0.0f},
  },
};

instance_material const instance_geometry_instance_materials_node_geosphere_0[] = {
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_coloreffectr198g224b87_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
};

instance_geometry const instance_geometries_node_geosphere[] = {
  {
    .geometry = &geometry_geom_geosphere,

    .instance_materials = instance_geometry_instance_materials_node_geosphere_0,
    .instance_materials_count = 1,
  },
};

instance_controller const instance_controllers_node_geosphere[] = {
};

instance_light const instance_lights_node_geosphere[] = {
};

channel const * const node_channels_node_geosphere[] = {
  &node_channel_node_geosphere_scale,
  &node_channel_node_geosphere_inversescaleaxisrotation,
  &node_channel_node_geosphere_scaleaxisrotation,
};

node const node_node_geosphere = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_geosphere,
  .transforms_count = 4,

  .instance_geometries = instance_geometries_node_geosphere,
  .instance_geometries_count = 1,

  .instance_controllers = instance_controllers_node_geosphere,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_geosphere,
  .instance_lights_count = 0,

  .channels = node_channels_node_geosphere,
  .channels_count = 3,
};

transform const transforms_node_light[] = {
  {
    .type = transform_type::TRANSLATE,
    .translate = {24.0f, 25.52443f, 19.04236f},
  },
  {
    .type = transform_type::SCALE,
    .scale = {3.523671f, 3.523671f, 3.523671f},
  },
};

instance_geometry const instance_geometries_node_light[] = {
};

instance_controller const instance_controllers_node_light[] = {
};

instance_light const instance_lights_node_light[] = {
  {
    .light = &light_light_light,
  }
};

channel const * const node_channels_node_light[] = {
  &node_channel_node_light_translation_x,
  &node_channel_node_light_translation_y,
};

node const node_node_light = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_light,
  .transforms_count = 2,

  .instance_geometries = instance_geometries_node_light,
  .instance_geometries_count = 0,

  .instance_controllers = instance_controllers_node_light,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_light,
  .instance_lights_count = 1,

  .channels = node_channels_node_light,
  .channels_count = 2,
};

transform const transforms_node_lightindicator[] = {
  {
    .type = transform_type::ROTATE,
    .rotate = {0.5773504f, -0.5773501f, -0.5773504f, -120.0f},
  },
};

instance_material const instance_geometry_instance_materials_node_lightindicator_0[] = {
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_lightemit_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
};

instance_geometry const instance_geometries_node_lightindicator[] = {
  {
    .geometry = &geometry_geom_lightindicator,

    .instance_materials = instance_geometry_instance_materials_node_lightindicator_0,
    .instance_materials_count = 1,
  },
};

instance_controller const instance_controllers_node_lightindicator[] = {
};

instance_light const instance_lights_node_lightindicator[] = {
};

channel const * const node_channels_node_lightindicator[] = {
};

node const node_node_lightindicator = {
  .parent_index = 6,

  .type = node_type::NODE,

  .transforms = transforms_node_lightindicator,
  .transforms_count = 1,

  .instance_geometries = instance_geometries_node_lightindicator,
  .instance_geometries_count = 1,

  .instance_controllers = instance_controllers_node_lightindicator,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_lightindicator,
  .instance_lights_count = 0,

  .channels = node_channels_node_lightindicator,
  .channels_count = 0,
};

transform const transforms_node_box001[] = {
};

instance_geometry const instance_geometries_node_box001[] = {
};

int const joint_node_indices_node_box001_geom_box001_skin1[] = {
  9, // joint0 node_bone001
  10, // joint1 node_bone002
};

instance_material const instance_controller_instance_materials_node_box001_0[] = {
  {
    .element_index = 1, // an index into mesh.triangles
    .material = &material_material__13_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_material__14_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
  {
    .element_index = 5, // an index into mesh.triangles
    .material = &material_material__15_1_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
  {
    .element_index = 3, // an index into mesh.triangles
    .material = &material_material__16_1_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
  {
    .element_index = 2, // an index into mesh.triangles
    .material = &material_material__17_1_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
  {
    .element_index = 4, // an index into mesh.triangles
    .material = &material_material__18_1_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
};

instance_controller const instance_controllers_node_box001[] = {
  {
    .controller = &controller_geom_box001_skin1,

    .joint_node_indices = joint_node_indices_node_box001_geom_box001_skin1,
    .joint_count = (sizeof (joint_node_indices_node_box001_geom_box001_skin1)) / (sizeof (int)),

    .instance_materials = instance_controller_instance_materials_node_box001_0,
    .instance_materials_count = 6,
  },
};

instance_light const instance_lights_node_box001[] = {
};

channel const * const node_channels_node_box001[] = {
};

node const node_node_box001 = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_box001,
  .transforms_count = 0,

  .instance_geometries = instance_geometries_node_box001,
  .instance_geometries_count = 0,

  .instance_controllers = instance_controllers_node_box001,
  .instance_controllers_count = 1,

  .instance_lights = instance_lights_node_box001,
  .instance_lights_count = 0,

  .channels = node_channels_node_box001,
  .channels_count = 0,
};

transform const transforms_node_bone001[] = {
  {
    .type = transform_type::ROTATE,
    .rotate = {1.32123e-06f, 1.0f, -1.49608e-06f, -90.0f},
  },
};

instance_geometry const instance_geometries_node_bone001[] = {
};

instance_controller const instance_controllers_node_bone001[] = {
};

instance_light const instance_lights_node_bone001[] = {
};

channel const * const node_channels_node_bone001[] = {
};

node const node_node_bone001 = {
  .parent_index = -1,

  .type = node_type::JOINT,

  .transforms = transforms_node_bone001,
  .transforms_count = 1,

  .instance_geometries = instance_geometries_node_bone001,
  .instance_geometries_count = 0,

  .instance_controllers = instance_controllers_node_bone001,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_bone001,
  .instance_lights_count = 0,

  .channels = node_channels_node_bone001,
  .channels_count = 0,
};

transform const transforms_node_bone002[] = {
  {
    .type = transform_type::TRANSLATE,
    .translate = {10.0f, 1.19209e-06f, 5.69993e-14f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {0.0f, 0.0f, 1.0f, 5.00896e-06f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {0.0f, 1.0f, 0.0f, -5.00896e-06f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {1.0f, 0.0f, 0.0f, -5.00896e-06f},
  },
};

instance_geometry const instance_geometries_node_bone002[] = {
};

instance_controller const instance_controllers_node_bone002[] = {
};

instance_light const instance_lights_node_bone002[] = {
};

channel const * const node_channels_node_bone002[] = {
  &node_channel_node_bone002_rotationz_angle,
};

node const node_node_bone002 = {
  .parent_index = 9,

  .type = node_type::JOINT,

  .transforms = transforms_node_bone002,
  .transforms_count = 4,

  .instance_geometries = instance_geometries_node_bone002,
  .instance_geometries_count = 0,

  .instance_controllers = instance_controllers_node_bone002,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_bone002,
  .instance_lights_count = 0,

  .channels = node_channels_node_bone002,
  .channels_count = 1,
};

transform const transforms_node_pyramid001[] = {
  {
    .type = transform_type::TRANSLATE,
    .translate = {10.76371f, -13.71933f, 0.01108826f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {0.0f, 0.0f, 1.0f, 0.0f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {0.0f, 1.0f, 0.0f, 0.0f},
  },
  {
    .type = transform_type::ROTATE,
    .rotate = {1.0f, 0.0f, 0.0f, 0.0f},
  },
};

instance_material const instance_geometry_instance_materials_node_pyramid001_0[] = {
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_material__21_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = 0 },
    .specular = { .input_set = -1 },
  },
};

instance_geometry const instance_geometries_node_pyramid001[] = {
  {
    .geometry = &geometry_geom_pyramid001,

    .instance_materials = instance_geometry_instance_materials_node_pyramid001_0,
    .instance_materials_count = 1,
  },
};

instance_controller const instance_controllers_node_pyramid001[] = {
};

instance_light const instance_lights_node_pyramid001[] = {
};

channel const * const node_channels_node_pyramid001[] = {
  &node_channel_node_pyramid001_translation_z,
  &node_channel_node_pyramid001_translation_y,
  &node_channel_node_pyramid001_translation_x,
  &node_channel_node_pyramid001_rotationx_angle,
  &node_channel_node_pyramid001_rotationy_angle,
  &node_channel_node_pyramid001_rotationz_angle,
};

node const node_node_pyramid001 = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_pyramid001,
  .transforms_count = 4,

  .instance_geometries = instance_geometries_node_pyramid001,
  .instance_geometries_count = 1,

  .instance_controllers = instance_controllers_node_pyramid001,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_pyramid001,
  .instance_lights_count = 0,

  .channels = node_channels_node_pyramid001,
  .channels_count = 6,
};

node const * const nodes[] = {
  &node_node_environmentambientlight, // 0
  &node_node_cube, // 1
  &node_node_torus, // 2
  &node_node_cylinder, // 3
  &node_node_plane, // 4
  &node_node_geosphere, // 5
  &node_node_light, // 6
  &node_node_lightindicator, // 7
  &node_node_box001, // 8
  &node_node_bone001, // 9
  &node_node_bone002, // 10
  &node_node_pyramid001, // 11
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

  .images = images,
  .images_count = (sizeof (images)) / (sizeof (images[0])),

  .position_normal_texture_buffer = L"RES_SCENES_CURVE_INTERPOLATION_VTX",
  .joint_weight_buffer = L"RES_SCENES_CURVE_INTERPOLATION_VJW",
  .index_buffer = L"RES_SCENES_CURVE_INTERPOLATION_IDX",
};

}
