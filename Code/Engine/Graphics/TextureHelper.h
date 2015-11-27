/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_TEXTUREHELPER_H
#define EAE6320_TEXTUREHELPER_H

// Header Files
//=============
#include "Texture.h"
#include "Context.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct TextureHelper
		{
		private:
			bool static LoadTextureFromFile(Texture& i_texture, const char* const i_texturePath, const Context& i_context);
		public:
			bool static LoadTextureFromFile(Texture& i_texture, const char* const i_texturePath);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
