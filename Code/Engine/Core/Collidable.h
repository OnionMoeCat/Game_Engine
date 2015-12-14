#ifndef EAE6320_COLLIDABLE_H
#define EAE6320_COLLIDABLE_H

#include "EntityHandle.h"

#include <vector>

namespace eae6320
{
	namespace Core
	{
		struct Collidable
		{
			float m_mass;
			std::vector<EntityHandle> m_collidings;
		};
	}
}
#endif