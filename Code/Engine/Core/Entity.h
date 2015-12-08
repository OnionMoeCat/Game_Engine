#ifndef EAE6320_ENTITY_H
#define EAE6320_ENTITY_H

// Header Files
//=============

#include "Transform.h"
#include "IController.h"
#include "../Graphics/Renderable.h"

namespace eae6320
{
	namespace Core
	{
		// forward declaration
		struct Collidable;

		struct Entity
		{
			Collidable* m_collidable = NULL;
			Transform* m_transform = NULL;
			IController* m_iController = NULL;
			eae6320::Graphics::Renderable* m_renderable = NULL;
			uint32_t m_handleIndex;
			uint32_t m_uniqueId;
		};
	}
}

#endif