#include "Core/Engine.h"
#include <iostream>

#include "Math/Vector2.h"

using namespace Blue;

// WinMain을 쓰려면 창 모드 (main 쓰려면 콘솔 모드)

// 창 모드로 할 때 메인 함수(Entry Point).
//int WINAPI WinMain(
//    _In_ HINSTANCE hInstance,           // HINSTANCE: 핸들 값 - 구조체 선언(창에 대한 포인터 값을 관리, 창을 제어하기 위한 메모리를 관리?)
//    _In_opt_ HINSTANCE hPrevInstance,   // 안 씀, 호환성 유지때문에 씀
//    _In_ LPSTR lpCmdLine,               // _In_ char* lpCmdLine와 같음
//    _In_ int nShowCmd)                  // 창 킬지 끌지 옵션값
//{
//    // 엔진 생성 및 실행.
//    Engine engine(1280, 800, TEXT("Engine Demo"), hInstance);       // 스마트 포인터를 쓰므로 엔진 내부에서는 동적 할당을 할 것
//    engine.Run();
//
//    return 0;
//}

#include "Render/Texture.h"

int main()
{
	// @Test: 텍스처 파일 로드 테스트.
	//Texture texture("T_coord.png");

	Engine engine(1280, 800, TEXT("Engine Demo"), GetModuleHandle(nullptr));
	engine.Run();

	// Vector2 테스트.
	//Vector2 v1(1.0f, 2.0f);
	//Vector2 v2(2.0f, 4.0f);

	//Vector2 v3 = v1 + v2;
	//Vector2 v4 = 2.0f * v1;
	//
	//std::wcout << v3.ToString() << " | " << (-v4).ToString() << "\n";
}