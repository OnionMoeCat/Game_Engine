#include "AI.h"

#include "EntityManager.h"

void eae6320::Core::AI::Update(float dt)
{
	for (size_t i = 0; i < EntityManager::Get().GetEntitySize(); i++)
	{
		eae6320::Core::EntityHandle entityHandle = eae6320::Core::EntityManager::Get().GetHandleAtIndex(i);
		if (entityHandle != eae6320::Core::EntityHandle::Null)
		{
			entityHandle.ToEntity()->m_iController->UpdateEntity(*entityHandle.ToEntity(), dt);
		}
	}
}
