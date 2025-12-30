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

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT InitDirect3DDevice();
void Render();

struct SimpleVertex {
  D3DXVECTOR3 Pos;
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
  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
  g_hWnd = CreateWindow(L"d3d10wc",
                        L"d3d10", WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        rc.right - rc.left, rc.bottom - rc.top,
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

  HRESULT hr;
  hr = D3D10CreateDeviceAndSwapChain(NULL,
                                     D3D10_DRIVER_TYPE_REFERENCE,
                                     NULL,
                                     D3D10_CREATE_DEVICE_DEBUG,
                                     D3D10_SDK_VERSION,
                                     &sd,
                                     &g_pSwapChain,
                                     &g_pd3dDevice);
  if (FAILED(hr)) {
    print("D3D10CreateDeviceAndSwapChain\n");
    return hr;
  }

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
  hr = D3DX10CreateEffectFromFile(L"c:/Users/dot/d3d10/main.fx",
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
    const char * pError = (const char *)pBlobErrors->GetBufferPointer();
    (void)pError;
    return hr;
  }

  g_pTechnique = g_pEffect->GetTechniqueByName("Render");

  // input layout
  D3D10_INPUT_ELEMENT_DESC layout[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
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

  // vertex buffer
  SimpleVertex vertices[] = {
    D3DXVECTOR3( 0.0f,  0.5f, 0.5f),
    D3DXVECTOR3( 0.5f, -0.5f, 0.5f),
    D3DXVECTOR3(-0.5f, -0.5f, 0.5f),
  };
  D3D10_BUFFER_DESC bd;
  bd.Usage = D3D10_USAGE_DEFAULT;
  bd.ByteWidth = (sizeof (SimpleVertex)) * 3;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  D3D10_SUBRESOURCE_DATA InitData;
  InitData.pSysMem = vertices;
  hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
  if (FAILED(hr)) {
    print("CreateBuffer\n");
    return hr;
  }
  UINT stride = (sizeof (SimpleVertex));
  UINT offset = 0;
  g_pd3dDevice->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

  g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  return S_OK;
}

void Render()
{
  float ClearColor[4] = { 0.0f, 0.125f, 0.6f, 1.0f };
  g_pd3dDevice->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

  D3D10_TECHNIQUE_DESC techDesc;
  g_pTechnique->GetDesc( &techDesc );
  for(UINT p = 0; p < techDesc.Passes; p++) {
    g_pTechnique->GetPassByIndex(p)->Apply(0);
    g_pd3dDevice->Draw(3, 0);
  }

  g_pSwapChain->Present(0, 0);
}
