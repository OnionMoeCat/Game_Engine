// Header Files
//=============

#include "CameraHelper.h"

#include <utility>

#include "../Math/Functions.h"

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

bool eae6320::Core::CameraHelper::OffsetTransform(Camera& i_camera, const eae6320::Math::cVector& i_offset_position, const eae6320::Math::cQuaternion& i_offset_rotation)
{
	i_camera.m_position += i_offset_position;
	i_camera.m_rotation = i_camera.m_rotation * i_offset_rotation;
	return true;
}

eae6320::Math::cMatrix_transformation eae6320::Core::CameraHelper::CreateWorldToViewTransform(const eae6320::Math::cQuaternion& i_cameraQuaternion, const eae6320::Math::cVector& i_cameraPosition)
{
	return std::move(eae6320::Math::cMatrix_transformation::CreateWorldToViewTransform(i_cameraQuaternion, i_cameraPosition));
}
eae6320::Math::cMatrix_transformation eae6320::Core::CameraHelper::CreateViewToScreenTransform(const float i_fov, const float i_aspect, const float i_nearZ, const float i_farZ)
{
	return std::move(eae6320::Math::cMatrix_transformation::CreateViewToScreenTransform(eae6320::Math::ConvertDegreesToRadians(i_fov), i_aspect, i_nearZ, i_farZ));
}
eae6320::Math::cVector eae6320::Core::CameraHelper::Up(const Camera& i_camera)
{
	eae6320::Math::cMatrix_transformation rotation(i_camera.m_rotation, eae6320::Math::cVector());
	return eae6320::Math::cVector(0.0f, 1.0f, 0.0f) * rotation;
}
eae6320::Math::cVector eae6320::Core::CameraHelper::Front(const Camera& i_camera)
{
	eae6320::Math::cMatrix_transformation rotation(i_camera.m_rotation, eae6320::Math::cVector());
	return eae6320::Math::cVector(0.0f, 0.0f, -1.0f) * rotation;
}
eae6320::Math::cVector eae6320::Core::CameraHelper::Side(const Camera& i_camera)
{
	eae6320::Math::cMatrix_transformation rotation(i_camera.m_rotation, eae6320::Math::cVector());
	return eae6320::Math::cVector(-1.0f, 0.0f, 0.0f) * rotation;
}