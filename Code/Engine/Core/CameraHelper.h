/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_CAMERAHELPER_H
#define EAE6320_CAMERAHELPER_H

// Header Files
//=============
#include "Camera.h"

// Interface
//==========

namespace eae6320
{
	namespace Core
	{
		struct CameraHelper
		{
			void static Initialize(Camera& i_camera, const eae6320::Math::cQuaternion& i_rotation, const eae6320::Math::cVector& i_position, const float i_fov, const float i_aspect, const float i_minZ, const float i_maxZ);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H