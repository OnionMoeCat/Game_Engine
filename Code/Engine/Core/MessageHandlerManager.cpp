// Header Files
//=============

#include "MessageHandlerManager.h"

#include <cassert>


// Interface
//==========

uint32_t eae6320::Core::MessageHandlerManager::g_UniqueID = 0;

eae6320::Core::MessageHandlerManager eae6320::Core::MessageHandlerManager::m_Instance;

eae6320::Core::MessageHandlerManager::MessageHandlerManager()
{

}

eae6320::Core::MessageHandlerManager& eae6320::Core::MessageHandlerManager::Get()
{
	return m_Instance;
}

void eae6320::Core::MessageHandlerManager::CleanUp()
{
	for (size_t i = 0; i < m_list.size(); i++)
	{
		delete m_list[i];
	}
	m_list.clear();
}

eae6320::Core::MessageHandlerHandle eae6320::Core::MessageHandlerManager::InsertAndGetIndex(eae6320::Core::IMessageHandler* i_messageHandler)
{
	for (uint32_t i = 0; i < m_list.size(); i++)
	{
		if (m_list[i] == NULL)
		{
			i_messageHandler->m_handleIndex = i;
		}
	}

	i_messageHandler->m_handleIndex = static_cast<uint32_t>(m_list.size());
	i_messageHandler->m_uniqueId = g_UniqueID;
	g_UniqueID++;
	m_list.push_back(i_messageHandler);
	return MessageHandlerHandle(*i_messageHandler);
}