#pragma once
#include "Component.h"
#include <vector>
#include <string>
#include <glm.hpp>
#include <SDL_image.h>

namespace Twengine
{
	class TextComponent;
	class Font;

}
enum class GameMode;

class MainMenuComponent final : public Twengine::Component
{
public:
	MainMenuComponent(Twengine::GameObject* owner);
	virtual ~MainMenuComponent() override = default;
	MainMenuComponent(const MainMenuComponent& other) = delete;
	MainMenuComponent(MainMenuComponent&& other) = delete;
	MainMenuComponent& operator=(const MainMenuComponent& other) = delete;
	MainMenuComponent& operator=(MainMenuComponent&& other) = delete;

	void ButtonPressed();
	void ButtonSwitched(int dir);

private:
	void AddButton(const std::string& text, GameMode gameMode, Twengine::Font* font);
	std::vector<std::pair<Twengine::TextComponent*, GameMode>> m_Buttons{};

	const glm::vec2 m_ButtonBasePos{150.f, 250.f};
	const float m_ButtonSpacing{40.f};

	const SDL_Color m_SelectedColor{0, 255, 0, 255};
	const SDL_Color m_UnselectedColor{255, 255, 255, 255};

	size_t m_CurrentButtonIndex{0};
};

