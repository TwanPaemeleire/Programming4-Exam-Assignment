#pragma once
#include "Component.h"
#include <string>

namespace Twengine
{
	class Texture2D;

	class AnimatedTextureComponent : public Component
	{
	public:
		AnimatedTextureComponent(GameObject* owner);
		virtual ~AnimatedTextureComponent() override = default;
		AnimatedTextureComponent(const AnimatedTextureComponent& other) = delete;
		AnimatedTextureComponent(AnimatedTextureComponent&& other) = delete;
		AnimatedTextureComponent& operator=(const AnimatedTextureComponent& other) = delete;
		AnimatedTextureComponent& operator=(AnimatedTextureComponent&& other) = delete;

		virtual void Update() override;
		virtual void Render() const override;

		void InitializeSpriteSheet(const std::string& filePath, int rows, int columns);
		void PlayAnimation(int row, int amountOfFrames, float frameDelay = 0.2f, int repeatStartColumn = 0);

	private:
		Texture2D* m_SpriteSheet{};

		// Spritesheet General
		float m_FrameWidth{};
		float m_FrameHeight{};
		int m_Rows{};
		int m_Columns{};

		// Animation Specific
		int m_CurrentRow{};
		int m_CurrentColumn{};
		int m_AmountOfFrames{};
		int m_RepeatStartFrame{};
		bool m_HasFinishedPlayingOnce{ false };
		float m_FrameDelay{ 0.2f };

		float m_DelayCounter{};
	};
}