// Header Files
//=============

#include "../TextureHelper.h"

#include "../../Windows/Includes.h"
#include "../../UserOutput/UserOutput.h"

#include <sstream>
#include <d3d9.h>
#include <d3dx9.h>

// Helper Function Declarations
//=============================

bool eae6320::Graphics::TextureHelper::LoadTextureFromFile(Texture& i_texture, const char* const i_texturePath, const Context& i_context)
{
	{
		const unsigned int useDimensionsFromFile = D3DX_DEFAULT_NONPOW2;
		const unsigned int useMipMapsFromFile = D3DX_FROM_FILE;
		const DWORD staticTexture = 0;
		const D3DFORMAT useFormatFromFile = D3DFMT_FROM_FILE;
		const D3DPOOL letD3dManageMemory = D3DPOOL_MANAGED;
		const DWORD useDefaultFiltering = D3DX_DEFAULT;
		const D3DCOLOR noColorKey = 0;
		D3DXIMAGE_INFO* noSourceInfo = NULL;
		PALETTEENTRY* noColorPalette = NULL;
		const HRESULT result = D3DXCreateTextureFromFileEx(i_context.device, i_texturePath, useDimensionsFromFile, useDimensionsFromFile, useMipMapsFromFile,
			staticTexture, useFormatFromFile, letD3dManageMemory, useDefaultFiltering, useDefaultFiltering, noColorKey, noSourceInfo, noColorPalette,
			&i_texture.textureData);
		if (FAILED(result))
		{
			std::stringstream errorMessage;
			errorMessage << "Direct3D failed to load texture from file path: " <<
				i_texturePath;
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
	}
	return true;
}