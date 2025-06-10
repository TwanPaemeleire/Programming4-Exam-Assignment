#pragma once
#include "Component.h"
#include <memory>

namespace Twengine 
{ 
	class Event; 
}

class LivesComponent final: public Twengine::Component
{
public:
	LivesComponent(Twengine::GameObject* owner);
	virtual ~LivesComponent() = default;
	LivesComponent(const LivesComponent& other) = delete;
	LivesComponent(LivesComponent&& other) = delete;
	LivesComponent& operator=(const LivesComponent& other) = delete;
	LivesComponent& operator=(LivesComponent&& other) = delete;

	int GetLives() const { return m_Lives; }
	void Kill();

	Twengine::Event* GetObjectDiedEvent() const { return m_ObjectDiedEvent.get(); }

private:
	const int m_MaxLives{4};
	int m_Lives{4};

	std::unique_ptr<Twengine::Event> m_ObjectDiedEvent;
};

