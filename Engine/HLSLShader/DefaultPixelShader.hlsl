struct PixelInput
{
    float4 position : SV_Position;
    float3 color : COLOR;
};

float4 main(PixelInput input) : SV_TARGET   // 픽셀 셰이더 출력은 픽셀이 나가야 함 (수정 불가)
{
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
    return float4(input.color, 1.0f);   // float3 타입이므로 알파값 x (뒤의 1.0f 값은 의미 없음)
}