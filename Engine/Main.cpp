#include <Windows.h>

#include "Core/Window.h"
#include "Render/Renderer.h"
#include "Core/Engine.h"

// â�� ���õ� �޽����� ó���ϴ� �ݹ� �Լ�.
LRESULT CALLBACK WindowProc(
    HWND handle, 
    UINT message, 
    WPARAM wparam, 
    LPARAM lparam)
{
    // �޽��� ó��.
    switch (message)
    {
        // â�� �����Ǹ� �����.
    case WM_DESTROY:
        // �̶� ���α׷� ���� �޽����� ����.
        PostQuitMessage(0);
        return 0;
    }

    // �⺻ �޽��� ó��.
    return DefWindowProc(handle, message, wparam, lparam);
}

using namespace Blue;

// â ���� �� �� ���� �Լ�(Entry Point).
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,           // HINSTANCE: �ڵ� �� - ����ü ����(â�� ���� ������ ���� ����, â�� �����ϱ� ���� �޸𸮸� ����?)
    _In_opt_ HINSTANCE hPrevInstance,   // �� ��, ȣȯ�� ���������� ��
    _In_ LPSTR lpCmdLine,               // _In_ char* lpCmdLine�� ����
    _In_ int nShowCmd)                  // â ų�� ���� �ɼǰ�
{
    // ���� ���� �� ����.
    Engine engine(1280, 800, TEXT("Engine Demo"), hInstance);       // ����Ʈ �����͸� ���Ƿ� ���� ���ο����� ���� �Ҵ��� �� ��
    engine.Run();

    return 0;
}