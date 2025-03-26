#pragma once
#include "Component.h"

namespace Twengine
{
	class AnimatedTextureComponent;
};

class DigDugComponent : public Twengine::Component
{
public:
	DigDugComponent(Twengine::GameObject* owner);
	virtual ~DigDugComponent() override = default;
	DigDugComponent(const DigDugComponent& other) = delete;
	DigDugComponent(DigDugComponent&& other) = delete;
	DigDugComponent& operator=(const DigDugComponent& other) = delete;
	DigDugComponent& operator=(DigDugComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;

private:
	double m_Angle{};

	Twengine::AnimatedTextureComponent* m_AnimationComponent{};
};

