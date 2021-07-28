// Nonnumeric values cannot be added to a cbuffer.
TextureCube cubeMap : register(t0);
SamplerState samplerType : register(s0);

cbuffer cBufferMaterial : register(b0)
{
	float4 diffuseColor;
	float4 ambientColor;
	float4 specularColor;

	bool hasTexture;
	bool isTerrain;
	bool hasNormMap;
	bool canMove;
};

struct PixelShaderInput
{
	float4 WVPPosition : SV_POSITION;
	float3 LPosition : POSITION;
};

float4 SkyPSMain(PixelShaderInput input) : SV_Target
{
	float4 difMaterial = diffuseColor;

	if (hasTexture == true) {
		difMaterial = cubeMap.Sample(samplerType, input.LPosition);
	}

	return difMaterial;
}