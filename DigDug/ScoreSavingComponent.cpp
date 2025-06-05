#include "ScoreSavingComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "ScoreComponent.h"
#include <string>
#include "ScoreFileComponent.h"
#include "Event.h"

ScoreSavingComponent::ScoreSavingComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_OnScoreSavedEvent = std::make_unique<Twengine::Event>();
	m_OnScoreSavedEvent->AddObserver(&GameManager::GetInstance());

	auto* font = Twengine::ResourceManager::GetInstance().LoadFont("GameFont.otf", 36);
	CreateLetterDisplayer(font);
	CreateLetterDisplayer(font);
	CreateLetterDisplayer(font);
	m_Letters[m_CurrentIndex].first->SetColor(m_SelectedColor);
	m_ScoreToSave = GameManager::GetInstance().GetScoreComponent()->GetScore();
	CreateScoreDisplayer(font);
}

void ScoreSavingComponent::CycleCharacter(int dir)
{
	int newChar = static_cast<int>(m_Letters[m_CurrentIndex].second);
	newChar += dir;
	if (newChar < m_MinmalCharIdx) newChar = m_MaximalCharIdx;
	else if (newChar > m_MaximalCharIdx) newChar = m_MinmalCharIdx;
	m_Letters[m_CurrentIndex].second = static_cast<char>(newChar);
	m_Letters[m_CurrentIndex].first->SetText(std::string(1,m_Letters[m_CurrentIndex].second));
}

void ScoreSavingComponent::CycleLetterIndex(int dir)
{
	m_Letters[m_CurrentIndex].first->SetColor(m_UnselectedColor);
	if (m_CurrentIndex == 0 && dir < 0) m_CurrentIndex = m_Letters.size() - 1;
	else if (m_CurrentIndex == m_Letters.size() - 1 && dir > 0) m_CurrentIndex = 0;
	else m_CurrentIndex += dir;
	m_Letters[m_CurrentIndex].first->SetColor(m_SelectedColor);
}

void ScoreSavingComponent::ConfirmSave()
{
	ScoreInfo scoreInfo{};
	scoreInfo.name[0] = m_Letters[0].second;
	scoreInfo.name[1] = m_Letters[1].second;
	scoreInfo.name[2] = m_Letters[2].second;
	scoreInfo.score = m_ScoreToSave;
	GameManager::GetInstance().GetScoreFileComponent()->AddHighScore(scoreInfo);
	m_OnScoreSavedEvent->NotifyObservers(GameEvent(make_sdbm_hash("GameFinished")), GetOwner());
}

void ScoreSavingComponent::CreateLetterDisplayer(Twengine::Font* font)
{
	auto& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();

	auto letterDisplayerObj = std::make_unique<Twengine::GameObject>();
	auto* textComp = letterDisplayerObj->AddComponent<Twengine::TextComponent>();
	textComp->SetFont(font);
	textComp->SetColor(m_UnselectedColor);
	textComp->SetText(static_cast<std::string>(std::string(1, static_cast<char>(m_MinmalCharIdx))));

	glm::vec2 pos = m_BasePosition;
	pos.x += m_LetterOffset * m_Letters.size();
	letterDisplayerObj->GetTransform()->SetLocalPosition(pos);

	m_Letters.emplace_back(textComp, static_cast<char>(m_MinmalCharIdx));
	scene.Add(std::move(letterDisplayerObj));
}

void ScoreSavingComponent::CreateScoreDisplayer(Twengine::Font* font)
{
	auto& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();
	auto scoreDisplayObj = std::make_unique<Twengine::GameObject>();
	auto* textComp = scoreDisplayObj->AddComponent<Twengine::TextComponent>();
	textComp->SetFont(font);
	textComp->SetColor(m_UnselectedColor);
	textComp->SetText("Score " + std::to_string(m_ScoreToSave));
	scoreDisplayObj->GetTransform()->SetLocalPosition(m_ScoreDisplayerPos);
	scene.Add(std::move(scoreDisplayObj));
}
