#include "TransformHelper.h"
#include "../Graphics/EffectHelper.h"

void eae6320::Core::TransformHelper::UpdateTransform(Transform& i_transform, eae6320::Graphics::Effect& i_effect, const float i_elapsedTime)
{
	i_transform.m_position = i_transform.m_position + (i_transform.m_prev_velocity + i_transform.m_velocity) * i_elapsedTime * 0.5;
	eae6320::Graphics::EffectHelper::CreateLocalToWorldTransform(i_effect, i_transform.m_rotation, i_transform.m_position);
}

void eae6320::Core::TransformHelper::SetVelocity(Transform& i_transform, const eae6320::Math::cVector i_velocity)
{
	i_transform.m_prev_velocity = i_transform.m_velocity;
	i_transform.m_velocity = i_velocity;
}

void eae6320::Core::TransformHelper::SetPosition(Transform& i_transform, eae6320::Graphics::Effect& i_effect, const eae6320::Math::cVector i_position)
{
	i_transform.m_position = i_position;
	eae6320::Graphics::EffectHelper::CreateLocalToWorldTransform(i_effect, i_transform.m_rotation, i_transform.m_position);
}

void eae6320::Core::TransformHelper::SetRotation(Transform& i_transform, eae6320::Graphics::Effect& i_effect, const eae6320::Math::cQuaternion i_rotation)
{
	i_transform.m_rotation = i_rotation;
}