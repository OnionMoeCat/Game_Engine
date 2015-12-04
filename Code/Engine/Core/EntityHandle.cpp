#include "EntityHandle.h"
#include "EntityManager.h"

eae6320::Core::EntityHandle::EntityHandle()
{

}

eae6320::Core::EntityHandle::EntityHandle(const Entity& entity) :
	m_handleIndex(entity.m_handleIndex),
	m_uniqueId(entity.m_uniqueId)
{

}
// This function dereferences the handle.
eae6320::Core::Entity* eae6320::Core::EntityHandle::ToEntity() const
{
	Entity* pEntity = EntityManager::Get().m_list[m_handleIndex];
	if (pEntity != NULL
		&& pEntity->m_uniqueId == m_uniqueId)
	{
		return pEntity;
	}
	return NULL;
}