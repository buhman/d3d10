#include <windows.h>
#include <d3d10.h>
#include <assert.h>

#include "directxmath/directxmath.h"

#include "print.hpp"
#include "collada_scene.hpp"
#include "new.hpp"
#include "render_state.hpp"

extern ID3D10Device * g_pd3dDevice;
extern XMVECTOR g_Eye;
extern XMMATRIX g_View;
extern XMMATRIX g_Projection;

namespace collada_scene {
  using namespace collada;

  ID3D10Effect * g_pEffect = NULL;
  ID3D10EffectTechnique * g_pTechniqueBlinn = NULL;

  ID3D10EffectMatrixVariable * g_pWorldVariable = NULL;
  ID3D10EffectMatrixVariable * g_pViewVariable = NULL;
  ID3D10EffectMatrixVariable * g_pProjectionVariable = NULL;

  ID3D10EffectVectorVariable * g_pViewEyeVariable = NULL;
  ID3D10EffectVectorVariable * g_pLightPosVariable = NULL;
  ID3D10EffectVectorVariable * g_pLightDirVariable = NULL;
  ID3D10EffectVectorVariable * g_pLightColorVariable = NULL;

  ID3D10EffectVectorVariable * g_pEmissionVariable = NULL;
  ID3D10EffectVectorVariable * g_pAmbientVariable = NULL;
  ID3D10EffectVectorVariable * g_pDiffuseVariable = NULL;
  ID3D10EffectVectorVariable * g_pSpecularVariable = NULL;
  ID3D10EffectScalarVariable * g_pShininessVariable = NULL;

  ID3D10EffectVectorVariable * g_pTextureChannelVariable = NULL;

  ID3D10EffectShaderResourceVariable * g_pTexEmissionVariable = NULL;
  ID3D10EffectShaderResourceVariable * g_pTexAmbientVariable = NULL;
  ID3D10EffectShaderResourceVariable * g_pTexDiffuseVariable = NULL;
  ID3D10EffectShaderResourceVariable * g_pTexSpecularVariable = NULL;

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

  void scene_state::update_light_instance_vectors(light const& light,
                                                  node_instance const& node_instance,
                                                  int light_index)
  {
    const XMVECTOR position = XMVectorSet(0, 0, 0, 1);
    const XMVECTOR direction = XMVectorSet(0, 0, -1, 0);
    XMVECTOR light_position = XMVector3Transform(position, node_instance.world);
    XMVECTOR light_direction = XMVector3TransformNormal(direction, node_instance.world);
    XMVECTOR light_color = XMLoadFloat3((XMFLOAT3 *)&light.color);

    XMStoreFloat4(&m_lightPositions[light_index], light_position);
    XMStoreFloat4(&m_lightDirections[light_index], light_direction);
    XMStoreFloat4(&m_lightColors[light_index], light_color);
  }

  void scene_state::update_light_instances()
  {
    int light_index = 0;
    for (int i = 0; i < m_descriptor->nodes_count; i++) {
      node const& node = *m_descriptor->nodes[i];
      node_instance const& node_instance = m_nodeInstances[i];
      for (int j = 0; j < node.instance_lights_count; j++) {
        light const& light = *node.instance_lights[j].light;
        update_light_instance_vectors(light, node_instance, light_index);
        light_index += 1;
      }
    }
  }

  void scene_state::allocate_light_instances()
  {
    // count light instances
    int count = 0;
    for (int i = 0; i < m_descriptor->nodes_count; i++) {
      count += m_descriptor->nodes[i]->instance_lights_count;
    }

    // allocate
    m_lightInstancesCount = count;
    m_lightPositions = New<XMFLOAT4>(m_lightInstancesCount);
    m_lightDirections = New<XMFLOAT4>(m_lightInstancesCount);
    m_lightColors = New<XMFLOAT4>(m_lightInstancesCount);
  }

  static void initialize_node_transforms(node const * const node,
                                         node_instance * const node_instance)
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
        transform.vector = XMLoadFloat4((XMFLOAT4 *)&node->transforms[i].rotate);
        break;
      case transform_type::SCALE:
        transform.vector = XMLoadFloat3((XMFLOAT3*)&node->transforms[i].scale);
        break;
      case transform_type::TRANSLATE:
        transform.vector = XMLoadFloat3((XMFLOAT3*)&node->transforms[i].translate);
        break;
      default:
        assert(false);
      }
    }
  }

  void scene_state::allocate_node_instance(node const * const node,
                                           node_instance * const node_instance)
  {
    node_instance->transforms = New<transform>(node->transforms_count);
    initialize_node_transforms(node, node_instance);
  }

  void scene_state::allocate_node_instances()
  {
    m_nodeInstances = New<node_instance>(m_descriptor->nodes_count);

    for (int i = 0; i < m_descriptor->nodes_count; i++) {
      allocate_node_instance(m_descriptor->nodes[i], &m_nodeInstances[i]);
    }
  }

  HRESULT scene_state::allocate_images()
  {
    HRESULT hr;
    m_pShaderResourceViews = New<ID3D10ShaderResourceView *>(m_descriptor->images_count);

    for (int i = 0; i < m_descriptor->images_count; i++) {
      image const * const image = m_descriptor->images[i];
      hr = LoadDDSTexture2D(image->resource_name,
                            &m_pShaderResourceViews[i]);
      if (FAILED(hr)) {
        return hr;
      }
    }

    return S_OK;
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

    hr = allocate_images();
    if (FAILED(hr))
      return E_FAIL;

    allocate_node_instances();
    allocate_light_instances();

    return S_OK;
  }

  HRESULT LoadLayout(inputs const& inputs, ID3D10InputLayout ** ppVertexLayout)
  {
    HRESULT hr;

    D3D10_INPUT_ELEMENT_DESC layout[inputs.elements_count];

    assert(inputs.elements_count == 3);
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

    g_pViewEyeVariable = g_pEffect->GetVariableByName("ViewEye")->AsVector();
    g_pLightPosVariable = g_pEffect->GetVariableByName("LightPos")->AsVector();
    g_pLightDirVariable = g_pEffect->GetVariableByName("LightDir")->AsVector();
    g_pLightColorVariable = g_pEffect->GetVariableByName("LightColor")->AsVector();

    g_pEmissionVariable = g_pEffect->GetVariableByName("Emission")->AsVector();
    g_pAmbientVariable = g_pEffect->GetVariableByName("Ambient")->AsVector();
    g_pDiffuseVariable = g_pEffect->GetVariableByName("Diffuse")->AsVector();
    g_pSpecularVariable = g_pEffect->GetVariableByName("Specular")->AsVector();
    g_pShininessVariable = g_pEffect->GetVariableByName("Shininess")->AsScalar();

    g_pTextureChannelVariable = g_pEffect->GetVariableByName("TextureChannel")->AsVector();

    g_pTexEmissionVariable = g_pEffect->GetVariableByName("TexEmission")->AsShaderResource();
    g_pTexAmbientVariable = g_pEffect->GetVariableByName("TexAmbient")->AsShaderResource();
    g_pTexDiffuseVariable = g_pEffect->GetVariableByName("TexDiffuse")->AsShaderResource();
    g_pTexSpecularVariable = g_pEffect->GetVariableByName("TexSpecular")->AsShaderResource();

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
      return XMMatrixTranslationFromVector(transform.vector);
    case transform_type::ROTATE:
      return XMMatrixRotationNormal(transform.vector,
                                    XMConvertToRadians(XMVectorGetW(transform.vector)));
    case transform_type::SCALE:
      return XMMatrixScalingFromVector(transform.vector);
    default:
      assert(false);
      break;
    }
  }

  void scene_state::set_color_or_texture(color_or_texture const& color_or_texture,
                                         ID3D10EffectVectorVariable * pVectorVariable,
                                         ID3D10EffectShaderResourceVariable * pShaderResourceVariable)
  {
    switch (color_or_texture.type) {
    case color_or_texture_type::COLOR:
      pVectorVariable->SetFloatVector((float *)&color_or_texture.color);
      break;
    case color_or_texture_type::TEXTURE:
      pShaderResourceVariable->SetResource(m_pShaderResourceViews[color_or_texture.texture.image_index]);
      break;
    default:
      assert(false);
    }
  }

  void scene_state::set_material(effect const& effect)
  {
    switch (effect.type) {
    case effect_type::BLINN:
      set_color_or_texture(effect.blinn.emission, g_pEmissionVariable, g_pTexEmissionVariable);
      set_color_or_texture(effect.blinn.ambient, g_pAmbientVariable, g_pTexAmbientVariable);
      set_color_or_texture(effect.blinn.diffuse, g_pDiffuseVariable, g_pTexDiffuseVariable);
      set_color_or_texture(effect.blinn.specular, g_pSpecularVariable, g_pTexSpecularVariable);
      g_pShininessVariable->SetFloat(effect.blinn.shininess);
      break;
    case effect_type::LAMBERT:
      set_color_or_texture(effect.lambert.emission, g_pEmissionVariable, g_pTexEmissionVariable);
      set_color_or_texture(effect.lambert.ambient, g_pAmbientVariable, g_pTexAmbientVariable);
      set_color_or_texture(effect.lambert.diffuse, g_pDiffuseVariable, g_pTexDiffuseVariable);
      break;
    case effect_type::PHONG:
      set_color_or_texture(effect.phong.emission, g_pEmissionVariable, g_pTexEmissionVariable);
      set_color_or_texture(effect.phong.ambient, g_pAmbientVariable, g_pTexAmbientVariable);
      set_color_or_texture(effect.phong.diffuse, g_pDiffuseVariable, g_pTexDiffuseVariable);
      set_color_or_texture(effect.phong.specular, g_pSpecularVariable, g_pTexSpecularVariable);
      g_pShininessVariable->SetFloat(effect.phong.shininess);
      break;
    case effect_type::CONSTANT:
      g_pEmissionVariable->SetFloatVector((float *)&effect.constant.color);
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
        set_material(*instance_material.material->effect);
        int texture_channels[4] = {
          instance_material.emission.input_set,
          instance_material.ambient.input_set,
          instance_material.diffuse.input_set,
          instance_material.specular.input_set,
        };
        g_pTextureChannelVariable->SetIntVector(texture_channels);

        g_pTechniqueBlinn->GetPassByIndex(0)->Apply(0);
        g_pd3dDevice->IASetInputLayout(m_pVertexLayouts[triangles.inputs_index]);
        g_pd3dDevice->DrawIndexed(triangles.count * 3, triangles.index_offset, 0);
      }
    }
  }

  static inline float fract(float f)
  {
    return f - floorf(f);
  }

  static inline float loop(float f, float n)
  {
    return fract(f / n) * n;
  }

  static inline int find_frame_ix(source const& source, float t)
  {
    for (int i = 0; i < source.count - 1; i++) {
      if (source.float_array[i] <= t && source.float_array[i+1] > t) {
        return i;
      }
    }
    return -1;
  }

  static inline float linear_interpolate_iv(source const& source, int frame_ix, float t)
  {
    float prev = source.float_array[(frame_ix+0) * source.stride];
    float next = source.float_array[(frame_ix+1) * source.stride];
    return (t - prev) / (next - prev);
  }

  static inline float linear_interpolate_value(source const& source, int frame_ix, int parameter_ix, float iv)
  {
    float prev = source.float_array[(frame_ix+0) * source.stride + parameter_ix];
    float next = source.float_array[(frame_ix+1) * source.stride + parameter_ix];
    return prev + iv * (next - prev);
  }

  static inline float pow3(float f)
  {
    return f * f * f;
  }

  static inline float pow2(float f)
  {
    return f * f;
  }

  static inline XMVECTOR bezier(XMVECTOR p0, XMVECTOR c0, XMVECTOR c1, XMVECTOR p1, float s)
  {
    return
        p0 * pow3(1 - s)
      + 3 * c0 * s * pow2(1 - s)
      + 3 * c1 * pow2(s) * (1 - s)
      + p1 * pow3(s);
  }

  static inline float bezier_binary_search(XMVECTOR p0, XMVECTOR c0, XMVECTOR c1, XMVECTOR p1, float want)
  {
    float low = 0.0f;
    float high = 1.0f;

    int iterations = 0;
    while (iterations < 20) {
      iterations += 1;

      float s = (high + low) * 0.5f;
      XMVECTOR bs = bezier(p0, c0, c1, p1, s);
      float t = XMVectorGetX(bs);

      const float epsilon = 0.001f;
      if (fabsf(t - want) < epsilon) {
        return XMVectorGetY(bs);
      }

      if (t > want) {
        high = s;
      } else {
        low = s;
      }
    }

    print("%f %f\n", XMVectorGetX(p0), XMVectorGetY(p0));
    print("%f %f\n", XMVectorGetX(c0), XMVectorGetY(c0));
    print("%f %f\n", XMVectorGetX(c1), XMVectorGetY(c1));
    print("%f %f\n", XMVectorGetX(p1), XMVectorGetY(p1));
    assert(false);
  }

  static inline XMFLOAT2 const * tangent_index(source const& source, int frame_ix, int parameter_ix)
  {
    int ix = frame_ix * source.stride + parameter_ix * 2;
    return (XMFLOAT2 const *)&source.float_array[ix];
  }

  static float bezier_sampler(sampler const * const sampler, int frame_ix, int parameter_ix, float t)
  {
    /*
      P0 is (INPUT[i] , OUTPUT[i])
      C0 (or T0) is (OUT_TANGENT[i][0] , OUT_TANGENT[i][1])
      C1 (or T1) is (IN_TANGENT[i+1][0], IN_TANGENT[i+1][1])
      P1 is (INPUT[i+1], OUTPUT[i+1])
    */

    float frame0_input = sampler->input.float_array[frame_ix+0];
    float frame1_input = sampler->input.float_array[frame_ix+1];

    float frame0_output = sampler->output.float_array[(frame_ix+0) * sampler->output.stride + parameter_ix];
    float frame1_output = sampler->output.float_array[(frame_ix+1) * sampler->output.stride + parameter_ix];

    XMVECTOR p0 = XMVectorSet(frame0_input, frame0_output, 0, 0);
    XMVECTOR c0 = XMLoadFloat2(tangent_index(sampler->out_tangent, frame_ix + 0, parameter_ix));
    XMVECTOR c1 = XMLoadFloat2(tangent_index(sampler->in_tangent, frame_ix + 1, parameter_ix));
    XMVECTOR p1 = XMVectorSet(frame1_input, frame1_output, 0, 0);

    return bezier_binary_search(p0, c0, c1, p1, t);
  }

  static void apply_transform_target(transform& transform,
                                     enum target_attribute channel_target_attribute,
                                     float value)
  {
    switch (transform.type) {
    case transform_type::TRANSLATE: __attribute__((fallthrough));
    case transform_type::SCALE:
      switch (channel_target_attribute) {
      case target_attribute::X: transform.vector = XMVectorSetX(transform.vector, value); return;
      case target_attribute::Y: transform.vector = XMVectorSetY(transform.vector, value); return;
      case target_attribute::Z: transform.vector = XMVectorSetZ(transform.vector, value); return;
      default: assert(false);
      }
    case transform_type::ROTATE:
      switch (channel_target_attribute) {
      case target_attribute::X: transform.vector = XMVectorSetX(transform.vector, value); return;
      case target_attribute::Y: transform.vector = XMVectorSetY(transform.vector, value); return;
      case target_attribute::Z: transform.vector = XMVectorSetZ(transform.vector, value); return;
      case target_attribute::ANGLE: transform.vector = XMVectorSetW(transform.vector, value); return;
      default: assert(false);
      }
    default:
      assert(false);
      break;
    }
  }

  static enum target_attribute const rotate_target_attributes[] = {
    target_attribute::X,
    target_attribute::Y,
    target_attribute::Z,
    target_attribute::ANGLE,
  };

  static enum target_attribute const translate_scale_target_attributes[] = {
    target_attribute::X,
    target_attribute::Y,
    target_attribute::Z,
  };

  static void animate_channel_segment(channel const& channel,
                                      transform& transform,
                                      int frame_ix, float t)
  {
    enum target_attribute const * target_attributes = &channel.target_attribute;
    int target_attributes_count = 1;
    if (channel.target_attribute == target_attribute::ALL) {
      switch (transform.type) {
      case transform_type::TRANSLATE: __attribute__((fallthrough));
      case transform_type::SCALE:
        target_attributes = translate_scale_target_attributes;
        target_attributes_count = 3;
        break;
      case transform_type::ROTATE:
        target_attributes = rotate_target_attributes;
        target_attributes_count = 4;
        break;
      default:
        assert(false);
        break;
      }
    }

    for (int parameter_ix = 0; parameter_ix < target_attributes_count; parameter_ix++) {

      enum collada::interpolation interpolation = channel.source_sampler->interpolation.interpolation_array[frame_ix];

      float value;
      if (interpolation == interpolation::BEZIER) {
        value = bezier_sampler(channel.source_sampler, frame_ix, parameter_ix, t);
      } else {
        float iv = linear_interpolate_iv(channel.source_sampler->input, frame_ix, t);
        value = linear_interpolate_value(channel.source_sampler->output, frame_ix, parameter_ix, iv);
      }

      apply_transform_target(transform, target_attributes[parameter_ix], value);
    }
  }

  static void animate_node(node const& node, node_instance& node_instance, float t)
  {
    for (int i = 0; i < node.channels_count; i++) {
      channel const& channel = *node.channels[i];
      transform& transform = node_instance.transforms[channel.target_transform_index];

      int frame_ix = find_frame_ix(channel.source_sampler->input, t);
      assert(frame_ix >= 0); // animation is missing a key frame

      animate_channel_segment(channel, transform, frame_ix, t);
    }
  }

  void scene_state::node_world_transform(node const& node, node_instance& node_instance)
  {
    // build the node's world transform matrix
    if (node.parent_index >= 0)
      node_instance.world = m_nodeInstances[node.parent_index].world;
    else
      node_instance.world = XMMatrixIdentity();

    for (int j = 0; j < node.transforms_count; j++) {
      node_instance.world = TransformMatrix(node_instance.transforms[j]) * node_instance.world;
    }
  }

  void scene_state::update(float t)
  {
    t = loop(t / 2.0f, 3.333333f);

    // animate and update all world transforms
    for (int i = 0; i < m_descriptor->nodes_count; i++) {
      node const& node = *m_descriptor->nodes[i];
      node_instance& node_instance = m_nodeInstances[i];

      animate_node(node, node_instance, t);
      node_world_transform(node, node_instance);
    }

    // update all lights (after world transforms are computed)
    update_light_instances();
  }

  static int min(int a, int b)
  {
    return a < b ? a : b;
  }

  void scene_state::render()
  {
    XMFLOAT4 eye;
    XMFLOAT4X4 projection;
    XMFLOAT4X4 view;
    XMStoreFloat4(&eye, g_Eye);
    XMStoreFloat4x4(&view, g_View);
    XMStoreFloat4x4(&projection, g_Projection);

    g_pViewVariable->SetMatrix((float *)&view);
    g_pProjectionVariable->SetMatrix((float *)&projection);

    g_pViewEyeVariable->SetFloatVector((float *)&eye);

    int lights = min(2, m_lightInstancesCount);
    g_pLightPosVariable->SetFloatVectorArray((float *)m_lightPositions, 0, lights);
    g_pLightDirVariable->SetFloatVectorArray((float *)m_lightDirections, 0, lights);
    g_pLightColorVariable->SetFloatVectorArray((float *)m_lightColors, 0, lights);

    g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    for (int i = 0; i < m_descriptor->nodes_count; i++) {
      node const& node = *m_descriptor->nodes[i];
      node_instance& node_instance = m_nodeInstances[i];

      // joints aren't rendered
      if (node.type != node_type::NODE)
        continue;

      g_pWorldVariable->SetMatrix((float *)&node_instance.world);

      render_geometries(node.instance_geometries, node.instance_geometries_count);
    }
  }
}
