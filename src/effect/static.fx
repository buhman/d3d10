struct VS_INPUT
{
  float4 Pos : POSITION;
  float3 Normal : NORMAL;
  float2 Tex : TEXCOORD0;
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

struct VS_INPUT_INSTANCED
{
  float4 Pos : POSITION;
  float3 Normal : NORMAL;
  float2 Tex : TEXCOORD0;
  row_major float4x4 mTransform : mTransform;
  uint InstanceID : SV_InstanceID;
};

struct PS_INPUT_INSTANCED
{
  float4 Pos : SV_POSITION;
  float4 Color : TEXCOORD0;
};

Texture3D txDiffuse;
SamplerState samLinear {
  Filter = MIN_MAG_MIP_LINEAR;
  AddressU = Wrap;
  AddressV = Wrap;
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

PS_INPUT_INSTANCED VSInstanced(VS_INPUT_INSTANCED input)
{
  PS_INPUT_INSTANCED output = (PS_INPUT_INSTANCED)0;

  output.Pos = mul(float4(input.Pos.xyz * 0.01f, 1), input.mTransform);
  output.Pos = float4(output.Pos.xyz * 4.0f, 1.0f);
  output.Pos = mul(output.Pos, World);
  output.Pos = mul(output.Pos, View);
  output.Pos = mul(output.Pos, Projection);

  output.Color = input.mTransform[3] * 0.5 + 0.5;

  return output;
}

float4 PSInstanced(PS_INPUT_INSTANCED input) : SV_TARGET0
{
  float4 texColor = txDiffuse.Sample(samLinear, input.Color.xyz);

  float c = pow(texColor.x, 3);

  //return input.Color;
  return float4(c, c, c, 1);
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
