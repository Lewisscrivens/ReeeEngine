cbuffer LightCBuf
{
	float3 pos;
	float3 ambientColor;
	float3 diffuseColor;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;
};

cbuffer ObjectCBuf
{
	float specularIntensity;
	float specularPower;
	float padding[2];
};

Texture2D tex;
SamplerState splr;

float4 main(float3 worldPos : Position, float3 n : Normal, float2 tc : Texcoord) : SV_Target
{
	// Return lit texture.
	const float4 skyboxTex = 0.2f * tex.Sample(splr, tc);

	// Return final color of pixel.
	return skyboxTex;
}