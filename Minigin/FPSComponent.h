#pragma once
#include "Component.h"

class TextComponent;

class FPSComponent final : public Component
{
public:
	FPSComponent(GameObject* owner);
	virtual ~FPSComponent() override = default;
	FPSComponent(const FPSComponent& other) = delete;
	FPSComponent(FPSComponent&& other) = delete;
	FPSComponent& operator=(const FPSComponent& other) = delete;
	FPSComponent& operator=(FPSComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;

private:
	TextComponent* m_TextComponent{};
};

