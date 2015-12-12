// Header Files
//=============

#include "EntityManager.h"
#include "EntityHelper.h"

#include <cassert>


// Interface
//==========

uint32_t eae6320::Core::EntityManager::g_UniqueID = 0;

eae6320::Core::EntityManager eae6320::Core::EntityManager::m_Instance;

eae6320::Core::EntityManager::EntityManager()
{

}

eae6320::Core::EntityManager& eae6320::Core::EntityManager::Get()
{
	return m_Instance;
}

void eae6320::Core::EntityManager::CleanUp()
{
	for (size_t i = 0; i < m_list.size(); i++)
	{
		EntityHelper::CleanUp(*m_list[i]);
		delete m_list[i];
	}
	m_list.clear();
}

bool eae6320::Core::EntityManager::CreateEntityFromFile(const char* const i_materialPath, const char* const i_meshPath, EntityHandle& o_handle)
{
	Entity* pEntity = new Entity();
	if (!EntityHelper::LoadEntityFromFile(*pEntity, i_materialPath, i_meshPath))
	{
		return false;
	}
	pEntity->m_uniqueId = g_UniqueID;
	g_UniqueID++;
	o_handle = InsertAndGetIndex(pEntity);
	return true;
}

eae6320::Core::EntityHandle eae6320::Core::EntityManager::InsertAndGetIndex(eae6320::Core::Entity* i_entity)
{
	for (uint32_t i = 0; i < m_list.size(); i++)
	{
		if (m_list[i] == NULL)
		{
			i_entity->m_handleIndex = i;
		}
	}

	i_entity->m_handleIndex = static_cast<uint32_t>(m_list.size());
	m_list.push_back(i_entity);	
	return EntityHandle(*i_entity);
}

void eae6320::Core::EntityManager::Release(const EntityHandle& i_handle)
{
	Entity* entity = i_handle.ToEntity();
	if (entity != NULL)
	{
		m_list[entity->m_handleIndex] = NULL;
		EntityHelper::CleanUp(*entity);
		delete entity;
	}
}

size_t eae6320::Core::EntityManager::GetEntitySize()
{
	return m_list.size();
}

eae6320::Core::EntityHandle eae6320::Core::EntityManager::GetHandleAtIndex(size_t i_index)
{
	if (m_list[i_index] == NULL)
	{	
		return EntityHandle::Null;
	}
	return EntityHandle(*m_list[i_index]);
}
