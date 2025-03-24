#include "AnimatedTextureComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "MyTime.h"
#include "Texture2D.h"

Twengine::AnimatedTextureComponent::AnimatedTextureComponent(GameObject* owner)
	:Component(owner)
{
}

void Twengine::AnimatedTextureComponent::Update()
{
	m_DelayCounter += Time::GetInstance().deltaTime;
	if (m_DelayCounter >= m_FrameDelay) // New Frame Should Be Triggered
	{
		if (m_CurrentColumn >= m_AmountOfFrames - 1) // Animation Reached The End Frame
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

void Twengine::AnimatedTextureComponent::Render() const
{
	const auto& pos = m_Transform->GetWorldPosition();
	const float srcX = m_FrameWidth * m_CurrentColumn;
	const float srcY = m_FrameHeight * m_CurrentRow;
	Renderer::GetInstance().RenderTextureRect(*m_SpriteSheet, pos.x, pos.y, srcX, srcY, m_FrameWidth, m_FrameHeight);
}

void Twengine::AnimatedTextureComponent::InitializeSpriteSheet(const std::string& filePath, int rows, int columns)
{
	m_SpriteSheet = ResourceManager::GetInstance().LoadTexture(filePath);
	m_Rows = rows;
	m_Columns = columns;
	m_FrameWidth = m_SpriteSheet->GetSize().x / static_cast<float>(columns);
	m_FrameHeight = m_SpriteSheet->GetSize().y / static_cast<float>(m_Rows);
}

void Twengine::AnimatedTextureComponent::PlayAnimation(int row, int amountOfFrames, float frameDelay, int repeatStartColumn)
{
	m_CurrentRow = row;
	m_CurrentColumn = 0;
	m_AmountOfFrames = amountOfFrames;
	m_FrameDelay = frameDelay;
	m_RepeatStartFrame = repeatStartColumn;
	m_HasFinishedPlayingOnce = false;
	m_DelayCounter = 0.f;
}
