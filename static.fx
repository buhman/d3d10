matrix World;
matrix View;
matrix Projection;

float3 vOutputColor;

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

PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT)0;

  output.Pos = mul(input.Pos, World);
  output.Pos = mul(output.Pos, View);
  output.Pos = mul(output.Pos, Projection);

  output.Normal = mul(output.Normal, World).xyz;

  output.Tex = input.Tex;

  return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
  return float4(vOutputColor, 1.0);
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
