#pragma once
#include "Command.h"

namespace Twengine
{
	class GameObject;

	class GameObjectCommand : public Command
	{
	public:
		GameObjectCommand(GameObject* gameObject);
		virtual ~GameObjectCommand() = default;
	protected:
		GameObject* GetGameObject() const { return m_GameObject; }
	private:
		GameObject* m_GameObject;
	};
}