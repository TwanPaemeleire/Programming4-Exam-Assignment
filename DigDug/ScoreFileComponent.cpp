#include "ScoreFileComponent.h"
#include <fstream>
#include <algorithm>

ScoreFileComponent::ScoreFileComponent(Twengine::GameObject* owner)
	:Component(owner)
{
}

std::vector<ScoreInfo>& ScoreFileComponent::GetHighScores() const
{
	if (m_HighScores.size() == 0) ReadHighScores();
	return m_HighScores;
}

void ScoreFileComponent::AddHighScore(ScoreInfo score)
{
	m_HighScores.emplace_back(score);

	std::sort(m_HighScores.begin(), m_HighScores.end(), [](const ScoreInfo& first, const ScoreInfo& second)
		{
			return (first.score > second.score);
		});

	if (m_HighScores.size() > m_AmountOfScoresToSave)
	{
		m_HighScores.resize(m_AmountOfScoresToSave);
	}
}

void ScoreFileComponent::ReadHighScores() const
{
	m_HighScores.clear();
	std::ifstream input;
	input.open("HighScores.bin", std::ios::binary);
	if (input.is_open())
	{
		if (input.peek() != std::ifstream::traits_type::eof()) // Check to make sure file isn't fully empty
		{
			while (!input.eof())
			{
				ReadOneHighScore(&input);
			}
		}
		while (m_HighScores.size() < m_AmountOfScoresToSave)
		{
			m_HighScores.emplace_back(ScoreInfo({ 'X', 'X', 'X' }, 0));
		}
		input.clear();
		input.close();
	}
	if (m_HighScores.size() > m_AmountOfScoresToSave)
	{
		m_HighScores.resize(m_AmountOfScoresToSave);
	}
}

void ScoreFileComponent::WriteHighScores() const
{
	if (m_HighScores.size() > m_AmountOfScoresToSave)
	{
		m_HighScores.resize(m_AmountOfScoresToSave);
	}
	std::ofstream output("HighScores.bin", std::ios::binary);

	if (output.is_open())
	{
		output.clear();
		for (const ScoreInfo& score : m_HighScores)
		{
			output.write(reinterpret_cast<const char*>(&score.name[0]), sizeof(char));
			output.write(reinterpret_cast<const char*>(&score.name[1]), sizeof(char));
			output.write(reinterpret_cast<const char*>(&score.name[2]), sizeof(char));
			output.write(reinterpret_cast<const char*>(&score.score), sizeof(int));
		}
	}
	output.close();
}

void ScoreFileComponent::ReadOneHighScore(std::ifstream* input) const
{
	ScoreInfo scoreInfo{};
	input->read(reinterpret_cast<char*>(&scoreInfo.name[0]), sizeof(char));
	input->read(reinterpret_cast<char*>(&scoreInfo.name[1]), sizeof(char));
	input->read(reinterpret_cast<char*>(&scoreInfo.name[2]), sizeof(char));
	input->read(reinterpret_cast<char*>(&scoreInfo.score), sizeof(int));
	m_HighScores.emplace_back(scoreInfo);
}
