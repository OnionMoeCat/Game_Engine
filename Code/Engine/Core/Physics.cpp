#include "Physics.h"

#include "EntityManager.h"
#include "../Math/FloatPointUtils.h"
#include "Collision.h"
#include "TransformHelper.h"

#include <cfloat>

//the update function in Physics system
void eae6320::Core::Physics::Update(float dt)
{
	// Epsilon for float comparision
	float epsilon = FLT_EPSILON * 4;

	size_t size = EntityManager::Get().GetEntitySize();
	float totalTime = dt;
	while (totalTime > 0)
	{
		float minTime = FLT_MAX;
		int index1 = -1;
		int index2 = -1;
		eae6320::Math::cVector normal;
		for (size_t i = 0; i < size; i++)
		{
			for (size_t j = i; j < size; j++)
			{
				if (i != j)
				{
					EntityHandle A = EntityManager::Get().GetHandleAtIndex(i);
					EntityHandle B = EntityManager::Get().GetHandleAtIndex(j);

					eae6320::Math::cVector tempNormal;
					float tempTime = FLT_MAX;
					//check if A and B is colliding in totalTime
					bool collide = Collision::CheckOOBBCollision(*(A.ToEntity()->m_transform),
						*(B.ToEntity()->m_transform), totalTime, tempNormal, tempTime);
					if (collide)
					{
						// if they are colliding, check if collision happens first and if they've ready collided.
						// if they've collided, ignore until they detached or one of them collides with another cube.
						// we only care about the first to collide because everything may change after collision resolve.
						if (tempTime < minTime && !(A.ToEntity()->m_collidable->m_colliding == B && B.ToEntity()->m_collidable->m_colliding == A))
						{
							index1 = i;
							index2 = j;
							minTime = fmax(tempTime, 0.0f);
							normal = tempNormal;
						}
					}
					else
					{
						//if they are not colliding and used to collide, set their colliding null.
						if (A.ToEntity()->m_collidable->m_colliding == B && B.ToEntity()->m_collidable->m_colliding == A)
						{
							A.ToEntity()->m_collidable->m_colliding = ;
							A.ToEntity()->m_collidable->m_colliding = ;
						}
					}
				}
			}
		}
		//if no pairs of cube are collding set updateTime to totalTime, if any, set updateTime to the collision time.
		float updateTime;
		if (index1 != -1 && index2 != -1)
		{
			updateTime = minTime;
		}
		else
		{
			updateTime = totalTime;
		}
		//if any pairs collide
		if (index1 != -1 && index2 != -1)
		{
			//set collidable->colliding (which object the collidable is collding with)
			EntityHandle A = EntityManager::Get().GetHandleAtIndex(index1);
			EntityHandle B = EntityManager::Get().GetHandleAtIndex(index2);
			//detach and attach
			if (!(A->GetColliding() == nullptr))
			{
				A->GetColliding()->SetColliding(SmartPtr<Collidable>(nullptr));
			}
			if (!(B->GetColliding() == nullptr))
			{
				B->GetColliding()->SetColliding(SmartPtr<Collidable>(nullptr));
			}
			A->SetColliding(B);
			B->SetColliding(A);

			//resolve collision
			Collision::ResolveCollsion(*(A.ToEntity()->m_transform), *(B.ToEntity()->m_transform), *(A.ToEntity()->m_collidable), *(B.ToEntity()->m_collidable), normal);
			//MessageSystem<SmartPtr<GameObject>, SmartPtr<GameObject>>::Get().SendingMessage("Collision", A->GetGameObject(), B->GetGameObject());
		}
		//update
		for (size_t i = 0; i < eae6320::Core::EntityManager::Get().GetEntitySize(); i++)
		{
			eae6320::Core::Entity* entity = eae6320::Core::EntityManager::Get().GetHandleAtIndex(i).ToEntity();
			eae6320::Core::TransformHelper::UpdateTransform(*entity->m_transform, *entity->m_renderable->m_material->m_effect, dt);
		}
		totalTime -= updateTime;
	}
}
