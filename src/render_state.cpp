#include <windows.h>
#include <assert.h>

#include <d3d10.h>
#include <d3dx10.h>

#include "globals.hpp"
#include "print.hpp"
#include "render_state.hpp"

HRESULT LoadTexture2D(const wchar_t * resourceName,
                      const int width,
                      const int height,
                      const int pitch,
                      const DXGI_FORMAT format,
                      ID3D10ShaderResourceView ** pTextureShaderResourceView)
{
  HRESULT hr;

  // texture
  HRSRC hRes = FindResource(NULL, resourceName, RT_RCDATA);
  if (hRes == NULL) {
    print("FindResource %s\n", resourceName);
    return -1;
  }
  DWORD dwResourceSize = SizeofResource(NULL, hRes);
  assert(pitch >= width);
  assert(pitch * height == dwResourceSize);

  HGLOBAL hData = LoadResource(NULL, hRes);
  D3D10_SUBRESOURCE_DATA subresourceData;
  subresourceData.pSysMem = LockResource(hData);
  subresourceData.SysMemPitch = pitch;
  D3D10_TEXTURE2D_DESC textureDesc;
  textureDesc.Width = width;
  textureDesc.Height = height;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = format;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;
  textureDesc.Usage = D3D10_USAGE_IMMUTABLE;
  textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
  textureDesc.CPUAccessFlags = 0;
  textureDesc.MiscFlags = 0;

  ID3D10Texture2D * pTexture;
  hr = g_pd3dDevice->CreateTexture2D(&textureDesc, &subresourceData, &pTexture);
  if (FAILED(hr)) {
    print("CreateTexture2D\n");
    return hr;
  }

  D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;
  descSRV.Format = textureDesc.Format;
  descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
  descSRV.Texture2D.MostDetailedMip = 0;
  descSRV.Texture2D.MipLevels = 1;
  hr = g_pd3dDevice->CreateShaderResourceView(pTexture, &descSRV, pTextureShaderResourceView);
  if (FAILED(hr)) {
    print("CreateShaderResourceView\n");
    return hr;
  }

  return S_OK;
}

HRESULT LoadTexture3D(const wchar_t * resourceName,
                      const int width,
                      const int height,
                      const int depth,
                      const int pitch,
                      const int slicePitch,
                      const DXGI_FORMAT format,
                      ID3D10ShaderResourceView ** pTextureShaderResourceView)
{
  HRESULT hr;

  HRSRC hRes = FindResource(NULL, resourceName, RT_RCDATA);
  if (hRes == NULL) {
    print("FindResource %s\n", resourceName);
    return -1;
  }

  DWORD dwResourceSize = SizeofResource(NULL, hRes);
  assert(pitch >= width);
  assert(slicePitch >= depth);
  assert(slicePitch * depth == dwResourceSize);

  HGLOBAL hData = LoadResource(NULL, hRes);
  D3D10_SUBRESOURCE_DATA subresourceData;
  subresourceData.pSysMem = LockResource(hData);
  subresourceData.SysMemPitch = pitch;
  subresourceData.SysMemSlicePitch = slicePitch;
  D3D10_TEXTURE3D_DESC textureDesc;
  textureDesc.Width = width;
  textureDesc.Height = height;
  textureDesc.Depth = depth;
  textureDesc.MipLevels = 1;
  textureDesc.Format = format;
  textureDesc.Usage = D3D10_USAGE_IMMUTABLE;
  textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
  textureDesc.CPUAccessFlags = 0;
  textureDesc.MiscFlags = 0;

  ID3D10Texture3D * pTexture;
  hr = g_pd3dDevice->CreateTexture3D(&textureDesc, &subresourceData, &pTexture);
  if (FAILED(hr)) {
    print("CreateTexture3D\n");
    return hr;
  }

  D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;
  descSRV.Format = textureDesc.Format;
  descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE3D;
  descSRV.Texture3D.MostDetailedMip = 0;
  descSRV.Texture3D.MipLevels = 1;
  hr = g_pd3dDevice->CreateShaderResourceView(pTexture, &descSRV, pTextureShaderResourceView);
  if (FAILED(hr)) {
    print("CreateShaderResourceView\n");
    return hr;
  }

  return S_OK;
}

HRESULT CreateTextureRenderTargetView(const int width,
                                      const int height,
                                      ID3D10RenderTargetView ** pRenderTargetView,
                                      ID3D10ShaderResourceView ** pTextureShaderResourceView)
{
  HRESULT hr;

  D3D10_TEXTURE2D_DESC textureDesc;
  textureDesc.Width = width;
  textureDesc.Height = height;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;
  textureDesc.Usage = D3D10_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
  textureDesc.CPUAccessFlags = 0;
  textureDesc.MiscFlags = 0;

  ID3D10Texture2D * pTexture;

  hr = g_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &pTexture);
  if (FAILED(hr)) {
    print("CreateTexture2D\n");
    return hr;
  }

  hr = g_pd3dDevice->CreateRenderTargetView(pTexture, NULL, pRenderTargetView);
  if (FAILED(hr)) {
    print("g_pd3dDevice->CreateRenderTargetView(pTexture)\n");
    return hr;
  }

  D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;
  descSRV.Format = textureDesc.Format;
  descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
  descSRV.Texture2D.MostDetailedMip = 0;
  descSRV.Texture2D.MipLevels = 1;
  hr = g_pd3dDevice->CreateShaderResourceView(pTexture, &descSRV, pTextureShaderResourceView);
  if (FAILED(hr)) {
    print("CreateShaderResourceView\n");
    return hr;
  }

  return S_OK;
}
