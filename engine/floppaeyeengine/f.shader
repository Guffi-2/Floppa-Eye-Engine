cbuffer ConstantBufferMatrixes : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);


struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 normal   : TEXCOORD1;
	float2 texcoord : TEXCOORD0;
};

PS_INPUT VShader(float4 position : POSITION, float3 normal : NORMAL, float2 texcoord : TEXCOORD0)
{
	PS_INPUT output = (PS_INPUT)0;

	output.position = mul(position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);
	output.normal = mul(normal, (float3x3)world);
	output.texcoord = texcoord;

	return output;
}

float4 PShader(PS_INPUT input) : SV_TARGET
{
	float4 color = shaderTexture.Sample(sampleType, input.texcoord);
	return color;
}