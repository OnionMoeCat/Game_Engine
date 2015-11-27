// Header Files
//=============

#include "TextureHelper.h"

// Interface
//==========

bool eae6320::Graphics::TextureHelper::LoadTextureFromFile(Texture& i_texture, const char* const i_texturePath)
{
	bool wereThereErrors = false;

	if (!LoadTextureFromFile(i_texture, i_texturePath, Context::Get()))
	{
		wereThereErrors = true;
		goto OnExit;
	}

OnExit:

	return !wereThereErrors;
}