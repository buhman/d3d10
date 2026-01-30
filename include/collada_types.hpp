#pragma once

namespace collada {

  struct float2 {
    float const x;
    float const y;
  };

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

  struct matrix {
    float const _11, _12, _13, _14;
    float const _21, _22, _23, _24;
    float const _31, _32, _33, _34;
    float const _41, _42, _43, _44;
  };

  //////////////////////////////////////////////////////////////////////
  // geometry
  //////////////////////////////////////////////////////////////////////

  enum class input_format {
    FLOAT1,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    INT1,
    INT2,
    INT3,
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
    int const inputs_index;
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
  // light
  //////////////////////////////////////////////////////////////////////

  enum class light_type {
    AMBIENT,
    DIRECTIONAL,
    POINT,
    SPOT,
  };

  struct light {
    light_type type;
    float3 color;
  };

  //////////////////////////////////////////////////////////////////////
  // image
  //////////////////////////////////////////////////////////////////////

  struct image {
    const wchar_t * resource_name;
  };

  //////////////////////////////////////////////////////////////////////
  // effect
  //////////////////////////////////////////////////////////////////////

  enum color_or_texture_type {
    COLOR,
    TEXTURE,
  };

  struct texture {
    int const image_index; // index in to images
  };

  struct color_or_texture {
    color_or_texture_type type;
    union {
      float4 color;
      texture texture;
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

  //////////////////////////////////////////////////////////////////////
  // node
  //////////////////////////////////////////////////////////////////////

  struct lookat {
    float3 const eye;
    float3 const at;
    float3 const up;
  };

  enum class transform_type {
    LOOKAT,
    MATRIX,
    ROTATE,
    SCALE,
    SKEW,
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

  struct material {
    effect const * const effect;
  };

  struct bind_vertex_input {
    int input_set; // TEXCOORD semantic input slot
  };

  struct instance_material {
    int const element_index; // an index into mesh.triangles
    material const * const material;

    // heavily simplified from collada data model
    bind_vertex_input const emission;
    bind_vertex_input const ambient;
    bind_vertex_input const diffuse;
    bind_vertex_input const specular;
  };

  struct instance_geometry {
    geometry const * const geometry;

    instance_material const * const instance_materials;
    int const instance_materials_count;
  };

  struct skin {
    geometry const * const geometry; // source

    matrix const * const inverse_bind_matrices; // one per joint

    int const vertex_buffer_offset;
    int const vertex_buffer_size;
  };

  struct controller {
    skin skin;
  };

  struct instance_controller {
    controller const * const controller;

    //node const * const skeleton; // a reference to the root of the joint heirarchy

    int const * const joint_node_indices; // one per joint
    int const joint_count;

    instance_material const * const instance_materials;
    int const instance_materials_count;
  };

  struct instance_light {
    light const * const light;
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
      enum interpolation const * const interpolation_array;
    };
    int const count;
    int const stride;
  };

  struct sampler {
    source const input;
    source const output;
    source const in_tangent;
    source const out_tangent;
    source const interpolation;
  };

  enum class target_attribute {
    A, // alpha color component
    ANGLE, // euler angle
    B, // blue color component
    G, // green color component
    P, // third texture component
    Q, // fourth texture component
    R, // red color component
    S, // first texture coordinate
    T, // second texture coordinate
    TIME, // time in seconds
    U, // first generic parameter
    V, // second generic parameter
    W, // fourth cartesian coordinate
    X, // first cartesian coordinate
    Y, // second cartesian coordinate
    Z, // third cartesian coordinate
    ALL,
  };

  struct channel {
    sampler const * const source_sampler;
    int const target_node_index; // an index into the nodes array
    int const target_transform_index;
    target_attribute const target_attribute;
  };

  /*
  struct animation {
    animation const * const animations; // nested animations
    int const animations_count;

    channels const * const channels;
    int const channels_count;
  };
  */

  //////////////////////////////////////////////////////////////////////
  // scene
  //////////////////////////////////////////////////////////////////////

  struct node {
    int const parent_index;

    node_type const type;

    transform const * const transforms;
    int const transforms_count;

    instance_geometry const * const instance_geometries;
    int const instance_geometries_count;

    instance_controller const * const instance_controllers;
    int const instance_controllers_count;

    instance_light const * const instance_lights;
    int const instance_lights_count;

    channel const * const * const channels;
    int const channels_count;

    //node const * const * const nodes;
    //int const nodes_count;
  };

  struct descriptor {
    // these are only the top-level nodes; nodes may have children
    node const * const * const nodes;
    int const nodes_count;

    inputs const * const inputs_list;
    int const inputs_list_count;

    image const * const * const images;
    int const images_count;

    //animation const * const animations;
    //int const animations_count;
  };
}
