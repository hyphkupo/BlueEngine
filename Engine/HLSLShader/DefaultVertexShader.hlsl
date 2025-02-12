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

VertexOutput main(VertexInput input)    // 정점 셰이더 출력값은 픽셀 셰이더 입력값 (수정 가능)
{                                       // 픽셀 단위에서 위치는 잘 다루지 않음
    VertexOutput output;
    output.position = float4(input.position, 1);     // 점이므로 마지막을 1로 채움
    output.color = input.color;
    
    return output;
}