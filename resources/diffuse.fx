float4x4 _WorldMat;
float _FogStart;
float _FogEnd;
float4 _FogColor;
float4 _CameraPos;
float _Time;
bool _IsFoliage;
texture _Texture;

sampler Sampler = sampler_state
{
  Texture = _Texture;
  MipFilter = LINEAR;
  MinFilter = LINEAR;
  MagFilter = LINEAR;
};

struct VertexIn
{
  float4 Pos : POSITION;
  float4 Norm : NORMAL0;
  float4 Color : COLOR0;
  float2 UV : TEXCOORD0;
};

struct VertexOut
{
  float4 Pos : POSITION;
  float4 Color : COLOR0;
  float2 UV : TEXCOORD0;
  float Fog : TEXCOORD1;
};

VertexOut VShader(VertexIn v)
{
  if(_IsFoliage)
  {
    const float4 windDir = float4(0.5, 0.5, 0.5, 0);
    const float windSpeed = 0.005;
    const float displacement = 0.008;
    float4 towardWind = normalize(v.Pos - windDir);
    float4 xz = float4(v.Pos.x, 0, v.Pos.z, 1);
    v.Pos.x += sin((_Time / dot(towardWind, xz)) * windSpeed) * displacement;
    v.Pos.z += cos((_Time / dot(towardWind, xz)) * windSpeed) * displacement;
  }

  VertexOut Vert = (VertexOut)0;
  Vert.Pos = mul(v.Pos, _WorldMat);
  Vert.Color = v.Color;
  Vert.UV = v.UV;
  float dist = length(_CameraPos - Vert.Pos);
  Vert.Fog = clamp((dist - _FogStart) / (_FogEnd - _FogStart), 0, 0.9f);
  
  return Vert;
}

float4 PShader(VertexOut v) : COLOR
{
  float4 color = tex2D(Sampler, v.UV);
  float4 newColor = lerp(color, _FogColor, v.Fog);
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
