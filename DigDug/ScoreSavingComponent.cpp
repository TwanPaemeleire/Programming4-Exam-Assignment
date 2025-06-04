#include "ScoreSavingComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "ScoreComponent.h"
#include <string>

ScoreSavingComponent::ScoreSavingComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	auto* font = Twengine::ResourceManager::GetInstance().LoadFont("GameFont.otf", 36);
	CreateLetterDisplayer(font);
	CreateLetterDisplayer(font);
	CreateLetterDisplayer(font);
	m_Letters[m_CurrentIndex].first->SetColor(m_SelectedColor);
	m_ScoreToSave = GameManager::GetInstance().GetScoreComponent()->GetScore();
}

void ScoreSavingComponent::CycleCharacter(int dir)
{
	int newChar = static_cast<int>(m_Letters[m_CurrentIndex].second);
	newChar += dir;
	if (newChar < m_MinmalCharIdx) newChar = m_MaximalCharIdx;
	else if (newChar > m_MaximalCharIdx) newChar = m_MinmalCharIdx;
	m_Letters[m_CurrentIndex].second = static_cast<char>(newChar);
	m_Letters[m_CurrentIndex].first->SetText(std::to_string(static_cast<char>(m_Letters[m_CurrentIndex].second)));
}

void ScoreSavingComponent::CycleLetterIndex(int dir)
{
	if (m_CurrentIndex == 0 && dir < 0) m_CurrentIndex = m_Letters.size() - 1;
	else if (m_CurrentIndex == m_Letters.size() - 1 && dir > 0) m_CurrentIndex = 0;
	else m_CurrentIndex += dir;
}

void ScoreSavingComponent::CreateLetterDisplayer(Twengine::Font* font)
{
	auto& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();

	auto letterDisplayerObj = std::make_unique<Twengine::GameObject>();
	auto* textComp = letterDisplayerObj->AddComponent<Twengine::TextComponent>();
	textComp->SetFont(font);
	textComp->SetColor(m_UnselectedColor);
	textComp->SetText(static_cast<std::string>(std::to_string(static_cast<char>(m_MinmalCharIdx))));

	glm::vec2 pos = m_BasePosition;
	pos.y += m_LetterOffset * m_Letters.size();
	letterDisplayerObj->GetTransform()->SetLocalPosition(pos);

	m_Letters.emplace_back(textComp, static_cast<char>(m_MinmalCharIdx));
	scene.Add(std::move(letterDisplayerObj));
}