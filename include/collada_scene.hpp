#pragma once

#include "collada_types.hpp"
#include "directxmath/directxmath.h"

namespace collada_scene {

  struct __attribute__((aligned(16))) lookat {
    XMVECTOR eye;
    XMVECTOR at;
    XMVECTOR up;
  };

  struct __attribute__((aligned(16))) transform {
    union {
      lookat lookat;
      XMMATRIX matrix;
      XMVECTOR vector;
    };
    collada::transform_type type;
  };

  struct node_instance {
    transform * transforms = NULL;
    XMMATRIX world;
  };

  struct scene_state {
    ID3D10Buffer * m_pVertexBuffers[2] = { NULL, NULL };
    ID3D10Buffer * m_pIndexBuffer = NULL;

    ID3D10InputLayout ** m_pVertexLayouts = NULL;
    ID3D10InputLayout ** m_pSkinnedVertexLayouts = NULL;
    UINT * m_iVertexLayoutStrides = NULL;

    node_instance * m_nodeInstances = NULL;

    ID3D10ShaderResourceView ** m_pShaderResourceViews = NULL;

    XMFLOAT4 * m_lightPositions = NULL;
    XMFLOAT4 * m_lightDirections = NULL;
    XMFLOAT4 * m_lightColors = NULL;

    int m_lightInstancesCount = 0;

    collada::descriptor const * m_descriptor;

    HRESULT load_scene(collada::descriptor const * const descriptor);
    void update(float t);
    void render();

  private:
    HRESULT load_layouts();
    HRESULT allocate_images();
    void allocate_node_instances();
    void allocate_node_instance(collada::node const * const node,
                                node_instance * node_instance);
    void allocate_light_instances();

    void node_world_transform(collada::node const& node, node_instance& node_instance);
    void update_light_instances();
    void update_light_instance_vectors(collada::light const& light,
                                       node_instance const& node_instance,
                                       int light_index);

    void render_geometry(collada::geometry const& geometry,
                         collada::instance_material const * const instance_materials,
                         int const instance_materials_count);
    void render_instance_geometries(collada::instance_geometry const * const instance_geometries,
                                    int const instance_geometries_count);
    void render_skin(collada::skin const& skin,
                     collada::instance_material const * const instance_materials,
                     int const instance_materials_count);
    void render_instance_controllers(collada::instance_controller const * const instance_controllers,
                                     int const instance_controllers_count);
    void set_instance_material(collada::instance_material const& instance_material);
    void set_color_or_texture(collada::color_or_texture const& color_or_texture,
                              ID3D10EffectVectorVariable * pVectorVariable,
                              ID3D10EffectShaderResourceVariable * pShaderResourceVariable);
  };

  HRESULT LoadEffect();
  HRESULT LoadLayout(collada::inputs const& inputs,
                     ID3D10InputLayout ** ppVertexLayout,
                     ID3D10InputLayout ** ppSkinnedVertexLayout,
                     UINT * iVertexLayoutStride);
  HRESULT LoadVertexBuffer(LPCWSTR name, ID3D10Buffer ** ppVertexBuffer);
  HRESULT LoadIndexBuffer(LPCWSTR name, ID3D10Buffer ** ppIndexBuffer);
}
