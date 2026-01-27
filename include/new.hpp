#pragma once

template <typename T>
T * New(int elements)
{
  return (T *)_aligned_malloc((sizeof (T)) * elements, 16);
}
