// Header Files
//=============

#include "../EffectHelper.h"

#include "../../Windows/Includes.h"

#include <d3dx9shader.h>
#include <sstream>
#include "../../UserOutput/UserOutput.h"

// Helper Function Declarations
//=============================

namespace
{
	bool LoadVertexShader(eae6320::Graphics::Effect& i_effect, const char* const i_vertexPath, IDirect3DDevice9* i_device);
	bool LoadFragmentShader(eae6320::Graphics::Effect& i_effect, const char* const i_fragmentPath, IDirect3DDevice9* i_device);
}

bool eae6320::Graphics::EffectHelper::LoadEffectFromFile(Effect& i_effect, const char* const i_vertexPath, const char* const i_fragmentPath, const LoadEffectContext& i_loadEffectContext)
{
	if (!LoadVertexShader(i_effect, i_vertexPath, i_loadEffectContext.device))
	{
		return false;
	}
	if (!LoadFragmentShader(i_effect, i_fragmentPath, i_loadEffectContext.device))
	{
		return false;
	}
	return true;
}
bool eae6320::Graphics::EffectHelper::Bind(Effect& i_effect, const BindEffectContext& i_bindEffectContext)
{
	HRESULT result = i_bindEffectContext.device->SetVertexShader(i_effect.m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	result = i_bindEffectContext.device->SetPixelShader(i_effect.m_fragmentShader);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}
bool eae6320::Graphics::EffectHelper::CleanUp(Effect& i_mesh, const CleanUpEffectContext& i_cleanUpEffectContext)
{
	if (i_mesh.m_vertexShader)
	{
		i_mesh.m_vertexShader->Release();
		i_mesh.m_vertexShader = NULL;
	}
	if (i_mesh.m_fragmentShader)
	{
		i_mesh.m_fragmentShader->Release();
		i_mesh.m_fragmentShader = NULL;
	}
	return true;
}

namespace
{
	bool LoadVertexShader(eae6320::Graphics::Effect& i_effect, const char* const i_vertexPath, IDirect3DDevice9* i_device)
	{
		// Load the source code from file and compile it
		ID3DXBuffer* compiledShader;
		{
			const D3DXMACRO defines[] =
			{
				{ "EAE6320_PLATFORM_D3D", "1" },
				{ NULL, NULL }
			};
			ID3DXInclude* noIncludes = NULL;
			const char* entryPoint = "main";
			const char* profile = "vs_3_0";
			const DWORD noFlags = 0;
			ID3DXBuffer* errorMessages = NULL;
			ID3DXConstantTable** noConstants = NULL;
			HRESULT result = D3DXCompileShaderFromFile(i_vertexPath, defines, noIncludes, entryPoint, profile, noFlags,
				&compiledShader, &errorMessages, noConstants);
			if (SUCCEEDED(result))
			{
				if (errorMessages)
				{
					errorMessages->Release();
				}
			}
			else
			{
				if (errorMessages)
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the vertex shader from the file " << i_vertexPath
						<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
					eae6320::UserOutput::Print(errorMessage.str());
					errorMessages->Release();
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the vertex shader from the file " << i_vertexPath;
					eae6320::UserOutput::Print(errorMessage.str());
				}
				return false;
			}
		}
		// Create the vertex shader object
		bool wereThereErrors = false;
		{
			HRESULT result = i_device->CreateVertexShader(reinterpret_cast<DWORD*>(compiledShader->GetBufferPointer()),
				&i_effect.m_vertexShader);
			if (FAILED(result))
			{
				eae6320::UserOutput::Print("Direct3D failed to create the vertex shader");
				wereThereErrors = true;
			}
			compiledShader->Release();
		}
		return !wereThereErrors;
	}
	bool LoadFragmentShader(eae6320::Graphics::Effect& i_effect, const char* const i_fragmentPath, IDirect3DDevice9* i_device)
	{
		// Load the source code from file and compile it
		ID3DXBuffer* compiledShader;
		{
			const D3DXMACRO defines[] =
			{
				{ "EAE6320_PLATFORM_D3D", "1" },
				{ NULL, NULL }
			};
			ID3DXInclude* noIncludes = NULL;
			const char* entryPoint = "main";
			const char* profile = "ps_3_0";
			const DWORD noFlags = 0;
			ID3DXBuffer* errorMessages = NULL;
			ID3DXConstantTable** noConstants = NULL;
			HRESULT result = D3DXCompileShaderFromFile(i_fragmentPath, defines, noIncludes, entryPoint, profile, noFlags,
				&compiledShader, &errorMessages, noConstants);
			if (SUCCEEDED(result))
			{
				if (errorMessages)
				{
					errorMessages->Release();
				}
			}
			else
			{
				if (errorMessages)
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the fragment shader from the file " << i_fragmentPath
						<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
					eae6320::UserOutput::Print(errorMessage.str());
					errorMessages->Release();
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the fragment shader from the file " << i_fragmentPath;
					eae6320::UserOutput::Print(errorMessage.str());
				}
				return false;
			}
		}
		// Create the fragment shader object
		bool wereThereErrors = false;
		{
			HRESULT result = i_device->CreatePixelShader(reinterpret_cast<DWORD*>(compiledShader->GetBufferPointer()),
				&i_effect.m_fragmentShader);
			if (FAILED(result))
			{
				eae6320::UserOutput::Print("Direct3D failed to create the fragment shader");
				wereThereErrors = true;
			}
			compiledShader->Release();
		}
		return !wereThereErrors;
	}
}