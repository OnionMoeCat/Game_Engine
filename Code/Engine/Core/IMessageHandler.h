#ifndef EAE6320_IMESSAGEHANDLER_H
#define EAE6320_IMESSAGEHANDLER_H

#include "Event.h"

#include <string>

namespace eae6320
{
	namespace Core
	{
		struct IMessageHandler
		{
			uint32_t m_handleIndex; // index into the handle
									// table
			uint32_t m_uniqueId; // unique id avoids stale
								 // handles
			virtual ~IMessageHandler();
			virtual void HandleMessage(const std::string i_Message, const Event& i_event) = 0;
		};
	}
}
#endif