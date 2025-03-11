#pragma once
#include "Component.h"
#include "Observer.h"

class TextComponent;

class DisplayPointsComponent : public Component, public Observer
{
public:
	DisplayPointsComponent(GameObject* owner);
	virtual ~DisplayPointsComponent() override = default;
	DisplayPointsComponent(const DisplayPointsComponent& other) = delete;
	DisplayPointsComponent(DisplayPointsComponent&& other) = delete;
	DisplayPointsComponent& operator=(const DisplayPointsComponent& other) = delete;
	DisplayPointsComponent& operator=(DisplayPointsComponent&& other) = delete;

	virtual void Start() override;
	virtual void Notify(GameEvent event, GameObject* observedObject) override;

private:
	TextComponent* m_TextComponent{};
};

