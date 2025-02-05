#include <Windows.h>

#include "Core/Window.h"
#include "Render/Renderer.h"
#include "Core/Engine.h"

// 창에 관련된 메시지를 처리하는 콜백 함수.
LRESULT CALLBACK WindowProc(
    HWND handle, 
    UINT message, 
    WPARAM wparam, 
    LPARAM lparam)
{
    // 메시지 처리.
    switch (message)
    {
        // 창이 삭제되면 실행됨.
    case WM_DESTROY:
        // 이때 프로그램 종료 메시지를 발행.
        PostQuitMessage(0);
        return 0;
    }

    // 기본 메시지 처리.
    return DefWindowProc(handle, message, wparam, lparam);
}

using namespace Blue;

// 창 모드로 할 때 메인 함수(Entry Point).
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,           // HINSTANCE: 핸들 값 - 구조체 선언(창에 대한 포인터 값을 관리, 창을 제어하기 위한 메모리를 관리?)
    _In_opt_ HINSTANCE hPrevInstance,   // 안 씀, 호환성 유지때문에 씀
    _In_ LPSTR lpCmdLine,               // _In_ char* lpCmdLine와 같음
    _In_ int nShowCmd)                  // 창 킬지 끌지 옵션값
{
    // 엔진 생성 및 실행.
    Engine engine(1280, 800, TEXT("Engine Demo"), hInstance);       // 스마트 포인터를 쓰므로 엔진 내부에서는 동적 할당을 할 것
    engine.Run();

    return 0;
}