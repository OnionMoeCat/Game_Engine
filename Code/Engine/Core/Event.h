#ifndef EAE6320_EVENT_H
#define EAE6320_EVENT_H

#include "Variant.h"

#include <unordered_map>

namespace eae6320
{
	namespace Core
	{
		struct Event
		{
			enum EventType
			{
				TYPE_COUNT // number of unique types
			};
			EventType m_type;
			std::unordered_map<std::string, Variant> m_args;
		};
	}
}

#endif