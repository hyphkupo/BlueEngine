struct VertexInput
{
    float3 position : POSITION;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
};

// ConstantBuffer.
cbuffer Transform : register(b0) // 버퍼는 b# 사용
{
    matrix worldMatrix;
};

struct VertexOutput     // 다음 단계 입력으로 쓰려면 출력으로 넘겨줘야 함
{
    float4 position : SV_Position;  // SystemValue
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
};

VertexOutput main(VertexInput input)    // 정점 셰이더 출력값은 픽셀 셰이더 입력값 (수정 가능)
{                                       // 픽셀 단위에서 위치는 잘 다루지 않음
    VertexOutput output;
    //output.position = float4(input.position, 1); // 점이므로 마지막을 1로 채움
    output.position = mul(float4(input.position, 1), worldMatrix);
    output.color = input.color;
    output.texCoord = input.texCoord;   // 입력 받은 걸 그대로 출력에 덮어 씌워줌
    
    return output;
}