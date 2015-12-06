#ifndef EAE6320_ICONTROLLER_H
#define EAE6320_ICONTROLLER_H

// Header Files
//=============


namespace eae6320
{
	namespace Core
	{
		struct Entity;

		struct IController
		{
		public:
			virtual void UpdateEntity(Entity& i_entity, float i_dt) = 0;
			virtual ~IController() {}
		};
	}
}

#endif#pragma once
