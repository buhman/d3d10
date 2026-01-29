#pragma once

#include "dds.h"

struct DDS_FILE {
  DWORD dwMagic;
  DDS_HEADER header;
};

void dds_validate(DDS_FILE const * const buf, unsigned int size,
                  D3D10_SUBRESOURCE_DATA * subresourceData);
