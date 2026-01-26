cbuffer cbEveryFrame
{
  matrix View;
  matrix Projection;
};

cbuffer cbMultiplePerFrame
{
  matrix World;
};

struct VS_INPUT
{
  float3 Pos : POSITION;
  float3 Norm : NORMAL;
  float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
  float4 Pos : SV_POSITION;
  float3 Norm : NORMAL;
  float2 Tex : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output;

  output.Pos = mul(float4(input.Pos, 1), World);
  output.Pos = mul(output.Pos, View);
  output.Pos = mul(output.Pos, Projection);

  output.Norm = mul(input.Norm, (float3x3)World);
  output.Tex = input.Tex;

  return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
  //return float4(input.Normal * 0.5 + 0.5, 1);
  return float4(input.Tex.xy, 0, 1);
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

technique10 Blinn
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
