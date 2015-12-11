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

	int size = static_cast<int>(EntityManager::Get().GetEntitySize());
	float totalTime = dt;
	while (totalTime > 0)
	{
		float minTime = FLT_MAX;
		int index1 = -1;
		int index2 = -1;
		eae6320::Math::cVector normal;
		for (int i = 0; i < size; i++)
		{
			for (int j = i; j < size; j++)
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
						if (tempTime < minTime)
						{
							index1 = i;
							index2 = j;
							minTime = fmax(tempTime, 0.0f);
							normal = tempNormal;
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
			//resolve collision
			Collision::ResolveCollsion(*(A.ToEntity()->m_transform), *(B.ToEntity()->m_transform), *(A.ToEntity()->m_collidable), *(B.ToEntity()->m_collidable), normal);
			//send message
			MessageSystem<SmartPtr<GameObject>, SmartPtr<GameObject>>::Get().SendingMessage("Collision", A->GetGameObject(), B->GetGameObject());
		}
		//update
		for (size_t i = 0; i < eae6320::Core::EntityManager::Get().GetEntitySize(); i++)
		{
			eae6320::Core::Entity* entity = eae6320::Core::EntityManager::Get().GetHandleAtIndex(i).ToEntity();
			eae6320::Core::TransformHelper::UpdateTransform(*entity->m_transform, *entity->m_renderable->m_material->m_effect, updateTime);
		}
		totalTime -= updateTime;
	}
}
