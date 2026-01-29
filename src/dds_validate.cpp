#include <assert.h>
#include <d3d10.h>

#include "dds_validate.hpp"

static inline unsigned int max(unsigned int a, unsigned int b)
{
  return (a > b) ? a : b;
}

struct dds_size_levels {
  unsigned int const size;
  unsigned int const levels;
};

static inline unsigned int dim(unsigned int d)
{
  return max(1, (d / 4));
}

static inline dds_size_levels dds_mip_total_size(uintptr_t data,
                                                 unsigned int height,
                                                 unsigned int width,
                                                 unsigned int max_mip_levels,
                                                 D3D10_SUBRESOURCE_DATA * subresourceData)
{
  unsigned int mip_total_size = 0;
  unsigned int mip_levels = 0;
  while (true) {
    unsigned int mip_size = dim(height) * dim(width) * 8;
    mip_total_size += mip_size;
    subresourceData[mip_levels].pSysMem = (const void *)data;
    subresourceData[mip_levels].SysMemPitch = dim(width) * 8;
    mip_levels += 1;
    assert(mip_levels <= max_mip_levels);
    data += mip_size;

    if (width == 1 && height == 1)
      break;

    height /= 2;
    width /= 2;
  }

  return (dds_size_levels){mip_total_size, mip_levels};
}

void dds_validate(DDS_FILE const * const dds, unsigned int size,
                  D3D10_SUBRESOURCE_DATA * subresourceData)
{
  assert(dds->dwMagic == DDS_MAGIC);
  assert(dds->header.dwSize == 124);
  assert(dds->header.ddspf.dwSize == 32);
  assert(dds->header.ddspf.dwFlags == DDS_FOURCC);
  assert(dds->header.ddspf.dwFourCC == MAKEFOURCC('D','X','T','1'));

  assert(dds->header.dwDepth == 0);

  dds_size_levels ret = dds_mip_total_size(((uintptr_t)dds) + (sizeof (DDS_FILE)),
                                           dds->header.dwHeight,
                                           dds->header.dwWidth,
                                           dds->header.dwMipMapCount,
                                           subresourceData);
  assert(ret.size + (sizeof (DDS_FILE)) == size);
  assert(ret.levels == dds->header.dwMipMapCount);
}
