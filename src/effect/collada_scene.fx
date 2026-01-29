cbuffer cbEveryFrame
{
  matrix View;
  matrix Projection;

  float4 ViewEye;

  float4 LightPos[2];
  float4 LightDir[2];
  float4 LightColor[2];
};

cbuffer cbMultiplePerFrame
{
  matrix World;
};

cbuffer cbPerMaterial
{
  float4 Emission;
  float4 Ambient;
  float4 Diffuse;
  float4 Specular;
  float Shininess;
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
  float4 WPos : POSITION;
};

PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output;

  float4 world_pos = mul(float4(input.Pos, 1), World);
  output.Pos = mul(world_pos, View);
  output.Pos = mul(output.Pos, Projection);

  output.Norm = mul(input.Norm, (float3x3)World);
  output.Tex = input.Tex;

  output.WPos = world_pos;

  return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
  float3 normal = normalize(input.Norm);
  float3 view_dir = normalize(ViewEye.xyz - input.Pos.xyz);

  float3 color = Emission.xyz;

  for (int i = 0; i < 2; i++) {
    float3 light_dir = normalize(-LightDir[i].xyz);
    float diffuse_intensity = max(dot(normal, light_dir), 0.0);
    color += Diffuse.xyz * diffuse_intensity * LightColor[i].xyz;
  }

  return float4(color.xyz, 1);
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
