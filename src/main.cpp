#include <windows.h>
#include <d3d10.h>
#include <assert.h>
#include <strsafe.h>

#include "directxmath/directxmath.h"

#include "globals.hpp"
#include "print.hpp"
#include "render_state.hpp"
#include "input.hpp"

#include "gltf.hpp"
#include "gltf_instance.hpp"

#include "robot_player.hpp"
#define ROOT_MESH_NODE robot_player::node_39
#include "cube.hpp"

HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;
ID3D10Device * g_pd3dDevice = NULL;
IDXGISwapChain * g_pSwapChain = NULL;
ID3D10Texture2D * g_pDepthStencil = NULL;
ID3D10RenderTargetView * g_pRenderTargetView = NULL;
ID3D10DepthStencilView * g_pDepthStencilView = NULL;

// mesh

ID3D10Effect * g_pEffect = NULL;
ID3D10EffectTechnique * g_pTechniqueRender = NULL;
ID3D10InputLayout * g_pVertexLayout = NULL;
ID3D10Buffer * g_pIndexBuffer = NULL;
const DWORD g_dwVertexBufferCount = 5;
ID3D10Buffer * g_pVertexBuffers[g_dwVertexBufferCount];
ID3D10ShaderResourceView * g_pTextureShaderResourceView = NULL;

ID3D10EffectMatrixVariable * g_pWorldVariable = NULL;
ID3D10EffectMatrixVariable * g_pViewVariable = NULL;
ID3D10EffectMatrixVariable * g_pProjectionVariable = NULL;
ID3D10EffectMatrixVariable * g_pJointVariable = NULL;
ID3D10EffectVectorVariable * g_pLightDirVariable = NULL;
ID3D10EffectVectorVariable * g_pLightColorVariable = NULL;
ID3D10EffectShaderResourceVariable * g_pDiffuseVariable = NULL;
XMMATRIX g_World1;
XMMATRIX g_World2;
XMMATRIX g_View;
XMMATRIX g_Projection;

// bloom
ID3D10RenderTargetView * g_pRenderTargetViewTexture[2] = { NULL, NULL };
ID3D10ShaderResourceView * g_pRenderTargetShaderResourceViewTexture[2] = { NULL, NULL };

ID3D10Effect * g_pEffectBloom = NULL;
ID3D10EffectTechnique * g_pTechniqueBloom = NULL;
ID3D10EffectTechnique * g_pTechniqueBloomBlend = NULL;
ID3D10InputLayout * g_pVertexLayoutBloom = NULL;
const DWORD g_dwVertexBufferCountBloom = 1;
ID3D10Buffer * g_pVertexBuffersBloom[g_dwVertexBufferCountBloom];
ID3D10EffectShaderResourceVariable * g_pDiffuseAVariableBloom = NULL;
ID3D10EffectVectorVariable * g_pInvScreenSizeVariableBloom = NULL;
ID3D10EffectVectorVariable * g_pDirVariableBloom = NULL;
ID3D10EffectScalarVariable * g_pExposureVariableBloom = NULL;
int g_bloomPasses = 4;
float g_exposure = 3.4f;

typedef XMFLOAT2 BloomVertex;

// font
ID3D10Effect * g_pEffectFont = NULL;
ID3D10EffectTechnique * g_pTechniqueFont = NULL;
ID3D10InputLayout * g_pVertexLayoutFont = NULL;
const DWORD g_dwVertexBufferCountFont = 1;
ID3D10Buffer * g_pVertexBuffersFont[g_dwVertexBufferCountFont];
ID3D10ShaderResourceView * g_pTextureShaderResourceViewFont = NULL;
ID3D10EffectVectorVariable * g_pInvScreenSizeVariableFont = NULL;
ID3D10EffectVectorVariable * g_pGlyphScaleVariableFont = NULL;
ID3D10EffectVectorVariable * g_pTexScaleVariableFont = NULL;
ID3D10EffectShaderResourceVariable * g_pDiffuseVariableFont = NULL;

const int g_iFontBufferLength = 512;
typedef XMFLOAT4 FontVertex;

// perlin
ID3D10Effect * g_pEffectVolume = NULL;
ID3D10EffectTechnique * g_pTechniqueVolume = NULL;
ID3D10InputLayout * g_pVertexLayoutVolume = NULL;
const DWORD g_dwVertexBufferCountVolume = 1;
ID3D10Buffer * g_pVertexBuffersVolume[g_dwVertexBufferCountVolume];
ID3D10ShaderResourceView * g_pTextureShaderResourceViewPerlin = NULL;
ID3D10EffectScalarVariable * g_pLayerVariableVolume = NULL;
ID3D10EffectShaderResourceVariable * g_pDiffuseVariableVolume = NULL;

// static
ID3D10Effect * g_pEffectStatic = NULL;
ID3D10EffectTechnique * g_pTechniqueStatic = NULL;
ID3D10EffectTechnique * g_pTechniqueStaticInstanced = NULL;
ID3D10InputLayout * g_pVertexLayoutStatic = NULL;
ID3D10InputLayout * g_pVertexLayoutStaticInstanced = NULL;

ID3D10EffectMatrixVariable * g_pWorldVariableStatic = NULL;
ID3D10EffectMatrixVariable * g_pViewVariableStatic = NULL;
ID3D10EffectMatrixVariable * g_pProjectionVariableStatic = NULL;
ID3D10EffectMatrixVariable * g_pWorldNormalVariableStatic = NULL;
ID3D10EffectVectorVariable * g_pOutputColorVariableStatic = NULL;
ID3D10EffectShaderResourceVariable * g_pDiffuseVariableStatic = NULL;

// instance data
ID3D10Buffer * g_pVolumeMeshInstanceData = NULL;
const int g_iMeshInstanceDim = 20;
const int g_iNumVolumeMeshInstances = g_iMeshInstanceDim * g_iMeshInstanceDim * g_iMeshInstanceDim;

// cube
const DWORD g_dwVertexBufferCountCube = 3;
ID3D10Buffer * g_pVertexBufferCube[g_dwVertexBufferCountCube];
ID3D10Buffer * g_pIndexBufferCube = NULL;

// lights
XMFLOAT4 g_vLightDirs[2];
XMFLOAT4 g_vLightColors[2] = {
  XMFLOAT4(0.0f, 0.9f, 0.9f, 1.0f),
  XMFLOAT4(0.9f, 0.0f, 0.0f, 1.0f)
};

//

XMVECTOR g_Eye = XMVectorSet(0.0f, 0.0f, -2.0f, 1.0f);
XMVECTOR g_At = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

// forward declarations

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT InitDirect3DDevice();
void Render(float t);
void Update(float t);
BOOL Resize();
void InitializeNodeInstances();

struct WindowSize {
  UINT Width;
  UINT Height;
};

struct FontSize {
  struct {
    UINT Width;
    UINT Height;
  } Glyph;
  struct {
    UINT Width;
    UINT Height;
  } Texture;
};

const FontSize g_FontSize = {
  { 6, 12 },
  { 128, 64 },
};

WindowSize g_ViewportSize;

float GetTime()
{
  static DWORD dwTimeStart = 0;
  DWORD dwTimeCur = GetTickCount();
  if (dwTimeStart == 0)
    dwTimeStart = dwTimeCur;
  float t = (dwTimeCur - dwTimeStart) / 1000.0f;

  return t;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
  //FreeConsole();
  //AllocConsole();
  //AttachConsole(GetCurrentProcessId());
  if (FAILED(InitWindow(hInstance, nCmdShow))) {
    print("InitWindow\n");
    return 0;
  }

  if (FAILED(InitDirect3DDevice())) {
    print("InitDirect3DDevice\n");
    return 0;
  }

  InitializeNodeInstances();

  if (FAILED(InitInput(hInstance))) {
    print("InitInput\n");
    return 0;
  }

  MSG msg = {};
  while (msg.message != WM_QUIT) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_KEYDOWN) {
      } else if (msg.message == WM_CHAR) {
        switch (msg.wParam) {
        case 'q': g_bloomPasses -= 1; break;
        case 'w': g_bloomPasses += 1; break;

        case 'a': g_exposure -= 0.1f; break;
        case 's': g_exposure += 0.1f; break;

        case 'z': g_exposure -= 0.5f; break;
        case 'x': g_exposure += 0.5f; break;

        default: break;
        }
      }
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } else {
      if (Resize()) {
        UpdateInput();
        float t = GetTime();
        Update(t);
        Render(t);
      }
    }
  }

  return 0;
}

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
  g_hInstance = hInstance;

  // window class
  WNDCLASSEX wcex = {};
  wcex.cbSize = (sizeof (WNDCLASSEX));
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.hInstance = hInstance;
  wcex.lpszClassName = L"d3d10wc";
  if (!RegisterClassEx(&wcex))
    return E_FAIL;

  // create window
  RECT rc = { 0, 0, 512, 512 };
  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;
  g_hWnd = CreateWindow(L"d3d10wc",
                        L"d3d10", WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        width,
                        height,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);
  if(!g_hWnd)
    return E_FAIL;

  ShowWindow(g_hWnd, nCmdShow);

  return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;

  switch(message) {
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    break;

  case WM_DESTROY:
    PostQuitMessage( 0 );
    break;

  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
}

HRESULT InitDirect3DViews()
{
  HRESULT hr;

  // back buffer
  ID3D10Texture2D * pBackBuffer;
  hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID *)&pBackBuffer);
  if (FAILED(hr)) {
    print("g_pSwapChain->GetBuffer\n");
    return hr;
  }

  D3D10_TEXTURE2D_DESC backBufferSurfaceDesc;
  pBackBuffer->GetDesc(&backBufferSurfaceDesc);

  hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
  pBackBuffer->Release();
  if (FAILED(hr)) {
    print("g_pd3dDevice->CreateRenderTargetView(pBackBuffer)\n");
    return hr;
  }

  // depth buffer
  D3D10_TEXTURE2D_DESC descDepth;
  descDepth.Width = backBufferSurfaceDesc.Width;
  descDepth.Height = backBufferSurfaceDesc.Height;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = DXGI_FORMAT_D32_FLOAT;
  descDepth.SampleDesc.Count = 1;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = D3D10_USAGE_DEFAULT;
  descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
  descDepth.CPUAccessFlags = 0;
  descDepth.MiscFlags = 0;

  hr = g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
  if (FAILED(hr)) {
    print("CreateTexture2D\n");
    return hr;
  }

  D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
  descDSV.Format = descDepth.Format;
  descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;

  hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
  if (FAILED(hr)) {
    print("CreateDepthStencilView\n");
    return hr;
  }

  // intermediate render targets
  for (int i = 0; i < 2; i++) {
    hr = CreateTextureRenderTargetView(backBufferSurfaceDesc.Width,
                                       backBufferSurfaceDesc.Height,
                                       &g_pRenderTargetViewTexture[i],
                                       &g_pRenderTargetShaderResourceViewTexture[i]);
    if (FAILED(hr)) {
      return hr;
    }
  }

  // viewport
  D3D10_VIEWPORT vp;
  vp.Width = backBufferSurfaceDesc.Width;
  vp.Height = backBufferSurfaceDesc.Height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  g_pd3dDevice->RSSetViewports(1, &vp);

  g_ViewportSize.Width = backBufferSurfaceDesc.Width;
  g_ViewportSize.Height = backBufferSurfaceDesc.Height;

  return true;
}

HRESULT LoadMesh()
{
  HRESULT hr;

  //////////////////////////////////////////////////////////////////////
  // vertex buffers
  //////////////////////////////////////////////////////////////////////
  D3D10_BUFFER_DESC bd;
  D3D10_SUBRESOURCE_DATA initData;

  const Mesh * mesh = ROOT_MESH_NODE.mesh;

  // position
  bd.Usage = D3D10_USAGE_IMMUTABLE;
  bd.ByteWidth = mesh->position_size;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = mesh->position;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pVertexBuffers[0]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  // normals
  bd.Usage = D3D10_USAGE_IMMUTABLE;
  bd.ByteWidth = mesh->normal_size;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = mesh->normal;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pVertexBuffers[1]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  // texcoords
  bd.Usage = D3D10_USAGE_IMMUTABLE;
  bd.ByteWidth = mesh->texcoord_0_size;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = mesh->texcoord_0;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pVertexBuffers[2]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  // weights
  bd.Usage = D3D10_USAGE_IMMUTABLE;
  bd.ByteWidth = mesh->weights_0_size;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = mesh->weights_0;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pVertexBuffers[3]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  // joints
  bd.Usage = D3D10_USAGE_IMMUTABLE;
  bd.ByteWidth = mesh->joints_0_size;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = mesh->joints_0;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pVertexBuffers[4]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  //////////////////////////////////////////////////////////////////////
  // index buffer
  //////////////////////////////////////////////////////////////////////

  bd.Usage = D3D10_USAGE_IMMUTABLE;
  bd.ByteWidth = mesh->indices_size;
  bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = mesh->indices;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pIndexBuffer);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  //////////////////////////////////////////////////////////////////////
  // textures
  //////////////////////////////////////////////////////////////////////

  hr = LoadTexture2D(L"RES_ROBOT_PLAYER",
                     64,     // width
                     64,     // height
                     64 * 4, // pitch
                     DXGI_FORMAT_R8G8B8A8_UNORM,
                     &g_pTextureShaderResourceView);
  if (FAILED(hr)) {
    print("LoadTexture2D\n");
    return hr;
  }

  hr = LoadTexture2D(L"RES_FONT_TERMINUS_6X12",
                     g_FontSize.Texture.Width,     // width
                     g_FontSize.Texture.Height,    // height
                     g_FontSize.Texture.Width * 1, // pitch
                     DXGI_FORMAT_R8_UNORM,
                     &g_pTextureShaderResourceViewFont);
  if (FAILED(hr)) {
    print("LoadTexture2D\n");
    return hr;
  }

  hr = LoadTexture3D(L"RES_PERLIN",
                     256, // width
                     256, // height
                     256, // depth
                     256 * 1, // pitch
                     256 * 256 * 1, // slicePitch
                     DXGI_FORMAT_R8_UNORM,
                     &g_pTextureShaderResourceViewPerlin);
  if (FAILED(hr)) {
    print("LoadTexture2D\n");
    return hr;
  }

  return S_OK;
}

HRESULT InitFontBuffers()
{
  HRESULT hr;

  //////////////////////////////////////////////////////////////////////
  // effect
  //////////////////////////////////////////////////////////////////////

  HRSRC hRes = FindResource(NULL, L"RES_FONT_FXO", RT_RCDATA);
  if (hRes == NULL) {
    print("FindResource RES_FONT_FXO\n");
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
                                   &g_pEffectFont
                                   );
  if (FAILED(hr)) {
    print("D3D10CreateEffectFromMemory\n");
    return hr;
  }

  g_pTechniqueFont = g_pEffectFont->GetTechniqueByName("Font");
  g_pInvScreenSizeVariableFont = g_pEffectFont->GetVariableByName("vInvScreenSize")->AsVector();
  g_pGlyphScaleVariableFont = g_pEffectFont->GetVariableByName("vGlyphScale")->AsVector();
  g_pTexScaleVariableFont = g_pEffectFont->GetVariableByName("vTexScale")->AsVector();
  g_pDiffuseVariableFont = g_pEffectFont->GetVariableByName("txDiffuse")->AsShaderResource();

  //////////////////////////////////////////////////////////////////////
  // layout
  //////////////////////////////////////////////////////////////////////

  D3D10_INPUT_ELEMENT_DESC layout[] = {
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0 , D3D10_INPUT_PER_VERTEX_DATA, 0},
  };
  UINT numElements = (sizeof (layout)) / (sizeof (layout[0]));

  D3D10_PASS_DESC passDesc;
  g_pTechniqueFont->GetPassByIndex(0)->GetDesc(&passDesc);

  hr = g_pd3dDevice->CreateInputLayout(layout, numElements,
                                       passDesc.pIAInputSignature,
                                       passDesc.IAInputSignatureSize,
                                       &g_pVertexLayoutFont);
  if (FAILED(hr)) {
    print("CreateInputLayout\n");
    return hr;
  }

  //////////////////////////////////////////////////////////////////////
  // vertex buffers
  //////////////////////////////////////////////////////////////////////

  D3D10_BUFFER_DESC bd;

  // position
  bd.Usage = D3D10_USAGE_DYNAMIC;
  bd.ByteWidth = (sizeof (FontVertex)) * g_iFontBufferLength;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
  bd.MiscFlags = 0;

  hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffersFont[0]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  return S_OK;
}

HRESULT InitVolumeBuffers()
{
  HRESULT hr;

  //////////////////////////////////////////////////////////////////////
  // effect
  //////////////////////////////////////////////////////////////////////

  HRSRC hRes = FindResource(NULL, L"RES_VOLUME_FXO", RT_RCDATA);
  if (hRes == NULL) {
    print("FindResource RES_VOLUME_FXO\n");
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
                                   &g_pEffectVolume
                                   );
  if (FAILED(hr)) {
    print("D3D10CreateEffectFromMemory\n");
    return hr;
  }

  g_pTechniqueVolume = g_pEffectVolume->GetTechniqueByName("Volume");

  g_pLayerVariableVolume = g_pEffectVolume->GetVariableByName("layer")->AsScalar();
  g_pDiffuseVariableVolume = g_pEffectVolume->GetVariableByName("txDiffuse")->AsShaderResource();

  //////////////////////////////////////////////////////////////////////
  // layout
  //////////////////////////////////////////////////////////////////////

  D3D10_INPUT_ELEMENT_DESC layout[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0 , D3D10_INPUT_PER_VERTEX_DATA, 0},
  };
  UINT numElements = (sizeof (layout)) / (sizeof (layout[0]));

  D3D10_PASS_DESC passDesc;
  g_pTechniqueVolume->GetPassByIndex(0)->GetDesc(&passDesc);

  hr = g_pd3dDevice->CreateInputLayout(layout, numElements,
                                       passDesc.pIAInputSignature,
                                       passDesc.IAInputSignatureSize,
                                       &g_pVertexLayoutVolume);
  if (FAILED(hr)) {
    print("CreateInputLayout\n");
    return hr;
  }

  //////////////////////////////////////////////////////////////////////
  // vertex buffers
  //////////////////////////////////////////////////////////////////////

  D3D10_BUFFER_DESC bd;
  D3D10_SUBRESOURCE_DATA initData;

  const XMFLOAT2 position[] = {
    XMFLOAT2(-1,  1),
    XMFLOAT2( 1,  1),
    XMFLOAT2(-1, -1),
    XMFLOAT2( 1, -1),
  };

  // position
  bd.Usage = D3D10_USAGE_DEFAULT;
  bd.ByteWidth = (sizeof (XMFLOAT2)) * 4;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = position;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pVertexBuffersVolume[0]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  return S_OK;
}

// +Y is up
XMFLOAT2 vtxBloom[] = {
  XMFLOAT2(-1, -1), // top left
  XMFLOAT2(-1,  1), // top right
  XMFLOAT2( 1, -1), // bottom left
  XMFLOAT2( 1,  1), // bottom right
};

HRESULT InitBloomBuffers()
{
  HRESULT hr;

  //////////////////////////////////////////////////////////////////////
  // effect
  //////////////////////////////////////////////////////////////////////

  HRSRC hRes = FindResource(NULL, L"RES_BLOOM_FXO", RT_RCDATA);
  if (hRes == NULL) {
    print("FindResource RES_BLOOM_FXO\n");
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
                                   &g_pEffectBloom
                                   );
  if (FAILED(hr)) {
    print("D3D10CreateEffectFromMemory\n");
    return hr;
  }

  g_pTechniqueBloom = g_pEffectBloom->GetTechniqueByName("Bloom");
  g_pTechniqueBloomBlend = g_pEffectBloom->GetTechniqueByName("BloomBlend");
  g_pInvScreenSizeVariableBloom = g_pEffectBloom->GetVariableByName("vInvScreenSize")->AsVector();
  g_pDirVariableBloom = g_pEffectBloom->GetVariableByName("vDir")->AsVector();
  g_pExposureVariableBloom = g_pEffectBloom->GetVariableByName("fExposure")->AsScalar();
  g_pDiffuseAVariableBloom = g_pEffectBloom->GetVariableByName("txDiffuseA")->AsShaderResource();

  //////////////////////////////////////////////////////////////////////
  // layout
  //////////////////////////////////////////////////////////////////////

  D3D10_INPUT_ELEMENT_DESC layout[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0 , D3D10_INPUT_PER_VERTEX_DATA, 0},
  };
  UINT numElements = (sizeof (layout)) / (sizeof (layout[0]));

  D3D10_PASS_DESC passDesc;
  g_pTechniqueBloom->GetPassByIndex(0)->GetDesc(&passDesc);

  hr = g_pd3dDevice->CreateInputLayout(layout, numElements,
                                       passDesc.pIAInputSignature,
                                       passDesc.IAInputSignatureSize,
                                       &g_pVertexLayoutBloom);
  if (FAILED(hr)) {
    print("CreateInputLayout\n");
    return hr;
  }

  //////////////////////////////////////////////////////////////////////
  // vertex buffers
  //////////////////////////////////////////////////////////////////////

  D3D10_BUFFER_DESC bd;
  D3D10_SUBRESOURCE_DATA initData;

  // position
  bd.Usage = D3D10_USAGE_IMMUTABLE;
  bd.ByteWidth = (sizeof (vtxBloom));
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = vtxBloom;

  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pVertexBuffersBloom[0]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  return S_OK;
}

HRESULT InitStaticEffect()
{
  HRESULT hr;

  //////////////////////////////////////////////////////////////////////
  // effect
  //////////////////////////////////////////////////////////////////////

  HRSRC hRes = FindResource(NULL, L"RES_STATIC_FXO", RT_RCDATA);
  if (hRes == NULL) {
    print("FindResource RES_STATIC_FXO\n");
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
                                   &g_pEffectStatic
                                   );
  if (FAILED(hr)) {
    print("D3D10CreateEffectFromMemory\n");
    return hr;
  }

  g_pTechniqueStatic = g_pEffectStatic->GetTechniqueByName("Static");
  g_pTechniqueStaticInstanced = g_pEffectStatic->GetTechniqueByName("StaticInstanced");


  //////////////////////////////////////////////////////////////////////
  // layout
  //////////////////////////////////////////////////////////////////////

  // static
  {
    D3D10_INPUT_ELEMENT_DESC layout[] = {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 , D3D10_INPUT_PER_VERTEX_DATA, 0},
      {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0 , D3D10_INPUT_PER_VERTEX_DATA, 0},
      {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    2, 0 , D3D10_INPUT_PER_VERTEX_DATA, 0},
    };
    UINT numElements = (sizeof (layout)) / (sizeof (layout[0]));

    D3D10_PASS_DESC passDesc;
    g_pTechniqueStatic->GetPassByIndex(0)->GetDesc(&passDesc);

    hr = g_pd3dDevice->CreateInputLayout(layout, numElements,
                                         passDesc.pIAInputSignature,
                                         passDesc.IAInputSignatureSize,
                                         &g_pVertexLayoutStatic);
    if (FAILED(hr)) {
      print("CreateInputLayout\n");
      return hr;
    }
  }

  // static instanced
  {
    D3D10_INPUT_ELEMENT_DESC layout[] = {
      {"POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0 , D3D10_INPUT_PER_VERTEX_DATA,   0},
      {"NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,    1, 0 , D3D10_INPUT_PER_VERTEX_DATA,   0},
      {"TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,       2, 0 , D3D10_INPUT_PER_VERTEX_DATA,   0},
      {"mTransform", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 0 , D3D10_INPUT_PER_INSTANCE_DATA, 1},
      {"mTransform", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 16, D3D10_INPUT_PER_INSTANCE_DATA, 1},
      {"mTransform", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 32, D3D10_INPUT_PER_INSTANCE_DATA, 1},
      {"mTransform", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 48, D3D10_INPUT_PER_INSTANCE_DATA, 1},
    };
    UINT numElements = (sizeof (layout)) / (sizeof (layout[0]));

    D3D10_PASS_DESC passDesc;
    g_pTechniqueStaticInstanced->GetPassByIndex(0)->GetDesc(&passDesc);

    hr = g_pd3dDevice->CreateInputLayout(layout, numElements,
                                         passDesc.pIAInputSignature,
                                         passDesc.IAInputSignatureSize,
                                         &g_pVertexLayoutStaticInstanced);
    if (FAILED(hr)) {
      print("CreateInputLayout\n");
      return hr;
    }
  }

  //////////////////////////////////////////////////////////////////////
  // variables
  //////////////////////////////////////////////////////////////////////

  g_pWorldVariableStatic = g_pEffectStatic->GetVariableByName("World")->AsMatrix();
  g_pViewVariableStatic = g_pEffectStatic->GetVariableByName("View")->AsMatrix();
  g_pProjectionVariableStatic = g_pEffectStatic->GetVariableByName("Projection")->AsMatrix();
  g_pWorldNormalVariableStatic = g_pEffectStatic->GetVariableByName("WorldNormal")->AsMatrix();
  g_pOutputColorVariableStatic = g_pEffectStatic->GetVariableByName("vOutputColor")->AsVector();
  g_pDiffuseVariableStatic = g_pEffectStatic->GetVariableByName("txDiffuse")->AsShaderResource();

  return S_OK;
}

static inline float lerp(float a, float b, float t) {
  return a + t * (b - a);
}

HRESULT InitInstancedVertexBuffer()
{
  HRESULT hr;

  //////////////////////////////////////////////////////////////////////
  // vertex buffers
  //////////////////////////////////////////////////////////////////////

  D3D10_BUFFER_DESC bd;

  // mTransform
  bd.Usage = D3D10_USAGE_DYNAMIC;
  bd.ByteWidth = (sizeof (XMMATRIX)) * g_iNumVolumeMeshInstances;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
  bd.MiscFlags = 0;

  hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pVolumeMeshInstanceData);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  //////////////////////////////////////////////////////////////////////
  // vertex buffer data
  //////////////////////////////////////////////////////////////////////

  FontVertex * pData;
  hr = g_pVolumeMeshInstanceData->Map(D3D10_MAP_WRITE_DISCARD,
                                      0,
                                      (void **)&pData);
  if (FAILED(hr)) {
    print("g_pVolumeMeshInstanceData->Map");
  }

  XMMATRIX transforms[g_iNumVolumeMeshInstances];
  const float dim = 1.0f / (float)(g_iMeshInstanceDim - 1);
  int ix = 0;
  for (int z = 0; z < g_iMeshInstanceDim; z++) {
    for (int y = 0; y < g_iMeshInstanceDim; y++) {
      for (int x = 0; x < g_iMeshInstanceDim; x++) {
        float fx = (float)x * dim;
        float fy = (float)y * dim;
        float fz = (float)z * dim;

        float px = lerp(-1.0f, 1.0f, fx);
        float py = lerp(-1.0f, 1.0f, fy);
        float pz = lerp(-1.0f, 1.0f, fz);

        transforms[ix++] = XMMatrixTranslation(px, py, pz);
      }
    }
  }

  memcpy(pData, transforms, (sizeof (XMMATRIX)) * g_iNumVolumeMeshInstances);

  g_pVolumeMeshInstanceData->Unmap();

  return S_OK;
}

HRESULT LoadMeshStatic(const Mesh * mesh)
{
  HRESULT hr;

  //////////////////////////////////////////////////////////////////////
  // vertex buffers
  //////////////////////////////////////////////////////////////////////

  D3D10_BUFFER_DESC bd;
  D3D10_SUBRESOURCE_DATA initData;

  // position
  bd.Usage = D3D10_USAGE_IMMUTABLE;
  bd.ByteWidth = mesh->position_size;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = mesh->position;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pVertexBufferCube[0]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  // normals
  bd.Usage = D3D10_USAGE_IMMUTABLE;
  bd.ByteWidth = mesh->normal_size;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = mesh->normal;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pVertexBufferCube[1]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  // texcoords
  bd.Usage = D3D10_USAGE_IMMUTABLE;
  bd.ByteWidth = mesh->texcoord_0_size;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = mesh->texcoord_0;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pVertexBufferCube[2]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  //////////////////////////////////////////////////////////////////////
  // index buffer
  //////////////////////////////////////////////////////////////////////

  bd.Usage = D3D10_USAGE_IMMUTABLE;
  bd.ByteWidth = mesh->indices_size;
  bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = mesh->indices;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pIndexBufferCube);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  return S_OK;
}

HRESULT InitDirect3DDevice()
{
  RECT rc;
  GetClientRect(g_hWnd, &rc);
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;

  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferCount = 1;
  sd.BufferDesc.Width = width;
  sd.BufferDesc.Height = height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = g_hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  D3D10_DRIVER_TYPE driverTypes[] = {
    D3D10_DRIVER_TYPE_HARDWARE,
    D3D10_DRIVER_TYPE_REFERENCE,
  };
  UINT numDriverTypes = (sizeof (driverTypes)) / (sizeof (driverTypes[0]));

  D3D10_CREATE_DEVICE_FLAG flags[] = {
    D3D10_CREATE_DEVICE_DEBUG,
    (D3D10_CREATE_DEVICE_FLAG)0,
  };
  UINT numFlags = (sizeof (flags)) / (sizeof (flags[0]));

  HRESULT hr;
  D3D10_DRIVER_TYPE driverType = D3D10_DRIVER_TYPE_NULL;
  for (UINT i = 0; i < numDriverTypes; i++) {
    for (UINT j = 0; j < numFlags; j++) {
      driverType = driverTypes[i];
      hr = D3D10CreateDeviceAndSwapChain(NULL,
					 driverType,
					 NULL,
					 flags[j],
					 D3D10_SDK_VERSION,
					 &sd,
					 &g_pSwapChain,
					 &g_pd3dDevice);
      if (SUCCEEDED(hr)) {
	if (j != 0) {
	  print("no debug layers\n");
	}
	goto end_driver_loop;
      }
    }
  }
 end_driver_loop:
  if (FAILED(hr)) {
    print("D3D10CreateDeviceAndSwapChain\n");
    return hr;
  }

  //////////////////////////////////////////////////////////////////////
  // rasterizer state
  //////////////////////////////////////////////////////////////////////

  D3D10_RASTERIZER_DESC RSDesc;
  RSDesc.FillMode = D3D10_FILL_SOLID;
  RSDesc.CullMode = D3D10_CULL_BACK;
  RSDesc.FrontCounterClockwise = FALSE;
  RSDesc.DepthBias = 0;
  RSDesc.SlopeScaledDepthBias = 0.0f;
  RSDesc.DepthBiasClamp= 0;
  RSDesc.DepthClipEnable = TRUE;
  RSDesc.ScissorEnable = FALSE;
  RSDesc.AntialiasedLineEnable = FALSE;
  RSDesc.MultisampleEnable = FALSE;

  ID3D10RasterizerState* pRState = NULL;
  hr = g_pd3dDevice->CreateRasterizerState(&RSDesc, &pRState);
  if (FAILED(hr)) {
    print("CreateRasterizerState\n");
    return hr;
  }

  g_pd3dDevice->RSSetState(pRState);

  //

  InitDirect3DViews();

  //////////////////////////////////////////////////////////////////////
  // effect
  //////////////////////////////////////////////////////////////////////

  HRSRC hRes = FindResource(NULL, L"RES_MAIN_FXO", RT_RCDATA);
  if (hRes == NULL) {
    print("FindResource RES_MAIN_FXO\n");
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
  g_pWorldVariable = g_pEffect->GetVariableByName("World")->AsMatrix();
  g_pViewVariable = g_pEffect->GetVariableByName("View")->AsMatrix();
  g_pProjectionVariable = g_pEffect->GetVariableByName("Projection")->AsMatrix();
  g_pJointVariable = g_pEffect->GetVariableByName("mJoint")->AsMatrix();
  g_pLightDirVariable = g_pEffect->GetVariableByName("vLightDir")->AsVector();
  g_pLightColorVariable = g_pEffect->GetVariableByName("vLightColor")->AsVector();
  g_pDiffuseVariable = g_pEffect->GetVariableByName("txDiffuse")->AsShaderResource();

  //////////////////////////////////////////////////////////////////////
  // input layout
  //////////////////////////////////////////////////////////////////////

  D3D10_INPUT_ELEMENT_DESC layout[] = {
    {"POSITION"    , 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL"      , 0, DXGI_FORMAT_R32G32B32_FLOAT,    1, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD"    , 0, DXGI_FORMAT_R32G32_FLOAT,       2, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
    {"BLENDWEIGHT" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
    {"BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
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

  //////////////////////////////////////////////////////////////////////
  // meshes
  //////////////////////////////////////////////////////////////////////

  hr = LoadMesh();
  if (FAILED(hr)) {
    print("LoadMesh\n");
    return hr;
  }

  hr = InitFontBuffers();
  if (FAILED(hr)) {
    print("InitFontBuffers\n");
    return hr;
  }

  hr = InitVolumeBuffers();
  if (FAILED(hr)) {
    print("InitVolumeBuffers\n");
    return hr;
  }

  hr = InitBloomBuffers();
  if (FAILED(hr)) {
    print("InitBloomBuffers\n");
    return hr;
  }

  hr = InitStaticEffect();
  if (FAILED(hr)) {
    print("InitStaticEffect\n");
    return hr;
  }

  hr = LoadMeshStatic(cube::node_0.mesh);
  if (FAILED(hr)) {
    print("LoadMeshStatic\n");
    return hr;
  }

  hr = InitInstancedVertexBuffer();
  if (FAILED(hr)) {
    print("InitInstancedVertexBuffer\n");
    return hr;
  }

  //////////////////////////////////////////////////////////////////////
  // transform matrices
  //////////////////////////////////////////////////////////////////////

  g_World1 = XMMatrixIdentity();
  g_World2 = XMMatrixIdentity();

  float fFov = XM_PI * 0.5f;
  float fAspect = width / (float)height;
  float fNear = 0.1f;
  float fFar = 100.0f;
  g_Projection = XMMatrixPerspectiveFovLH(fFov,
                                          fAspect,
                                          fNear,
                                          fFar);

  return S_OK;
}

BOOL Resize()
{
  RECT rc;
  GetClientRect(g_hWnd, &rc);
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;

  if (width == 0 || height == 0)
    return false;

  // no need to resize if the client area is equal to the current buffer area
  if (width == g_ViewportSize.Width && height == g_ViewportSize.Height)
    return true;

  g_pd3dDevice->OMSetRenderTargets(0, NULL, NULL);
  g_pRenderTargetView->Release();
  g_pDepthStencil->Release();
  g_pDepthStencilView->Release();

  g_pSwapChain->ResizeBuffers(1,
                              width,
                              height,
                              DXGI_FORMAT_R8G8B8A8_UNORM,
                              0);

  InitDirect3DViews();

  float fFov = XM_PI * 0.5f;
  float fAspect = width / (float)height;
  float fNear = 0.1f;
  float fFar = 100.0f;
  g_Projection = XMMatrixPerspectiveFovLH(fFov,
                                          fAspect,
                                          fNear,
                                          fFar);

  return true;
}

static inline XMMATRIX MatrixTRS(FXMVECTOR translation,
                                 FXMVECTOR rotation,
                                 FXMVECTOR scaling)
{
  XMMATRIX mTranslation = XMMatrixTranslationFromVector(translation);

  XMMATRIX mRotation = XMMatrixRotationQuaternion(rotation);

  XMMATRIX mScaling = XMMatrixScalingFromVector(scaling);

  //return mScaling * mRotation * mTranslation;
  return mScaling * mRotation * mTranslation;
}

static inline float fract(float f)
{
  return f - floor(f);
}

static inline float loop(float f, float n)
{
  return fract(f / n) * n;
}

static inline int FindFrame(const float * frames, int length, float t)
{
  for (int i = 0; i < length - 1; i++) {
    if (frames[i] <= t && frames[i+1] >= t) {
      return i;
    }
  }
  return 0;
}

static inline float Lerp(const float * frames, float t, int frame_ix)
{
  return (t - frames[frame_ix]) / (frames[frame_ix + 1] - frames[frame_ix]);
}

const int joints_length = robot_player::skin_0__joints__length;
XMMATRIX mJoints[joints_length];
NodeInstance node_inst[robot_player::nodes__length];

void InitializeNodeInstances()
{
  const Node ** nodes = robot_player::nodes;
  for (int i = 0; i < robot_player::nodes__length; i++) {
    node_inst[i].translation = XMLoadFloat3((const XMFLOAT3 *)&nodes[i]->translation);
    node_inst[i].rotation = XMLoadFloat4((const XMFLOAT4 *)&nodes[i]->rotation);
    node_inst[i].scale = XMLoadFloat3((const XMFLOAT3 *)&nodes[i]->scale);
  }
}

XMMATRIX GlobalTransform(int node_ix)
{
  const NodeInstance * instance = &node_inst[node_ix];
  const Node * node = robot_player::nodes[node_ix];
  XMMATRIX local_transform = MatrixTRS(XMLoadFloat3((const XMFLOAT3*)&instance->translation),
                                       XMLoadFloat4((const XMFLOAT4*)&instance->rotation),
                                       XMLoadFloat3((const XMFLOAT3*)&instance->scale));
  if (((int)node->parent_ix) != 40) {
    return local_transform * GlobalTransform(node->parent_ix);
  } else {
    return local_transform;
  }
}

void Animate(float t)
{
  const AnimationChannel * channels = robot_player::animation_1__channels;
  const int channels_length = robot_player::animation_1__channels__length;

  t = loop(t, 3.75);

  // sample all channels
  for (int i = 0; i < channels_length; i++) {

    // find frame and lerp (same accessor for all channels)
    const float * input = channels[i].sampler->input;
    const int input_length = channels[i].sampler->length;
    int frame_ix = FindFrame(input, input_length, t);
    float lerp = Lerp(input, t, frame_ix);

    const AnimationSampler * sampler = channels[i].sampler;
    NodeInstance * instance = &node_inst[channels[i].target.node_ix];
    switch (channels[i].target.path) {
    case ACP__TRANSLATION:
      {
        const XMFLOAT3 * output = (const XMFLOAT3 *)sampler->output;
        instance->translation = XMVectorLerp(XMLoadFloat3(&output[frame_ix]),
                                             XMLoadFloat3(&output[frame_ix+1]),
                                             lerp);
        break;
      }
    case ACP__ROTATION:
      {
        const XMFLOAT4 * output = (const XMFLOAT4 *)sampler->output;
        instance->rotation = XMQuaternionSlerp(XMLoadFloat4(&output[frame_ix]),
                                               XMLoadFloat4(&output[frame_ix+1]),
                                               lerp);
        break;
      }
    case ACP__SCALE:
      {
        const XMFLOAT3 * output = (const XMFLOAT3 *)sampler->output;
        instance->scale = XMVectorLerp(XMLoadFloat3(&output[frame_ix]),
                                       XMLoadFloat3(&output[frame_ix+1]),
                                       lerp);
        break;
      }
    default:
      assert(!"invalid sampler path");
      break;
    }
  }

  // transform all joints
  const Skin * skin = ROOT_MESH_NODE.skin;
  for (int i = 0; i < skin->joints_length; i++) {
    const int joint_ix = skin->joints[i];
    assert(joint_ix >= 0);

    const XMMATRIX inverse_bind_matrix = XMMATRIX((const float*)&skin->inverse_bind_matrices[i]);
    mJoints[i] = inverse_bind_matrix * GlobalTransform(joint_ix);
  }
}

void RenderModel(float t)
{
  for (int i = 0; i < joints_length; i++) {
    mJoints[i] = XMMatrixIdentity();
  }
  Animate(t);

  XMMATRIX rx = XMMatrixRotationX(XM_PI * -0.0f);
  XMMATRIX ry = XMMatrixRotationY(XM_PI * -1.0f + t);
  XMMATRIX mWorldTranslate = XMMatrixTranslation(0.0f, 0.0f, 2.0f);
  g_World1 = XMMatrixMultiply(rx, ry) * mWorldTranslate;

  // matrices
  g_pViewVariable->SetMatrix((float *)&g_View);
  g_pProjectionVariable->SetMatrix((float *)&g_Projection);
  g_pWorldVariable->SetMatrix((float *)&g_World1);
  g_pDiffuseVariable->SetResource(g_pTextureShaderResourceView);

  g_pJointVariable->SetMatrixArray((float *)mJoints, 0, joints_length);

  // lights
  g_pLightDirVariable->SetFloatVectorArray((float *)g_vLightDirs, 0, 2);
  g_pLightColorVariable->SetFloatVectorArray((float *)g_vLightColors, 0, 2);

  // render first cube
  const Mesh * mesh = ROOT_MESH_NODE.mesh;

  UINT stride[] = {
    (sizeof (mesh->position[0])),
    (sizeof (mesh->normal[0])),
    (sizeof (mesh->texcoord_0[0])),
    (sizeof (mesh->weights_0[0])),
    (sizeof (mesh->joints_0[0])),
  };
  UINT offset[] = { 0, 0, 0, 0, 0 };
  g_pd3dDevice->IASetInputLayout(g_pVertexLayout);
  g_pd3dDevice->IASetVertexBuffers(0, g_dwVertexBufferCount, g_pVertexBuffers, stride, offset);
  g_pd3dDevice->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
  g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  D3D10_TECHNIQUE_DESC techDesc;
  g_pTechniqueRender->GetDesc(&techDesc);

  int indices_length = ROOT_MESH_NODE.mesh->indices_size / (sizeof (DWORD));
  for (UINT p = 0; p < techDesc.Passes; p++) {
    g_pTechniqueRender->GetPassByIndex(p)->Apply(0);
    g_pd3dDevice->DrawIndexed(indices_length, 0, 0);
  }
}

void RenderMeshStatic(const Mesh * mesh, float t)
{
  g_pViewVariableStatic->SetMatrix((float *)&g_View);
  g_pProjectionVariableStatic->SetMatrix((float *)&g_Projection);

  UINT stride[] = {
    (sizeof (mesh->position[0])),
    (sizeof (mesh->normal[0])),
    (sizeof (mesh->texcoord_0[0])),
  };
  UINT offset[] = { 0, 0, 0 };
  g_pd3dDevice->IASetInputLayout(g_pVertexLayoutStatic);
  g_pd3dDevice->IASetVertexBuffers(0, g_dwVertexBufferCountCube, g_pVertexBufferCube, stride, offset);
  g_pd3dDevice->IASetIndexBuffer(g_pIndexBufferCube, DXGI_FORMAT_R32_UINT, 0);
  g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  D3D10_TECHNIQUE_DESC techDesc;
  g_pTechniqueStatic->GetDesc(&techDesc);
  int indices_length = mesh->indices_size / (sizeof (DWORD));

  for (int m = 0; m < 2; m++) {
    XMVECTOR vDir = XMLoadFloat4(&g_vLightDirs[m]);
    XMVECTOR vLightPos = vDir * (0.8f * m + 1.0f);

    XMMATRIX mLightRotate = XMMatrixRotationX(t * (1 + -2 * m));
    XMMATRIX mLightTranslation = XMMatrixTranslationFromVector(vLightPos);
    XMMATRIX mLightScale = XMMatrixScaling(0.05f, 0.05f, 0.05f);

    XMMATRIX mWorldTranslate = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

    XMMATRIX mLight = mLightRotate * mLightScale * mLightTranslation * mWorldTranslate;

    g_pWorldVariableStatic->SetMatrix((float *)&mLight);

    XMMATRIX mLightNormal = XMMatrixTranspose(XMMatrixInverse(NULL, mLight));
    g_pWorldNormalVariableStatic->SetMatrix((float *)&mLightNormal);

    g_pOutputColorVariableStatic->SetFloatVector((float *)&g_vLightColors[m]);

    for (UINT p = 0; p < techDesc.Passes; p++) {
      g_pTechniqueStatic->GetPassByIndex(p)->Apply(0);
      g_pd3dDevice->DrawIndexed(indices_length, 0, 0);
    }
  }
}

static char sprintbuf[512];
static inline int sprint(LPCSTR fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  char * end = sprintbuf;
  StringCbVPrintfExA(sprintbuf,
                     (sizeof (sprintbuf)),
                     &end,
                     NULL,
                     0,
                     fmt,
                     args);
  va_end(args);
  int length = (int)(end - sprintbuf);
  return length;
}

void RenderFont()
{
  HRESULT hr;

  //////////////////////////////////////////////////////////////////////
  // dynamic vertex buffer
  //////////////////////////////////////////////////////////////////////

  FontVertex * pData;
  hr = g_pVertexBuffersFont[0]->Map(D3D10_MAP_WRITE_DISCARD,
                                    0,
                                    (void **)&pData);
  if (FAILED(hr)) {
    print("g_pVertexBuffersFont->Map");
  }

  sprint("bloomPasses: %d\n"
         "   exposure: %f\n\n"
         "triggerL: % 5.3f\n"
         "triggerR: % 5.3f\n"
         " thumbLX: % 5.3f\n"
         " thumbLY: % 5.3f\n"
         " thumbRX: % 5.3f\n"
         " thumbRY: % 5.3f",
         g_bloomPasses,
         g_exposure,
         g_Joystate.triggerL,
         g_Joystate.triggerR,
         g_Joystate.thumbLX,
         g_Joystate.thumbLY,
         g_Joystate.thumbRX,
         g_Joystate.thumbRY);

  const char start_advance = 10;
  int hadvance = start_advance;
  int vadvance = -10;
  const char * s = sprintbuf;
  int ix = 0;
  const int charStride = (int)(g_FontSize.Texture.Width / g_FontSize.Glyph.Width);
  while (ix < g_iFontBufferLength && *s) {
    char c = *s++;

    float px = (float)hadvance;
    float py = (float)vadvance;

    float cx = 0;
    float cy = 0;
    if (c == '\n') {
      hadvance = start_advance;
      vadvance -= g_FontSize.Glyph.Height;
      continue;
    } else if (c == ' ') {
      hadvance += g_FontSize.Glyph.Width;
      continue;
    } else if (c >= 0x20 && c <= 0x7f) {
      c -= 0x20;
      cx = (float)(c % charStride);
      cy = (float)(c / charStride);
    }

    pData[ix++] = FontVertex(px, py, cx, cy);
    hadvance += g_FontSize.Glyph.Width;
  }
  g_pVertexBuffersFont[0]->Unmap();

  //////////////////////////////////////////////////////////////////////
  // effect variables
  //////////////////////////////////////////////////////////////////////

  XMFLOAT2 invScreenSize = XMFLOAT2(2.0f / (float)g_ViewportSize.Width,
                                    2.0f / (float)g_ViewportSize.Height);

  XMFLOAT2 glyphScale = XMFLOAT2((float)g_FontSize.Glyph.Width,
                                 (float)g_FontSize.Glyph.Height);

  XMFLOAT2 texScale = XMFLOAT2(glyphScale.x / (float)g_FontSize.Texture.Width,
                               glyphScale.y / (float)g_FontSize.Texture.Height);

  g_pInvScreenSizeVariableFont->SetFloatVector((float *)&invScreenSize);
  g_pGlyphScaleVariableFont->SetFloatVector((float *)&glyphScale);
  g_pTexScaleVariableFont->SetFloatVector((float *)&texScale);
  g_pDiffuseVariableFont->SetResource(g_pTextureShaderResourceViewFont);

  //////////////////////////////////////////////////////////////////////

  UINT stride[] = {
    (sizeof (FontVertex)),
  };
  UINT offset[] = { 0 };
  g_pd3dDevice->IASetInputLayout(g_pVertexLayoutFont);
  g_pd3dDevice->IASetVertexBuffers(0, g_dwVertexBufferCountFont, g_pVertexBuffersFont, stride, offset);
  g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

  D3D10_TECHNIQUE_DESC techDesc;
  g_pTechniqueFont->GetDesc(&techDesc);

  for (UINT p = 0; p < techDesc.Passes; p++) {
    g_pTechniqueFont->GetPassByIndex(p)->Apply(0);
    g_pd3dDevice->Draw(ix, 0);
  }
}

void RenderBloom()
{
  //////////////////////////////////////////////////////////////////////
  // effect variables
  //////////////////////////////////////////////////////////////////////

  XMFLOAT2 invScreenSize = XMFLOAT2(1.0f / (float)g_ViewportSize.Width,
                                      1.0f / (float)g_ViewportSize.Height);

  g_pInvScreenSizeVariableBloom->SetFloatVector((float *)&invScreenSize);

  //////////////////////////////////////////////////////////////////////

  UINT stride[] = {
    (sizeof (BloomVertex)),
  };
  UINT offset[] = { 0 };
  g_pd3dDevice->IASetInputLayout(g_pVertexLayoutBloom);
  g_pd3dDevice->IASetVertexBuffers(0, g_dwVertexBufferCountBloom, g_pVertexBuffersBloom, stride, offset);
  g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

  D3D10_TECHNIQUE_DESC techDesc;
  g_pTechniqueBloom->GetDesc(&techDesc);

  g_pExposureVariableBloom->SetFloat(g_exposure);

  XMFLOAT2 dirHorizontal = XMFLOAT2(1.0, 0.0);
  XMFLOAT2 dirVertical = XMFLOAT2(0.0, 1.0);

  // horizontal
  g_pDirVariableBloom->SetFloatVector((float *)&dirHorizontal);
  g_pDiffuseAVariableBloom->SetResource(g_pRenderTargetShaderResourceViewTexture[0]);
  g_pd3dDevice->OMSetRenderTargets(1, &g_pRenderTargetViewTexture[1], NULL);
  for (UINT p = 0; p < techDesc.Passes; p++) {
    g_pTechniqueBloom->GetPassByIndex(p)->Apply(0);
    g_pd3dDevice->Draw(4, 0);
  }
  g_pd3dDevice->OMSetRenderTargets(0, NULL, NULL);

  ID3D10ShaderResourceView * srv[] = { NULL };

  for (int i = 0; i < g_bloomPasses; i++) {
    g_pd3dDevice->PSSetShaderResources(0, 1, srv);

    // vertical
    g_pDirVariableBloom->SetFloatVector((float *)&dirVertical);
    g_pDiffuseAVariableBloom->SetResource(g_pRenderTargetShaderResourceViewTexture[1]);
    g_pd3dDevice->OMSetRenderTargets(1, &g_pRenderTargetViewTexture[0], NULL);
    for (UINT p = 0; p < techDesc.Passes; p++) {
      g_pTechniqueBloom->GetPassByIndex(p)->Apply(0);
      g_pd3dDevice->Draw(4, 0);
    }

    g_pd3dDevice->PSSetShaderResources(0, 1, srv);

    // horizontal
    g_pDirVariableBloom->SetFloatVector((float *)&dirHorizontal);
    g_pDiffuseAVariableBloom->SetResource(g_pRenderTargetShaderResourceViewTexture[0]);
    g_pd3dDevice->OMSetRenderTargets(1, &g_pRenderTargetViewTexture[1], NULL);
    for (UINT p = 0; p < techDesc.Passes; p++) {
      g_pTechniqueBloom->GetPassByIndex(p)->Apply(0);
      g_pd3dDevice->Draw(4, 0);
    }
  }

  D3D10_TECHNIQUE_DESC techDescBlend;
  g_pTechniqueBloomBlend->GetDesc(&techDescBlend);

  // vertical
  g_pDirVariableBloom->SetFloatVector((float *)&dirVertical);
  g_pDiffuseAVariableBloom->SetResource(g_pRenderTargetShaderResourceViewTexture[1]);
  g_pd3dDevice->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

  for (UINT p = 0; p < techDescBlend.Passes; p++) {
    g_pTechniqueBloomBlend->GetPassByIndex(p)->Apply(0);
    g_pd3dDevice->Draw(4, 0);
  }
}

float deadzone(float f)
{
  const float threshold = 0.25f;
  if (fabsf(f) <= threshold)
    return 0.0f;
  else {
    return copysignf(fabsf(f) - threshold, f);
  }
}

void Update(float t)
{
  XMVECTOR vLightDirs[2] = {
    {-0.577f, 0.577f, 0.0, 1.0},
    {1.0f, 1.5f, 0.0f, 1.0},
  };

  XMMATRIX mRotate1 = XMMatrixRotationY(-1.0f * t);
  XMVECTOR lightDir1 = XMVector4Transform(vLightDirs[1], mRotate1);
  XMStoreFloat4(&g_vLightDirs[1], lightDir1);

  XMMATRIX mRotate0 = XMMatrixRotationY(0.4f * t);
  XMVECTOR lightDir0 = XMVector4Transform(vLightDirs[0], mRotate0);
  XMStoreFloat4(&g_vLightDirs[0], lightDir0);

  // view
  XMMATRIX mRotateView
    = XMMatrixRotationY(deadzone(g_Joystate.thumbLX) * 0.002f)
    * XMMatrixRotationX(deadzone(g_Joystate.thumbLY) * 0.002f);

  XMMATRIX mTranslateView = XMMatrixTranslation(deadzone(g_Joystate.thumbRX) * 0.002f,
                                                0.0f,
                                                deadzone(g_Joystate.thumbRY) * 0.002f);

  g_Eye = XMVector4Transform(g_Eye, mTranslateView * mRotateView);
  XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

  g_View = XMMatrixLookAtLH(g_Eye, g_At, Up);

}

void RenderVolume(float t)
{

  UINT stride[] = {
    (sizeof (XMFLOAT2)),
  };
  UINT offset[] = { 0 };
  g_pd3dDevice->IASetInputLayout(g_pVertexLayoutVolume);
  g_pd3dDevice->IASetVertexBuffers(0, g_dwVertexBufferCountVolume, g_pVertexBuffersVolume, stride, offset);
  g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

  g_pLayerVariableVolume->SetFloat(t * 0.1f);
  g_pDiffuseVariableVolume->SetResource(g_pTextureShaderResourceViewPerlin);

  D3D10_TECHNIQUE_DESC techDescVolume;
  g_pTechniqueVolume->GetDesc(&techDescVolume);

  g_pd3dDevice->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
  for (UINT p = 0; p < techDescVolume.Passes; p++) {
    g_pTechniqueVolume->GetPassByIndex(p)->Apply(0);
    g_pd3dDevice->Draw(4, 0);
  }
}

void RenderVolumeMesh()
{
  const Mesh * mesh = cube::node_0.mesh;
  int index_count_per_instance = mesh->indices_size / (sizeof (DWORD));

  g_pViewVariableStatic->SetMatrix((float *)&g_View);
  g_pProjectionVariableStatic->SetMatrix((float *)&g_Projection);

  g_pDiffuseVariableStatic->SetResource(g_pTextureShaderResourceViewPerlin);

  ID3D10Buffer * pVB[4] = {
    g_pVertexBufferCube[0],
    g_pVertexBufferCube[1],
    g_pVertexBufferCube[2],
    g_pVolumeMeshInstanceData,
  };

  UINT stride[] = {
    (sizeof (mesh->position[0])),
    (sizeof (mesh->normal[0])),
    (sizeof (mesh->texcoord_0[0])),
    (sizeof (XMMATRIX)),
  };
  UINT offset[] = { 0, 0, 0, 0 };
  g_pd3dDevice->IASetInputLayout(g_pVertexLayoutStaticInstanced);
  g_pd3dDevice->IASetVertexBuffers(0, 4, pVB, stride, offset);
  g_pd3dDevice->IASetIndexBuffer(g_pIndexBufferCube, DXGI_FORMAT_R32_UINT, 0);
  g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  D3D10_TECHNIQUE_DESC techDesc;
  g_pTechniqueStaticInstanced->GetDesc(&techDesc);

  XMMATRIX mWorldScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
  XMMATRIX mWorldTranslate = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
  XMMATRIX mWorld = mWorldScale * mWorldTranslate;

  g_pWorldVariableStatic->SetMatrix((float *)&mWorld);
  XMMATRIX mWorldNormal = XMMatrixIdentity();
  g_pWorldNormalVariableStatic->SetMatrix((float *)&mWorldNormal);
  XMVECTOR vColor = XMVectorSet(0.0f, 0.4f, 0.3f, 1.0f);
  g_pOutputColorVariableStatic->SetFloatVector((float *)&vColor);

  int instance_count = g_iNumVolumeMeshInstances;
  for (UINT p = 0; p < techDesc.Passes; p++) {
    g_pTechniqueStaticInstanced->GetPassByIndex(p)->Apply(0);
    g_pd3dDevice->DrawIndexedInstanced(index_count_per_instance,
                                       instance_count,
                                       0, 0, 0);
  }
}

void Render(float t)
{
  // clear

  const float ClearColor[4] = { 0.2f, 0.125f, 0.2f, 1.0f };
  g_pd3dDevice->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
  g_pd3dDevice->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
  g_pd3dDevice->ClearDepthStencilView(g_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);

  // render
  RenderModel(t);

  const float ClearColorZero[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
  ID3D10RenderTargetView * RenderTargets[] = {
    g_pRenderTargetView,
    g_pRenderTargetViewTexture[0],
  };
  g_pd3dDevice->OMSetRenderTargets(2, RenderTargets, g_pDepthStencilView);
  g_pd3dDevice->ClearRenderTargetView(g_pRenderTargetViewTexture[0], ClearColorZero);
  RenderMeshStatic(cube::node_0.mesh, t);

  //RenderBloom();
  //print("%f\n", t);
  //RenderVolume(t);
  RenderVolumeMesh();

  RenderFont();

  // present
  g_pSwapChain->Present(0, 0);
}
