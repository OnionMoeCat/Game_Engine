#ifndef MESSAGESYSTEM_H
#define MESSAGESYSTEM_H

#include "Event.h"

#include <unordered_map>

namespace eae6320
{
	namespace Core
	{
		struct MessageSystem
		{
		public:
			struct IMessageHandler
			{
			public:
				virtual ~IMessageHandler();
				virtual void HandleMessage(const std::string i_Message, const Event& i_event) = 0;;
			};
			void AddMessageHandler(const std::string i_Message, IMessageHandler* i_pHandler);
			bool RemoveMessageHandler(const std::string i_Message, IMessageHandler* i_pHandler);
			void SendingMessage(const std::string i_Message, const Event& i_event);
			static MessageSystem& Get();
			void CleanUp();
		private:
			std::unordered_map<std::string, std::vector<IMessageHandler*>> 	m_MessageHandlers;
			static MessageSystem m_Instance;
		};
	}
}
#endif