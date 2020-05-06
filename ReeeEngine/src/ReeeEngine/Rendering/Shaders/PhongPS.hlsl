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
	// Fragment to light vector data.
	const float3 lightDir = pos - worldPos;
	const float distToL = length(lightDir);
	const float3 dirToL = lightDir / distToL;
	
	// Attenuation.
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	
	// Diffuse intensity.
	const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
	
	// Reflected light vector.
	const float3 w = n * dot(lightDir, n);
	const float3 r = w * 2.0f - lightDir;
	
	// Calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function.
	const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);

	// Return calculated point light view of pixel.
	float2 flippedTexCoord = float2(tc.x, tc.y * -1);// Quick fix....
	const float4 pointLightOut = float4(saturate(diffuse + ambientColor + specular), 1.0f) * tex.Sample(splr, flippedTexCoord);

	// Return final color of pixel.
	return pointLightOut;
}