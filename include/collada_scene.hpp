#pragma once

#include "collada_types.hpp"

namespace collada_scene {

  struct scene_state {
    const UINT numBuffers = 1; // FIXME
    const UINT strides[1] = {3 * 3 * 4}; // FIXME
    const UINT offsets[1] = {0}; // FIXME

    ID3D10Buffer * pVertexBuffers[1] = {}; // FIXME
    ID3D10Buffer * pIndexBuffer = NULL;

    ID3D10InputLayout ** pVertexLayouts = NULL;

    HRESULT load_vertex_buffer(LPCWSTR name);
    HRESULT load_index_buffer(LPCWSTR name);
    HRESULT load_layout(int inputs_index, collada::inputs const& inputs);
    HRESULT load_layouts(collada::descriptor const& descriptor);
  };

  HRESULT LoadScene(collada::descriptor const& descriptor, scene_state& state);
  void Render(collada::descriptor const& descriptor, scene_state const& state);
}
