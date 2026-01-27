#include <windows.h>
#include <d3d10.h>
#include <assert.h>

#include "directxmath/directxmath.h"

#include "print.hpp"
#include "collada_scene.hpp"

extern ID3D10Device * g_pd3dDevice;
extern XMMATRIX g_View;
extern XMMATRIX g_Projection;

namespace collada_scene {
  using namespace collada;

  ID3D10Effect * g_pEffect = NULL;
  ID3D10EffectTechnique * g_pTechniqueBlinn = NULL;

  ID3D10EffectMatrixVariable * g_pWorldVariable = NULL;
  ID3D10EffectMatrixVariable * g_pViewVariable = NULL;
  ID3D10EffectMatrixVariable * g_pProjectionVariable = NULL;

  ID3D10EffectVectorVariable * g_pEmissionVariable = NULL;
  ID3D10EffectVectorVariable * g_pAmbientVariable = NULL;
  ID3D10EffectVectorVariable * g_pDiffuseVariable = NULL;
  ID3D10EffectVectorVariable * g_pSpecularVariable = NULL;
  ID3D10EffectScalarVariable * g_pShininessVariable = NULL;

  static inline DXGI_FORMAT dxgi_format(input_format format)
  {
    switch (format) {
    case input_format::FLOAT1: return DXGI_FORMAT_R32_FLOAT;
    case input_format::FLOAT2: return DXGI_FORMAT_R32G32_FLOAT;
    case input_format::FLOAT3: return DXGI_FORMAT_R32G32B32_FLOAT;
    case input_format::FLOAT4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
    default:
      assert(false);
    }
  }

  static inline int format_size(input_format format)
  {
    switch (format) {
    case input_format::FLOAT1: return 1 * 4;
    case input_format::FLOAT2: return 2 * 4;
    case input_format::FLOAT3: return 3 * 4;
    case input_format::FLOAT4: return 4 * 4;
    default:
      assert(false);
    }
  }

  HRESULT scene_state::load_vertex_buffer(LPCWSTR name)
  {
    HRESULT hr;
    HRSRC hRes = FindResource(NULL, name, RT_RCDATA);
    if (hRes == NULL) {
      printW(L"FindResource %s\n", name);
      return E_FAIL;
    }
    DWORD dwResSize = SizeofResource(NULL, hRes);
    void * pData = LockResource(LoadResource(NULL, hRes));

    D3D10_BUFFER_DESC bd;
    D3D10_SUBRESOURCE_DATA initData;

    bd.Usage = D3D10_USAGE_IMMUTABLE;
    bd.ByteWidth = dwResSize;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    initData.pSysMem = pData;
    hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &pVertexBuffers[0]);
    if (FAILED(hr)) {
      print("CreateBuffer: D3D10_BIND_VERTEX_BUFFER\n");
      return hr;
    }

    return S_OK;
  }

  HRESULT scene_state::load_index_buffer(LPCWSTR name)
  {
    HRESULT hr;
    HRSRC hRes = FindResource(NULL, name, RT_RCDATA);
    if (hRes == NULL) {
      printW(L"FindResource %s\n", name);
      return E_FAIL;
    }
    DWORD dwResSize = SizeofResource(NULL, hRes);
    void * pData = LockResource(LoadResource(NULL, hRes));

    D3D10_BUFFER_DESC bd;
    D3D10_SUBRESOURCE_DATA initData;

    bd.Usage = D3D10_USAGE_IMMUTABLE;
    bd.ByteWidth = dwResSize;
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    initData.pSysMem = pData;
    hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &pIndexBuffer);
    if (FAILED(hr)) {
      print("CreateBuffer: D3D10_BIND_VERTEX_BUFFER\n");
      return hr;
    }

    return S_OK;
  }

  HRESULT scene_state::load_layout(int inputs_index, inputs const& inputs)
  {
    HRESULT hr;

    D3D10_INPUT_ELEMENT_DESC layout[inputs.elements_count];

    int byte_offset = 0;
    for (int i = 0; i < inputs.elements_count; i++) {
      layout[i].SemanticName = inputs.elements[i].semantic;
      layout[i].SemanticIndex = inputs.elements[i].semantic_index;
      layout[i].Format = dxgi_format(inputs.elements[i].format);
      layout[i].InputSlot = 0;
      layout[i].AlignedByteOffset = byte_offset;
      layout[i].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
      layout[i].InstanceDataStepRate = 0;

      byte_offset += format_size(inputs.elements[i].format);
    }

    D3D10_PASS_DESC passDesc;
    g_pTechniqueBlinn->GetPassByIndex(0)->GetDesc(&passDesc);

    hr = g_pd3dDevice->CreateInputLayout(layout, inputs.elements_count,
                                         passDesc.pIAInputSignature,
                                         passDesc.IAInputSignatureSize,
                                         &pVertexLayouts[inputs_index]);
    if (FAILED(hr)) {
      print("CreateInputLayout\n");
      return hr;
    }

    return S_OK;
  }

  template <typename T>
  T New(int elements)
  {
    return (T)malloc((sizeof (T)) * elements);
  }

  HRESULT scene_state::load_layouts(descriptor const& descriptor)
  {
    HRESULT hr;

    //this->pVertexLayouts = new ID3D10InputLayout *[descriptor.inputs_list_count];
    this->pVertexLayouts = New<ID3D10InputLayout **>(descriptor.inputs_list_count);

    for (int i = 0; i < descriptor.inputs_list_count; i++) {
      hr = load_layout(i, descriptor.inputs_list[i]);
      if (FAILED(hr))
        return hr;
    }

    return S_OK;
  }

  HRESULT LoadEffect()
  {
    HRESULT hr;
    HRSRC hRes = FindResource(NULL, L"RES_COLLADA_SCENE_FXO", RT_RCDATA);
    if (hRes == NULL) {
      print("FindResource RES_COLLADA_SCENE_FXO\n");
      return E_FAIL;
    }
    DWORD dwResSize = SizeofResource(NULL, hRes);
    HGLOBAL hData = LoadResource(NULL, hRes);
    void * pData = LockResource(hData);
    hr = D3D10CreateEffectFromMemory(pData,
                                     dwResSize,
                                     0,
                                     g_pd3dDevice,
                                     NULL,
                                     &g_pEffect
                                     );
    if (FAILED(hr)) {
      print("D3D10CreateEffectFromMemory\n");
      return hr;
    }

    g_pTechniqueBlinn = g_pEffect->GetTechniqueByName("Blinn");

    g_pWorldVariable = g_pEffect->GetVariableByName("World")->AsMatrix();
    g_pViewVariable = g_pEffect->GetVariableByName("View")->AsMatrix();
    g_pProjectionVariable = g_pEffect->GetVariableByName("Projection")->AsMatrix();

    g_pEmissionVariable = g_pEffect->GetVariableByName("Emission")->AsVector();
    g_pAmbientVariable = g_pEffect->GetVariableByName("Ambient")->AsVector();
    g_pDiffuseVariable = g_pEffect->GetVariableByName("Diffuse")->AsVector();
    g_pSpecularVariable = g_pEffect->GetVariableByName("Specular")->AsVector();
    g_pShininessVariable = g_pEffect->GetVariableByName("Shininess")->AsScalar();

    return S_OK;
  }

  HRESULT LoadScene(descriptor const& descriptor, scene_state& state)
  {
    HRESULT hr;

    // effects/techniques
    hr = LoadEffect();
    if (FAILED(hr))
      return E_FAIL;

    // layouts
    hr = state.load_layouts(descriptor);
    if (FAILED(hr))
      return E_FAIL;

    hr = state.load_vertex_buffer(L"RES_MODELS_CURVE_INTERPOLATION_VTX");
    if (FAILED(hr))
      return E_FAIL;

    hr = state.load_index_buffer(L"RES_MODELS_CURVE_INTERPOLATION_IDX");
    if (FAILED(hr))
      return E_FAIL;

    return S_OK;
  }

  static inline XMMATRIX transform_matrix(transform const& transform)
  {
    switch (transform.type) {
    case transform_type::TRANSLATE:
      return XMMatrixTranslationFromVector(XMLoadFloat3((XMFLOAT3 *)&transform.translate.x));
    case transform_type::ROTATE:
      return XMMatrixRotationNormal(XMLoadFloat3((XMFLOAT3 *)&transform.rotate.x),
                                    XMConvertToRadians(transform.rotate.w));
    case transform_type::SCALE:
      return XMMatrixScalingFromVector(XMLoadFloat3((XMFLOAT3 *)&transform.scale.x));
    default:
      assert(false);
      break;
    }
  }

  static inline void SetMaterial(effect const& effect)
  {
    switch (effect.type) {
    case effect_type::BLINN:
      g_pEmissionVariable->SetFloatVector((float *)&effect.blinn.emission.color.x);
      g_pAmbientVariable->SetFloatVector((float *)&effect.blinn.ambient.color.x);
      g_pDiffuseVariable->SetFloatVector((float *)&effect.blinn.diffuse.color.x);
      g_pSpecularVariable->SetFloatVector((float *)&effect.blinn.specular.color.x);
      g_pShininessVariable->SetFloat(effect.blinn.shininess);
      break;
    case effect_type::LAMBERT:
      g_pEmissionVariable->SetFloatVector((float *)&effect.lambert.emission.color.x);
      g_pAmbientVariable->SetFloatVector((float *)&effect.lambert.ambient.color.x);
      g_pDiffuseVariable->SetFloatVector((float *)&effect.lambert.diffuse.color.x);
      break;
    case effect_type::PHONG:
      g_pEmissionVariable->SetFloatVector((float *)&effect.phong.emission.color.x);
      g_pAmbientVariable->SetFloatVector((float *)&effect.phong.ambient.color.x);
      g_pDiffuseVariable->SetFloatVector((float *)&effect.phong.diffuse.color.x);
      g_pSpecularVariable->SetFloatVector((float *)&effect.phong.specular.color.x);
      g_pShininessVariable->SetFloat(effect.phong.shininess);
      break;
    case effect_type::CONSTANT:
      g_pEmissionVariable->SetFloatVector((float *)&effect.constant.color.x);
      break;
    default:
      break;
    }
  }

  void RenderGeometries(scene_state const& state,
                        instance_geometry const * const instance_geometries,
                        int const instance_geometries_count)
  {
    for (int i = 0; i < instance_geometries_count; i++) {
      instance_geometry const &instance_geometry = instance_geometries[i];
      mesh const& mesh = instance_geometry.geometry->mesh;

      UINT strides[1] = { 3 * 3 * 4 };
      UINT offsets[1] = { (UINT)mesh.vertex_buffer_offset };
      g_pd3dDevice->IASetVertexBuffers(0, state.numBuffers, state.pVertexBuffers, strides, offsets);
      g_pd3dDevice->IASetIndexBuffer(state.pIndexBuffer, DXGI_FORMAT_R32_UINT, mesh.index_buffer_offset);

      D3D10_TECHNIQUE_DESC techDesc;
      g_pTechniqueBlinn->GetDesc(&techDesc);

      for (int j = 0; j < instance_geometry.instance_materials_count; j++) {
        instance_material const& instance_material = instance_geometry.instance_materials[j];
        triangles const& triangles = mesh.triangles[instance_material.element_index];
        SetMaterial(*instance_material.material->effect);

        g_pTechniqueBlinn->GetPassByIndex(0)->Apply(0);
        g_pd3dDevice->IASetInputLayout(state.pVertexLayouts[triangles.inputs_index]);
        g_pd3dDevice->DrawIndexed(triangles.count * 3, triangles.index_offset, 0);
      }
    }
  }

  void Render(descriptor const& descriptor, scene_state const& state)
  {
    g_pViewVariable->SetMatrix((float *)&g_View);
    g_pProjectionVariable->SetMatrix((float *)&g_Projection);

    g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    for (int i = 0; i < descriptor.nodes_count; i++) {
      node const& node = *descriptor.nodes[i];
      if (node.type != node_type::NODE)
        continue;

      XMMATRIX World = XMMatrixIdentity();
      // build the world transform
      for (int j = 0; j < node.transforms_count; j++) {
        transform const& transform = node.transforms[j];
        World = transform_matrix(transform) * World;
      }
      g_pWorldVariable->SetMatrix((float *)&World);

      RenderGeometries(state, node.instance_geometries, node.instance_geometries_count);
    }
  }
}
