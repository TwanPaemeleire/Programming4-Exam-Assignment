#pragma once
#include "GameObjectCommand.h"

// UP
class MoveUpCommand : public GameObjectCommand
{
public:
	MoveUpCommand(GameObject* gameObject);
	virtual void Execute() override;

	void SetSpeed(float speed) { m_Speed = speed; }

private:
	float m_Speed{};
};

// DOWN
class MoveDownCommand : public GameObjectCommand
{
public:
	MoveDownCommand(GameObject* gameObject);
	virtual void Execute() override;

	void SetSpeed(float speed) { m_Speed = speed; }

private:
	float m_Speed{};
};

// LEFT
class MoveLeftCommand : public GameObjectCommand
{
public:
	MoveLeftCommand(GameObject* gameObject);
	virtual void Execute() override;

	void SetSpeed(float speed) { m_Speed = speed; }

private:
	float m_Speed{};
};

// RIGHT
class MoveRightCommand : public GameObjectCommand
{
public:
	MoveRightCommand(GameObject* gameObject);
	virtual void Execute() override;

	void SetSpeed(float speed) { m_Speed = speed; }

private:
	float m_Speed{};
};

