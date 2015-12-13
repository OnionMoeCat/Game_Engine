#ifndef EAE6320_ICAMERA_H
#define EAE6320_ICAMERA_H

// Header Files
//=============

#include "../Math/cQuaternion.h"
#include "../Math/cVector.h"
#include "../Math/cMatrix_transformation.h"

namespace eae6320
{
	namespace Core
	{
		struct ICamera
		{
			eae6320::Math::cQuaternion m_rotation;
			eae6320::Math::cVector m_position;
			virtual eae6320::Math::cMatrix_transformation GetViewToScreenTransformMatrix() const = 0;
			virtual ~ICamera() {}
			ICamera(const eae6320::Math::cVector& i_position, const eae6320::Math::cQuaternion& i_rotation);
			void OffsetPosition(const eae6320::Math::cVector& i_offset_position);
		};
	}
}

#endif
