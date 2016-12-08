/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_CAMERAHELPER_H
#define EAE6320_CAMERAHELPER_H

// Header Files
//=============
#include "Camera.h"
#include "../Math/cMatrix_transformation.h"

// Interface
//==========

namespace eae6320
{
	namespace Core
	{
		struct CameraHelper
		{
			void static Initialize(Camera& i_camera, const eae6320::Math::cQuaternion& i_rotation, const eae6320::Math::cVector& i_position, const float i_fov, const float i_aspect, const float i_minZ, const float i_maxZ);
			bool static OffsetTransform(Camera& i_camera, const eae6320::Math::cVector& i_offset_position, const eae6320::Math::cQuaternion& i_offset_rotation);
			eae6320::Math::cMatrix_transformation static CreateWorldToViewTransform(const eae6320::Math::cQuaternion& i_cameraQuaternion, const eae6320::Math::cVector& i_cameraPosition);
			eae6320::Math::cMatrix_transformation static CreateViewToScreenTransform(const float i_fov, const float i_aspect, const float i_nearZ, const float i_farZ);
			eae6320::Math::cVector static Up(const Camera& i_camera);
			eae6320::Math::cVector static Front(const Camera& i_camera);
			eae6320::Math::cVector static Side(const Camera& i_camera);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H