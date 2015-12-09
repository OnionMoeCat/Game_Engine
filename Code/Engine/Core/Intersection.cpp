#include "Intersection.h"
#include "../Math/MathFloat.h"
#include "../Math/FloatPointUtils.h"

#include <cfloat>
#include <cmath>

namespace
{
	//Coefficient for float comparation
	const float EPSILON = 4 * FLT_EPSILON;
	void GetOverlapSeparateTime(const float i_Begin, const float i_End, const float i_Min, const float i_Max, float& o_TimeOverlap, float& o_TimeSeparate, float& o_Direction);
}

namespace
{
	void GetOverlapSeparateTime(const float i_Begin, const float i_End, const float i_Min, const float i_Max, float& o_TimeOverlap, float& o_TimeSeparate, float& o_Direction)
	{
		//see if two points are all outside of [Min Max]	
		float N_A_P = FLT_MAX;
		float N_A_N = -FLT_MAX;
		//on the same edge  -> no overlap: overlap = FLT_MAX separation = -FLT_MAX
		if ((i_Begin < i_Min && i_End < i_Min) || (i_Begin > i_Max && i_End > i_Max))
		{
			o_TimeOverlap = N_A_P; o_TimeSeparate = N_A_N;
			o_Direction = 0.0f;
		}
		//on the differ edge -> overlap and separate in middle: calculate time to reach two min and max, overlap = min(t1, t2), separation = max(t1, t2)
		else if ((i_Begin < i_Min && i_End > i_Max) || (i_Begin > i_Max && i_End < i_Min))
		{
			float XMinTime = eae6320::Math::MathFloat::GetLambda(i_Begin, i_End, i_Min);
			float XMaxTime = eae6320::Math::MathFloat::GetLambda(i_Begin, i_End, i_Max);
			o_TimeOverlap = fmin(XMinTime, XMaxTime);
			o_TimeSeparate = fmax(XMinTime, XMaxTime);
			if (i_Begin < i_End)
			{
				o_Direction = -1;
			}
			else if (i_Begin > i_End)
			{
				o_Direction = 1;
			}
			else
			{
				o_Direction = 0;
			}
		}
		//else see if two points are all inside [Min Max] -> all overlap overlap = 0.0 separation = N/A
		else if (
			(i_Begin > i_Min || eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(i_Begin, i_Min, EPSILON, EPSILON))
			&& (i_Begin < i_Max || eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(i_Begin, i_Max, EPSILON, EPSILON))
			&& (i_End > i_Min || eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(i_End, i_Min, EPSILON, EPSILON))
			&& (i_End < i_Max || eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(i_End, i_Max, EPSILON, EPSILON))
			)
		{
			o_TimeOverlap = N_A_N; o_TimeSeparate = N_A_P;
			if (i_Begin < i_End)
			{
				o_Direction = -1;
			}
			else if (i_Begin > i_End)
			{
				o_Direction = 1;
			}
			else
			{
				o_Direction = 0;
			}
		}
		//else one is inside of [Min Max] and the other not -> overlap/separate in middle
		else
		{
			//if begin is inside-> overlap = -FLT_MAX, calculate time to reach two min and max, separate = the time in [0, 1]
			if (
				(i_Begin > i_Min || eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(i_Begin, i_Min, EPSILON, EPSILON))
				&& (i_Begin < i_Max || eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(i_Begin, i_Max, EPSILON, EPSILON))
				)
			{
				o_TimeOverlap = N_A_N;
				if (i_Begin < i_End)
				{
					o_Direction = 1;
				}
				else if (i_Begin > i_End)
				{
					o_Direction = -1;
				}
				else
				{
					o_Direction = 0;
				}
				float XMinTime = eae6320::Math::MathFloat::GetLambda(i_Begin, i_End, i_Min);
				float XMaxTime = eae6320::Math::MathFloat::GetLambda(i_Begin, i_End, i_Max);
				if (
					(XMinTime > 0.0f || eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(XMinTime, 0.0f, EPSILON, EPSILON))
					&& (XMinTime < 1.0f || eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(XMinTime, 1.0f, EPSILON, EPSILON))
					)
				{
					o_TimeSeparate = XMinTime;
				}
				else
				{
					o_TimeSeparate = XMaxTime;
				}
			}
			//if begin is outside-> separate = -FLT_MAX, calculate time to reach two min and max, overlap = the time in [0, 1]
			else
			{
				o_TimeSeparate = N_A_P;
				if (i_Begin < i_End)
				{
					o_Direction = -1;
				}
				else if (i_Begin > i_End)
				{
					o_Direction = 1;
				}
				else
				{
					o_Direction = 0;
				}
				float XMinTime = eae6320::Math::MathFloat::GetLambda(i_Begin, i_End, i_Min);
				float XMaxTime = eae6320::Math::MathFloat::GetLambda(i_Begin, i_End, i_Max);
				if (
					(XMinTime > 0.0f || eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(XMinTime, 0.0f, EPSILON, EPSILON))
					&& (XMinTime < 1.0f || eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(XMinTime, 1.0f, EPSILON, EPSILON))
					)
				{
					o_TimeOverlap = XMinTime;
				}
				else
				{
					o_TimeOverlap = XMaxTime;
				}
			}
		}
	}
}

// Now we want to do our swept separating axis check.
// For each axis find the separation distance normalized to Object A's movement along that axis during the frame (i.e Calculate
// the distance from the projection of Object A's transform Center onto that axis to the nearest of Object B's bounding
// edges on that axis. Dividing that by Object A's movement along that axis during the frame will give us the time within
// this frame that it takes to collapse the separation between them.)
// This is the overlap time.
// If that time is < 0.0f the overlap happened before the frame started (i.e. we started the frame overlapped)
// If that time is > 1.0f the overlap happened after the frame has ended (i.e. we didn't overlap during this frame, it'll happen in the future)
// In this case we can exit out early because we've found a separating axis.

//Check if box A and box B intersection in geometry.
bool eae6320::Core::Intersection::CheckOOBBIntersection(const eae6320::Math::cVector & i_BoxA, const eae6320::Math::cVector & i_VelocityA, const eae6320::Math::cMatrix_transformation& i_ObjAtoWorld, const eae6320::Math::cVector & i_BoxB, const eae6320::Math::cVector & i_VelocityB, const eae6320::Math::cMatrix_transformation& i_ObjBtoWorld,
	eae6320::Math::cVector & o_Normal, float& o_Time)
{
	float AToBOverlapTime, AToBSeparationTime;
	float BToAOverlapTime, BToASeparationTime;
	float AToBXOverlapTime, AToBYOverlapTime, AToBZOverlapTime;
	float BToAXOverlapTime, BToAYOverlapTime, BToAZOverlapTime;
	float AToBXDirection, AToBYDirection, AToBZDirection;
	float BToAXDirection, BToAYDirection, BToAZDirection;
	eae6320::Math::cVector AToBVelocity, BToAVelocity;
	{
		eae6320::Math::cMatrix_transformation WorldToObjB = eae6320::Math::cMatrix_transformation::Invert(i_ObjBtoWorld);
		// Muliply by ObjAToWorld to get from ObjA's coordinate system to ObjB's
		eae6320::Math::cMatrix_transformation ObjAToObjB = WorldToObjB * i_ObjAtoWorld;

		// Transform Object A's Center (or Offset) into Object B's coordinate system (using our Object A to Obj B transform matrix)
		eae6320::Math::cVector Center(0.0f, 0.0f, 0.0f);

		eae6320::Math::cVector ACenterInB = ObjAToObjB * Center;

		// Project Object A's bounding box extents onto each of Object B's axes
		// 1. Treat each of Object A's extents (width, high, depth) as vector along each of Object A's axes
		// (i.e. the extent of Object A's bounding box along it's X axis (width) would be the vector [Extent.X, 0.0f, 0.0f]
		// in Object A's coordinate system
		float xExt = i_BoxA.x;
		float yExt = i_BoxA.y;
		float zExt = i_BoxA.z;

		// 2. Transform these extent vectors from Object A's coordinate system to Object B's coordinate system (using our ObjAtoObjB matrix)
		eae6320::Math::cVector xVector(xExt, 0.0f, 0.0f);
		eae6320::Math::cVector AxExtInB = ObjAToObjB * xVector;

		eae6320::Math::cVector yVector(0.0f, yExt, 0.0f);
		eae6320::Math::cVector AyExtInB = ObjAToObjB * yVector;

		eae6320::Math::cVector zVector(0.0f, 0.0f, zExt);
		eae6320::Math::cVector AzExtInB = ObjAToObjB * zVector;

		// 3. For each of Object B's axes accumulate the influence of (project) each of the 3 transformed Object A's extent vectors on that axis
		float totalXExt = abs(AxExtInB.x) + abs(AyExtInB.x) + abs(AzExtInB.x);
		float totalYExt = abs(AxExtInB.y) + abs(AyExtInB.y) + abs(AzExtInB.y);
		float totalZExt = abs(AxExtInB.z) + abs(AyExtInB.z) + abs(AzExtInB.z);

		// Expand Object B's bounding box by Object A's projected extents
		float XMin = -totalXExt - i_BoxB.x;
		float XMax = -XMin;
		float YMin = -totalYExt - i_BoxB.y;
		float YMax = -YMin;
		float ZMin = -totalZExt - i_BoxB.z;
		float ZMax = -ZMin;

		// Now that we've expanded Object B's bounding box by Object A's projected extents we can treat Object A as a point
		// (determined by transforming it's Center (or Offset) from it's coordinate system to Object B's
		// Find Object A's frame movement vector relative to Object B's (essentially treating Object B as stationary)
		// 1. Transform both Objects frame movement vectors from World coordinate system to Object B's coordinate system

		eae6320::Math::cVector velocityA = WorldToObjB * i_VelocityA;
		eae6320::Math::cVector velocityB = WorldToObjB * i_VelocityB;

		// 2. Subtract Object A's frame movement vector (calculated in Object B's coordinate system) from Object B's (also in it's coordinate system)
		eae6320::Math::cVector vAtoB = velocityA - velocityB;
		AToBVelocity = vAtoB;

		// Calculate the begin and end point for each axis.
		float XBegin = ACenterInB.x;
		float YBegin = ACenterInB.y;
		float ZBegin = ACenterInB.z;
		float vX = vAtoB.x;
		float vY = vAtoB.y;
		float vZ = vAtoB.z;
		float XEnd = XBegin + vX;
		float YEnd = YBegin + vY;
		float ZEnd = ZBegin + vZ;

		float XOverlap, XSeparate;
		float YOverlap, YSeparate;
		float ZOverlap, ZSeparate;

		// Now we want to do our swept separating axis check.
		GetOverlapSeparateTime(XBegin, XEnd, XMin, XMax, XOverlap, XSeparate, AToBXDirection);
		GetOverlapSeparateTime(YBegin, YEnd, YMin, YMax, YOverlap, YSeparate, AToBYDirection);
		GetOverlapSeparateTime(ZBegin, ZEnd, ZMin, ZMax, ZOverlap, ZSeparate, AToBZDirection);

		AToBXOverlapTime = XOverlap;
		AToBYOverlapTime = YOverlap;
		AToBZOverlapTime = ZOverlap;

		// As we go along doing each axis keep track of the latest overlap time and earliest separation time so we can compare the two
		// when we've done all checks.
		AToBOverlapTime = fmax(fmax(XOverlap, YOverlap), ZOverlap);
		AToBSeparationTime = fmin(fmin(XSeparate, YSeparate), ZSeparate);
	}

	// Now check Object B against Object A in Object A's coordinate system continuing to find the latest overlap and first separation
	{
		eae6320::Math::cMatrix_transformation WorldToObjA = eae6320::Math::cMatrix_transformation::Invert(i_ObjAtoWorld);

		eae6320::Math::cMatrix_transformation ObjBToObjA = WorldToObjA* i_ObjBtoWorld;

		eae6320::Math::cVector Center(0.0f, 0.0f, 0.0f);
		eae6320::Math::cVector BCenterInA = ObjBToObjA * Center;

		float xExt = i_BoxB.x;
		float yExt = i_BoxB.y;
		float zExt = i_BoxB.z;

		eae6320::Math::cVector xVector(xExt, 0.0f, 0.0f);
		eae6320::Math::cVector BxExtInA = ObjBToObjA * xVector;

		eae6320::Math::cVector yVector(0.0f, yExt, 0.0f);
		eae6320::Math::cVector ByExtInA = ObjBToObjA * yVector;

		eae6320::Math::cVector zVector(0.0f, 0.0f, zExt);
		eae6320::Math::cVector BzExtInA = ObjBToObjA * zVector;

		float totalXExt = abs(BxExtInA.x) + abs(ByExtInA.x) + abs(BzExtInA.x);
		float totalYExt = abs(BxExtInA.y) + abs(ByExtInA.y) + abs(BzExtInA.y);
		float totalZExt = abs(BxExtInA.z) + abs(ByExtInA.z) + abs(BzExtInA.z);

		float XMin = -totalXExt - i_BoxA.x;
		float XMax = -XMin;
		float YMin = -totalYExt - i_BoxA.y;
		float YMax = -YMin;
		float ZMin = -totalZExt - i_BoxA.z;
		float ZMax = -ZMin;

		eae6320::Math::cVector velocityB = WorldToObjA * i_VelocityB;
		eae6320::Math::cVector velocityA = WorldToObjA * i_VelocityA;
		eae6320::Math::cVector vBtoA = velocityB - velocityA;
		BToAVelocity = vBtoA;

		float XBegin = BCenterInA.x;
		float YBegin = BCenterInA.y;
		float ZBegin = BCenterInA.z;
		float vX = vBtoA.x;
		float vY = vBtoA.y;
		float vZ = vBtoA.z;
		float XEnd = XBegin + vX;
		float YEnd = YBegin + vY;
		float ZEnd = ZBegin + vZ;

		float XOverlap, XSeparate;
		float YOverlap, YSeparate;
		float ZOverlap, ZSeparate;

		GetOverlapSeparateTime(XBegin, XEnd, XMin, XMax, XOverlap, XSeparate, BToAXDirection);
		GetOverlapSeparateTime(YBegin, YEnd, YMin, YMax, YOverlap, YSeparate, BToAYDirection);
		GetOverlapSeparateTime(ZBegin, ZEnd, ZMin, ZMax, ZOverlap, ZSeparate, BToAZDirection);

		BToAXOverlapTime = XOverlap;
		BToAYOverlapTime = YOverlap;
		BToAZOverlapTime = ZOverlap;

		BToAOverlapTime = fmax(fmax(XOverlap, YOverlap), ZOverlap);
		BToASeparationTime = fmin(fmin(XSeparate, YSeparate), ZSeparate);
	}

	float OverlapTime = fmax(AToBOverlapTime, BToAOverlapTime);
	float SeparationTime = fmin(AToBSeparationTime, BToASeparationTime);

	// If we get to here we didn't find a separation that lasted the whole frame so we need to compare the time of last overlap
	// against the time of first separation
	if (OverlapTime < SeparationTime
		|| eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(OverlapTime, SeparationTime, EPSILON, EPSILON))
	{
		o_Time = OverlapTime;
		//If two cubes collide, find the colliding normal. 
		//To find normal, find which face/edge/point two cubes collide.
		//Find which axises actually begins to collide at OverlapTime.
		//If only one axis begins to collide at OverlapTime, collision happens on face. For cubes, the normal is vector from center to face center point(i.e. normal of the face).
		//If two axises begin to collide at OverlapTime, collision happens on edge. For cubes, the normal is vector from center to the middle point of edge.
		//If three axises begin to collide at OverlapTime, collision happens on corner. For cubes, the normal is vector from center to the point.
		//We notice that colliding with edge is in fact colliding with two faces at the same time. 
		//Colliding with point is in fact colliding with three faces at the same time.
		//We can use Vector3 to represent normal vector. If it collides with left face, (-1 , 0 , 0). For colliding with edge where left and top intersects, 
		//(-1 , 0 , 0) + (0 , 0 , 1) = (-1 , 0 , 1)
		//So, for each axis, -1 means colliding with negative face(left, down, bottom), 0 means no colliding, 1 means collding with positive face.
		//We can consider axis seperately.
		//This vector is related to cube, we need to transform it from cube's coordinate to world.
		if ((OverlapTime > 0.0f
			|| eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(OverlapTime, 0.0f, EPSILON, EPSILON))
			&& (OverlapTime < 1.0f
				|| eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(OverlapTime, 1.0f, EPSILON, EPSILON)))
		{
			//if collide happens on axises in AToB
			if (eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(AToBOverlapTime, OverlapTime, EPSILON, EPSILON))
			{
				//normal vector
				eae6320::Math::cVector temp(0.0f, 0.0f, 0.0f);
				//if collides happens on X, set temp.X
				if (eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(AToBXOverlapTime, OverlapTime, EPSILON, EPSILON))
				{
					//MessagedAssert(!FloatPointUtils::AlmostEqualRelativeAndAbs(AToBVelocity.X(), 0.0f, EPSILON, EPSILON), "vx should not be 0");
					temp.x = AToBXDirection;
				}
				//if collides happens on Y, set temp.Y
				if (eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(AToBYOverlapTime, OverlapTime, EPSILON, EPSILON))
				{
					//MessagedAssert(!FloatPointUtils::AlmostEqualRelativeAndAbs(AToBVelocity.Y(), 0.0f, EPSILON, EPSILON), "vy should not be 0");
					temp.y = AToBYDirection;
				}
				//if collides happens on Z, set temp.Z
				if (eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(AToBZOverlapTime, OverlapTime, EPSILON, EPSILON))
				{
					//MessagedAssert(!FloatPointUtils::AlmostEqualRelativeAndAbs(AToBVelocity.Z(), 0.0f, EPSILON, EPSILON), "vz should not be 0");
					temp.z = AToBZDirection;
				}
				//the vector is in B. Use i_ObjBtoWorld to transform the vector from B to World.
				o_Normal = i_ObjBtoWorld * temp;
			}
			//if collide happens on axises in BToA
			else if (eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(BToAOverlapTime, OverlapTime, EPSILON, EPSILON))
			{
				eae6320::Math::cVector temp(0.0f, 0.0f, 0.0f);
				if (eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(BToAXOverlapTime, OverlapTime, EPSILON, EPSILON))
				{
					//MessagedAssert(!FloatPointUtils::AlmostEqualRelativeAndAbs(BToAVelocity.X(), 0.0f, EPSILON, EPSILON), "vx should not be 0");
					temp.x = BToAXDirection;
				}
				if (eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(BToAYOverlapTime, OverlapTime, EPSILON, EPSILON))
				{
					//MessagedAssert(!FloatPointUtils::AlmostEqualRelativeAndAbs(BToAVelocity.Y(), 0.0f, EPSILON, EPSILON), "vy should not be 0");
					temp.y = BToAYDirection;
				}
				if (eae6320::Math::FloatPointUtils::AlmostEqualRelativeAndAbs(BToAZOverlapTime, OverlapTime, EPSILON, EPSILON))
				{
					//MessagedAssert(!FloatPointUtils::AlmostEqualRelativeAndAbs(BToAVelocity.Z(), 0.0f, EPSILON, EPSILON), "vz should not be 0");
					temp.z = BToAZDirection;
				}
				//the vector is in A. Use i_ObjBtoWorld to transform the vector from A to World.
				o_Normal = i_ObjAtoWorld * temp;
			}
		}
		return true;
	}
	else
	{
		o_Time = FLT_MAX;
		o_Normal = eae6320::Math::cVector(0.0f, 0.0f, 0.0f);
		return false;
	}
}