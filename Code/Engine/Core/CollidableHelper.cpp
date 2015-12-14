#include "CollidableHelper.h"

void eae6320::Core::CollidableHelper::SetMass(Collidable& i_collidable, const float i_mass)
{
	i_collidable.m_mass = i_mass;
}

void eae6320::Core::CollidableHelper::SetColliding(Collidable& i_collidable, const EntityHandle& i_colliding)
{
	if (i_collidable.m_collidings.size() == 0)
	{
		i_collidable.m_collidings.push_back(i_colliding);
		return;
	}
	std::vector<EntityHandle>::iterator colliding = std::find(i_collidable.m_collidings.begin(), i_collidable.m_collidings.end(), i_colliding);
	if (colliding == i_collidable.m_collidings.end())
	{
		i_collidable.m_collidings.push_back(i_colliding);
	}
}

bool eae6320::Core::CollidableHelper::IsColliding(Collidable& i_collidable, const EntityHandle& i_colliding)
{
	if (i_collidable.m_collidings.size() == 0)
	{
		return false;
	}
	std::vector<EntityHandle>::iterator colliding = std::find(i_collidable.m_collidings.begin(), i_collidable.m_collidings.end(), i_colliding);
	return (colliding != i_collidable.m_collidings.end());
}

void eae6320::Core::CollidableHelper::RemoveColliding(Collidable& i_collidable, const EntityHandle& i_colliding)
{
	if (i_collidable.m_collidings.size() == 0)
	{
		return;
	}
	std::vector<EntityHandle>::iterator colliding = std::find(i_collidable.m_collidings.begin(), i_collidable.m_collidings.end(), i_colliding);
	if (colliding == i_collidable.m_collidings.end())
	{
		i_collidable.m_collidings.erase(colliding);
	}
}