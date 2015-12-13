#ifndef EAE6320_CAMERAORTHOGRAPHIC_H
#define EAE6320_CAMERAORTHOGRAPHIC_H

// Header Files
//=============

#include "ICamera.h"

namespace eae6320
{
	namespace Core
	{
		struct CameraOrthographic : ICamera
		{
			float m_x_halfLength;
			float m_y_halfLength;
			float m_nearZ;
			float m_farZ;
			eae6320::Math::cMatrix_transformation GetViewToScreenTransformMatrix() const ;
			CameraOrthographic(const eae6320::Math::cVector& i_position, const eae6320::Math::cQuaternion& i_rotation, const float i_x_halfLength, const float i_y_halfLength, const float i_nearZ, const float i_farZ);
		};
	}
}

#endif
