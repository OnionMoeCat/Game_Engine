#ifndef EAE6320_COLLIDABLE_H
#define EAE6320_COLLIDABLE_H

#include "EntityHandle.h"

namespace eae6320
{
	namespace Core
	{
		struct Collidable
		{
		private:
			EntityHandle m_colliding;			
		public:
			void SetColliding(EntityHandle i_pCollidable);
			EntityHandle GetColliding();
		};
	}
}
#endif