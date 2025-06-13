#pragma once
#include "Component.h"
#include <string>
#include <memory>
#include <unordered_map>
#include "SdbmHash.h"

using AnimationId = unsigned int;

namespace Twengine
{
	class Texture2D;

	struct AnimationData
	{
		Texture2D* spriteSheet{};
		int columns{};
		float frameWidth{};
		float frameHeight{};
		int repeatStartColumn{};
	};

	class AnimationComponent : public Component
	{
	public:
		AnimationComponent(GameObject* owner);
		virtual ~AnimationComponent() override = default;
		AnimationComponent(const AnimationComponent& other) = delete;
		AnimationComponent(AnimationComponent&& other) = delete;
		AnimationComponent& operator=(const AnimationComponent& other) = delete;
		AnimationComponent& operator=(AnimationComponent&& other) = delete;

		virtual void Update() override;
		virtual void Render() const override;

		void AddAnimation(const std::string& filePath, AnimationId id, int columns, int repeatStartColumn = 0);
		void PlayAnimation(AnimationId id, float frameDelay = 0.2f, bool playAutomatically = true);
		void GoToNextFrame();
		void GoToPreviousFrame();
		bool IsPlayingAnimation(AnimationId id) const;

		float GetAnimationFrameWidth() const { return m_FrameWidth; }
		float GetAnimationFrameHeight() const { return m_FrameHeight; }
		int GetCurrentFrameIndex() const { return m_CurrentColumn; }
		int GetAmountOfFrames()  const { return m_MaxColumnIndex; }
		bool HasFinishedPlayingOnce() const { return m_HasFinishedPlayingOnce; }

		void SetFlipHorizontal(bool flipped) { m_FlipHorizontal = flipped; }
		void SetFlipVertical(bool flipped) { m_FlipVertical = flipped; }
		void SetRotationAngle(double angle) { m_RotationAngle = angle; }

		const Texture2D* GetTexture2D() const { return m_CurrentAnimation->spriteSheet; }

	private:

		// General
		std::unordered_map<AnimationId, std::unique_ptr<AnimationData>> m_Animations;
		bool m_FlipHorizontal{ false };
		bool m_FlipVertical{ false };
		double m_RotationAngle{};

		// Animation specific
		AnimationData* m_CurrentAnimation{nullptr};
		float m_FrameWidth{};
		float m_FrameHeight{};
		int m_CurrentColumn{};
		int m_MaxColumnIndex{};
		int m_RepeatStartFrame{};
		bool m_HasFinishedPlayingOnce{ false };
		float m_FrameDelay{ 0.2f };
		bool m_PlaysAutomatically{false};

		float m_DelayCounter{};
	};
}