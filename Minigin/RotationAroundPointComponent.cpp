#include "RotationAroundPointComponent.h"
#include "Time.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "GameObject.h"

RotationAroundPointComponent::RotationAroundPointComponent(GameObject* owner)
	:Component(owner)
{
}

void RotationAroundPointComponent::Update()
{
    m_Angle += m_Speed * Time::GetInstance().deltaTime;

    if (m_Angle > 2.f * static_cast<float>(M_PI))
    {
        m_Angle -= 2.f * static_cast<float>(M_PI);
    }

    glm::vec3 pos{ m_RotationPoint.x + m_Radius * cos(m_Angle), m_RotationPoint.y + m_Radius * sin(m_Angle), 0};
    m_Transform->SetLocalPosition(pos);
}
