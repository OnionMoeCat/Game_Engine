#ifndef EAE6320_INTERSECTION_H
#define EAE6320_INTERSECTION_H

#include "../Math/cVector.h"
#include "../Math/cMatrix_transformation.h"

namespace eae6320
{
	namespace Core
	{
		namespace Intersection
		{
			bool CheckOOBBIntersection(const eae6320::Math::cVector& i_BoxA, const eae6320::Math::cVector& i_VelocityA, const eae6320::Math::cMatrix_transformation& i_ObjAtoWorld, const eae6320::Math::cVector& i_BoxB, const eae6320::Math::cVector& i_VelocityB, const eae6320::Math::cMatrix_transformation& i_ObjBtoWorld, eae6320::Math::cVector& o_Normal, float& o_Time);
		}
	}
}
#endif
