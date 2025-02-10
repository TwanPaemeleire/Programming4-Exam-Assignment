#pragma once
#include "Component.h"

class TextComponent;

class FPSComponent : public Component
{
public:
	virtual void Start() override;
	virtual void Update() override;

	FPSComponent() = default;
	virtual ~FPSComponent() override = default;

	FPSComponent(const FPSComponent& other) = delete;
	FPSComponent(FPSComponent&& other) = delete;
	FPSComponent& operator=(const FPSComponent& other) = delete;
	FPSComponent& operator=(FPSComponent&& other) = delete;

private:
	TextComponent* m_TextComponent{};
};

