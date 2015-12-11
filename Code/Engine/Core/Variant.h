#ifndef EAE6320_VARIANT_H
#define EAE6320_VARIANT_H

#include <cstdint>

namespace eae6320
{
	namespace Core
	{
		struct Variant
		{
			enum Type
			{
				TYPE_INTEGER,
				TYPE_UINTEGER,
				TYPE_FLOAT,
				TYPE_COUNT // number of unique types
			};
			Type m_type;

			union
			{
				int32_t m_asInteger;
				uint32_t m_asUInteger;
				float m_asFloat;
				bool m_asBool;
			};
		};
	}
}

#endif