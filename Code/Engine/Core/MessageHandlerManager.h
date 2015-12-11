/*
This file contains the function declarations for mesh
*/

#ifndef EAE6320_MESSAGE_ENTITY_MANAGER_H
#define EAE6320_MESSAGE_ENTITY_MANAGER_H

// Header Files
//=============
#include "IMessageHandler.h"
#include "MessageHandlerHandle.h"

#include <vector>

// Interface
//==========

namespace eae6320
{
	namespace Core
	{
		struct MessageHandlerManager
		{
		public:
			static MessageHandlerManager& Get();
			void CleanUp();
			friend struct MessageHandlerHandle;
		private:
			static uint32_t g_UniqueID;
			typedef std::vector<IMessageHandler*> MessageHandlerList;
			MessageHandlerList m_list;
			static MessageHandlerManager m_Instance;
			MessageHandlerManager();
			MessageHandlerHandle InsertAndGetIndex(IMessageHandler* i_entity);
		};
	}
}

#endif	// EAE6320_MESH_H
