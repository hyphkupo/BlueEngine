#pragma once

#include <memory>
#include <string>
#include <Windows.h>
#include "Type.h"
#include <d3d11.h>

namespace Blue
{
	// 엔진 클래스.
	class Engine
	{
	public:
		Engine(uint32 width,
			uint32 height,
			const std::wstring& title,
			HINSTANCE hInstance);
		virtual ~Engine();

		// 엔진 실행 함수.
		void Run();

		// 메인 레벨 설정 함수.
		void SetLevel(std::shared_ptr<class Level> newLevel);

		// 윈도우 메시지 처리 루프.
		static LRESULT CALLBACK WindowProc(
			HWND handle,
			UINT message,
			WPARAM wparam,
			LPARAM lparam);

		// 싱글톤 접근 함수.
		static Engine& Get();

		// Getter.
		ID3D11Device& Device() const;	// ~const: 참조로 반환 (포인터이므로 참조 반환)
		ID3D11DeviceContext& Context() const;

	protected:
		// 창 객체.
		std::shared_ptr<class Window> window;

		// 렌더러 객체.
		std::shared_ptr<class Renderer> renderer;

		// 셰이더 로더 객체.
		std::unique_ptr<class ShaderLoader> shaderLoader;

		// 텍스처 로더 객체.
		std::unique_ptr<class TextureLoader> textureLoader;

		// 모델 로더 객체.
		std::unique_ptr<class ModelLoader> modelLoader;
		
		// 메인 레벨.
		std::shared_ptr<class Level> mainLevel;

		// 싱글톤 객체.
		static Engine* instance;
	};
}