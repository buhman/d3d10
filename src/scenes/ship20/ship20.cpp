#include "collada_types.hpp"

namespace ship20 {

using namespace collada;

light const light_environmentambientlight = {
  .type = light_type::AMBIENT,
  .color = { 0.0f, 0.0f, 0.0f },
};

light const light_omni002_light = {
  .type = light_type::POINT,
  .color = { 1.0f, 1.0f, 1.0f },
};

light const light_omni003_light = {
  .type = light_type::POINT,
  .color = { 1.0f, 1.0f, 1.0f },
};

// shipple2_png
image const image_shipple2_png = {
  .resource_name = L"_0_SHIPPLE2_PNG",
};

image const * const images[] = {
  &image_shipple2_png,
};

effect const effect_diffusetexture = {
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
      .texture = { .image_index = 0 }, // shipple2_png
    },
    .specular = {
      .type = color_or_texture_type::COLOR,
      .color = {0.5f, 0.5f, 0.5f, 1.0f},
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

effect const effect_cyanengine = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .type = color_or_texture_type::COLOR,
      .color = {0.0f, 0.9647059f, 1.0f, 1.0f},
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

effect const effect_emissivetexture = {
  .type = effect_type::BLINN,
  .blinn = {
    .emission = {
      .type = color_or_texture_type::TEXTURE,
      .texture = { .image_index = 0 }, // shipple2_png
    },
    .ambient = {
      .type = color_or_texture_type::COLOR,
      .color = {0.588f, 0.588f, 0.588f, 1.0f},
    },
    .diffuse = {
      .type = color_or_texture_type::TEXTURE,
      .texture = { .image_index = 0 }, // shipple2_png
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

material const material_diffusetexture_material = {
  .effect = &effect_diffusetexture,
};

material const material_cyanengine_material = {
  .effect = &effect_cyanengine,
};

material const material_emissivetexture_material = {
  .effect = &effect_emissivetexture,
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

triangles const triangles_geom_ship[] = {
  {
    .count = 2949, // triangles
    .index_offset = 0, // indices
    .inputs_index = 0, // index into inputs_list
  },
  {
    .count = 60, // triangles
    .index_offset = 8847, // indices
    .inputs_index = 0, // index into inputs_list
  },
  {
    .count = 239, // triangles
    .index_offset = 9027, // indices
    .inputs_index = 0, // index into inputs_list
  },
};

geometry const geometry_geom_ship = {
  .mesh = {
    .triangles = triangles_geom_ship,
    .triangles_count = 3,

    .vertex_buffer_offset = 0,
    .vertex_buffer_size = 133272,

    .index_buffer_offset = 0,
    .index_buffer_size = 38976,
  }
};

geometry const * const geometries[] = {
  &geometry_geom_ship,
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

transform const transforms_node_ship[] = {
  {
    .type = transform_type::ROTATE,
    .rotate = {0.0f, 0.0f, 1.0f, -180.0f},
  },
};

instance_material const instance_geometry_instance_materials_node_ship_0[] = {
  {
    .element_index = 1, // an index into mesh.triangles
    .material = &material_cyanengine_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = -1 },
    .specular = { .input_set = -1 },
  },
  {
    .element_index = 0, // an index into mesh.triangles
    .material = &material_diffusetexture_material,

    .emission = { .input_set = -1 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = 0 },
    .specular = { .input_set = -1 },
  },
  {
    .element_index = 2, // an index into mesh.triangles
    .material = &material_emissivetexture_material,

    .emission = { .input_set = 0 },
    .ambient = { .input_set = -1 },
    .diffuse = { .input_set = 0 },
    .specular = { .input_set = -1 },
  },
};

instance_geometry const instance_geometries_node_ship[] = {
  {
    .geometry = &geometry_geom_ship,

    .instance_materials = instance_geometry_instance_materials_node_ship_0,
    .instance_materials_count = 3,
  },
};

instance_controller const instance_controllers_node_ship[] = {
};

instance_light const instance_lights_node_ship[] = {
};

channel const * const node_channels_node_ship[] = {
};

node const node_node_ship = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_ship,
  .transforms_count = 1,

  .instance_geometries = instance_geometries_node_ship,
  .instance_geometries_count = 1,

  .instance_controllers = instance_controllers_node_ship,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_ship,
  .instance_lights_count = 0,

  .channels = node_channels_node_ship,
  .channels_count = 0,
};

transform const transforms_node_omni002[] = {
  {
    .type = transform_type::TRANSLATE,
    .translate = {-286.5521f, 395.7583f, 161.5579f},
  },
};

instance_geometry const instance_geometries_node_omni002[] = {
};

instance_controller const instance_controllers_node_omni002[] = {
};

instance_light const instance_lights_node_omni002[] = {
  {
    .light = &light_omni002_light,
  }
};

channel const * const node_channels_node_omni002[] = {
};

node const node_node_omni002 = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_omni002,
  .transforms_count = 1,

  .instance_geometries = instance_geometries_node_omni002,
  .instance_geometries_count = 0,

  .instance_controllers = instance_controllers_node_omni002,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_omni002,
  .instance_lights_count = 1,

  .channels = node_channels_node_omni002,
  .channels_count = 0,
};

transform const transforms_node_omni003[] = {
  {
    .type = transform_type::TRANSLATE,
    .translate = {333.2103f, -314.4593f, 161.5578f},
  },
};

instance_geometry const instance_geometries_node_omni003[] = {
};

instance_controller const instance_controllers_node_omni003[] = {
};

instance_light const instance_lights_node_omni003[] = {
  {
    .light = &light_omni003_light,
  }
};

channel const * const node_channels_node_omni003[] = {
};

node const node_node_omni003 = {
  .parent_index = -1,

  .type = node_type::NODE,

  .transforms = transforms_node_omni003,
  .transforms_count = 1,

  .instance_geometries = instance_geometries_node_omni003,
  .instance_geometries_count = 0,

  .instance_controllers = instance_controllers_node_omni003,
  .instance_controllers_count = 0,

  .instance_lights = instance_lights_node_omni003,
  .instance_lights_count = 1,

  .channels = node_channels_node_omni003,
  .channels_count = 0,
};

node const * const nodes[] = {
  &node_node_environmentambientlight, // 0
  &node_node_ship, // 1
  &node_node_omni002, // 2
  &node_node_omni003, // 3
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

  .position_normal_texture_buffer = L"RES_SCENES_SHIP20_VTX",
  .joint_weight_buffer = L"RES_SCENES_SHIP20_VJW",
  .index_buffer = L"RES_SCENES_SHIP20_IDX",
};

}
