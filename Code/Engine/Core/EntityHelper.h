#ifndef EAE6320_ENTITY_HELPER_H
#define EAE6320_ENTITY_HELPER_H

// Header Files
//=============

#include "Entity.h"
#include "Camera.h"

namespace eae6320
{
	namespace Core
	{
		struct EntityHelper
		{
			bool static LoadEntityFromFile(Entity& i_entity, const char* const i_materialPath, const char* i_meshPath);
			bool static OffsetPosition(Entity& i_entity, const eae6320::Math::cVector& i_offset_position);
			bool static CleanUp(Entity& i_entity);
			bool static ToCameraScreen(Entity& i_entity, const Camera& i_camera);
			bool static Submit(Entity& i_entity);
		};
	}
}

#endif
