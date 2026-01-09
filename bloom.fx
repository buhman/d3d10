float2 vInvScreenSize;
float2 vDir;

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

static const float offset[11] = {
  -9.406430666971303,
  -7.425801606895373,
  -5.445401742210555,
  -3.465172537482815,
  -1.485055021558738,
  0.4950160492928826,
  2.4751038298192056,
  4.455269417428358,
  6.435576703455285,
  8.41608382089975,
  10
};

static const float weight[11] = {
  0.0276904183309881,
  0.05417056378718292,
  0.09049273288108622,
  0.12908964856395883,
  0.15725301673321052,
  0.16358389071865348,
  0.14531705460040129,
  0.11023607138371759,
  0.0714102715628023,
  0.03950209624702099,
  0.011254235190977919
};

float4 PS(PS_INPUT input) : SV_Target
{
  float4 texColor = float4(0, 0, 0, 0);
  for (int i = 0; i < 11; i++) {
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
  float4 texColor = float4(0, 0, 0, 0);
  for (int i = 0; i < 11; i++) {
    float2 texOffset = vDir * offset[i] * vInvScreenSize;
    texColor += txDiffuseA.Sample(samPoint, input.Tex + texOffset) * weight[i];
  }

  texColor = float4(1, 1, 1, 1) - exp2(-texColor * 3.0);

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
