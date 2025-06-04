#pragma once
#include "Component.h"
#include "Observer.h"

class GridComponent;
namespace Twengine
{
	class Texture2D;
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
	virtual void Render() const override;
	virtual void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

private:
	Twengine::Texture2D* m_LifeTexture{};
	float m_LifeDrawOffset{32.f};
	int m_LivesLeft{3};
};

