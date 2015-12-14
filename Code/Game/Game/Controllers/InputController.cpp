#include "InputController.h"
#include "../../../Engine/UserInput/UserInput.h"
#include "../../../Engine/Core/TransformHelper.h"

eae6320::Game::InputController::InputController(const float i_displaceMent, const eae6320::Math::cVector i_boundaryLeftBottom, const eae6320::Math::cVector i_boundaryRightTop) : m_displacement(i_displaceMent), m_leftBottom(i_boundaryLeftBottom), m_rightTop(i_boundaryRightTop)
{
	for (size_t i = 0; i < NumBits; i++)
	{
		m_Bits[i] = 0;
	}
}

void eae6320::Game::InputController::UpdateEntity(eae6320::Core::Entity& i_entity, float dt)
{
	eae6320::Math::cVector deltaPosition(0.0f, 0.0f, 0.0f);
	{
		// Get the direction
		{
			if (eae6320::UserInput::IsKeyPressed(VK_LEFT) && !GetKeyPressed(VK_LEFT))
			{
				deltaPosition.x -= m_displacement;
				SetKeyPressed(VK_LEFT, true);
			}
			else
			{
				SetKeyPressed(VK_LEFT, false);
			}
			if (eae6320::UserInput::IsKeyPressed(VK_RIGHT) && !GetKeyPressed(VK_RIGHT))
			{
				deltaPosition.x += m_displacement;
				SetKeyPressed(VK_RIGHT, true);
			}
			else
			{
				SetKeyPressed(VK_RIGHT, false);
			}
			if (eae6320::UserInput::IsKeyPressed(VK_UP) && !GetKeyPressed(VK_UP))
			{
				deltaPosition.y += m_displacement;
				SetKeyPressed(VK_UP, true);
			}
			else
			{
				SetKeyPressed(VK_UP, false);
			}
			if (eae6320::UserInput::IsKeyPressed(VK_DOWN) && !GetKeyPressed(VK_DOWN))
			{
				deltaPosition.y -= m_displacement;
				SetKeyPressed(VK_DOWN, true);
			}
			else
			{
				SetKeyPressed(VK_DOWN, false);
			}
		}
	}
	eae6320::Math::cVector position = i_entity.m_transform->m_position + deltaPosition;
	if (position.x >= m_leftBottom.x && position.x <= m_rightTop.x && position.y >= m_leftBottom.y && position.y <= m_rightTop.y)
	{
		eae6320::Core::TransformHelper::SetPosition(*i_entity.m_transform, *i_entity.m_renderable->m_material->m_effect, position);
	}
}

bool eae6320::Game::InputController::GetKeyPressed(const unsigned int i_keyCode)
{
	unsigned int index = i_keyCode / 32;
	unsigned int shift = i_keyCode % 32;
	return (m_Bits[index] & (0x1 << shift)) != 0;
}

void eae6320::Game::InputController::SetKeyPressed(const unsigned int i_keyCode, bool i_pressed)
{
	unsigned int index = i_keyCode / 32;
	unsigned int shift = i_keyCode % 32;
	if (i_pressed)
	{
		m_Bits[index] = m_Bits[index] | (0x1 << shift);
	}
	else
	{
		m_Bits[index] = m_Bits[index] & (~(0x1 << shift));
	}
}