#include "Window.h"

namespace Blue
{
	Window::Window(uint32 width, uint32 height, const std::wstring& title, HINSTANCE instance, WNDPROC messageProcedure)
        : width(width), height(height), title(title), instance(instance)
    {
        // â�� ����� Ŭ���� �̸�.
        //className = TEXT("Sample Window Class");     // TEXT: �����ڵ�� L�� �ٿ��ش� (�Ӽ�-���-���� ����-��Ƽ����Ʈ�� x)

        // â ������ �ʿ��� ���� ����ü.
        WNDCLASS wc = { };      // { }: �⺻������ 0 �־��ִ� ���� (cpp�� �ڵ����� �⺻���� 0...ó�� �־����� �ʴ´�)

        // �޽��� ó���� ���� �ݹ� ����.
        wc.lpfnWndProc = messageProcedure;        // �ý��ۿ� �ݹ� ����

        // ���α׷� �ּ� ����.
        wc.hInstance = instance;

        // â ������ ����� Ŭ���� �̸� ����.
        wc.lpszClassName = className.c_str();   // c_str(): const wchar_t*�� �ٲ��ش�

        // Ŭ���� ���.
        if (!RegisterClass(&wc))
        {
            // ���� �޽��� Ȯ��.
            //auto error = GetLastError();

            // �޽��� ���#1 - ���â(Output) �̿�.
            OutputDebugStringA("Failed to register a window class\n");    // OutputDebugStringW(L"â ��� �ߵ�\n");�� ����

            // �޽��� �ڽ� �̿�.
            MessageBoxA(nullptr, "ailed to register a window", "Error", MB_OK);

            // �ߴ��� ����.
            __debugbreak();
        }

        // â�� ���� ũ��.
        //unsigned int width = 1280;
        //unsigned int height = 800;
        // ȭ�� ��� ��ġ ����.
        unsigned int positionX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
        unsigned int positionY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

        // â ũ�� ����.
        RECT rect = { 0, 0, (long)width, (long)height };
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);    // FALSE: 0(int), TRUE: 1(0�� �ƴϸ� �� TRUE)
        //GetClientRect

        // â ũ�� �缳��.
        unsigned int windowWidth = rect.right - rect.left;
        unsigned int windowHeight = rect.bottom - rect.top;

        // ����.
        handle = CreateWindow(
            className.c_str(),          // Window class
            title.c_str(),              // Window text
            WS_OVERLAPPEDWINDOW,        // Window style
            // Size and position
            positionX, positionY,
            windowWidth, windowHeight,
            nullptr,       // Parent window    
            nullptr,       // Menu
            instance,  // Instance handle
            nullptr        // Additional application data
        );

        // ���� �� ���� �޽��� ���.
        if (handle == nullptr)
        {
            // �޽��� ���#1 - ���â(Output) �̿�.
            OutputDebugStringA("Failed to register a window class\n");    // OutputDebugStringW(L"â ��� �ߵ�\n");�� ����

            // �޽��� �ڽ� �̿�.
            MessageBoxA(nullptr, "ailed to register a window", "Error", MB_OK);

            // �ߴ��� ����
            __debugbreak();
        }

        // â ���̱�.
        ShowWindow(handle, SW_SHOW);

        // â �޽��� ������Ʈ.
        UpdateWindow(handle);
	}

	Window::~Window()
	{
        // Ŭ���� ��� ����.
        UnregisterClass(className.c_str(), instance);
	}
	
	void Window::SetWidthHeigth(uint32 width, uint32 height)
	{
		this->width = width;
		this->height = height;
	}
}