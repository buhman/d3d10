float2 vInvScreenSize;
float2 vPosition;
float2 vGlyphScale;
float2 vCharCoord;
float2 vTexScale;

Texture2D txDiffuse;
SamplerState samPoint {
  Filter = MIN_MAG_MIP_POINT;
  AddressU = Wrap;
  AddressV = Wrap;
};

struct VS_INPUT
{
  float2 Pos : POSITION;
};

struct PS_INPUT
{
  float4 Pos : SV_POSITION;
  float2 Tex : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT)0;

  float2 Pos = input.Pos * vGlyphScale + vPosition;
  Pos = Pos * vInvScreenSize + float2(-1, 1);
  output.Pos = float4(Pos.xy, 0, 1);

  float2 Tex = float2(input.Pos.x, -input.Pos.y);

  output.Tex = (Tex + vCharCoord) * vTexScale;
  return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
  float4 texColor = txDiffuse.Sample(samPoint, input.Tex);

  float c = texColor.x == 0 ? 0.0 : 1.0;

  return float4(c, c, c, 1);
}

technique10 Font
{
  pass P0
  {
    SetVertexShader(CompileShader(vs_4_0, VS()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_4_0, PS()));
  }
}
