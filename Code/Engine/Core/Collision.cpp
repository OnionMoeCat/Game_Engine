#include "Collision.h"
#include "Intersection.h"
#include "TransformHelper.h"

//given two gameobjects and time, check if they collide in the time, if they collide, return true and write time to o_time, write normal to o_Normal.
bool eae6320::Core::Collision::CheckOOBBCollision(const Transform& i_A, const Transform& i_B, const float i_totalTime, eae6320::Math::cVector& o_Normal, float& o_Time)
{
	eae6320::Math::cMatrix_transformation ObjAToWorld(i_A.m_rotation, i_A.m_position);

	eae6320::Math::cMatrix_transformation ObjBToWorld(i_B.m_rotation, i_B.m_position);

	bool collision = Intersection::CheckOOBBIntersection(i_A.m_AABB, i_A.m_velocity * i_totalTime, ObjAToWorld, i_B.m_AABB, i_B.m_velocity * i_totalTime, ObjBToWorld, o_Normal, o_Time);
	o_Time = o_Time * i_totalTime;
	return collision;
}
	//resolve collision given collidable A and collidable B and normal.
void eae6320::Core::Collision::ResolveCollsion(Transform& i_A, Transform& i_B, const Collidable& i_collidableA, const Collidable& i_collidableB, const eae6320::Math::cVector& i_normal)
{
	//change the speed according to formula from http://chrishecker.com/images/e/e7/Gdmphys3.pdf
	//va' = va + (j / ma)n
	//vb' = vb - (j / mb)n
	//j = -(1+e)(va - vb)n /((1/ma + 1/mb)n*n)
	eae6320::Math::cVector vA = i_A.m_velocity;
	eae6320::Math::cVector vB = i_B.m_velocity;
	eae6320::Math::cVector vAB = vA - vB;
	float mA = i_collidableA.m_mass;
	float mB = i_collidableB.m_mass;
	float j = -(eae6320::Math::Dot(vAB, i_normal) / eae6320::Math::Dot(i_normal, i_normal) / (1 / mA + 1 / mB));
	TransformHelper::SetVelocity(i_A, vA + i_normal * (j / mA));
	TransformHelper::SetVelocity(i_B, vB - i_normal * (j / mB));
}
