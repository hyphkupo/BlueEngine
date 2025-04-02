#include "Level.h"
#include "Actor/Actor.h"
#include "Component/CameraComponent.h"

namespace Blue
{
	Level::Level()
	{
	}

	Level::~Level()
	{
	}

	void Level::BeginPlay()
	{
		for (const auto& actor : actors)
		{
			actor->BeginPlay();
		}
	}

	void Level::Tick(float deltaTime)
	{
		if (cameraActor)
		{
			cameraActor->Tick(deltaTime);
		}

		for (const auto& actor : actors)
		{
			actor->Tick(deltaTime);
		}
	}

	void Level::AddActor(std::shared_ptr<Actor> newActor)
	{
		// 새로 추가하는 액터가 카메라 컴포넌트를 가졌는지 확인.
		// 가졌다면, 메인 카메라로 설정.
		//bool hasCamera = false;
		for (auto component : newActor->components)
		{
			std::shared_ptr<CameraComponent> cameraComp
				= std::dynamic_pointer_cast<CameraComponent>(component);

			if (cameraComp)
			{
				cameraActor = newActor;
				return;		// 카메라는 액터에 넣어 주지 않는다 (따로 관리)
			}
		}

		actors.emplace_back(newActor);
	}

	std::shared_ptr<Actor> Level::GetActor(int index) const
	{
		// 예외 처리.
		if (index < 0 || index >= (int)actors.size())
		{
			return nullptr;	// shared는 실체가 null 반환 가능/weak는 실체가 없어서 null 반환 불가능
		}

		return actors[index];
	}

	const uint32 Level::ActorCount() const
	{
		return (uint32)actors.size();
	}

	std::shared_ptr<Actor> Level::GetCamera() const
	{
		return cameraActor;
	}
}