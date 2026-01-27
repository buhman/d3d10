#include <windows.h>
#include <d3d10.h>
#include <assert.h>

#include "directxmath/directxmath.h"

#include "print.hpp"
#include "collada_scene.hpp"
#include "new.hpp"

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

  HRESULT scene_state::load_layouts()
  {
    HRESULT hr;

    m_pVertexLayouts = New<ID3D10InputLayout *>(m_descriptor->inputs_list_count);

    for (int i = 0; i < m_descriptor->inputs_list_count; i++) {
      hr = LoadLayout(m_descriptor->inputs_list[i], &m_pVertexLayouts[i]);
      if (FAILED(hr))
        return hr;
    }

    return S_OK;
  }

  static int count_nodes(node const * const node)
  {
    int count = 1;
    for (int i = 0; i < node->nodes_count; i++) {
      count += count_nodes(&node->nodes[i]);
    }
    return count;
  }

  static void initialize_node_transforms(node const * const node,
                                         node_instance * node_instance)
  {
    for (int i = 0; i < node->transforms_count; i++) {
      transform& transform = node_instance->transforms[i];
      transform.type = node->transforms[i].type;
      switch (transform.type) {
      case transform_type::LOOKAT:
        transform.lookat.eye = XMLoadFloat3((XMFLOAT3 *)&node->transforms[i].lookat.eye);
        transform.lookat.at = XMLoadFloat3((XMFLOAT3 *)&node->transforms[i].lookat.at);
        transform.lookat.up = XMLoadFloat3((XMFLOAT3 *)&node->transforms[i].lookat.up);
        break;
      case transform_type::MATRIX:
        transform.matrix = XMLoadFloat4x4((XMFLOAT4X4 *)&node->transforms[i].matrix);
        break;
      case transform_type::ROTATE:
        transform.rotate = XMLoadFloat4((XMFLOAT4 *)&node->transforms[i].rotate);
        break;
      case transform_type::SCALE:
        transform.scale = XMLoadFloat3((XMFLOAT3 *)&node->transforms[i].scale);
        break;
      case transform_type::TRANSLATE:
        transform.translate = XMLoadFloat3((XMFLOAT3 *)&node->transforms[i].translate);
        break;
      default:
        assert(false);
      }
    }
  }

  static void allocate_node_instance(node const * const node,
                                     node_instance * node_instance)
  {
    node_instance->transforms = New<transform>(node->transforms_count);
    initialize_node_transforms(node, node_instance);
  }

  node_instance * scene_state::apply_node_instances1(node const * const node,
                                                     node_instance * node_instance,
                                                     apply_node_func_t func)
  {
    func(node, node_instance);
    node_instance = &node_instance[1];

    for (int i = 0; i < node->nodes_count; i++) {
      node_instance = apply_node_instances1(&node->nodes[i], node_instance, func);
    }

    return node_instance;
  }

  void scene_state::apply_node_instances(apply_node_func_t func)
  {
    node_instance * node_instance = m_nodeInstances;
    for (int i = 0; i < m_descriptor->nodes_count; i++) {
      node_instance = apply_node_instances1(m_descriptor->nodes[i], node_instance, func);
    }
  }

  void scene_state::allocate_node_instances()
  {
    int count = 0;
    for (int i = 0; i < m_descriptor->nodes_count; i++) {
      count += count_nodes(m_descriptor->nodes[i]);
    }

    m_nodeInstances = New<node_instance>(count);

    apply_node_instances(allocate_node_instance);
  }

  HRESULT scene_state::load_scene(collada::descriptor const * const descriptor)
  {
    HRESULT hr;

    m_descriptor = descriptor;

    // layouts
    hr = load_layouts();
    if (FAILED(hr))
      return E_FAIL;

    hr = LoadVertexBuffer(L"RES_MODELS_CURVE_INTERPOLATION_VTX", &m_pVertexBuffers[0]);
    if (FAILED(hr))
      return E_FAIL;

    hr = LoadIndexBuffer(L"RES_MODELS_CURVE_INTERPOLATION_IDX", &m_pIndexBuffer);
    if (FAILED(hr))
      return E_FAIL;

    allocate_node_instances();

    return S_OK;
  }

  HRESULT LoadLayout(inputs const& inputs, ID3D10InputLayout ** ppVertexLayout)
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
                                         ppVertexLayout);
    if (FAILED(hr)) {
      print("CreateInputLayout\n");
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

  HRESULT LoadVertexBuffer(LPCWSTR name, ID3D10Buffer ** ppVertexBuffer)
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
    hr = g_pd3dDevice->CreateBuffer(&bd, &initData, ppVertexBuffer);
    if (FAILED(hr)) {
      print("CreateBuffer: D3D10_BIND_VERTEX_BUFFER\n");
      return hr;
    }

    return S_OK;
  }

  HRESULT LoadIndexBuffer(LPCWSTR name, ID3D10Buffer ** ppIndexBuffer)
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
    hr = g_pd3dDevice->CreateBuffer(&bd, &initData, ppIndexBuffer);
    if (FAILED(hr)) {
      print("CreateBuffer: D3D10_BIND_VERTEX_BUFFER\n");
      return hr;
    }

    return S_OK;
  }

  static inline XMMATRIX TransformMatrix(transform const& transform)
  {
    switch (transform.type) {
    case transform_type::TRANSLATE:
      return XMMatrixTranslationFromVector(transform.translate);
    case transform_type::ROTATE:
      return XMMatrixRotationNormal(transform.rotate,
                                    XMConvertToRadians(XMVectorGetW(transform.rotate)));
    case transform_type::SCALE:
      return XMMatrixScalingFromVector(transform.scale);
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

  void scene_state::render_geometries(instance_geometry const * const instance_geometries,
                                      int const instance_geometries_count)
  {
    for (int i = 0; i < instance_geometries_count; i++) {
      instance_geometry const &instance_geometry = instance_geometries[i];
      mesh const& mesh = instance_geometry.geometry->mesh;

      UINT strides[1] = { 3 * 3 * 4 };
      UINT offsets[1] = { (UINT)mesh.vertex_buffer_offset };
      g_pd3dDevice->IASetVertexBuffers(0, m_numBuffers, m_pVertexBuffers, strides, offsets);
      g_pd3dDevice->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, mesh.index_buffer_offset);

      D3D10_TECHNIQUE_DESC techDesc;
      g_pTechniqueBlinn->GetDesc(&techDesc);

      for (int j = 0; j < instance_geometry.instance_materials_count; j++) {
        instance_material const& instance_material = instance_geometry.instance_materials[j];
        triangles const& triangles = mesh.triangles[instance_material.element_index];
        SetMaterial(*instance_material.material->effect);

        g_pTechniqueBlinn->GetPassByIndex(0)->Apply(0);
        g_pd3dDevice->IASetInputLayout(m_pVertexLayouts[triangles.inputs_index]);
        g_pd3dDevice->DrawIndexed(triangles.count * 3, triangles.index_offset, 0);
      }
    }
  }

  void scene_state::render()
  {
    g_pViewVariable->SetMatrix((float *)&g_View);
    g_pProjectionVariable->SetMatrix((float *)&g_Projection);

    g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    for (int i = 0; i < m_descriptor->nodes_count; i++) {
      node const& node = *m_descriptor->nodes[i];
      assert(node.nodes_count == 0);
      if (node.type != node_type::NODE)
        continue;

      node_instance const& node_instance = m_nodeInstances[i];
      XMMATRIX World = XMMatrixIdentity();
      // build the world transform
      for (int j = 0; j < node.transforms_count; j++) {
        World = TransformMatrix(node_instance.transforms[j]) * World;
      }
      g_pWorldVariable->SetMatrix((float *)&World);

      render_geometries(node.instance_geometries, node.instance_geometries_count);
    }
  }
}
