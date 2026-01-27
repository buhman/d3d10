#pragma once

#include <stdlib.h>

template <typename T>
T * New(int elements)
{
  return (T *)malloc((sizeof (T)) * elements);
}
