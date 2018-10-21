float4x4 TransformMat;
float FogStart;
float FogEnd;
float4 FogColor;
float4 CameraPos;
texture Texture;

sampler Sampler = sampler_state
{
  Texture = Texture;
  MipFilter = LINEAR;
  MinFilter = LINEAR;
  MagFilter = LINEAR;
};

struct VertexOut
{
  float4 Pos : POSITION;
  float2 Tex : TEXCOORD0;
  float Fog : TEXCOORD1;
};

VertexOut VShader(float4 Pos : POSITION, float2 Tex : TEXCOORD0)
{
  VertexOut Vert = (VertexOut)0;
  Vert.Pos = mul(Pos, TransformMat);
  Vert.Tex = Tex;
  float dist = length(CameraPos - Vert.Pos);
  Vert.Fog = clamp((dist - FogStart) / (FogEnd - FogStart), 0, 0.9f);
  return Vert;
}

float4 PShader(VertexOut v) : COLOR
{
  float4 color = tex2D(Sampler, v.Tex);
  float4 newColor = lerp(color, FogColor, v.Fog);
  newColor.a = color.a;
  return newColor;
}

technique FirstTechnique
{
  pass FirstPass
  {
    Lighting = FALSE;
    ZEnable = TRUE;
    AlphaRef = 0x0000008f;
    AlphaTestEnable = TRUE;
    AlphaFunc = GREATEREQUAL;
    VertexShader = compile vs_2_0 VShader();
    PixelShader = compile ps_2_0 PShader();
  }
}
