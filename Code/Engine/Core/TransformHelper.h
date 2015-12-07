#ifndef EAE6320_TRANSFORMHELPER_H
#define EAE6320_TRANSFORMHELPER_H

// Header Files
//=============

#include "Transform.h"
#include "Entity.h"

namespace eae6320
{
	namespace Core
	{
		struct TransformHelper
		{
			void static UpdateTransform(Transform& i_transform, eae6320::Graphics::Effect& i_effect, const float i_elapsedTime);
			void static SetVelocity(Transform& i_transform, const eae6320::Math::cVector& i_velocity);
			void static SetPosition(Transform& i_transform, eae6320::Graphics::Effect& i_effect, const eae6320::Math::cVector& i_position);
			void static SetRotation(Transform& i_transform, eae6320::Graphics::Effect& i_effect, const eae6320::Math::cQuaternion& i_rotation);
			void static SetAABB(Transform& i_transform, const eae6320::Math::cVector& i_AABB);
		};
	}
}

#endif
