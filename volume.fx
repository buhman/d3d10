float layer;

Texture3D txDiffuse;
SamplerState samPoint {
  Filter = MIN_MAG_MIP_LINEAR;
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

  output.Pos = float4(input.Pos.xy, 0, 1);
  output.Tex = input.Pos.xy * float2(0.5, -0.5) + float2(0.5, 0.5);

  return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
  float4 texColor = txDiffuse.Sample(samPoint, float3(input.Tex.xy, layer));
  return float4(texColor.xxx, 1);
}

BlendState DisableBlending
{
  BlendEnable[0] = False;
};

DepthStencilState DisableDepth
{
  DepthEnable = FALSE;
  DepthWriteMask = ZERO;
};

technique10 Volume
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
