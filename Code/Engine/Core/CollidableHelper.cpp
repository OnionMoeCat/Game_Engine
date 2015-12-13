#include "CollidableHelper.h"

void eae6320::Core::CollidableHelper::SetMass(Collidable& i_collidable, const float i_mass)
{
	i_collidable.m_mass = i_mass;
}

void eae6320::Core::CollidableHelper::SetColliding(Collidable& i_collidable, const EntityHandle& i_colliding)
{
	i_collidable.m_colliding = i_colliding;
}