#ifndef MESSAGESYSTEM_H
#define MESSAGESYSTEM_H

#include "Event.h"
#include "IMessageHandler.h"
#include "MessageHandlerHandle.h"

#include <unordered_map>

namespace eae6320
{
	namespace Core
	{
		struct MessageSystem
		{
		public:
			void AddMessageHandler(const std::string i_Message, const MessageHandlerHandle& i_pHandler);
			bool RemoveMessageHandler(const std::string i_Message, const MessageHandlerHandle& i_pHandler);
			void SendingMessage(const std::string i_Message, const Event& i_event);
			static MessageSystem& Get();
		private:
			std::unordered_map<std::string, std::vector<MessageHandlerHandle>> 	m_MessageHandlers;
			static MessageSystem m_Instance;
		};
	}
}
#endif