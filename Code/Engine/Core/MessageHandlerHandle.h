#ifndef EAE6320_MESSAGEHANDLERHANDLE_H
#define EAE6320_MESSAGEHANDLERHANDLE_H

#include "IMessageHandler.h"

#include <cstdint>

namespace eae6320
{
	namespace Core
	{
		struct MessageHandlerHandle
		{
		private:
			uint32_t m_handleIndex; // index into the handle
									// table
			uint32_t m_uniqueId; // unique id avoids stale
								 // handles
		public:
			explicit MessageHandlerHandle(const IMessageHandler& entity);
			MessageHandlerHandle();
			// This function dereferences the handle.
			IMessageHandler* ToMessageHandler() const;
			bool operator==(const MessageHandlerHandle& rhs);
			bool operator!=(const MessageHandlerHandle& rhs);
		};
	}
}

#endif