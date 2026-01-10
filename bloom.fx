float2 vInvScreenSize;
float2 vDir;
float fExposure;

Texture2D txDiffuseA;
SamplerState samPoint {
  Filter = MIN_MAG_MIP_LINEAR;
  AddressU = CLAMP;
  AddressV = CLAMP;
};

struct VS_INPUT
{
  float4 Pos : POSITION;
};

struct PS_INPUT
{
  float4 Pos : SV_POSITION;
  float2 Tex : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output;

  output.Pos = input.Pos;
  output.Tex = input.Pos.xy * float2(0.5, -0.5) + float2(0.5, 0.5);

  return output;
}

static const float offset[6] = {
  -4.1802892785260735,
  -2.3013875682370335,
  -0.45807799821605544,
  1.3765284379445557,
  3.2355245111649937,
  5
};

static const float weight[6] = {
  0.019171156084708173,
  0.16924312602932176,
  0.4273923867820408,
  0.31401440082441,
  0.06672257638049649,
  0.003456353899022774
};

float4 PS(PS_INPUT input) : SV_Target
{
  float4 texColor = float4(0, 0, 0, 0);
  for (int i = 0; i < 6; i++) {
    float2 texOffset = vDir * offset[i] * vInvScreenSize;
    texColor += txDiffuseA.Sample(samPoint, input.Tex + texOffset) * weight[i];
  }
  return texColor;
}

BlendState DisableBlending
{
  BlendEnable[0] = FALSE;
};

BlendState EnableBlending
{
  BlendEnable[0] = TRUE;
  SrcBlend = ONE;
  DestBlend = ONE;
  BlendOp = ADD;
  SrcBlendAlpha = ZERO;
  DestBlendAlpha = ZERO;
  BlendOpAlpha = ADD;
};

DepthStencilState DisableDepth
{
  DepthEnable = FALSE;
  DepthWriteMask = ZERO;
};

float4 PSBlend(PS_INPUT input) : SV_Target
{
  float4 texColor = PS(input);
  texColor = float4(1, 1, 1, 1) - exp2(-texColor * fExposure);
  return float4(texColor.xyz, 1);
}

technique10 Bloom
{
  pass P0
  {
    SetVertexShader(CompileShader(vs_4_0, VS()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_4_0, PS()));
    SetBlendState(DisableBlending, float4(0.0, 0.0, 0.0, 0.0), 0xffffffff);
    SetDepthStencilState(DisableDepth, 0);
  }
}

technique10 BloomBlend
{
  pass P0
  {
    SetVertexShader(CompileShader(vs_4_0, VS()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_4_0, PSBlend()));
    SetBlendState(EnableBlending, float4(0.0, 0.0, 0.0, 0.0), 0xffffffff);
    SetDepthStencilState(DisableDepth, 0);
  }
}
