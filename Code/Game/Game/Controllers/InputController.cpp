#include "InputController.h"
#include "../../../Engine/UserInput/UserInput.h"
#include "../../../Engine/Core/TransformHelper.h"

eae6320::Game::InputController::InputController(const float i_velocityMag) : m_velocityMag(i_velocityMag)
{
}

void eae6320::Game::InputController::UpdateEntity(eae6320::Core::Entity& i_entity, float dt)
{
	eae6320::Math::cVector velocity(0.0f, 0.0f, 0.0f);
	{
		// Get the direction
		{
			if (eae6320::UserInput::IsKeyPressed(VK_LEFT))
			{
				velocity.x -= 1.0f;
			}
			if (eae6320::UserInput::IsKeyPressed(VK_RIGHT))
			{
				velocity.x += 1.0f;
			}
			if (eae6320::UserInput::IsKeyPressed(VK_UP))
			{
				velocity.y += 1.0f;
			}
			if (eae6320::UserInput::IsKeyPressed(VK_DOWN))
			{
				velocity.y -= 1.0f;
			}
			if (eae6320::UserInput::IsKeyPressed(VK_HOME))
			{
				velocity.z += 1.0f;
			}
			if (eae6320::UserInput::IsKeyPressed(VK_END))
			{
				velocity.z -= 1.0f;
			}
		}	
		velocity *= m_velocityMag;
	}
	eae6320::Core::TransformHelper::SetVelocity(*i_entity.m_transform, velocity);
}