struct PixelInput       // ���� ���̴��� ����� ����� �Ѿ� ��
{
    float4 position : SV_Position;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
};

// Texture.
Texture2D diffuseMap : register(t0);    // register ���� ���� �� �ϸ� ��ġ�� ������� 0, 1, 2, ... �� (�����ϸ� ���� ������ ��ȣ���)
SamplerState diffuseSampler : register(s0);

float4 main(PixelInput input) : SV_TARGET   // �ȼ� ���̴� ����� �ȼ��� ������ �� (���� �Ұ�)
{
    // Sampling.
    float4 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord);
    float4 inputColor = float4(input.color, 1);
    
    //float4 finalColor = texColor * inputColor;
    float4 finalColor = texColor;
    
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
    //return float4(input.color, 1.0f);     // float3 Ÿ���̹Ƿ� ���İ� x (���� 1.0f ���� �ǹ� ����)
    //return float4(input.texCoord, 0.0f, 1.0f);    // x, y, z, w �Ǵ� r(u�� �Ѿ), g(v�� �Ѿ), b, a
   
    return finalColor;
}