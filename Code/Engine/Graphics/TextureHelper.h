/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_TEXTUREHELPER_H
#define EAE6320_TEXTUREHELPER_H

// Header Files
//=============
#include "Includes.h"
#include "Texture.h"
#include "Effect.h"
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
			bool static CleanUp(Texture& i_texture);
			bool static LoadTextureFromFile(Texture& i_texture, const char* const i_texturePath);
			bool static GetSampleID(Texture& i_texture, const Effect& i_effect, const char* const i_textureName, const ShaderTypes::eShaderType shaderType);
			bool static SetTexture(Texture& i_texture, 
#ifdef EAE6320_GRAPHICS_ISTEXTUREINDEXREQUIREDTOSETTEXTURE
				const int i_textureUnit,
#endif
				const Context& i_context);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
