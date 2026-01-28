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

    collada::descriptor const * m_descriptor;

    HRESULT load_scene(collada::descriptor const * const descriptor);
    void render(float t);

  private:
    HRESULT load_layouts();
    void allocate_node_instances();
    void allocate_node_instance(collada::node const * const node,
                                node_instance * node_instance);

    void render_geometries(collada::instance_geometry const * const instance_geometries,
                           int const instance_geometries_count);
    void node_world_transform(collada::node const& node, node_instance& node_instance);
  };

  HRESULT LoadEffect();
  HRESULT LoadLayout(collada::inputs const& inputs, ID3D10InputLayout ** ppVertexLayout);
  HRESULT LoadVertexBuffer(LPCWSTR name, ID3D10Buffer ** ppVertexBuffer);
  HRESULT LoadIndexBuffer(LPCWSTR name, ID3D10Buffer ** ppIndexBuffer);
}
