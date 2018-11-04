float4x4 _ObjectToWorld;
float4 _FogColor;
float4 _CameraPos;
float4 _ObjectPos;
float _Time;
float _FogStart;
float _FogEnd;
float _BendScale;
float _WindSpeed;
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

float4 SmoothCurve(float4 x)
{
  return x * x * (3.0 - 2.0 * x);
}

float4 TriangleWave(float4 x)
{
  return abs(frac(x + 0.5) * 2.0 - 1.0);
}

float4 SmoothTriangleWave(float4 x)
{
  return SmoothCurve(TriangleWave(x));
}

VertexOut VShader(VertexIn v)
{
  if(_IsFoliage)
  {
    float greenComp = clamp(v.Color.g - v.Color.b - v.Color.r, 0.2, 1);
    float objPhase = dot(mul(_ObjectPos, _ObjectToWorld), 1);
    float vtxPhase = dot(v.Pos, objPhase);
    float2 wavesIn = _Time + float2(vtxPhase, objPhase);
    float4 wavesOut = (frac(wavesIn.xxyy * float4(1.975, 0.793, 0.375, 0.193))
      * 2.0 - 1.0) * _BendScale * _WindSpeed;
    wavesOut = SmoothTriangleWave(wavesOut);
    float2 wavesSum = wavesOut.xz + wavesOut.yw;
    v.Pos.xyz += wavesSum.xxy * float3(greenComp * v.Norm.xy, greenComp);
  }

  VertexOut Vert = (VertexOut)0;
  Vert.Pos = mul(v.Pos, _ObjectToWorld);
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
