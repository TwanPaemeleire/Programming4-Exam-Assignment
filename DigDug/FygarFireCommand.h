#pragma once
#include "GameObjectCommand.h"

class FygarFireCommand final : public Twengine::GameObjectCommand
{
public:
	FygarFireCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;
	Twengine::GameObject* GetFireObject() const { return m_FireGameObject; }

private:
	Twengine::GameObject* m_FireGameObject{};
};

