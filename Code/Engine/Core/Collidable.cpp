#include "Collidable.h"

void eae6320::Core::Collidable::SetColliding(const EntityHandle& i_colliding)
{
	m_colliding = i_colliding;
}

eae6320::Core::EntityHandle eae6320::Core::Collidable::GetColliding()
{
	return m_colliding;
}

void eae6320::Core::Collidable::SetMass(const float i_mass)
{
	m_mass = i_mass;
}

float eae6320::Core::Collidable::GetMass()
{
	return m_mass;
}