#ifndef GLTF_HPP_
#define GLTF_HPP_

struct Mesh {
  const D3DXVECTOR3 * position;
  const DWORD position_size;

  const D3DXVECTOR3 * normal;
  const DWORD normal_size;

  const D3DXVECTOR2 * texcoord_0;
  const DWORD texcoord_0_size;

  const D3DXVECTOR4 * weights_0;
  const DWORD weights_0_size;

  const D3DXVECTOR4 * joints_0;
  const DWORD joints_0_size;

  const DWORD * indices;
  const DWORD indices_size;
};

struct Skin;

struct Node {
  const DWORD parent_ix;
  const Skin * skin; // skin index (global)
  const Mesh * mesh; // mesh index (global)
  const D3DXVECTOR3 translation;
  const D3DXQUATERNION rotation;
  const D3DXVECTOR3 scale;
};

struct Skin {
  const D3DXMATRIX * inverse_bind_matrices; // accessor
  const int * joints;
  DWORD joints_length;
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
