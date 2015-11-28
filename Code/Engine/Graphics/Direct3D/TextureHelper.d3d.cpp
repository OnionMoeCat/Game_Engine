// Header Files
//=============

#include "../TextureHelper.h"

#include "../../Windows/Includes.h"
#include "../../UserOutput/UserOutput.h"
#include "../EffectHelper.h"

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

bool eae6320::Graphics::TextureHelper::GetSampleID(Texture& i_texture, const Effect& i_effect, const char* const i_textureName, const ShaderTypes::eShaderType i_shaderType)
{
	tUniformHandle tempHandle;
	{
		if (!EffectHelper::GetUniformHandler(i_effect, i_textureName, i_shaderType, &tempHandle))
		{
			return false;
		}
		
		if (i_shaderType == ShaderTypes::eShaderType::Vertex)
		{
			i_texture.samplerID = static_cast<DWORD>(i_effect.m_vertexShaderConstantTable->GetSamplerIndex(tempHandle));
		}
		else if (i_shaderType == ShaderTypes::eShaderType::Fragment)
		{
			i_texture.samplerID = static_cast<DWORD>(i_effect.m_fragmentShaderConstantTable->GetSamplerIndex(tempHandle));;
		}
		else
		{
			return false;
		}		
	}
	return true;
}

bool eae6320::Graphics::TextureHelper::SetTexture(Texture& i_texture, const Context& i_context)
{
	const HRESULT result = i_context.device->SetTexture(i_texture.samplerID, i_texture.textureData);
	return SUCCEEDED(result);
}

bool eae6320::Graphics::TextureHelper::CleanUp(Texture& i_texture)
{
	i_texture.textureData->Release();
	return true;
}