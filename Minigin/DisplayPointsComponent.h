#pragma once
#include "Component.h"
#include "Observer.h"

namespace Twengine
{
	class TextComponent;
}

class DisplayPointsComponent final: public Twengine::Component, public Twengine::Observer
{
public:
	DisplayPointsComponent(Twengine::GameObject* owner);
	virtual ~DisplayPointsComponent() override = default;
	DisplayPointsComponent(const DisplayPointsComponent& other) = delete;
	DisplayPointsComponent(DisplayPointsComponent&& other) = delete;
	DisplayPointsComponent& operator=(const DisplayPointsComponent& other) = delete;
	DisplayPointsComponent& operator=(DisplayPointsComponent&& other) = delete;

	virtual void Start() override;
	virtual void Notify(GameEvent event, Twengine::GameObject* observedObject) override;

private:
	Twengine::TextComponent* m_TextComponent{};
};

