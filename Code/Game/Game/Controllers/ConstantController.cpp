#include "ConstantController.h"
#include "../../../Engine/Core/TransformHelper.h"

eae6320::Game::ConstantController::ConstantController(eae6320::Math::cVector i_velocity): m_velocity(i_velocity)
{
	
}

void eae6320::Game::ConstantController::UpdateEntity(eae6320::Core::Entity& i_entity, float dt)
{
	eae6320::Core::TransformHelper::SetVelocity(*i_entity.m_transform, m_velocity);
}