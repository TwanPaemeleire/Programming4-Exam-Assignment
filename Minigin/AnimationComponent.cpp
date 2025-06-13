#include "AnimationComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "MyTime.h"
#include "Texture2D.h"
#include <filesystem>
#include <iostream>

Twengine::AnimationComponent::AnimationComponent(GameObject* owner)
	:Component(owner)
{
}

void Twengine::AnimationComponent::Update()
{
	if (m_PlaysAutomatically)
	{
		m_DelayCounter += Time::GetInstance().deltaTime;
		if (m_DelayCounter >= m_FrameDelay) // New frame should be triggered
		{
			if (m_CurrentColumn >= m_MaxColumnIndex) // Animation reached the end frame
			{
				m_CurrentColumn = m_RepeatStartFrame;
				m_HasFinishedPlayingOnce = true;
			}
			else // Go to next frame
			{
				++m_CurrentColumn;
			}

			m_DelayCounter -= m_FrameDelay;
		}
	}
}

void Twengine::AnimationComponent::Render() const
{
	if (m_CurrentAnimation == nullptr) return;
	const auto& pos = m_Transform->GetWorldPosition();
	const float srcX = m_FrameWidth * m_CurrentColumn;
	const float srcY = 0;
	Renderer::GetInstance().RenderTextureRect(*m_CurrentAnimation->spriteSheet,
		pos.x, pos.y, srcX, srcY,
		m_FrameWidth, m_FrameHeight,
		m_FlipHorizontal, m_FlipVertical, m_RotationAngle);
}

void Twengine::AnimationComponent::AddAnimation(const std::string& filePath, AnimationId id, int columns, int repeatStartColumn)
{
	std::unique_ptr<AnimationData> tempAnimation = std::make_unique<AnimationData>();
	tempAnimation->spriteSheet = ResourceManager::GetInstance().LoadTexture(filePath);
	tempAnimation->columns = columns;
	tempAnimation->repeatStartColumn = repeatStartColumn;
	tempAnimation->frameWidth = (tempAnimation->spriteSheet->GetSize().x / static_cast<float>(columns));
	tempAnimation->frameHeight = static_cast<float>(tempAnimation->spriteSheet->GetSize().y);

	m_Animations.emplace(id, std::move(tempAnimation));
}

void Twengine::AnimationComponent::PlayAnimation(AnimationId id, float frameDelay, bool playAutomatically)
{
	m_CurrentColumn = 0;
	m_DelayCounter = 0.f;
	m_FrameDelay = frameDelay;
	m_CurrentAnimation = m_Animations.find(id)->second.get();
	m_MaxColumnIndex = m_CurrentAnimation->columns - 1;
	m_FrameWidth = m_CurrentAnimation->frameWidth;
	m_FrameHeight = m_CurrentAnimation->frameHeight;
	m_HasFinishedPlayingOnce = false;
	m_PlaysAutomatically = playAutomatically;
	m_RepeatStartFrame = m_CurrentAnimation->repeatStartColumn;
}

void Twengine::AnimationComponent::GoToNextFrame()
{
	if (m_CurrentColumn >= m_MaxColumnIndex) // Animation reached the end frame
	{
		m_CurrentColumn = m_RepeatStartFrame;
		m_HasFinishedPlayingOnce = true;
	}
	else // Go to next frame
	{
		++m_CurrentColumn;
	}
}

void Twengine::AnimationComponent::GoToPreviousFrame()
{
	m_CurrentColumn = std::max(m_CurrentColumn - 1, 0);
}

bool Twengine::AnimationComponent::IsPlayingAnimation(AnimationId id) const
{
	return (m_Animations.find(id)->second.get() == m_CurrentAnimation);
}
