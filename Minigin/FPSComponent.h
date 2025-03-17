#pragma once
#include "Component.h"


namespace Twengine
{
	class TextComponent;
	class FPSComponent final : public Twengine::Component
	{
	public:
		FPSComponent(Twengine::GameObject* owner);
		virtual ~FPSComponent() override = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

		virtual void Start() override;
		virtual void Update() override;

	private:
		Twengine::TextComponent* m_TextComponent{};
	};
}