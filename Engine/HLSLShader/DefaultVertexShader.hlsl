struct VertexInput
{
    float3 position : POSITION;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
};

// ConstantBuffer.
cbuffer Transform : register(b0) // ���۴� b# ���
{
    matrix worldMatrix;
};

struct VertexOutput     // ���� �ܰ� �Է����� ������ ������� �Ѱ���� ��
{
    float4 position : SV_Position;  // SystemValue
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
};

VertexOutput main(VertexInput input)    // ���� ���̴� ��°��� �ȼ� ���̴� �Է°� (���� ����)
{                                       // �ȼ� �������� ��ġ�� �� �ٷ��� ����
    VertexOutput output;
    //output.position = float4(input.position, 1); // ���̹Ƿ� �������� 1�� ä��
    output.position = mul(float4(input.position, 1), worldMatrix);
    output.color = input.color;
    output.texCoord = input.texCoord;   // �Է� ���� �� �״�� ��¿� ���� ������
    
    return output;
}