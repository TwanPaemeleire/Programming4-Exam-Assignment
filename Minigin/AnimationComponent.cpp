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
	m_DelayCounter += Time::GetInstance().deltaTime;
	if (m_DelayCounter >= m_FrameDelay) // New Frame Should Be Triggered
	{
		if (m_CurrentColumn >= m_MaxColumnIndex) // Animation Reached The End Frame
		{
			m_CurrentColumn = m_RepeatStartFrame;
			m_HasFinishedPlayingOnce = true;
		}
		else // Go To Next Frame
		{
			++m_CurrentColumn;
		}

		m_DelayCounter -= m_FrameDelay;
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

void Twengine::AnimationComponent::AddAnimation(const std::string& filePath, int columns, int repeatStartColumn)
{
	std::unique_ptr<AnimationData> tempAnimation = std::make_unique<AnimationData>();
	tempAnimation->spriteSheet = ResourceManager::GetInstance().LoadTexture(filePath);
	tempAnimation->columns = columns;
	tempAnimation->repeatStartColumn = repeatStartColumn;
	tempAnimation->frameWidth = (tempAnimation->spriteSheet->GetSize().x / static_cast<float>(columns));
	tempAnimation->frameHeight = static_cast<float>(tempAnimation->spriteSheet->GetSize().y);

	// Extract Just The FileName Which Can Be Used As A Key		"Enemies/Pooka/PookaMove.png" => "PookaMove"
	std::string key = std::filesystem::path(filePath).stem().string();
	m_Animations.emplace(key, std::move(tempAnimation));
}

void Twengine::AnimationComponent::PlayAnimation(const std::string& key, float frameDelay)
{
	m_CurrentColumn = 0;
	m_DelayCounter = 0.f;
	m_FrameDelay = frameDelay;
	m_CurrentAnimation = m_Animations.find(key)->second.get();
	m_MaxColumnIndex = m_CurrentAnimation->columns - 1;
	m_FrameWidth = m_CurrentAnimation->frameWidth;
	m_FrameHeight = m_CurrentAnimation->frameHeight;
	m_HasFinishedPlayingOnce = false;
}
