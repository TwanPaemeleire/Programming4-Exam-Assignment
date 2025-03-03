#pragma once
#include "Command.h"
class GameObject;

class GameObjectCommand : public Command
{
public:
	GameObjectCommand(GameObject* gameObject);
	virtual ~GameObjectCommand();
protected:
	GameObject* GetGameObject() const { return m_GameObject; }
private:
	GameObject* m_GameObject;

};

