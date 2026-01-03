#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <strsafe.h>

#include "gltf.hpp"
#include "skin.hpp"

HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;
ID3D10Device * g_pd3dDevice = NULL;
IDXGISwapChain * g_pSwapChain = NULL;
ID3D10Texture2D * g_pDepthStencil = NULL;
ID3D10RenderTargetView * g_pRenderTargetView = NULL;
ID3D10DepthStencilView * g_pDepthStencilView = NULL;
ID3D10Effect * g_pEffect = NULL;
ID3D10EffectTechnique * g_pTechniqueRender = NULL;
ID3D10EffectTechnique * g_pTechniqueRenderLight = NULL;
ID3D10InputLayout * g_pVertexLayout = NULL;
//ID3D10Buffer * g_pVertexBuffer = NULL;
ID3D10Buffer * g_pIndexBuffer = NULL;

ID3D10Texture2D * g_pTexture = NULL;
ID3D10ShaderResourceView * g_pTextureShaderResourceView = NULL;

ID3D10EffectMatrixVariable * g_pWorldVariable = NULL;
ID3D10EffectMatrixVariable * g_pViewVariable = NULL;
ID3D10EffectMatrixVariable * g_pProjectionVariable = NULL;
ID3D10EffectMatrixVariable * g_pJointVariable = NULL;
ID3D10EffectVectorVariable * g_pLightDirVariable = NULL;
ID3D10EffectVectorVariable * g_pLightColorVariable = NULL;
ID3D10EffectVectorVariable * g_pOutputColorVariable = NULL;
ID3D10EffectShaderResourceVariable * g_pDiffuseVariable = NULL;
D3DXMATRIX g_World1;
D3DXMATRIX g_World2;
D3DXMATRIX g_View;
D3DXMATRIX g_Projection;

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT InitDirect3DDevice();
void Render();
BOOL Resize();

struct WindowSize {
  UINT Width;
  UINT Height;
};

WindowSize g_ViewportSize;

void print(LPCSTR fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  char buf[512];
  STRSAFE_LPSTR end;

  StringCbVPrintfExA(buf,
                     (sizeof (buf)),
                     &end,
                     NULL,
                     STRSAFE_NULL_ON_FAILURE,
                     fmt,
                     args);
  va_end(args);
  size_t length = end - &buf[0];
  #ifdef _DEBUG
  OutputDebugStringA(buf);
  #endif
  //HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  //WriteConsoleA(hOutput, buf, (DWORD)length, NULL, NULL);
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

  MSG msg = {};
  while (msg.message != WM_QUIT) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } else {
      if (Resize())
        Render();
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
  RECT rc = { 0, 0, 320, 240 };
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
    print("g_pSwapChain->CreateRenderTargetView\n");
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
  if (FAILED(hr))
    return hr;

  D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
  descDSV.Format = descDepth.Format;
  descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;
  hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
  if (FAILED(hr))
    return hr;

  g_pd3dDevice->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

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

  HRESULT hr;
  D3D10_DRIVER_TYPE driverType = D3D10_DRIVER_TYPE_NULL;
  for (UINT i = 0; i < numDriverTypes; i++) {
    driverType = driverTypes[i];
    hr = D3D10CreateDeviceAndSwapChain(NULL,
                                       driverType,
                                       NULL,
                                       D3D10_CREATE_DEVICE_DEBUG,
                                       D3D10_SDK_VERSION,
                                       &sd,
                                       &g_pSwapChain,
                                       &g_pd3dDevice);
    if (SUCCEEDED(hr))
      break;
  }
  if (FAILED(hr)) {
    print("D3D10CreateDeviceAndSwapChain\n");
    return hr;
  }

  //////////////////////////////////////////////////////////////////////
  // rasterizer state
  //////////////////////////////////////////////////////////////////////
  D3D10_RASTERIZER_DESC RSDesc;
  RSDesc.FillMode = D3D10_FILL_SOLID;
  //RSDesc.CullMode = D3D10_CULL_BACK;
  RSDesc.CullMode = D3D10_CULL_NONE;
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
  if (FAILED(hr))
    return hr;

  g_pd3dDevice->RSSetState(pRState);

  //

  InitDirect3DViews();

  // texture
  HRSRC hSeafloorRes = FindResource(NULL, L"RES_SEAFLOOR", RT_RCDATA);
  if (hSeafloorRes == NULL) {
    print("FindResource\n");
    return -1;
  }
  DWORD dwSeafloorResSize = SizeofResource(NULL, hSeafloorRes);
  HGLOBAL hSeafloorData = LoadResource(NULL, hSeafloorRes);
  D3D10_SUBRESOURCE_DATA initSeafloorData;
  initSeafloorData.pSysMem = LockResource(hSeafloorData);
  initSeafloorData.SysMemPitch = 256 * 4;
  D3D10_TEXTURE2D_DESC descTexture;
  descTexture.Width = 256;
  descTexture.Height = 256;
  descTexture.MipLevels = 1;
  descTexture.ArraySize = 1;
  descTexture.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  descTexture.SampleDesc.Count = 1;
  descTexture.SampleDesc.Quality = 0;
  descTexture.Usage = D3D10_USAGE_DEFAULT;
  descTexture.BindFlags = D3D10_BIND_SHADER_RESOURCE;
  descTexture.CPUAccessFlags = 0;
  descTexture.MiscFlags = 0;
  hr = g_pd3dDevice->CreateTexture2D(&descTexture, &initSeafloorData, &g_pTexture);
  if (FAILED(hr))
    return hr;

  D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;
  descSRV.Format = descTexture.Format;
  descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
  descSRV.Texture2D.MostDetailedMip = 0;
  descSRV.Texture2D.MipLevels = 1;
  hr = g_pd3dDevice->CreateShaderResourceView(g_pTexture, &descSRV, &g_pTextureShaderResourceView);
  if (FAILED(hr))
    return hr;

  // effect
  HRSRC hRes = FindResource(NULL, L"RES_MAIN_FXO", RT_RCDATA);
  if (hRes == NULL) {
    print("FindResource\n");
    return -1;
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
  g_pTechniqueRenderLight = g_pEffect->GetTechniqueByName("RenderLight");

  // variables
  g_pWorldVariable = g_pEffect->GetVariableByName("World")->AsMatrix();
  g_pViewVariable = g_pEffect->GetVariableByName("View")->AsMatrix();
  g_pProjectionVariable = g_pEffect->GetVariableByName("Projection")->AsMatrix();
  g_pJointVariable = g_pEffect->GetVariableByName("mJoint")->AsMatrix();
  g_pLightDirVariable = g_pEffect->GetVariableByName("vLightDir")->AsVector();
  g_pLightColorVariable = g_pEffect->GetVariableByName("vLightColor")->AsVector();
  g_pOutputColorVariable = g_pEffect->GetVariableByName("vOutputColor")->AsVector();
  g_pDiffuseVariable = g_pEffect->GetVariableByName("txDiffuse")->AsShaderResource();

  // input layout
  D3D10_INPUT_ELEMENT_DESC layout[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 , D3D10_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
    //{"NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
    //{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
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
  g_pd3dDevice->IASetInputLayout(g_pVertexLayout);

  //

  D3D10_BUFFER_DESC bd;
  D3D10_SUBRESOURCE_DATA initData;

  //////////////////////////////////////////////////////////////////////
  // vertex buffers
  //////////////////////////////////////////////////////////////////////
#define MESH mesh_0

  ID3D10Buffer * pVertexBuffers[3];

  // position
  bd.Usage = D3D10_USAGE_DEFAULT;
  bd.ByteWidth = MESH.position_size;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = MESH.position;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &pVertexBuffers[0]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  // weights
  bd.Usage = D3D10_USAGE_DEFAULT;
  bd.ByteWidth = MESH.weights_0_size;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = MESH.weights_0;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &pVertexBuffers[1]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  // joints
  bd.Usage = D3D10_USAGE_DEFAULT;
  bd.ByteWidth = MESH.joints_0_size;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = MESH.joints_0;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &pVertexBuffers[2]);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }

  UINT stride[] = {
    (sizeof (MESH.position[0])),
    (sizeof (MESH.weights_0[0])),
    (sizeof (MESH.joints_0[0])),
  };
  UINT offset[] = { 0, 0, 0 };
  g_pd3dDevice->IASetVertexBuffers(0, 3, pVertexBuffers, stride, offset);

  //////////////////////////////////////////////////////////////////////
  // index buffer
  //////////////////////////////////////////////////////////////////////

  bd.Usage = D3D10_USAGE_DEFAULT;
  bd.ByteWidth = MESH.indices_size;
  //bd.ByteWidth = (sizeof (DWORD)) * indices_length;
  bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = MESH.indices;
  //initData.pSysMem = indices;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pIndexBuffer);
  if (FAILED(hr))
    return hr;

  g_pd3dDevice->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

  g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  //////////////////////////////////////////////////////////////////////
  // transform matrices
  //////////////////////////////////////////////////////////////////////
  D3DXMatrixIdentity(&g_World1);
  D3DXMatrixIdentity(&g_World2);

  D3DXVECTOR3 Eye(0.0f, 0.0f, -3.0f);
  D3DXVECTOR3 At(0.0f, 0.0f, 0.0f);
  D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
  D3DXMatrixLookAtLH(&g_View, &Eye, &At, &Up);

  float fFov = (float)D3DX_PI * 0.5f;
  float fAspect = width / (float)height;
  float fNear = 0.1f;
  float fFar = 100.0f;
  D3DXMatrixPerspectiveFovLH(&g_Projection,
                             fFov,
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

  //g_pd3dDevice->OMSetRenderTargets(1, NULL, NULL);
  g_pRenderTargetView->Release();
  g_pDepthStencil->Release();
  g_pDepthStencilView->Release();

  g_pSwapChain->ResizeBuffers(1,
                              width,
                              height,
                              DXGI_FORMAT_R8G8B8A8_UNORM,
                              0);

  InitDirect3DViews();

  float fFov = (float)D3DX_PI * 0.5f;
  float fAspect = width / (float)height;
  float fNear = 0.1f;
  float fFar = 100.0f;
  D3DXMatrixPerspectiveFovLH(&g_Projection,
                             fFov,
                             fAspect,
                             fNear,
                             fFar);

  return true;
}

/*
void Animate(float t)
{
  const float * frames = accessor_2;
  const int frames_length = accessor_2_length;

  while (t >= frames[frames_length - 1]) {
    t -= frames[frames_length - 1];
  }

  // find frame
  int prev_ix = -1;
  for (int i = 0; i < frames_length - 1; i++) {
    if (frames[i] <= t && frames[i+1] >= t) {
      prev_ix = i;
      break;
    }
  }
  if (prev_ix == -1)
    return;
  int next_ix = prev_ix + 1;
  if (next_ix >= frames_length)
    return;

  float lerp = (t - frames[prev_ix]) / (frames[next_ix] - frames[prev_ix]);
  print("%f prev %d next %d lerp %f\n", t, prev_ix, next_ix, lerp);

  const D3DXQUATERNION * animation = (D3DXQUATERNION *)accessor_3;
  D3DXQUATERNION rotation;

  D3DXQuaternionSlerp(&rotation,
                      &animation[prev_ix],
                      &animation[next_ix],
                      lerp);

  D3DXMatrixRotationQuaternion(&g_World1, &rotation);
}
*/

static inline void MatrixTRS(D3DXMATRIX * transform,
                             const D3DXVECTOR3 * translation,
                             const D3DXQUATERNION * rotation,
                             const D3DXVECTOR3 * scaling)
{
  D3DXMATRIX mTranslation;
  D3DXMatrixTranslation(&mTranslation, translation->x, translation->y, translation->z);

  D3DXMATRIX mRotation;
  D3DXMatrixRotationQuaternion(&mRotation, rotation);

  D3DXMATRIX mScaling;
  D3DXMatrixScaling(&mScaling, scaling->x, scaling->y, scaling->z);

  D3DXMatrixMultiply(transform, &mRotation, &mTranslation);
  D3DXMatrixMultiply(transform, transform, &mScaling);
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

D3DXMATRIX mJoints[2];

void Animate(float t)
{
  t = loop(t, 5.5);

  // animation_0__sampler_0
  const AnimationSampler * sampler = &animation_0__sampler_0;
  const float * input = sampler->input;
  const D3DXQUATERNION * output = (D3DXQUATERNION *)sampler->output;

  int frame_ix = FindFrame(sampler->input, sampler->length, t);
  float lerp = Lerp(sampler->input, t, frame_ix);
  D3DXQUATERNION rotation;
  D3DXQuaternionSlerp(&rotation,
                      &output[frame_ix],
                      &output[frame_ix+1],
                      lerp);

  // joint 1
  const Skin * skin = &skin_0;
  const Node * node = skin->joints[1];

  // T * R * S

  D3DXMATRIX global_transform;
  MatrixTRS(&global_transform, &node->translation, &rotation, &node->scale);

  D3DXMatrixIdentity(&mJoints[0]);
  D3DXMatrixIdentity(&mJoints[1]);

  const D3DXMATRIX * inverse_bind_matrix = &skin->inverse_bind_matrices[1];
  //D3DXMatrixMultiply(&mJoints[1], &global_transform, inverse_bind_matrix);
  //D3DXMatrixIdentity(&mJoints[1]);
  //g_World1 = global_transform;

  D3DXMatrixMultiply(&mJoints[1], inverse_bind_matrix, &global_transform);
  //mJoints[1] = global_transform;
  //g_World1 = *inverse_bind_matrix;
}

void Render()
{
  static float t = 0.0f;
#ifdef _DEBUG
    t += (float)D3DX_PI * 0.0125f * 0.5;
#else
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if (dwTimeStart == 0)
      dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;
#endif

  Animate(t);

  // first cube
  //D3DXMatrixRotationZ(&g_World1, t);

  // lights
  D3DXVECTOR4 vLightDirs[2] = {
    D3DXVECTOR4(-0.577f, 0.577f, -0.577f, 1.0f),
    D3DXVECTOR4(0.0f, 0.0f, -1.0f, 1.0f),
  };
  D3DXVECTOR4 vLightColors[2] = {
    D3DXVECTOR4(0.0f, 0.5f, 0.5f, 1.0f),
    D3DXVECTOR4(0.5f, 0.0f, 0.0f, 1.0f)
  };

  // rotate the second light around the origin
  D3DXMATRIX mRotate;
  D3DXVECTOR4 vOutDir;
  D3DXMatrixRotationY(&mRotate, -2.0f * t);
  D3DXVec3Transform(&vLightDirs[1], (D3DXVECTOR3 *)&vLightDirs[1], &mRotate);

  D3DXMatrixRotationY(&mRotate, 0.4f * t);
  D3DXVec3Transform(&vLightDirs[0], (D3DXVECTOR3 *)&vLightDirs[0], &mRotate);

  // clear
  float ClearColor[4] = { 0.0f, 0.125f, 0.6f, 1.0f };
  g_pd3dDevice->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
  g_pd3dDevice->ClearDepthStencilView(g_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);

  // matrices
  g_pViewVariable->SetMatrix((float *)&g_View);
  g_pProjectionVariable->SetMatrix((float *)&g_Projection);
  g_pWorldVariable->SetMatrix((float *)&g_World1);
  g_pDiffuseVariable->SetResource(g_pTextureShaderResourceView);

  g_pJointVariable->SetMatrixArray((float *)mJoints, 0, 2);

  // color
  g_pOutputColorVariable->SetFloatVector((float *)&vLightColors[0]);

  // lights
  g_pLightDirVariable->SetFloatVectorArray((float *)vLightDirs, 0, 2);
  g_pLightColorVariable->SetFloatVectorArray((float *)vLightColors, 0, 2);

  // render first cube
  D3D10_TECHNIQUE_DESC techDesc;
  g_pTechniqueRender->GetDesc(&techDesc);
  for (UINT p = 0; p < techDesc.Passes; p++) {
    g_pTechniqueRender->GetPassByIndex(p)->Apply(0);
    g_pd3dDevice->DrawIndexed(accessor_0_length, 0, 0);
  }

  // render the lights
  /*
  for (int m = 0; m < 2; m++) {
    D3DXMATRIX mLight;
    D3DXMATRIX mLightScale;
    D3DXVECTOR3 vLightPos = vLightDirs[m] * 4.0f;
    D3DXMatrixTranslation( &mLight, vLightPos.x, vLightPos.y, vLightPos.z );
    D3DXMatrixScaling( &mLightScale, 0.2f, 0.2f, 0.2f );
    mLight = mLightScale * mLight;

    g_pWorldVariable->SetMatrix((float *)&mLight);
    g_pOutputColorVariable->SetFloatVector((float *)&vLightColors[m]);

    g_pTechniqueRenderLight->GetDesc( &techDesc );
    for (UINT p = 0; p < techDesc.Passes; p++) {
      g_pTechniqueRenderLight->GetPassByIndex(p)->Apply(0);
      g_pd3dDevice->DrawIndexed(indices_length, 0, 0);
    }
  }
  */

  // present
  g_pSwapChain->Present(0, 0);
}
