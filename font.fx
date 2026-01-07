float2 vInvScreenSize;
float2 vGlyphScale;
float2 vTexScale;

Texture2D txDiffuse;
SamplerState samPoint {
  Filter = MIN_MAG_MIP_POINT;
  AddressU = Wrap;
  AddressV = Wrap;
};

struct VS_INPUT
{
  float4 Pos : TEXCOORD;
};

struct GS_INPUT
{
  float4 Pos : SV_POSITION;
};

struct PS_INPUT
{
  float4 Pos : SV_POSITION;
  float2 Tex : TEXCOORD0;
};


GS_INPUT VS(VS_INPUT input)
{
  GS_INPUT output;

  output.Pos = input.Pos;

  return output;
}

static const float2 vertices[] = {
  float2( 0.0f,  0.0f), // -- top right
  float2( 1.0f,  0.0f), // -- top left
  float2( 0.0f, -1.0f), // -- bottom right
  float2( 1.0f, -1.0f), // -- bottom left
};

[maxvertexcount(4)]
void GS (point GS_INPUT input[1], inout TriangleStream<PS_INPUT> TriStream)
{
  PS_INPUT output;

  for (int i = 0; i < 4; i++) {
    float2 Pos;
    Pos = vertices[i] * vGlyphScale + input[0].Pos.xy;
    Pos = Pos * vInvScreenSize + float2(-1, 1);
    output.Pos = float4(Pos.xy, 0, 1);

    output.Tex = vertices[i] * float2(1, -1);
    output.Tex = (output.Tex + input[0].Pos.zw) * vTexScale;

    TriStream.Append(output);
  }
  TriStream.RestartStrip();
}

float4 PS(PS_INPUT input) : SV_Target
{
  float4 texColor = txDiffuse.Sample(samPoint, input.Tex);

  float c = texColor.x == 0 ? 0.0 : 1.0;

  return float4(c.xxx, 1);
}

BlendState Blending
{
  BlendEnable[0] = TRUE;
  SrcBlend = ONE;
  DestBlend = ONE;
  BlendOp = ADD;
  SrcBlendAlpha = ZERO;
  DestBlendAlpha = ZERO;
  BlendOpAlpha = ADD;
};

technique10 Font
{
  pass P0
  {
    SetVertexShader(CompileShader(vs_4_0, VS()));
    SetGeometryShader(CompileShader(gs_4_0, GS()));
    SetPixelShader(CompileShader(ps_4_0, PS()));
    SetBlendState(Blending, float4(0.0, 0.0, 0.0, 0.0), 0xffffffff);
  }
}
