union Accessor {
  DWORD * dw;
  float * f;
  D3DXVECTOR3 * v3;
  D3DXVECTOR4 * v4;
};

struct Mesh {
  int position;
  int normal;
  int texcoord_0;
  int indices;
};
