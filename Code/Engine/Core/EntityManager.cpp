// Header Files
//=============

#include "EntityManager.h"
#include "EntityHelper.h"

// Interface
//==========

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
	for (int i = 0; i < m_list.size(); i++)
	{
		EntityHelper::CleanUp(m_list[i]);
	}
	m_list.clear();
}

bool eae6320::Core::EntityManager::CreateEntityFromFile(const char* const i_materialPath, const char* const i_meshPath)
{
	Entity entity;
	if (!EntityHelper::LoadEntityFromFile(entity, i_materialPath, i_meshPath))
	{
		return false;
	}
	m_list.push_back(entity);
	return true;
}
