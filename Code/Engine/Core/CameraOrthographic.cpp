#include "CameraOrthographic.h"

eae6320::Math::cMatrix_transformation eae6320::Core::CameraOrthographic::GetViewToScreenTransformMatrix() const 
{
	return eae6320::Math::cMatrix_transformation::CreateViewToScreenTransformOrthographic(m_x_halfLength, m_y_halfLength, m_nearZ, m_farZ);
}

eae6320::Core::CameraOrthographic::CameraOrthographic(const eae6320::Math::cVector& i_position, const eae6320::Math::cQuaternion& i_rotation, const float i_x_halfLength, const float i_y_halfLength, const float i_nearZ, const float i_farZ) : eae6320::Core::ICamera(i_position, i_rotation), m_x_halfLength(i_x_halfLength), m_y_halfLength(i_y_halfLength), m_nearZ(i_nearZ), m_farZ(i_nearZ)
{

}