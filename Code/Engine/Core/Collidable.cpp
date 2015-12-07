#include "Collidable.h"

void eae6320::Core::Collidable::SetColliding(EntityHandle i_colliding)
{
	m_colliding = i_colliding;
}

eae6320::Core::EntityHandle eae6320::Core::Collidable::GetColliding()
{
	return m_colliding;
}
