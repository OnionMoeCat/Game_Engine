#ifndef EAE6320_COLLISION_H
#define EAE6320_COLLISION_H

#include "Transform.h"
#include "Collidable.h"

namespace eae6320
{
	namespace Core
	{
		namespace Collision
		{
			bool CheckOOBBCollision(const Transform& i_A, const Transform& i_B, const float i_totalTime, eae6320::Math::cVector& o_Normal, float& o_Time);
			void ResolveCollsion(Transform& i_A, Transform& i_B, const Collidable& i_collidableA, const Collidable& i_collidableB, const eae6320::Math::cVector& i_normal);
		};
	}
}
#endif
