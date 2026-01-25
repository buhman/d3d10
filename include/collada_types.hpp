#pragma once

namespace collada {

  struct float3 {
    float const x;
    float const y;
    float const z;
  };

  struct float4 {
    float const x;
    float const y;
    float const z;
    float const w;
  };

  //////////////////////////////////////////////////////////////////////
  // animation
  //////////////////////////////////////////////////////////////////////

  enum class interpolation {
    LINEAR,
    BEZIER,
  };

  struct source {
    union {
      float const * const float_array;
      enum interpolation const name_array;
    };
    int const count;
    int const stride;
  };

  struct sampler {
    source const input;
    source const output;
    source const intangent;
    source const outangent;
    source const interpolation;
  };

  //////////////////////////////////////////////////////////////////////
  // geometry
  //////////////////////////////////////////////////////////////////////

  enum class input_format {
    FLOAT3,
    FLOAT4,
    INT4,
  };

  struct input_element {
    char const * const semantic;
    int const semantic_index;
    enum input_format const format;
  };

  struct mesh {
    input_element const * const input_elements;
    int const input_elements_count;

    int const vertex_buffer_offset;
    int const vertex_buffer_size;

    int const index_buffer_offset;
    int const index_buffer_size;
  };

  struct geometry {
    mesh mesh;
  };

  //////////////////////////////////////////////////////////////////////
  // node
  //////////////////////////////////////////////////////////////////////

  struct lookat {
    float3 const eye;
    float3 const at;
    float3 const up;
  };

  struct matrix {
    float const _11, _12, _13, _14;
    float const _21, _22, _23, _24;
    float const _31, _32, _33, _34;
    float const _41, _42, _43, _44;
  };

  enum class transform_type {
    LOOKAT,
    MATRIX,
    ROTATE,
    SCALE,
    TRANSLATE,
  };

  struct transform {
    transform_type const type;
    union {
      lookat const lookat;
      matrix const matrix;
      float4 const rotate;
      float3 const scale;
      float3 const translate;
    };
  };

  enum class node_type {
    JOINT,
    NODE,
  };

  struct node {
    node_type const type;

    transform const * const transforms;
    int const transforms_count;

    geometry const * const geometries;
    int const geometries_count;

    node const * const nodes;
    int const nodes_count;
  };
}
