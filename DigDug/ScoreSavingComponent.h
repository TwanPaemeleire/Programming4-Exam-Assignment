#pragma once
#include "Component.h"
#include <vector>
#include <SDL_image.h>
#include <glm.hpp>

namespace Twengine
{
	class TextComponent;
	class Font;
}

class ScoreSavingComponent final : public Twengine::Component
{
public:
	ScoreSavingComponent(Twengine::GameObject* owner);
	virtual ~ScoreSavingComponent() override = default;
	ScoreSavingComponent(const ScoreSavingComponent& other) = delete;
	ScoreSavingComponent(ScoreSavingComponent&& other) = delete;
	ScoreSavingComponent& operator=(const ScoreSavingComponent& other) = delete;
	ScoreSavingComponent& operator=(ScoreSavingComponent&& other) = delete;

	void CycleCharacter(int dir);
	void CycleLetterIndex(int dir);
private:
	void CreateLetterDisplayer(Twengine::Font* font);

	std::vector< std::pair<Twengine::TextComponent*, char>> m_Letters{};
	size_t m_CurrentIndex{0};
	int m_MinmalCharIdx{ 65 };
	int m_MaximalCharIdx{ 90 };

	glm::vec2 m_BasePosition{};
	const float m_LetterOffset{50};
	SDL_Color m_UnselectedColor{255, 255, 255, 255};
	SDL_Color m_SelectedColor{0, 255, 0, 255};

	int m_ScoreToSave{};
};

