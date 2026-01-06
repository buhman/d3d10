#include <windows.h>
#include <assert.h>

#include <d3d10.h>
#include <d3dx10.h>

#include "globals.hpp"
#include "print.hpp"
#include "render_state.hpp"

HRESULT LoadTexture(const wchar_t * resourceName,
                    const int width,
                    const int height,
                    ID3D10ShaderResourceView ** pTextureShaderResourceView)
{
  HRESULT hr;

  // texture
  HRSRC hRobotPlayerRes = FindResource(NULL, resourceName, RT_RCDATA);
  if (hRobotPlayerRes == NULL) {
    print("FindResource %s\n");
    return -1;
  }
  DWORD dwResourceSize = SizeofResource(NULL, hRobotPlayerRes);
  assert(width * height * 4 == dwResourceSize);

  HGLOBAL hRobotPlayerData = LoadResource(NULL, hRobotPlayerRes);
  D3D10_SUBRESOURCE_DATA subresourceData;
  subresourceData.pSysMem = LockResource(hRobotPlayerData);
  subresourceData.SysMemPitch = width * 4;
  D3D10_TEXTURE2D_DESC textureDesc;
  textureDesc.Width = width;
  textureDesc.Height = height;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;
  textureDesc.Usage = D3D10_USAGE_DEFAULT;
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
