#ifndef EAE6320_COLLIDABLE_H
#define EAE6320_COLLIDABLE_H

#include "EntityHandle.h"

namespace eae6320
{
	namespace Core
	{
		struct Collidable
		{
			EntityHandle m_colliding;	
			float m_mass;
		};
	}
}
#endif