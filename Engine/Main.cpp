#include "Core/Engine.h"
#include <iostream>

#include "Math/Vector2.h"

using namespace Blue;

// WinMain�� ������ â ��� (main ������ �ܼ� ���)

// â ���� �� �� ���� �Լ�(Entry Point).
//int WINAPI WinMain(
//    _In_ HINSTANCE hInstance,           // HINSTANCE: �ڵ� �� - ����ü ����(â�� ���� ������ ���� ����, â�� �����ϱ� ���� �޸𸮸� ����?)
//    _In_opt_ HINSTANCE hPrevInstance,   // �� ��, ȣȯ�� ���������� ��
//    _In_ LPSTR lpCmdLine,               // _In_ char* lpCmdLine�� ����
//    _In_ int nShowCmd)                  // â ų�� ���� �ɼǰ�
//{
//    // ���� ���� �� ����.
//    Engine engine(1280, 800, TEXT("Engine Demo"), hInstance);       // ����Ʈ �����͸� ���Ƿ� ���� ���ο����� ���� �Ҵ��� �� ��
//    engine.Run();
//
//    return 0;
//}

#include "Render/Texture.h"

int main()
{
	// @Test: �ؽ�ó ���� �ε� �׽�Ʈ.
	//Texture texture("T_coord.png");

	Engine engine(1280, 800, TEXT("Engine Demo"), GetModuleHandle(nullptr));
	engine.Run();

	// Vector2 �׽�Ʈ.
	//Vector2 v1(1.0f, 2.0f);
	//Vector2 v2(2.0f, 4.0f);

	//Vector2 v3 = v1 + v2;
	//Vector2 v4 = 2.0f * v1;
	//
	//std::wcout << v3.ToString() << " | " << (-v4).ToString() << "\n";
}