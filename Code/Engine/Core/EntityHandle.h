#ifndef EAE6320_ENTITYHANDLE_H
#define EAE6320_ENTITYHANDLE_H

#include "Entity.h"

#include <cstdint>

namespace eae6320
{
	namespace Core
	{
		struct EntityHandle
		{
		private:
			int32_t m_handleIndex = -1; // index into the handle
							  // table
			uint32_t m_uniqueId = 0; // unique id avoids stale
									 // handles
		public:
			explicit EntityHandle(const Entity& entity);
			EntityHandle();
			// This function dereferences the handle.
			Entity* ToEntity() const;
			bool operator==(const EntityHandle& rhs);
			bool operator!=(const EntityHandle& rhs);
			const static EntityHandle Null;
		};
	}
}

#endif