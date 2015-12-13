#include "CameraPerspective.h"
#include "../Math/Functions.h"

eae6320::Math::cMatrix_transformation eae6320::Core::CameraPerspective::GetViewToScreenTransformMatrix() const 
{
	return eae6320::Math::cMatrix_transformation::CreateViewToScreenTransformPerspective(eae6320::Math::ConvertDegreesToRadians(m_fov), m_aspect, m_nearZ, m_farZ);
}

eae6320::Core::CameraPerspective::CameraPerspective(const eae6320::Math::cVector& i_position, const eae6320::Math::cQuaternion& i_rotation, const float i_fov, const float i_aspect, const float i_nearZ, const float i_farZ) : eae6320::Core::ICamera(i_position, i_rotation), m_fov(i_fov), m_aspect(i_aspect), m_nearZ(i_nearZ), m_farZ(i_farZ)
{

}