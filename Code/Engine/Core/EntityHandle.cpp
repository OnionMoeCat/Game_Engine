#include "EntityHandle.h"
#include "EntityManager.h"

const eae6320::Core::EntityHandle eae6320::Core::EntityHandle::Null;

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
	if (m_handleIndex == -1)
	{
		return NULL;
	}

	Entity* pEntity = EntityManager::Get().m_list[m_handleIndex];
	if (pEntity != NULL
		&& pEntity->m_uniqueId == m_uniqueId)
	{
		return pEntity;
	}
	return NULL;
}

bool eae6320::Core::EntityHandle::operator==(const EntityHandle& rhs) const
{
	return (this->m_uniqueId == rhs.m_uniqueId && this->m_handleIndex == rhs.m_handleIndex);
}

bool eae6320::Core::EntityHandle::operator!=(const EntityHandle& rhs) const
{ 
	return !(*this == rhs); 
}