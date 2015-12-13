#ifndef EAE6320_ENTITY_HELPER_H
#define EAE6320_ENTITY_HELPER_H

// Header Files
//=============

#include "Entity.h"
#include "ICamera.h"

namespace eae6320
{
	namespace Core
	{
		struct EntityHelper
		{
			bool static LoadEntityFromFile(Entity& i_entity, const char* const i_materialPath, const char* i_meshPath);
			bool static SetTransform(Entity& i_entity, const eae6320::Math::cVector& i_position, const eae6320::Math::cQuaternion& i_quaternion, const eae6320::Math::cVector& i_AABB);
			bool static SetController(Entity& i_entity, IController* i_controller);
			bool static SetCollidable(Entity& i_entity, const float i_mass);
			bool static SetName(Entity& i_entity, const char* const i_name);
			bool static SetAlive(Entity& i_entity, const bool i_alive);
			bool static CleanUp(Entity& i_entity);
			bool static ToCameraScreen(Entity& i_entity, const ICamera& i_camera);
			bool static Submit(Entity& i_entity);
		};
	}
}

#endif
