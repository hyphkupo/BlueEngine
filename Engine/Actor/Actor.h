#pragma once

#include <string>
#include <vector>	// ���ʹ� ������Ʈ �迭�� ������ �־�� �ϹǷ�
#include <memory>
#include "Math/Transform.h"	// ���� �� ����� ����.

namespace Blue
{
	// ���� �� ������� (DDD).
	// ���͸� �󸶳� �߰� �ɰ���, Draw�� ���⿡ ���� ����� ������
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

		// ������Ʈ �߰� �Լ�.
		void AddComponent(std::shared_ptr<class Component> newComponent);

		const bool IsActive() const;
		const bool HasInitialized() const;

	public:
		// ������ TRS ����(��ȯ ����)�� �����ϴ� Ʈ������.
		Transform transform;

	protected:
		// ������ �̸� (�Ϲ������δ� Hash�� ��ȯ�ؼ� ���).
		std::wstring name = TEXT("Actor");
		// wstring: �ȿ� �����Ͱ� ����
		
		// ������ �ʱ�ȭ ����.
		bool hasInitialized = false;
		
		// ������ Ȱ��ȭ ����.
		bool isActive = false;

		// ���� ����.
		bool hasDestroyed = false;

		// ������Ʈ �迭.
		std::vector<std::shared_ptr<class Component>> components;
	};
}