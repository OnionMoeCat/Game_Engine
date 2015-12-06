#ifndef EAE6320_CONSTANTCONTROLLER_H
#define EAE6320_CONSTANTCONTROLLER_H

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
		struct ConstantController : eae6320::Core::IController
		{
		private:
			eae6320::Math::cVector m_velocity;
		public:
			ConstantController(eae6320::Math::cVector i_velocity);
			void UpdateEntity(eae6320::Core::Entity& i_entity, float dt);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
