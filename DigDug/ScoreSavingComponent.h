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
	void SetScoreToSave(int score) { m_ScoreToSave = score; }

private:
	void CreateLetterDisplayer(Twengine::Font* font);
	void CreateScoreDisplayer(Twengine::Font* font);

	std::vector< std::pair<Twengine::TextComponent*, char>> m_Letters{};
	size_t m_CurrentIndex{0};
	const int m_MinmalCharIdx{ 65 };
	const int m_MaximalCharIdx{ 90 };

	const glm::vec2 m_BasePosition{155.f, 100.f};
	const float m_LetterOffset{50.f};
	const SDL_Color m_UnselectedColor{255, 255, 255, 255};
	const SDL_Color m_SelectedColor{0, 255, 0, 255};

	Twengine::TextComponent* m_ScoreDisplayer{};
	const glm::vec2 m_ScoreDisplayerPos{25.f, 25.f};
	int m_ScoreToSave{};
};

