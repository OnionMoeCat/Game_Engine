#ifndef EAE6320_INPUTCONTROLLER_H
#define EAE6320_INPUTCONTROLLER_H

// Header Files
//=============
#include "../../../Engine/Math/cVector.h"
#include "../../../Engine/Core/IController.h"

#include <cstdint>

// Interface
//==========

namespace eae6320
{
	namespace Game
	{
		struct InputController : eae6320::Core::IController
		{
		private:
			float m_displacement;
			static const unsigned int NumBits = 256 / (sizeof(uint32_t) * 8); // num bytes * 8 bits / byte
			uint32_t m_Bits[NumBits];
			eae6320::Math::cVector m_leftBottom;
			eae6320::Math::cVector m_rightTop;
		public:
			InputController(const float i_velocity, const eae6320::Math::cVector i_boundaryLeftBottom, const eae6320::Math::cVector i_boundaryRightTop);
			void UpdateEntity(eae6320::Core::Entity& i_entity, float dt);
			bool GetKeyPressed(const unsigned int i_keyCode);
			void SetKeyPressed(const unsigned int i_keyCode, bool i_pressed);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
