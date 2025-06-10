#pragma once
#include "Component.h"
#include <vector>
#include <functional>
#include <string>

struct ScoreInfo
{
	char name[3]{};
	int score{};
};

class ScoreFileComponent final : public Twengine::Component
{
public:
	ScoreFileComponent(Twengine::GameObject* owner);
	virtual ~ScoreFileComponent() override = default;
	ScoreFileComponent(const ScoreFileComponent& other) = delete;
	ScoreFileComponent(ScoreFileComponent&& other) = delete;
	ScoreFileComponent& operator=(const ScoreFileComponent& other) = delete;
	ScoreFileComponent& operator=(ScoreFileComponent&& other) = delete;

	std::vector<ScoreInfo>& GetHighScores() const;
	void AddHighScore(ScoreInfo score);
	void ReadHighScores() const;
	void WriteHighScores() const;

private:
	void ReadOneHighScore(std::ifstream* input)  const;
	const size_t m_AmountOfScoresToSave{5};
	mutable std::vector<ScoreInfo> m_HighScores{};
};

