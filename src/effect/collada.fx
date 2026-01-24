matrix World;
matrix View;
matrix Projection;

matrix mJoint[2];

struct VS_INPUT
{
  float4 Pos : POSITION;
  float3 Normal : NORMAL;
  float2 Tex : TEXCOORD0;
  float4 Joint : BLENDINDICES0;
  float4 Weight : BLENDWEIGHT0;
};

struct PS_INPUT
{
  float4 Pos : SV_POSITION;
  float3 Normal : NORMAL;
  float2 Tex : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output;

  matrix mSkin
    = input.Weight.x * mJoint[int(input.Joint.x)]
    + input.Weight.y * mJoint[int(input.Joint.y)]
    + input.Weight.z * mJoint[int(input.Joint.z)]
    + input.Weight.w * mJoint[int(input.Joint.w)]
    ;

  output.Pos = mul(input.Pos, mSkin);
  output.Pos = mul(output.Pos, World);
  output.Pos = mul(output.Pos, View);
  output.Pos = mul(output.Pos, Projection);

  output.Normal = input.Normal;
  output.Tex = input.Tex;

  return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
  //return float4(input.Normal * 0.5 + 0.5, 1);
  return float4(input.Tex.xy, 0, 1);
}

BlendState DisableBlending
{
  BlendEnable[0] = FALSE;
};

DepthStencilState EnableDepth
{
  DepthEnable = TRUE;
  DepthWriteMask = ALL;
};

technique10 Render
{
  pass P0
  {
    SetVertexShader(CompileShader(vs_4_0, VS()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_4_0, PS()));
    SetBlendState(DisableBlending, float4(0.0, 0.0, 0.0, 0.0), 0xffffffff);
    SetDepthStencilState(EnableDepth, 0);
  }
}
