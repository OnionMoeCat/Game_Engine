#ifndef EAE6320_CAMERA_H
#define EAE6320_CAMERA_H

// Header Files
//=============

#include "../Math/cQuaternion.h"
#include "../Math/cVector.h"

namespace eae6320
{
	namespace Core
	{
		struct Camera
		{
			eae6320::Math::cQuaternion m_rotation;
			eae6320::Math::cVector m_position;
			float m_fov;
			float m_aspect;
			float m_nearZ;
			float m_farZ;
		};
	}
}

#endif
