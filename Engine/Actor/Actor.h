#pragma once

#include <string>
#include <vector>	// 액터는 컴포넌트 배열을 가지고 있어야 하므로
#include <memory>
#include "Math/Transform.h"	// 설계 시 고려할 사항.

namespace Blue
{
	// 설계 시 고려사항 (DDD).
	// 액터를 얼마나 잘게 쪼갤지, Draw를 여기에 둘지 고민해 볼만함
	class Actor
	{
		friend class Renderer;

	public:
		Actor();
		virtual ~Actor();

		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		void Destroy();

		// 컴포넌트 추가 함수.
		void AddComponent(std::shared_ptr<class Component> newComponent);

		const bool IsActive() const;
		const bool HasInitialized() const;

	public:
		// 액터의 TRS 정보(변환 정보)를 관리하는 트랜스폼.
		Transform transform;

	protected:
		// 액터의 이름 (일반적으로는 Hash로 변환해서 사용).
		std::wstring name = TEXT("Actor");
		// wstring: 안에 포인터가 있음
		
		// 액터의 초기화 여부.
		bool hasInitialized = false;
		
		// 액터의 활성화 여부.
		bool isActive = false;

		// 삭제 여부.
		bool hasDestroyed = false;

		// 컴포넌트 배열.
		std::vector<std::shared_ptr<class Component>> components;
	};
}