#ifndef GLTF_HPP_
#define GLTF_HPP_

struct Mesh {
  const XMFLOAT3 * position;
  const int position_size;

  const XMFLOAT3 * normal;
  const int normal_size;

  const XMFLOAT2 * texcoord_0;
  const int texcoord_0_size;

  const XMFLOAT4 * weights_0;
  const int weights_0_size;

  const XMFLOAT4 * joints_0;
  const int joints_0_size;

  const int * indices;
  const int indices_size;
};

struct Skin;

struct Node {
  const int parent_ix;
  const Skin * skin; // skin index (global)
  const Mesh * mesh; // mesh index (global)
  const XMFLOAT3 translation;
  const XMFLOAT4 rotation;
  const XMFLOAT3 scale;
};

struct Skin {
  const XMMATRIX * inverse_bind_matrices; // accessor
  const int * joints;
  int joints_length;
};

enum AnimationChannelPath {
  ACP__WEIGHTS,
  ACP__ROTATION,
  ACP__TRANSLATION,
  ACP__SCALE,
};

struct AnimationSampler {
  const float * input;  // accessor index, containing keyframe timestamps
  const void * output; // accessor index, containing keyframe values (type depends on channel target path)
  const int length;
};

struct AnimationChannel {
  const AnimationSampler * sampler; // sampler index, this animation
  struct {
    const int node_ix;
    const AnimationChannelPath path; // property to animate
  } target;
};

#endif
