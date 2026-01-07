#ifndef _RENDER_STATE_HPP_
#define _RENDER_STATE_HPP_

HRESULT LoadTexture(const wchar_t * resourceName,
                    const int width,
                    const int height,
                    const int pitch,
                    const DXGI_FORMAT format,
                    ID3D10ShaderResourceView ** pTextureShaderResourceView);

#endif _RENDER_STATE_HPP_
