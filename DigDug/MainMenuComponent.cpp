#include "MainMenuComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include <SDL.h>

MainMenuComponent::MainMenuComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	auto* font = Twengine::ResourceManager::GetInstance().LoadFont("GameFont.otf", 12);
	AddButton("SinglePlayer", GameMode::SinglePlayer, font);
	AddButton("Co-Op", GameMode::Coop, font);
	AddButton("Versus", GameMode::Versus, font);

	m_Buttons[m_CurrentButtonIndex].first->SetColor(m_SelectedColor);
}

void MainMenuComponent::AddButton(const std::string& text, GameMode gameMode, Twengine::Font* font)
{
	auto& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();

	auto buttonObj = std::make_unique<Twengine::GameObject>();
	Twengine::TextComponent* textComp = buttonObj->AddComponent<Twengine::TextComponent>();
	textComp->SetFont(font);
	textComp->SetText(text);
	textComp->SetColor(m_UnselectedColor);
	glm::vec2 pos = m_ButtonBasePos;
	pos.y += m_ButtonSpacing * m_Buttons.size();
	buttonObj->GetTransform()->SetLocalPosition(pos);
	m_Buttons.emplace_back(std::pair{ textComp, gameMode });
	scene.Add(std::move(buttonObj));

}

void MainMenuComponent::ButtonPressed()
{
}

void MainMenuComponent::ButtonSwitched(int dir)
{
	m_Buttons[m_CurrentButtonIndex].first->SetColor(m_UnselectedColor);
	if (m_CurrentButtonIndex == 0 && dir < 0) m_CurrentButtonIndex = m_Buttons.size() - 1;
	else if (m_CurrentButtonIndex == m_Buttons.size() - 1 && dir > 0) m_CurrentButtonIndex = 0;
	else m_CurrentButtonIndex += dir;
	m_Buttons[m_CurrentButtonIndex].first->SetColor(m_SelectedColor);
}
