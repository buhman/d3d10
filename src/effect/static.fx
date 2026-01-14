struct VS_INPUT
{
  float4 Pos : POSITION;
  float3 Normal : NORMAL;
  float2 Tex : TEXCOORD0;
};

struct VS_INPUT_INSTANCED
{
  float4 Pos : POSITION;
  float3 Normal : NORMAL;
  float2 Tex : TEXCOORD0;
  row_major float4x4 mTransform : mTransform;
  uint InstanceID : SV_InstanceID;
};

struct PS_INPUT
{
  float4 Pos : SV_POSITION;
  float3 Normal : NORMAL;
  float2 Tex : TEXCOORD0;
};

struct PS_OUTPUT
{
  float4 color0 : SV_TARGET0;
  float4 color1 : SV_TARGET1;
};

cbuffer ceveryframe
{
  matrix World;
  matrix View;
  matrix Projection;
  matrix<float, 3, 3> WorldNormal;
};

cbuffer crarely
{
  float3 vOutputColor;
};

PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT)0;

  output.Pos = mul(input.Pos, World);
  output.Pos = mul(output.Pos, View);
  output.Pos = mul(output.Pos, Projection);

  output.Normal = mul(input.Normal, WorldNormal).xyz;

  output.Tex = input.Tex;

  return output;
}

PS_OUTPUT PS(PS_INPUT input)
{
  PS_OUTPUT output;
  output.color0 = float4(vOutputColor, 1.0);
  output.color1 = float4(vOutputColor, 1.0);
  return output;
}

PS_INPUT VSInstanced(VS_INPUT_INSTANCED input)
{
  VS_INPUT input_vs = (VS_INPUT)0;

  input_vs.Pos = mul(float4(input.Pos.xyz, 1), input.mTransform);
  //input_vs.Pos = float4(input.Pos.xyz, 1) + float4(0, input.InstanceID * 3, 0, 0);
  input_vs.Normal = input.Normal;
  input_vs.Tex = input.Tex;

  return VS(input_vs);
}

float4 PSInstanced(PS_INPUT input) : SV_TARGET0
{
  return float4(1, 1, 1, 1);
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

technique10 Static
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

technique10 StaticInstanced
{
  pass P0
  {
    SetVertexShader(CompileShader(vs_4_0, VSInstanced()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_4_0, PSInstanced()));
    SetBlendState(DisableBlending, float4(0.0, 0.0, 0.0, 0.0), 0xffffffff);
    SetDepthStencilState(EnableDepth, 0);
  }
}
