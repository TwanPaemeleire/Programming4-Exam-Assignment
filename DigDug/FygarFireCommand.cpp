#include "FygarFireCommand.h"
#include "GameManager.h"
#include "GameObject.h"
#include <memory>
#include "FygarFireComponent.h"
#include "GridComponent.h"
#include "SceneManager.h"

FygarFireCommand::FygarFireCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
}

void FygarFireCommand::Execute()
{
	std::unique_ptr<Twengine::GameObject> fire = std::make_unique<Twengine::GameObject>();
	FygarFireComponent* fireComponent = fire->AddComponent<FygarFireComponent>();
	m_FireGameObject = fire.get();
	fire->SetParent(GetGameObject(), false);

	const glm::vec2 fygarPos = GetGameObject()->GetTransform()->GetWorldPosition();
	const glm::vec2 playerPos = GameManager::GetInstance().GetClosestPlayerTransform(fygarPos)->GetWorldPosition();
	const int playerColumn = GameManager::GetInstance().GetGrid()->GetIndexFromPosition(playerPos).second;
	const int fygarColumn = GameManager::GetInstance().GetGrid()->GetIndexFromPosition(fygarPos).second;

	if (playerColumn >= fygarColumn) // Should shoot the fire to the right
	{
		fireComponent->ShootToRight();
	}

	Twengine::SceneManager::GetInstance().GetCurrentScene().Add(std::move(fire));
}
