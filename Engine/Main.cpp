#include "Core/Engine.h"
#include "Level/DemoLevel.h"
#include <iostream>

//#include "Math/Vector2.h"

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

//#include "Core/Common.h"
//#include <typeinfo>
//#include "Shader/Shader.h"
//#include "Shader/TextureMappingShader.h"
//
//template<typename T, typename std::enable_if<std::is_base_of<Shader, T>::value>::type* = nullptr>
//void TestClass()
//{
//	std::boolalpha(std::cout);
//	std::cout << typeid(T).name() << "\n";
//	std::cout << std::is_base_of<Shader, T>::value << "\n";
//}

/*
* Todo List.
* - Input Controller (Window Message Process).
* - Actor(Entity/GameObject) / Component / Scene(Level). => ���� ������ ������ ����
* - FBX Loader (Assimp/FBX SDK -> Submesh).
* - Camera ( View / Projection(Perspective/Othographic) ).
*	- Screen Coordinate System (NDC - Normalized Device Coordinates).
* - Shade (Shader) ( Light / Brightness / Darkness ).
*	- ���� ó���� �Ϲ������� 3�ܰ�� ������.
*	- Ambient(Static/GI, ���� ����, �⺻ ����) / Diffuse(Lambert .../Roughness) / Specular(Highlight, Phong/Blinn-Phong / Metalic).
*	- Diffuse, Specular�� �� �ϱ� ���� PBR ���
* - Normal Mapping.
* - Render Target (Draw to texture). => ������ ������ ���� �ʿ�
* - Shadow Mapping (Shadow Map).
* - Environment Mapping (Sky Sphere(Box)).
* - Study (Tone Mapping).
* 
* - Skeletal Mesh (Skinned Mesh) - Skeleton.
*	- Skeletal Mesh Animation.
* - Toy Project...
* 
* - Study (Physics �浹ó��).
*/

/*
* IA(�Է� ����) -> VS ...(Option) -> RS/������ -> PS -> OM(��� ����).
* - VS (���� ��ȯ Object->World->View->Projection).
*	- Skeletal Mesh�� VS���� ó��. ������ ���븦 ���� ����� ���� ����
* - PS (���� ó��) - PBR / ��ǻ�� / ����ŧ��.
* - ���� ������ (������(Renderer) - ������).
*/

int main()
{
	// @Test: �ؽ�ó ���� �ε� �׽�Ʈ.
	//Texture texture("T_coord.png");
	
	//TestClass<Engine>();		=> Ÿ���� �߸� �����ϸ� ���ٰ� ���´�
	//ThrowIfFailed(E_FAIL, TEXT("Test Error"));

	Engine engine(1280, 800, TEXT("Engine Demo"), GetModuleHandle(nullptr));
	engine.SetLevel(std::make_shared<DemoLevel>());
	engine.Run();

	/*
	// Vector2 �׽�Ʈ.
	//Vector2 v1(1.0f, 2.0f);
	//Vector2 v2(2.0f, 4.0f);

	//Vector2 v3 = v1 + v2;
	//Vector2 v4 = 2.0f * v1;
	//
	//std::wcout << v3.ToString() << " | " << (-v4).ToString() << "\n";
	*/
}