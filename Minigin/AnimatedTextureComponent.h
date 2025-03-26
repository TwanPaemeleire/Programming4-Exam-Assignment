#pragma once
#include "Component.h"
#include <string>
#include <memory>
#include <unordered_map>

namespace Twengine
{
	class Texture2D;

	struct Animation
	{
		Texture2D* spriteSheet{};
		int columns{};
		float frameWidth{};
		float frameHeight{};
		int repeatStartColumn{};
	};

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

		void AddAnimation(const std::string& filePath, int columns, int repeatStartColumn = 0);
		void PlayAnimation(const std::string& key, float frameDelay = 0.2f);

		void SetFlipHorizontal(bool flipped) { m_FlipHorizontal = flipped; }
		void SetFlipVertical(bool flipped) { m_FlipVertical = flipped; }
		void SetRotationAngle(double angle) { m_RotationAngle = angle; }

	private:

		// General
		std::unordered_map<std::string, std::unique_ptr<Animation>> m_Animations;
		bool m_FlipHorizontal{ false };
		bool m_FlipVertical{ false };
		double m_RotationAngle{};

		// Animation Specific
		Animation* m_CurrentAnimation{};
		float m_FrameWidth{};
		float m_FrameHeight{};
		int m_CurrentColumn{};
		int m_MaxColumnIndex{};
		int m_RepeatStartFrame{};
		bool m_HasFinishedPlayingOnce{ false };
		float m_FrameDelay{ 0.2f };

		float m_DelayCounter{};
	};
}