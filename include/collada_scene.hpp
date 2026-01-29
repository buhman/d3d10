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
    const UINT m_numBuffers = 1; // FIXME
    const UINT m_strides[1] = {3 * 3 * 4}; // FIXME
    const UINT m_offsets[1] = {0}; // FIXME

    ID3D10Buffer * m_pVertexBuffers[1] = {}; // FIXME
    ID3D10Buffer * m_pIndexBuffer = NULL;

    ID3D10InputLayout ** m_pVertexLayouts = NULL;

    node_instance * m_nodeInstances = NULL;

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
    void allocate_node_instances();
    void allocate_node_instance(collada::node const * const node,
                                node_instance * node_instance);
    void allocate_light_instances();

    void node_world_transform(collada::node const& node, node_instance& node_instance);
    void update_light_instances();
    void update_light_instance_vectors(collada::light const& light,
                                       node_instance const& node_instance,
                                       int light_index);

    void render_geometries(collada::instance_geometry const * const instance_geometries,
                           int const instance_geometries_count);
  };

  HRESULT LoadEffect();
  HRESULT LoadLayout(collada::inputs const& inputs, ID3D10InputLayout ** ppVertexLayout);
  HRESULT LoadVertexBuffer(LPCWSTR name, ID3D10Buffer ** ppVertexBuffer);
  HRESULT LoadIndexBuffer(LPCWSTR name, ID3D10Buffer ** ppIndexBuffer);
}
