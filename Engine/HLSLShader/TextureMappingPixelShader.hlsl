struct PixelInput       // 정점 셰이더의 출력이 여기로 넘어 옴
{
    float4 position : SV_Position;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
};

// Texture.
Texture2D diffuseMap : register(t0);    // register 순번 지정 안 하면 배치한 순서대로 0, 1, 2, ... 번 (지정하면 내가 지정한 번호대로)
SamplerState diffuseSampler : register(s0);

float4 main(PixelInput input) : SV_TARGET   // 픽셀 셰이더 출력은 픽셀이 나가야 함 (수정 불가)
{
    // Sampling.
    float4 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord);
    float4 inputColor = float4(input.color, 1);
    
    //float4 finalColor = texColor * inputColor;
    float4 finalColor = texColor;
    
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
    //return float4(input.color, 1.0f);     // float3 타입이므로 알파값 x (뒤의 1.0f 값은 의미 없음)
    //return float4(input.texCoord, 0.0f, 1.0f);    // x, y, z, w 또는 r(u가 넘어감), g(v가 넘어감), b, a
   
    return finalColor;
}