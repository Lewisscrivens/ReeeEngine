cbuffer CBuf
{
	matrix modelView;
	matrix modelViewProj;
}

struct VSOut
{
	float3 ViewPos : Position;
	float4 Pos : SV_Position;
};

VSOut main(float3 pos : Position)
{
	VSOut vsOut;
	vsOut.ViewPos = (float3) mul(float4(pos, 1.0f), modelView);
	vsOut.Pos = mul(float4(pos, 1.0f), modelViewProj);
	return vsOut;
}