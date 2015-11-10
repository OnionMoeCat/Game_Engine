// Header Files
//=============

#include "CameraHelper.h"

// Interface
//==========

void eae6320::Core::CameraHelper::Initialize(Camera& i_camera, const eae6320::Math::cQuaternion& i_rotation, const eae6320::Math::cVector& i_position, const float i_fov, const float i_aspect, const float i_nearZ, const float i_farZ)
{
	i_camera.m_rotation = i_rotation;
	i_camera.m_position = i_position;
	i_camera.m_fov = i_fov;
	i_camera.m_aspect = i_aspect;
	i_camera.m_nearZ = i_nearZ;
	i_camera.m_farZ = i_farZ;
}

bool eae6320::Core::CameraHelper::OffsetPosition(Camera& i_camera, const eae6320::Math::cVector& i_offset_position)
{
	i_camera.m_position += i_offset_position;
	return true;
}