#include "RotationAroundPointComponent.h"
#include "Time.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "GameObject.h"

RotationAroundPointComponent::RotationAroundPointComponent(Twengine::GameObject* owner)
	:Component(owner)
{
}

void RotationAroundPointComponent::Update()
{
    static constexpr float twoPi = 2.f * static_cast<float>(M_PI);
    m_Angle += m_Speed * Twengine::Time::GetInstance().deltaTime;

    if (m_Angle > twoPi)
    {
        m_Angle -= twoPi;
    }

    m_Transform->SetLocalPosition(m_Radius * cos(m_Angle), m_Radius * sin(m_Angle));
}
