#pragma once
#include "Component.h"
#include <vector>
#include <glm.hpp>

namespace Twengine
{
	class Font;
};

struct ScoreInfo;

class HighScoresDisplayComponent final : public Twengine::Component
{
public:
	HighScoresDisplayComponent(Twengine::GameObject* owner);
	virtual ~HighScoresDisplayComponent() override = default;
	HighScoresDisplayComponent(const HighScoresDisplayComponent& other) = delete;
	HighScoresDisplayComponent(HighScoresDisplayComponent&& other) = delete;
	HighScoresDisplayComponent& operator=(const HighScoresDisplayComponent& other) = delete;
	HighScoresDisplayComponent& operator=(HighScoresDisplayComponent&& other) = delete;

	virtual void Start();

	void ChangeFontSize(uint8_t newSize);
	void ChangeBaseDrawPosition(const glm::vec2& pos) { m_BaseDrawPosition = pos; }

private:
	Twengine::Font* m_Font{};
	std::vector<ScoreInfo>* m_Scores{};

	glm::vec2 m_BaseDrawPosition{20.f, 20.f};
	const float m_DrawOffset{25.f};
};

