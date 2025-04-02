#include "Core/Engine.h"
#include "Level/DemoLevel.h"
#include <iostream>

//#include "Math/Vector2.h"

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
* - Actor(Entity/GameObject) / Component / Scene(Level). => 레벨 단위로 렌더링 가능
* - FBX Loader (Assimp/FBX SDK -> Submesh).
* - Camera ( View / Projection(Perspective/Othographic) ).
*	- Screen Coordinate System (NDC - Normalized Device Coordinates).
* - Shade (Shader) ( Light / Brightness / Darkness ).
*	- 조명 처리는 일반적으로 3단계로 나눈다.
*	- Ambient(Static/GI, 간접 조명, 기본 조명) / Diffuse(Lambert .../Roughness) / Specular(Highlight, Phong/Blinn-Phong / Metalic).
*	- Diffuse, Specular를 잘 하기 위해 PBR 사용
* - Normal Mapping.
* - Render Target (Draw to texture). => 섀도우 매핑을 위해 필요
* - Shadow Mapping (Shadow Map).
* - Environment Mapping (Sky Sphere(Box)).
* - Study (Tone Mapping).
* 
* - Skeletal Mesh (Skinned Mesh) - Skeleton.
*	- Skeletal Mesh Animation.
* - Toy Project...
* 
* - Study (Physics 충돌처리).
*/

/*
* IA(입력 정리) -> VS ...(Option) -> RS/보간기 -> PS -> OM(출력 조립).
* - VS (정점 변환 Object->World->View->Projection).
*	- Skeletal Mesh는 VS에서 처리. 가상의 뼈대를 만들어서 사용할 정점 관리
* - PS (조명 처리) - PBR / 디퓨즈 / 스페큘러.
* - 엔진 개발자 (렌더러(Renderer) - 개발자).
*/

int main()
{
	// @Test: 텍스처 파일 로드 테스트.
	//Texture texture("T_coord.png");
	
	//TestClass<Engine>();		=> 타입을 잘못 지정하면 없다고 나온다
	//ThrowIfFailed(E_FAIL, TEXT("Test Error"));

	Engine engine(1280, 800, TEXT("Engine Demo"), GetModuleHandle(nullptr));
	engine.SetLevel(std::make_shared<DemoLevel>());
	engine.Run();

	/*
	// Vector2 테스트.
	//Vector2 v1(1.0f, 2.0f);
	//Vector2 v2(2.0f, 4.0f);

	//Vector2 v3 = v1 + v2;
	//Vector2 v4 = 2.0f * v1;
	//
	//std::wcout << v3.ToString() << " | " << (-v4).ToString() << "\n";
	*/
}