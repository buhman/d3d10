float2 vInvScreenSize;
float2 vDir;

Texture2D txDiffuse;
SamplerState samPoint {
  Filter = MIN_MAG_MIP_POINT;
  AddressU = Wrap;
  AddressV = Wrap;
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

static const float offset[] = {0.0, 1.0, 2.0, 3.0, 4.0};
static const float weight[] = { // sigma 1.75, radius 4
  0.22720403874968215,
  0.19382872916158458,
  0.12033793944552469,
  0.05436431541122633,
  0.01786699660682331,
};

float4 PS(PS_INPUT input) : SV_Target
{
  float4 texColor = txDiffuse.Sample(samPoint, input.Tex) * weight[0];
  for (int i = 1; i < 5; i++) {
    float2 texOffset = vDir * offset[i] * vInvScreenSize;
    texColor += txDiffuse.Sample(samPoint, input.Tex + texOffset) * weight[i];
    texColor += txDiffuse.Sample(samPoint, input.Tex - texOffset) * weight[i];
  }

  return texColor;
}

BlendState Blending
{
  BlendEnable[0] = FALSE;
};

DepthStencilState DisableDepth
{
  DepthEnable = FALSE;
  DepthWriteMask = ZERO;
};

technique10 Bloom
{
  pass P0
  {
    SetVertexShader(CompileShader(vs_4_0, VS()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_4_0, PS()));
    SetBlendState(Blending, float4(0.0, 0.0, 0.0, 0.0), 0xffffffff);
    SetDepthStencilState(DisableDepth, 0);
  }
}
