#ifndef _RENDER_STATE_HPP_
#define _RENDER_STATE_HPP_

HRESULT LoadTexture2D(const wchar_t * resourceName,
                      const int width,
                      const int height,
                      const int pitch,
                      const DXGI_FORMAT format,
                      ID3D10ShaderResourceView ** pTextureShaderResourceView);

HRESULT LoadTexture3D(const wchar_t * resourceName,
                      const int width,
                      const int height,
                      const int depth,
                      const int pitch,
                      const int slicePitch,
                      const DXGI_FORMAT format,
                      ID3D10ShaderResourceView ** pTextureShaderResourceView);

HRESULT CreateTextureRenderTargetView(const int width,
                                      const int height,
                                      ID3D10Texture2D ** pTexture,
                                      ID3D10RenderTargetView ** pRenderTargetView);

HRESULT CreateTextureRenderTargetView(const int width,
                                      const int height,
                                      ID3D10RenderTargetView ** pRenderTargetView,
                                      ID3D10ShaderResourceView ** pTextureShaderResourceView);

#endif _RENDER_STATE_HPP_
