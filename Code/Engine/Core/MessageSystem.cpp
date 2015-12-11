#include "MessageSystem.h"

eae6320::Core::MessageSystem eae6320::Core::MessageSystem::m_Instance;

eae6320::Core::MessageSystem& eae6320::Core::MessageSystem::Get()
{
	return m_Instance;
}

void eae6320::Core::MessageSystem::AddMessageHandler(const std::string i_Message, const MessageHandlerHandle& i_pHandler)
{
	std::unordered_map<std::string, std::vector<MessageHandlerHandle>>::iterator temp = m_MessageHandlers.find(i_Message);
	if (temp != m_MessageHandlers.end())
	{
		temp->second.push_back(i_pHandler);
	}
	else
	{
		m_MessageHandlers[i_Message] = std::vector<MessageHandlerHandle>();
		m_MessageHandlers[i_Message].push_back(i_pHandler);
	}
}

bool eae6320::Core::MessageSystem::RemoveMessageHandler(const std::string i_Message, const MessageHandlerHandle& i_pHandler)
{
	std::unordered_map<std::string, std::vector<MessageHandlerHandle>>::iterator temp = m_MessageHandlers.find(i_Message);
	if (temp != m_MessageHandlers.end())
	{
		std::vector<MessageHandlerHandle>::iterator location = std::find(m_MessageHandlers[i_Message].begin(), m_MessageHandlers[i_Message].end(), i_pHandler);
		if (location == m_MessageHandlers[i_Message].end())
		{
			return false;
		}
		else
		{
			m_MessageHandlers[i_Message].erase(location);
			return true;
		}
	}
	else
	{
		return false;
	}
}

void eae6320::Core::MessageSystem::SendingMessage(const std::string i_Message, const Event& i_event)
{
	std::unordered_map<std::string, std::vector<MessageHandlerHandle>>::iterator temp = m_MessageHandlers.find(i_Message);
	if (temp != m_MessageHandlers.end())
	{
		std::vector<MessageHandlerHandle>& handlers = temp->second;
		for (std::vector<MessageHandlerHandle>::iterator it = handlers.begin(); it != handlers.end(); it++)
		{
			(*it).ToMessageHandler()->HandleMessage(i_Message, i_event);
		}
	}
}