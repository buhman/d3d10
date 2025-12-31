#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <strsafe.h>

HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;
ID3D10Device * g_pd3dDevice = NULL;
IDXGISwapChain * g_pSwapChain = NULL;
ID3D10RenderTargetView * g_pRenderTargetView = NULL;
ID3D10Effect * g_pEffect = NULL;
ID3D10EffectTechnique * g_pTechnique = NULL;
ID3D10InputLayout * g_pVertexLayout = NULL;
ID3D10Buffer * g_pVertexBuffer = NULL;
ID3D10Buffer * g_pIndexBuffer = NULL;

ID3D10EffectMatrixVariable * g_pWorldVariable = NULL;
ID3D10EffectMatrixVariable * g_pViewVariable = NULL;
ID3D10EffectMatrixVariable * g_pProjectionVariable = NULL;
D3DXMATRIX g_World;
D3DXMATRIX g_View;
D3DXMATRIX g_Projection;

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT InitDirect3DDevice();
void Render();

struct SimpleVertex {
  D3DXVECTOR3 Pos;
  D3DXVECTOR4 Color;
};

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
  HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  WriteConsoleA(hOutput, buf, (DWORD)length, NULL, NULL);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
  FreeConsole();
  AllocConsole();
  AttachConsole(GetCurrentProcessId());
  print("WinMain\n");
  if (FAILED(InitWindow(hInstance, nCmdShow))) {
    print("InitWindow\n");
    return 0;
  }

  if (FAILED(InitDirect3DDevice())) {
    print("InitDirect3DDevice\n");
    system("pause");
    return 0;
  }

  MSG msg = {};
  while (msg.message != WM_QUIT) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } else {
      Render();
    }
  }

  system("pause");
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
  RECT rc = { 0, 0, 640, 480 };
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;
  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
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

HRESULT InitDirect3DDevice()
{
  RECT rc;
  GetClientRect(g_hWnd, &rc);
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;

  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferCount = 1;
  sd.BufferDesc.Width = 640;
  sd.BufferDesc.Height = 480;
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
                                       0,
                                       //D3D10_CREATE_DEVICE_DEBUG,
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
  print("driverType %d\n", driverType);

  ID3D10Texture2D * pBackBuffer;
  hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID *)&pBackBuffer);
  if (FAILED(hr)) {
    print("g_pSwapChain->GetBuffer\n");
    return hr;
  }
  hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
  pBackBuffer->Release();
  if (FAILED(hr)) {
    print("g_pSwapChain->CreateRenderTargetView\n");
    return hr;
  }

  g_pd3dDevice->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

  // viewport
  D3D10_VIEWPORT vp;
  vp.Width = 640;
  vp.Height = 480;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  g_pd3dDevice->RSSetViewports(1, &vp);

  // effect

  ID3D10Blob * pBlobErrors = NULL;
  DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG;
  hr = D3DX10CreateEffectFromResource(NULL,
                                      L"RES_MAIN_FXO",
                                      L"main.fxo",
                                      NULL,
                                      NULL,
                                      "fx_4_0",
                                      dwShaderFlags,
                                      0,
                                      g_pd3dDevice,
                                      NULL,
                                      NULL,
                                      &g_pEffect,
                                      &pBlobErrors,
                                      NULL);
  if (FAILED(hr)) {
    print("D3DX10CreateEffectFromFile\n");
    if (pBlobErrors != NULL) {
      const char * pError = (const char *)pBlobErrors->GetBufferPointer();
      print("pError: %p\n", pError);
    }
    return hr;
  }

  g_pTechnique = g_pEffect->GetTechniqueByName("Render");

  // variables

  g_pWorldVariable = g_pEffect->GetVariableByName("World")->AsMatrix();
  g_pViewVariable = g_pEffect->GetVariableByName("View")->AsMatrix();
  g_pProjectionVariable = g_pEffect->GetVariableByName("Projection")->AsMatrix();

  // input layout
  D3D10_INPUT_ELEMENT_DESC layout[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 0 , D3D10_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
  };
  UINT numElements = (sizeof (layout)) / (sizeof (layout[0]));

  D3D10_PASS_DESC passDesc;
  g_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
  print("pass desc: `%s`\n", passDesc.Name);

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

  // vertex buffer
  SimpleVertex vertices[] = {
    { D3DXVECTOR3(-1.0f,  1.0f, -1.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f) },
    { D3DXVECTOR3( 1.0f,  1.0f, -1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f) },
    { D3DXVECTOR3( 1.0f,  1.0f,  1.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f) },
    { D3DXVECTOR3(-1.0f,  1.0f,  1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f) },
    { D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f) },
    { D3DXVECTOR3( 1.0f, -1.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f) },
    { D3DXVECTOR3( 1.0f, -1.0f,  1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f) },
    { D3DXVECTOR3(-1.0f, -1.0f,  1.0f), D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f) },
  };
  bd.Usage = D3D10_USAGE_DEFAULT;
  bd.ByteWidth = (sizeof (SimpleVertex)) * 8;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = vertices;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pVertexBuffer);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }
  UINT stride = (sizeof (SimpleVertex));
  UINT offset = 0;
  g_pd3dDevice->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

  // index buffer
  DWORD indices[] = {
    3,1,0,
    2,1,3,

    0,5,4,
    1,5,0,

    3,4,7,
    0,4,3,

    1,6,5,
    2,6,1,

    2,7,6,
    3,7,2,

    6,4,5,
    7,4,6,
  };
  bd.Usage = D3D10_USAGE_DEFAULT;
  bd.ByteWidth = (sizeof (DWORD)) * 36;
  bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  initData.pSysMem = indices;
  hr = g_pd3dDevice->CreateBuffer(&bd, &initData, &g_pIndexBuffer);
  if (FAILED(hr))
    return hr;

  g_pd3dDevice->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

  g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // transform matrices
  D3DXMatrixIdentity(&g_World);

  D3DXVECTOR3 Eye(0.0f, 2.0f, -5.0f);
  D3DXVECTOR3 At(0.0f, 1.0f, 0.0f);
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

void Render()
{
  static float t = 0.0f;
  t += (float)D3DX_PI * 0.0125f;
  D3DXMatrixRotationY(&g_World, t);

  float ClearColor[4] = { 0.0f, 0.125f, 0.6f, 1.0f };
  g_pd3dDevice->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

  g_pWorldVariable->SetMatrix((float *)&g_World);
  g_pViewVariable->SetMatrix((float *)&g_View);
  g_pProjectionVariable->SetMatrix((float *)&g_Projection);

  D3D10_TECHNIQUE_DESC techDesc;
  g_pTechnique->GetDesc( &techDesc );
  for(UINT p = 0; p < techDesc.Passes; p++) {
    g_pTechnique->GetPassByIndex(p)->Apply(0);
    //g_pd3dDevice->Draw(3, 0);
    g_pd3dDevice->DrawIndexed(36, 0, 0);
  }

  g_pSwapChain->Present(0, 0);
}
