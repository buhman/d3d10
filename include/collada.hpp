#pragma once

#ifndef COLLADA_HPP_
#define COLLADA_HPP_

namespace collada {
  HRESULT LoadEffect();
  HRESULT LoadMesh();
  void Render(float t);
}

#endif
