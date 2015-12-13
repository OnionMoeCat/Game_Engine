#include "ICamera.h"

void eae6320::Core::ICamera::OffsetPosition(const eae6320::Math::cVector& i_offset_position)
{
	m_position += i_offset_position;
}

eae6320::Core::ICamera::ICamera(const eae6320::Math::cVector& i_position, const eae6320::Math::cQuaternion& i_rotation) : m_position(i_position), m_rotation(i_rotation)
{

}