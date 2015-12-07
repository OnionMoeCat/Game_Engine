#ifndef EAE6320_INTERSECTION_H
#define EAE6320_INTERSECTION_H

#include "../Math/cVector.h"
#include "../Math/cMatrix_transformation.h"

namespace eae6320
{
	namespace Core
	{
		struct Intersection
		{
		private:
			static const float EPSILON;
			static void GetOverlapSeparateTime(const float i_Begin, const float i_End, const float i_Min, const float i_Max, float& o_TimeOverlap, float& o_TimeSeparate, float& o_Direction);
		public:
			static bool CheckOOBBIntersection(const eae6320::Math::cVector& i_BoxA, const eae6320::Math::cVector& i_VelocityA, const eae6320::Math::cMatrix_transformation& i_ObjAtoWorld, const eae6320::Math::cVector& i_BoxB, const eae6320::Math::cVector& i_VelocityB, const eae6320::Math::cMatrix_transformation& i_ObjBtoWorld, const eae6320::Math::cVector& o_Normal, float& o_Time);
		};
	}
}
#endif
