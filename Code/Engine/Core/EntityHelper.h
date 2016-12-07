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
			eae6320::Math::cMatrix_transformation static CreateLocalToWorldTransform(const eae6320::Math::cQuaternion& i_rotation, const eae6320::Math::cVector& i_position, const eae6320::Math::cVector& i_scale);
			bool static LoadEntityFromFile(Entity& i_entity, const char* const i_materialPath, const char* i_meshPath);
			void static Scale(Entity& i_entity, const eae6320::Math::cVector& i_scale);
			bool static OffsetTransform(Entity& i_entity, const eae6320::Math::cVector& i_offset_position, const eae6320::Math::cQuaternion& i_offset_rotation);
			bool static CleanUp(Entity& i_entity);
			bool static ToCameraScreen(Entity& i_entity, const Camera& i_camera);
			bool static AddLights(Entity& i_entity, eae6320::Math::cVector& i_light_position);
			bool static Submit(Entity& i_entity);
		};
	}
}

#endif
