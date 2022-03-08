float4x4 _ObjectToWorld;
float4 _Color;

struct VertexIn
{
  float4 Pos : POSITION;
  float4 Norm : NORMAL0;
  float4 Color : COLOR0;
};

struct VertexOut
{
  float4 Pos : POSITION;
  float4 Color : COLOR0;
};

VertexOut VShader(VertexIn v)
{
  VertexOut Vert = (VertexOut)0;
  Vert.Pos = mul(v.Pos, _ObjectToWorld);
  Vert.Color = v.Color;

  return Vert;
}

float4 PShader(VertexOut v) : COLOR
{
  return _Color;
}

technique FirstTechnique
{
  pass FirstPass
  {
    CullMode = 1;
    AlphaBlendEnable = TRUE;
    BlendOp = 1;
    SrcBlend = 5;
    DestBlend = 6;
    VertexShader = compile vs_2_0 VShader();
    PixelShader = compile ps_2_0 PShader();
  }
}
