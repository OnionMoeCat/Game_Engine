#include "AI.h"

#include "EntityManager.h"

void eae6320::Core::AI::Update(float dt)
{
	for (size_t i = 0; i < EntityManager::Get().GetEntitySize(); i++)
	{
		eae6320::Core::Entity* entity = eae6320::Core::EntityManager::Get().GetHandleAtIndex(i).ToEntity();
		entity->m_iController->UpdateEntity(*entity, dt);
	}
}
