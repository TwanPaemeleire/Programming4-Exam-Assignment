#include "HighScoresDisplayComponent.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "GameManager.h"
#include "ScoreFileComponent.h"
#include "SceneManager.h"

HighScoresDisplayComponent::HighScoresDisplayComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_Font = Twengine::ResourceManager::GetInstance().LoadFont("GameFont.otf", 12);
}

void HighScoresDisplayComponent::Start()
{
	m_Scores = &GameManager::GetInstance().GetScoreFileComponent()->GetHighScores();

	auto& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();

	for(size_t scoreIdx = 0; scoreIdx < (*m_Scores).size(); ++scoreIdx)
	{
		auto scoreDisplayer = std::make_unique<Twengine::GameObject>();
		auto* textComponent = scoreDisplayer->AddComponent<Twengine::TextComponent>();
		textComponent->SetFont(m_Font);
		const std::string name = (*m_Scores)[scoreIdx].name;
		textComponent->SetText(std::string(std::to_string(scoreIdx + 1) + ". " + name + "     " + std::to_string((*m_Scores)[scoreIdx].score)));
		glm::vec2 pos = m_BaseDrawPosition;
		pos.y += m_DrawOffset * scoreIdx;
		scoreDisplayer->GetTransform()->SetLocalPosition(pos);
		scene.Add(std::move(scoreDisplayer));
	}
}

void HighScoresDisplayComponent::ChangeFontSize(uint8_t newSize)
{
	m_Font = Twengine::ResourceManager::GetInstance().LoadFont("GameFont.otf", newSize);
}
