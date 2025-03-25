#pragma once
#include "Component.h"
#include "Observer.h"

namespace Twengine
{
	class TextComponent;
}

class DisplayLivesComponent final: public Twengine::Component, public Twengine::Observer
{
public:
	DisplayLivesComponent(Twengine::GameObject* owner);
	virtual ~DisplayLivesComponent() override = default;
	DisplayLivesComponent(const DisplayLivesComponent& other) = delete;
	DisplayLivesComponent(DisplayLivesComponent&& other) = delete;
	DisplayLivesComponent& operator=(const DisplayLivesComponent& other) = delete;
	DisplayLivesComponent& operator=(DisplayLivesComponent&& other) = delete;

	virtual void Start() override;
	virtual void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

private:
	Twengine::TextComponent* m_TextComponent{};
};

