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

  struct float7 {
    float const a;
    float const b;
    float const c;
    float const d;
    float const e;
    float const f;
    float const g;
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

  // inputs uniqueness is by evaluted pointer
  struct inputs {
    input_element const * const elements;
    int const elements_count;
  };

  struct triangles {
    int const count;
    int const index_offset;
    inputs const inputs;
  };

  struct mesh {
    // `triangles` must become a union if non-triangles are implemented.
    // instance_geometry is an index into this array.
    triangles const * triangles;
    int const triangles_count;

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
      float7 const skew;
      float3 const translate;
    };
  };

  enum class node_type {
    JOINT,
    NODE,
  };


  struct color_or_texture {
    union {
      float4 color;
    };
  };

  struct blinn {
    color_or_texture const emission;
    color_or_texture const ambient;
    color_or_texture const diffuse;
    color_or_texture const specular;
    float const shininess;
    color_or_texture const reflective;
    float const reflectivity;
    color_or_texture const transparent;
    float const transparency;
    float const index_of_refraction;
  };

  struct lambert {
    color_or_texture const emission;
    color_or_texture const ambient;
    color_or_texture const diffuse;
    color_or_texture const reflective;
    float const reflectivity;
    color_or_texture const transparent;
    float const transparency;
    float const index_of_refraction;
  };

  struct phong {
    color_or_texture const emission;
    color_or_texture const ambient;
    color_or_texture const diffuse;
    color_or_texture const specular;
    float const shininess;
    color_or_texture const reflective;
    float const reflectivity;
    color_or_texture const transparent;
    float const transparency;
    float const index_of_refraction;
  };

  struct constant {
    float4 const color;
    color_or_texture const reflective;
    float const reflectivity;
    color_or_texture const transparent;
    float const transparency;
    float const index_of_refraction;
  };

  enum class effect_type {
    BLINN,
    LAMBERT,
    PHONG,
    CONSTANT,
  };

  struct effect {
    effect_type const type;
    union {
      blinn const blinn;
      lambert const lambert;
      phong const phong;
      constant const constant;
    };
  };

  struct material {
    effect const * const effect;
  };

  struct instance_material {
    int element_index;
    material const * const material;
  };

  struct instance_geometry {
    geometry const * const geometry;

    instance_material const * const instance_materials;
    int const instance_materials_count;
  };

  struct node {
    node_type const type;

    transform const * const transforms;
    int const transforms_count;

    instance_geometry const * const instance_geometries;
    int const instance_geometries_count;

    node const * const nodes;
    int const nodes_count;
  };
}
