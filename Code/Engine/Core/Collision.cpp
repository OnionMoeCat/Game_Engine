#include "Collision.h"
#include "Intersection.h"

//given two gameobjects and time, check if they collide in the time, if they collide, return true and write time to o_time, write normal to o_Normal.
bool eae6320::Core::Collision::CheckOOBBCollision(const Transform& i_A, const Transform& i_B, const float i_totalTime, eae6320::Math::cVector& o_Normal, float& o_Time)
{
	// Create a matrix that transforms from ObjA's local coordinate system to the world
	// via rotation and translation.
	eae6320::Math::cMatrix_transformation ObjARotationX;
	eae6320::Math::cMatrix_transformation ObjARotationY;
	eae6320::Math::cMatrix_transformation ObjARotationZ;
	eae6320::Math::cMatrix_transformation ObjARotationBeta;
	eae6320::Math::cMatrix_transformation ObjATranslation;

	ObjARotationX.CreateXRotation(i_A->GetRotation().X());
	ObjARotationY.CreateYRotation(i_A->GetRotation().Y());
	ObjARotationZ.CreateZRotation(i_A->GetRotation().Z());

	ObjATranslation.CreateTranslation(i_A.m_position);

	eae6320::Math::cMatrix_transformation ObjAToWorld = ObjATranslation * ObjARotationZ * ObjARotationY * ObjARotationX;

	// Create a matrix that transforms from ObjB's local coordinate system to the world
	// via rotation and translation.
	eae6320::Math::cMatrix_transformation ObjBRotationX;
	eae6320::Math::cMatrix_transformation ObjBRotationY;
	eae6320::Math::cMatrix_transformation ObjBRotationZ;
	eae6320::Math::cMatrix_transformation ObjBRotationBeta;
	eae6320::Math::cMatrix_transformation ObjBTranslation;

	ObjBRotationX.CreateXRotation(i_B->GetRotation().X());
	ObjBRotationY.CreateYRotation(i_B->GetRotation().Y());
	ObjBRotationZ.CreateZRotation(i_B->GetRotation().Z());

	ObjBTranslation.CreateTranslation(i_B.m_position);

	eae6320::Math::cMatrix_transformation ObjBToWorld = ObjBTranslation * ObjBRotationZ * ObjBRotationY * ObjBRotationX;

	bool collision = Intersection::CheckOOBBIntersection(i_A->GetAABB(), i_A->GetVelocity() * i_totalTime, ObjAToWorld, i_B->GetAABB(), i_B->GetVelocity() * i_totalTime, ObjBToWorld, o_Normal, o_Time);
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
	float j = -(1 + e) * (vAB * i_normal) / (i_normal * i_normal) / (1 / mA + 1 / mB);
	i_A->GetGameObject()->SetVelocity(vA + i_normal * (j / mA));
	i_B->GetGameObject()->SetVelocity(vB - i_normal * (j / mB));
}
