#ifndef EAE6320_ENTITY_H
#define EAE6320_ENTITY_H

// Header Files
//=============

#include "../Graphics/Renderable.h"

namespace eae6320
{
	namespace Core
	{
		struct Entity
		{
			eae6320::Graphics::Renderable* m_renderable = NULL;
			eae6320::Math::cVector m_position;
			eae6320::Math::cQuaternion m_rotation;
			uint32_t m_handleIndex;
			uint32_t m_uniqueId;
		};
	}
}

#endif