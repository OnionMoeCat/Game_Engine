// Header Files
//=============

#include "MessageHandlerHandle.h"
#include "MessageHandlerManager.h"

eae6320::Core::MessageHandlerHandle::MessageHandlerHandle()
{

}

eae6320::Core::MessageHandlerHandle::MessageHandlerHandle(const IMessageHandler& entity) :
	m_handleIndex(entity.m_handleIndex),
	m_uniqueId(entity.m_uniqueId)
{

}
// This function dereferences the handle.
eae6320::Core::IMessageHandler* eae6320::Core::MessageHandlerHandle::ToMessageHandler() const
{
	IMessageHandler* pMessageHandler = MessageHandlerManager::Get().m_list[m_handleIndex];
	if (pMessageHandler != NULL
		&& pMessageHandler->m_uniqueId == m_uniqueId)
	{
		return pMessageHandler;
	}
	return NULL;
}

bool eae6320::Core::MessageHandlerHandle::operator==(const MessageHandlerHandle& rhs) const
{
	return (this->m_uniqueId == rhs.m_uniqueId && this->m_handleIndex == rhs.m_handleIndex);
}

bool eae6320::Core::MessageHandlerHandle::operator!=(const MessageHandlerHandle& rhs) const
{
	return !(*this == rhs);
}