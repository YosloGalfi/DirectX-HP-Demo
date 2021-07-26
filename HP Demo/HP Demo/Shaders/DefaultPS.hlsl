Texture2D diffuseMap : register(t0);
TextureCube cubeMap : register(t1);
Texture2D normalMap : register(t2);

SamplerState defaultSampleType : register(s0);

cbuffer cBufferLight : register(b0)
{
	float4 ambientLightColor;
	float4 diffuseLightColor;
	float4 specularLightColor;
	float3 lightPosition;
	float lightRange;

	float3 lightAttenuation;
	float padding;
};

cbuffer cBufferMaterial : register(b1)
{
	float4 diffuseColor;
	float4 ambientColor;
	float4 specularColor;
	float4 reflectionColor;

	bool hasTexture;
	//bool hasReflection;
	bool isTerrain;

	float3 padding2;

	float4 translation;

	bool hasNormMap;
	float3 padding3;

	float3 padding4;
};


struct PixelInput
{
	float4 WVPPosition : SV_POSITION;
	float4 WPosition : WPOSITION;
	float2 WTexCoord : TEXCOORD;
	float3 WNormal : NORMAL;
	float3 WTangent : TANGENT;
	float3 ViewDir : TEXCOORD1;
};

float4 PSMain(PixelInput input) : SV_TARGET
{
	float4 textureColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.	
	if (hasTexture)
		textureColor = diffuseMap.Sample(defaultSampleType, input.WTexCoord);

	//FOR NORMAL MAP
	if (hasNormMap == true)
	{
		float3 tempTangent;

		//load normal from normal map
		float4 normalMapTemp = normalMap.Sample(defaultSampleType, input.WTexCoord);

		//Change normal map range from [0,1] to [-1,1]
		normalMapTemp = (2.0f * normalMapTemp) - 1.0f;

		//Create the biTangent
		float3 biTangent = cross(input.WTangent, input.WNormal);

		//Create the "texture space" matrix (TBN)
		float3x3 texSpace = float3x3(input.WTangent, biTangent, input.WNormal);

		//Convert normal from normal map to texture space and store it in input.inNormal
		input.WNormal = mul(normalMapTemp, texSpace);
	}

	// Normalize normals
	float3 normalizedNormal = normalize(input.WNormal);

	// Colors that gonne be combined, light + material
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Vector from lightsource to point thats gonna be shaded, take distance from that vector
	float3 lightVector = lightPosition.xyz - input.WPosition.xyz;
	float distance = length(lightVector);

	// Check range
	if (distance > lightRange) {
		return 0;
	}

	// Normalize lightvector
	lightVector /= distance;

	// Calc ambient
	ambient = ambientColor * ambientLightColor;

	// Diffuse factor
	float diffuseFactor = dot(lightVector, normalizedNormal);

	if (diffuseFactor > 0.0f)
	{
		// Specular shade with reflection
		float3 lightReflection = reflect(-lightVector, normalizedNormal);

		float specularShade = max(dot(lightReflection, input.ViewDir), 0.0f);
		float specularFactor = pow(specularShade, specularColor.w);		//specularColor.w  =  SpecularPower

		// Set diffuse and specular
		diffuse = diffuseFactor * diffuseColor * diffuseLightColor;
		specular = specularFactor * specularColor * specularLightColor;
	}

	// Calc attenuation factor
	float attenuationFactor = 1.0f / lightAttenuation[0] + lightAttenuation[1] * distance + lightAttenuation[2] * distance * distance;

	// Set attenuation to diffuse and specular
	diffuse *= attenuationFactor;
	specular *= attenuationFactor;

	diffuse = saturate(diffuse);
	specular = saturate(specular);
	ambient = saturate(ambient);

	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	/*if (hasReflection) {
		float3 invView = -input.ViewDir;
		float3 reflectionVector = reflect(invView, input.WNormal);
		float4 cubemapColor = cubeMap.Sample(defaultSampleType, reflectionVector);

		return reflectionColor * cubemapColor;
	}*/

	// Removes the specularity from the terrain
	if (isTerrain)
		finalColor = textureColor * (ambient + diffuse);
	// Complete phong shading
	else
		finalColor = textureColor * (ambient + diffuse) + specular;

	return finalColor;
}