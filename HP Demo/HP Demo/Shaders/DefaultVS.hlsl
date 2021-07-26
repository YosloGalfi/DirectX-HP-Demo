
cbuffer cbPerObject : register(b0)
{
	row_major matrix worldViewProjection;
	row_major matrix worldspace;
	row_major matrix InverseTransposeWorldMatrix;
};

cbuffer cBufferCamera : register(b1)
{
	float3 cameraPosition;
	float padding;
};

struct VertexInput
{
	float3 Position : POSITION;
	float2 TexCoord : TEXCOORD;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
};

struct VertexOutput
{
	float4 WVPPosition : SV_POSITION;
	float4 WPosition : WPOSITION;
	float2 WTexCoord : TEXCOORD;
	float3 WNormal : NORMAL;
	float3 WTangent : TANGENT;
	float3 ViewDir : TEXCOORD1;
};

VertexOutput VSMain(VertexInput input) {

	VertexOutput output = (VertexOutput)0;

	output.WVPPosition = mul(worldViewProjection, float4(input.Position, 1.0f));
	output.WPosition = mul(worldspace, float4(input.Position, 1.0f));
	output.WTexCoord = input.TexCoord;
	output.WNormal = mul((float3x3)InverseTransposeWorldMatrix, input.Normal);
	output.WTangent = mul((float3x3)InverseTransposeWorldMatrix, input.Tangent);

	// Determine view direction based onm cam position and position vertex
	output.ViewDir = cameraPosition.xyz - output.WPosition.xyz;
	output.ViewDir = normalize(output.ViewDir);

	return output;
}