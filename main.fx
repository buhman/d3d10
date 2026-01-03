matrix World;
matrix View;
matrix Projection;

matrix mJoint[2];

float4 vLightDir[2];
float4 vLightColor[2];
float4 vOutputColor;

Texture2D txDiffuse;
SamplerState samLinear {
  Filter = MIN_MAG_MIP_LINEAR;
  AddressU = Wrap;
  AddressV = Wrap;
};

struct VS_INPUT
{
  float4 Pos : POSITION;
  float4 Weight : TEXCOORD0;
  float4 Joint : TEXCOORD1;
  //float3 Normal : NORMAL;
  //float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
  float4 Pos : SV_POSITION;
  float4 Weight : TEXCOORD0;
  float4 Joint : TEXCOORD1;
  //float3 Normal : TEXCOORD0;
  //float2 Tex : TEXCOORD1;
};

PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT)0;

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

  output.Weight = input.Weight;
  output.Joint = input.Joint;

  //output.Normal = mul(input.Normal, World);
  //output.Tex = input.Tex;
  return output;
}

/*
float4 PS(PS_INPUT input) : SV_Target
{
  float4 texColor = txDiffuse.Sample(samLinear, input.Tex);

  float4 intensityColor = 0;
  for (int i = 0; i < 2; i++) {
    intensityColor += saturate(dot((float3)vLightDir[i], input.Normal) * vLightColor[i]);
  }
  intensityColor.a = 1;

  return texColor * intensityColor;
}
*/

float4 PSSolid(PS_INPUT input) : SV_Target
{
  //return vOutputColor;
  return float4(input.Weight.xy, input.Joint.y, 1);
}

technique10 Render
{
  pass P0
  {
    SetVertexShader(CompileShader(vs_4_0, VS()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_4_0, PSSolid()));
  }
}

technique10 RenderLight
{
  pass P0
  {
    SetVertexShader(CompileShader(vs_4_0, VS()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_4_0, PSSolid()));
  }
}
