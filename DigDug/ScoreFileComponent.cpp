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
	auto it = std::find_if(m_HighScores.begin(), m_HighScores.end(), [&](const ScoreInfo& scoreInfo)
		{
			return (score.score >= scoreInfo.score);
		});

	if(it!= m_HighScores.end()) *it = score;
}

void ScoreFileComponent::ReadHighScores() const
{
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
	}
	input.close();
}

void ScoreFileComponent::WriteHighScores() const
{
	std::ofstream output("HighScores.bin", std::ios::binary);

	if (output.is_open())
	{
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
