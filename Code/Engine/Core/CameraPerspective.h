#ifndef EAE6320_CAMERAPERSPECTIVE_H
#define EAE6320_CAMERAPERSPECTIVE_H

// Header Files
//=============

#include "ICamera.h"

namespace eae6320
{
	namespace Core
	{
		struct CameraPerspective : ICamera
		{
			float m_fov;
			float m_aspect;
			float m_nearZ;
			float m_farZ;
			eae6320::Math::cMatrix_transformation GetViewToScreenTransformMatrix() const ;
			CameraPerspective(const eae6320::Math::cVector& i_position, const eae6320::Math::cQuaternion& i_rotation, const float i_fov, const float i_aspect, const float i_nearZ, const float i_farZ);
		};
	}
}

#endif
