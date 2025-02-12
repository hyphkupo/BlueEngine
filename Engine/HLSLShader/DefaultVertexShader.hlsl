struct VertexInput
{
    float3 position : POSITION;
    float3 color : COLOR;
};

struct VertexOutput
{
    float4 position : SV_Position;  // SystemValue
    float3 color : COLOR;
};

VertexOutput main(VertexInput input)    // ���� ���̴� ��°��� �ȼ� ���̴� �Է°� (���� ����)
{                                       // �ȼ� �������� ��ġ�� �� �ٷ��� ����
    VertexOutput output;
    output.position = float4(input.position, 1);     // ���̹Ƿ� �������� 1�� ä��
    output.color = input.color;
    
    return output;
}