#pragma once
#include "Component.h"
#include "Observer.h"

class TextComponent;

class DisplayLivesComponent : public Component, public Observer
{
public:
	DisplayLivesComponent(GameObject* owner);
	virtual ~DisplayLivesComponent() override = default;
	DisplayLivesComponent(const DisplayLivesComponent& other) = delete;
	DisplayLivesComponent(DisplayLivesComponent&& other) = delete;
	DisplayLivesComponent& operator=(const DisplayLivesComponent& other) = delete;
	DisplayLivesComponent& operator=(DisplayLivesComponent&& other) = delete;

	virtual void Start() override;
	virtual void Notify(GameEvent event, GameObject* observedObject) override;

private:
	TextComponent* m_TextComponent{};
};

