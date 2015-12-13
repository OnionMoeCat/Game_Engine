#ifndef EAE6320_COLLIDABLEHELPER_H
#define EAE6320_COLLIDABLEHELPER_H

// Header Files
//=============

#include "Collidable.h"

namespace eae6320
{
	namespace Core
	{
		namespace CollidableHelper
		{
			void SetMass(Collidable& i_collidable, const float i_elapsedTime);
			void SetColliding(Collidable& i_collidable, const EntityHandle& i_colliding);
		};
	}
}

#endif
