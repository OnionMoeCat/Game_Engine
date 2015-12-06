#ifndef EAE6320_INPUTCONTROLLER_H
#define EAE6320_INPUTCONTROLLER_H

// Header Files
//=============
#include "../../../Engine/Math/cVector.h"
#include "../../../Engine/Core/IController.h"

// Interface
//==========

namespace eae6320
{
	namespace Game
	{
		struct InputController : eae6320::Core::IController
		{
		private:
			float m_velocityMag;
		public:
			InputController(const float i_velocity);
			void UpdateEntity(eae6320::Core::Entity& i_entity, float dt);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
