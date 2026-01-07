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

struct GSPS_INPUT
{
  float4 Pos : SV_POSITION;
  float2 Tex : TEXCOORD0;
};

GSPS_INPUT VS(VS_INPUT input)
{
  GSPS_INPUT output = (GSPS_INPUT)0;

  output.Pos = float4(0, 0, 0, 0);
  output.Tex = float2(0, 0);

  return output;
}

static const float2 vertices[] = {
  float2( 0.0f,  0.0f), // -- top right
  float2( 1.0f,  0.0f), // -- top left
  float2( 0.0f, -1.0f), // -- bottom right
  float2( 1.0f, -1.0f), // -- bottom left
};

[maxvertexcount(4)]
void GS (point GSPS_INPUT input[1], inout TriangleStream<GSPS_INPUT> TriStream)
{
  GSPS_INPUT output;

  for (int i = 0; i < 4; i++) {
    float2 Pos;
    Pos = vertices[i] * vGlyphScale + vPosition;
    Pos = Pos * vInvScreenSize + float2(-1, 1);
    output.Pos = float4(Pos.xy, 0, 1);

    output.Tex = vertices[i] * float2(1, -1);
    output.Tex = (output.Tex + vCharCoord) * vTexScale;

    TriStream.Append(output);
  }
  TriStream.RestartStrip();
}

float4 PS(GSPS_INPUT input) : SV_Target
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
    SetGeometryShader(CompileShader(gs_4_0, GS()));
    SetPixelShader(CompileShader(ps_4_0, PS()));
  }
}
