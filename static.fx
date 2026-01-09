matrix World;
matrix View;
matrix Projection;

matrix WorldNormal;

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

struct PS_OUTPUT
{
  float4 color0 : SV_TARGET0;
  float4 color1 : SV_TARGET1;
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
