#include <windows.h>
#include <d3d10.h>

#include "directxmath/directxmath.h"

#include "print.hpp"
#include "collada.hpp"

extern ID3D10Device * g_pd3dDevice;
extern XMMATRIX g_View;
extern XMMATRIX g_Projection;

namespace collada {
  static const float vertex_buffer[] = {
    -5.0, -5.0, 0.0, 0.0, 0.0, -1.0, 1.0, 0.0, 0.0,
    -5.0, 5.0, 0.0, 0.0, 0.0, -1.0, 1.0, 1.0, 0.0,
    5.0, 5.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0,
    5.0, -5.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0,
    -5.0, -5.0, 20.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
    5.0, -5.0, 20.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0,
    5.0, 5.0, 20.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0,
    -5.0, 5.0, 20.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0,
    -5.0, -5.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0,
    5.0, -5.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0, 0.0,
    5.0, -5.0, 4.0, 0.0, -1.0, 0.0, 1.0, 0.2, 0.0,
    -5.0, -5.0, 4.0, 0.0, -1.0, 0.0, 0.0, 0.2, 0.0,
    5.0, -5.0, 8.0, 0.0, -1.0, 0.0, 1.0, 0.4, 0.0,
    -5.0, -5.0, 8.0, 0.0, -1.0, 0.0, 0.0, 0.4, 0.0,
    5.0, -5.0, 12.0, 0.0, -1.0, 0.0, 1.0, 0.6, 0.0,
    -5.0, -5.0, 12.0, 0.0, -1.0, 0.0, 0.0, 0.6, 0.0,
    5.0, -5.0, 16.0, 0.0, -1.0, 0.0, 1.0, 0.8, 0.0,
    -5.0, -5.0, 16.0, 0.0, -1.0, 0.0, 0.0, 0.8, 0.0,
    5.0, -5.0, 20.0, 0.0, -1.0, 0.0, 1.0, 1.0, 0.0,
    -5.0, -5.0, 20.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0,
    5.0, -5.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    5.0, 5.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
    5.0, 5.0, 4.0, 1.0, 0.0, 0.0, 1.0, 0.2, 0.0,
    5.0, -5.0, 4.0, 1.0, 0.0, 0.0, 0.0, 0.2, 0.0,
    5.0, 5.0, 8.0, 1.0, 0.0, 0.0, 1.0, 0.4, 0.0,
    5.0, -5.0, 8.0, 1.0, 0.0, 0.0, 0.0, 0.4, 0.0,
    5.0, 5.0, 12.0, 1.0, 0.0, 0.0, 1.0, 0.6, 0.0,
    5.0, -5.0, 12.0, 1.0, 0.0, 0.0, 0.0, 0.6, 0.0,
    5.0, 5.0, 16.0, 1.0, 0.0, 0.0, 1.0, 0.8, 0.0,
    5.0, -5.0, 16.0, 1.0, 0.0, 0.0, 0.0, 0.8, 0.0,
    5.0, 5.0, 20.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0,
    5.0, -5.0, 20.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0,
    5.0, 5.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
    -5.0, 5.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0,
    -5.0, 5.0, 4.0, 0.0, 1.0, 0.0, 1.0, 0.2, 0.0,
    5.0, 5.0, 4.0, 0.0, 1.0, 0.0, 0.0, 0.2, 0.0,
    -5.0, 5.0, 8.0, 0.0, 1.0, 0.0, 1.0, 0.4, 0.0,
    5.0, 5.0, 8.0, 0.0, 1.0, 0.0, 0.0, 0.4, 0.0,
    -5.0, 5.0, 12.0, 0.0, 1.0, 0.0, 1.0, 0.6, 0.0,
    5.0, 5.0, 12.0, 0.0, 1.0, 0.0, 0.0, 0.6, 0.0,
    -5.0, 5.0, 16.0, 0.0, 1.0, 0.0, 1.0, 0.8, 0.0,
    5.0, 5.0, 16.0, 0.0, 1.0, 0.0, 0.0, 0.8, 0.0,
    -5.0, 5.0, 20.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0,
    5.0, 5.0, 20.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
    -5.0, 5.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    -5.0, -5.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
    -5.0, -5.0, 4.0, -1.0, 0.0, 0.0, 1.0, 0.2, 0.0,
    -5.0, 5.0, 4.0, -1.0, 0.0, 0.0, 0.0, 0.2, 0.0,
    -5.0, -5.0, 8.0, -1.0, 0.0, 0.0, 1.0, 0.4, 0.0,
    -5.0, 5.0, 8.0, -1.0, 0.0, 0.0, 0.0, 0.4, 0.0,
    -5.0, -5.0, 12.0, -1.0, 0.0, 0.0, 1.0, 0.6, 0.0,
    -5.0, 5.0, 12.0, -1.0, 0.0, 0.0, 0.0, 0.6, 0.0,
    -5.0, -5.0, 16.0, -1.0, 0.0, 0.0, 1.0, 0.8, 0.0,
    -5.0, 5.0, 16.0, -1.0, 0.0, 0.0, 0.0, 0.8, 0.0,
    -5.0, -5.0, 20.0, -1.0, 0.0, 0.0, 1.0, 1.0, 0.0,
    -5.0, 5.0, 20.0, -1.0, 0.0, 0.0, 0.0, 1.0, 0.0,
  };

  static const int index_buffer[] = {
    0, 1, 2,
    2, 3, 0,
    4, 5, 6,
    6, 7, 4,
    8, 9, 10,
    10, 11, 8,
    11, 10, 12,
    12, 13, 11,
    13, 12, 14,
    14, 15, 13,
    15, 14, 16,
    16, 17, 15,
    17, 16, 18,
    18, 19, 17,
    20, 21, 22,
    22, 23, 20,
    23, 22, 24,
    24, 25, 23,
    25, 24, 26,
    26, 27, 25,
    27, 26, 28,
    28, 29, 27,
    29, 28, 30,
    30, 31, 29,
    32, 33, 34,
    34, 35, 32,
    35, 34, 36,
    36, 37, 35,
    37, 36, 38,
    38, 39, 37,
    39, 38, 40,
    40, 41, 39,
    41, 40, 42,
    42, 43, 41,
    44, 45, 46,
    46, 47, 44,
    47, 46, 48,
    48, 49, 47,
    49, 48, 50,
    50, 51, 49,
    51, 50, 52,
    52, 53, 51,
    53, 52, 54,
    54, 55, 53,
  };

  static const float inverse_bind_matrices[] = {
      0.0f,   0.0f,  -1.0f,   0.0f,
      0.0f,   1.0f,   0.0f,   0.0f,
      1.0f,   0.0f,   0.0f,   0.0f,
      0.0f,   0.0f,   0.0f,   1.0f,

      0.0f,   0.0f,  -1.0f,   0.0f,
      0.0f,   1.0f,   0.0f,   0.0f,
      1.0f,   0.0f,   0.0f,   0.0f,
    -10.0f,   0.0f,   0.0f,   1.0f,
  };

  static const float joints_weights[] = {
    0, 0, 0, 0, 1.0, 0, 0, 0,
    0, 0, 0, 0, 1.0, 0, 0, 0,
    0, 0, 0, 0, 1.0, 0, 0, 0,
    0, 0, 0, 0, 1.0, 0, 0, 0,
    1, 0, 0, 0, 1.0, 0, 0, 0,
    1, 0, 0, 0, 1.0, 0, 0, 0,
    1, 0, 0, 0, 1.0, 0, 0, 0,
    1, 0, 0, 0, 1.0, 0, 0, 0,
    0, 0, 0, 0, 1.0, 0, 0, 0,
    0, 0, 0, 0, 1.0, 0, 0, 0,
    1, 0, 0, 0, 0.2, 0.8, 0, 0,
    1, 0, 0, 0, 0.2, 0.8, 0, 0,
    1, 0, 0, 0, 0.4, 0.6, 0, 0,
    1, 0, 0, 0, 0.4, 0.6, 0, 0,
    1, 0, 0, 0, 0.6, 0.4, 0, 0,
    1, 0, 0, 0, 0.6, 0.4, 0, 0,
    1, 0, 0, 0, 0.8, 0.2, 0, 0,
    1, 0, 0, 0, 0.8, 0.2, 0, 0,
    1, 0, 0, 0, 1.0, 0, 0, 0,
    1, 0, 0, 0, 1.0, 0, 0, 0,
    0, 0, 0, 0, 1.0, 0, 0, 0,
    0, 0, 0, 0, 1.0, 0, 0, 0,
    1, 0, 0, 0, 0.2, 0.8, 0, 0,
    1, 0, 0, 0, 0.2, 0.8, 0, 0,
    1, 0, 0, 0, 0.4, 0.6, 0, 0,
    1, 0, 0, 0, 0.4, 0.6, 0, 0,
    1, 0, 0, 0, 0.6, 0.4, 0, 0,
    1, 0, 0, 0, 0.6, 0.4, 0, 0,
    1, 0, 0, 0, 0.8, 0.2, 0, 0,
    1, 0, 0, 0, 0.8, 0.2, 0, 0,
    1, 0, 0, 0, 1.0, 0, 0, 0,
    1, 0, 0, 0, 1.0, 0, 0, 0,
    0, 0, 0, 0, 1.0, 0, 0, 0,
    0, 0, 0, 0, 1.0, 0, 0, 0,
    1, 0, 0, 0, 0.2, 0.8, 0, 0,
    1, 0, 0, 0, 0.2, 0.8, 0, 0,
    1, 0, 0, 0, 0.4, 0.6, 0, 0,
    1, 0, 0, 0, 0.4, 0.6, 0, 0,
    1, 0, 0, 0, 0.6, 0.4, 0, 0,
    1, 0, 0, 0, 0.6, 0.4, 0, 0,
    1, 0, 0, 0, 0.8, 0.2, 0, 0,
    1, 0, 0, 0, 0.8, 0.2, 0, 0,
    1, 0, 0, 0, 1.0, 0, 0, 0,
    1, 0, 0, 0, 1.0, 0, 0, 0,
    0, 0, 0, 0, 1.0, 0, 0, 0,
    0, 0, 0, 0, 1.0, 0, 0, 0,
    1, 0, 0, 0, 0.2, 0.8, 0, 0,
    1, 0, 0, 0, 0.2, 0.8, 0, 0,
    1, 0, 0, 0, 0.4, 0.6, 0, 0,
    1, 0, 0, 0, 0.4, 0.6, 0, 0,
    1, 0, 0, 0, 0.6, 0.4, 0, 0,
    1, 0, 0, 0, 0.6, 0.4, 0, 0,
    1, 0, 0, 0, 0.8, 0.2, 0, 0,
    1, 0, 0, 0, 0.8, 0.2, 0, 0,
    1, 0, 0, 0, 1.0, 0, 0, 0,
    1, 0, 0, 0, 1.0, 0, 0, 0,
  };

  ID3D10Effect * g_pEffect = NULL;
  ID3D10EffectTechnique * g_pTechniqueRender = NULL;
  ID3D10InputLayout * g_pVertexLayout = NULL;

  ID3D10Buffer * g_pVertexBufferPNT;
  ID3D10Buffer * g_pVertexBufferJW;
  ID3D10Buffer * g_pIndexBuffer;

  ID3D10EffectMatrixVariable * g_pJointVariable = NULL;

  ID3D10EffectMatrixVariable * g_pWorldVariable = NULL;
  ID3D10EffectMatrixVariable * g_pViewVariable = NULL;
  ID3D10EffectMatrixVariable * g_pProjectionVariable = NULL;

  HRESULT LoadEffect()
  {
    HRESULT hr;

    HRSRC hRes = FindResource(NULL, L"RES_COLLADA_FXO", RT_RCDATA);
    if (hRes == NULL) {
      print("FindResource RES_COLLADA_FXO\n");
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
    g_pTechniqueRender = g_pEffect->GetTechniqueByName("Render");

    // variables
    g_pJointVariable = g_pEffect->GetVariableByName("mJoint")->AsMatrix();

    g_pWorldVariable = g_pEffect->GetVariableByName("World")->AsMatrix();
    g_pViewVariable = g_pEffect->GetVariableByName("View")->AsMatrix();
    g_pProjectionVariable = g_pEffect->GetVariableByName("Projection")->AsMatrix();

    // layout
    D3D10_INPUT_ELEMENT_DESC layout[] = {
      {"POSITION"    , 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0 , D3D10_INPUT_PER_VERTEX_DATA, 0},
      {"NORMAL"      , 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
      {"TEXCOORD"    , 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
      {"BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
      {"BLENDWEIGHT" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D10_INPUT_PER_VERTEX_DATA, 0},
    };
    UINT numElements = (sizeof (layout)) / (sizeof (layout[0]));

    D3D10_PASS_DESC passDesc;
    g_pTechniqueRender->GetPassByIndex(0)->GetDesc(&passDesc);

    hr = g_pd3dDevice->CreateInputLayout(layout, numElements,
					 passDesc.pIAInputSignature,
					 passDesc.IAInputSignatureSize,
					 &g_pVertexLayout);
    if (FAILED(hr)) {
      print("CreateInputLayout\n");
      return hr;
    }

    return S_OK;
  }

  HRESULT LoadMesh()
  {
    HRESULT hr;

    D3D10_BUFFER_DESC bd;
    D3D10_SUBRESOURCE_DATA initData;

    bd.Usage = D3D10_USAGE_IMMUTABLE;
    bd.ByteWidth = (sizeof (vertex_buffer));
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    initData.pSysMem = vertex_buffer;
    hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pVertexBufferPNT);
    if (FAILED(hr)) {
      print("CreateBuffer: D3D10_BIND_VERTEX_BUFFER\n");
      return hr;
    }

    bd.Usage = D3D10_USAGE_IMMUTABLE;
    bd.ByteWidth = (sizeof (joints_weights));
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    initData.pSysMem = joints_weights;
    hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pVertexBufferJW);
    if (FAILED(hr)) {
      print("CreateBuffer: D3D10_BIND_VERTEX_BUFFER\n");
      return hr;
    }

    bd.Usage = D3D10_USAGE_IMMUTABLE;
    bd.ByteWidth = (sizeof (index_buffer));
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    initData.pSysMem = index_buffer;
    hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pIndexBuffer);
    if (FAILED(hr)) {
      print("CreateBuffer: D3D10_BIND_INDEX_BUFFER\n");
      return hr;
    }

    return S_OK;
  }

  void Render(float t)
  {
    XMMATRIX World = XMMatrixScaling(1, 1, 1);

    // XMMATRIX    XM_CALLCONV     XMMatrixRotationNormal(FXMVECTOR NormalAxis, float Angle) noexcept;
    //XMConvertToRadians
    XMVECTOR axis = XMVectorSet(0, 1, 0, 0);
    XMVECTOR axisZ = XMVectorSet(0, 0, 1, 0);
    XMMATRIX joint0 = XMMatrixScaling(1, 1, 1);
    //joint0 = XMMatrixTranslation(10, 0, 0) * joint0;
    joint0 = XMMatrixRotationNormal(axis, XMConvertToRadians(-90)) * joint0;

    XMMATRIX joint1 = XMMatrixScaling(1, 1, 1);

    /*
    joint1 = joint1 * XMMatrixRotationNormal(axisZ, sin(t));
    joint1 = joint1 * XMMatrixTranslation(10, 0, 0);
    joint1 = joint1 * joint0;
    */

    joint1 = joint0 * joint1;
    joint1 = XMMatrixTranslation(10, 0, 0) * joint1;
    joint1 = XMMatrixRotationNormal(axisZ, sin(t)) * joint1;
    /*
      = XMMatrixRotationNormal(axisZ, sin(t)) * XMMatrixTranslation(10, 0, 0) * joint0
      ;
    */
    XMMATRIX joint0ibm = XMLoadFloat4x4((XMFLOAT4X4*)&inverse_bind_matrices[0 * 16]);
    XMMATRIX joint1ibm = XMLoadFloat4x4((XMFLOAT4X4*)&inverse_bind_matrices[1 * 16]);

    XMFLOAT4X4 foo;
    XMStoreFloat4x4(&foo, joint0);
    print("  %.1f %.1f %.1f %.1f\n", foo._11, foo._12, foo._13, foo._14);
    print("  %.1f %.1f %.1f %.1f\n", foo._21, foo._22, foo._23, foo._24);
    print("  %.1f %.1f %.1f %.1f\n", foo._31, foo._32, foo._33, foo._34);
    print("  %.1f %.1f %.1f %.1f\n", foo._41, foo._42, foo._43, foo._44);
    print("\n");

    XMStoreFloat4x4(&foo, joint1);
    print("joint1ibm:\n");
    print("  %.1f %.1f %.1f %.1f\n", foo._11, foo._12, foo._13, foo._14);
    print("  %.1f %.1f %.1f %.1f\n", foo._21, foo._22, foo._23, foo._24);
    print("  %.1f %.1f %.1f %.1f\n", foo._31, foo._32, foo._33, foo._34);
    print("  %.1f %.1f %.1f %.1f\n", foo._41, foo._42, foo._43, foo._44);
    print("\n");
    exit(0);

    XMMATRIX mJoints[2] = {
      joint0ibm * joint0,
      joint1ibm * joint1,
    };

    g_pJointVariable->SetMatrixArray((float *)mJoints, 0, 2);

    g_pWorldVariable->SetMatrix((float *)&World);
    g_pViewVariable->SetMatrix((float *)&g_View);
    g_pProjectionVariable->SetMatrix((float *)&g_Projection);

    UINT stride[] = {
      3 * 3 * 4, // position normal texture
      4 * 2 * 4, // joint weight
    };
    UINT offset[] = { 0, 0 };
    ID3D10Buffer * vertex_buffers[] = {
      g_pVertexBufferPNT,
      g_pVertexBufferJW,
    };

    g_pd3dDevice->IASetInputLayout(g_pVertexLayout);
    g_pd3dDevice->IASetVertexBuffers(0, 2, vertex_buffers, stride, offset);
    g_pd3dDevice->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D10_TECHNIQUE_DESC techDesc;
    g_pTechniqueRender->GetDesc(&techDesc);

    int indices_length = (sizeof (index_buffer)) / (sizeof (int));
    for (UINT p = 0; p < techDesc.Passes; p++) {
      g_pTechniqueRender->GetPassByIndex(p)->Apply(0);
      g_pd3dDevice->DrawIndexed(indices_length, 0, 0);
    }
  }
}
