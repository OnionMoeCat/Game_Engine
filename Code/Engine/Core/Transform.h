#ifndef EAE6320_TRANSFORM_H
#define EAE6320_TRANSFORM_H

// Header Files
//=============

#include "../Graphics/Renderable.h"

namespace eae6320
{
	namespace Core
	{
		struct Transform
		{
			eae6320::Math::cVector m_velocity;
			eae6320::Math::cVector m_prev_velocity;
			eae6320::Math::cVector m_position;
			eae6320::Math::cVector m_AABB;
			eae6320::Math::cQuaternion m_rotation;
		};
	}
}

#endif#pragma once
