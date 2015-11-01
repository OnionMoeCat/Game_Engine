// Header Files
//=============

#include "../EffectHelper.h"

#include "../../Windows/Includes.h"

#include "../../UserOutput/UserOutput.h"
#include <assert.h>
#include <sstream>
#include "../../Windows/Functions.h"

// Helper Function Declarations
//=============================

namespace
{
	bool LoadVertexShader(eae6320::Graphics::Effect& i_effect, const char* const i_vertexPath, IDirect3DDevice9* i_device);
	bool LoadFragmentShader(eae6320::Graphics::Effect& i_effect, const char* const i_fragmentPath, IDirect3DDevice9* i_device);
	bool ReadDataFromFile(const char* const i_path, void** i_temporaryBuffer);
}

bool eae6320::Graphics::EffectHelper::LoadEffectFromFile(Effect& i_effect, const char* const i_vertexPath, const char* const i_fragmentPath, const Context& i_context)
{
	if (i_context.device == NULL)
	{
		return false;
	}
	if (!LoadVertexShader(i_effect, i_vertexPath, i_context.device))
	{
		return false;
	}
	if (!LoadFragmentShader(i_effect, i_fragmentPath, i_context.device))
	{
		return false;
	}
	return true;
}
bool eae6320::Graphics::EffectHelper::Bind(Effect& i_effect, const Context& i_context)
{
	if (i_context.device == NULL)
	{
		return false;
	}
	HRESULT result = i_context.device->SetVertexShader(i_effect.m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	result = i_context.device->SetPixelShader(i_effect.m_fragmentShader);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}
bool eae6320::Graphics::EffectHelper::CleanUp(Effect& i_effect, const Context& i_context)
{
	if (i_effect.m_vertexShader)
	{
		i_effect.m_vertexShader->Release();
		i_effect.m_vertexShader = NULL;
	}
	if (i_effect.m_fragmentShader)
	{
		i_effect.m_fragmentShader->Release();
		i_effect.m_fragmentShader = NULL;
	}
	return true;
}
bool eae6320::Graphics::EffectHelper::SetDrawCallUniforms(Effect& i_effect, const eae6320::Math::cVector& i_vector, const Context& i_Context)
{
	if (i_effect.m_vertexShaderConstantTable != NULL)
	{
		D3DXHANDLE handle = i_effect.m_vertexShaderConstantTable->GetConstantByName(NULL, "g_position_offset");
		if (handle != NULL)
		{
			const unsigned int floatCount = 2;
			const float floatArray[floatCount] = { i_vector.x, i_vector.y };
			if (i_Context.device == NULL)
			{
				return false;
			}
			HRESULT result = i_effect.m_vertexShaderConstantTable->SetFloatArray(i_Context.device, handle, floatArray, floatCount);
			if (FAILED(result))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}

namespace
{
	bool LoadVertexShader(eae6320::Graphics::Effect& i_effect, const char* const i_vertexPath, IDirect3DDevice9* i_device)
	{
		bool wereThereErrors = false;
		// Load binary vertex shader file
		void* temporaryBuffer = NULL;
		{
			if (!ReadDataFromFile(i_vertexPath, &temporaryBuffer))
			{
				wereThereErrors = true;
				goto OnExit;
			}
		}
		// Create the vertex shader object
		{
			if (i_device != NULL)
			{
				HRESULT result = i_device->CreateVertexShader(reinterpret_cast<DWORD*>(temporaryBuffer),
					&i_effect.m_vertexShader);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create the vertex shader");
					wereThereErrors = true;
					goto OnExit;
				}

				ID3DXConstantTable* vertexShaderConstantTable = NULL;
				result = D3DXGetShaderConstantTable(reinterpret_cast<const DWORD*>(temporaryBuffer), &vertexShaderConstantTable);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to load constant table of vertex shader");
					wereThereErrors = true;
					goto OnExit;
				}
				else
				{
					i_effect.m_vertexShaderConstantTable = vertexShaderConstantTable;
				}
			}
			else
			{
				wereThereErrors = true;
				goto OnExit;
			}
		}

	OnExit:

		if (temporaryBuffer != NULL)
		{
			free(temporaryBuffer);
		}
		return !wereThereErrors;
	}
	bool LoadFragmentShader(eae6320::Graphics::Effect& i_effect, const char* const i_fragmentPath, IDirect3DDevice9* i_device)
	{
		bool wereThereErrors = false;
		// Load binary fragment shader file
		void* temporaryBuffer = NULL;
		{
			if (!ReadDataFromFile(i_fragmentPath, &temporaryBuffer))
			{
				wereThereErrors = true;
				goto OnExit;
			}
		}
		// Create the fragment shader object
		{
			if (i_device != NULL)
			{
				HRESULT result = i_device->CreatePixelShader(reinterpret_cast<DWORD*>(temporaryBuffer),
					&i_effect.m_fragmentShader);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create the fragment shader");
					wereThereErrors = true;
				}
			}
			else
			{
				wereThereErrors = true;
			}
		}

	OnExit:

		if (temporaryBuffer != NULL)
		{
			free(temporaryBuffer);
		}
		return !wereThereErrors;
	}
}

namespace
{
	bool ReadDataFromFile(const char* const i_path, void** i_temporaryBuffer)
	{
		bool wereThereErrors = false;

		size_t size = 0;
		HANDLE fileHandle = INVALID_HANDLE_VALUE;
		void* temporaryBuffer = NULL;

		// Open the file
		{
			const DWORD desiredAccess = FILE_GENERIC_READ;
			const DWORD otherProgramsCanStillReadTheFile = FILE_SHARE_READ;
			SECURITY_ATTRIBUTES* useDefaultSecurity = NULL;
			const DWORD onlySucceedIfFileExists = OPEN_EXISTING;
			const DWORD useDefaultAttributes = FILE_ATTRIBUTE_NORMAL;
			const HANDLE dontUseTemplateFile = NULL;
			fileHandle = CreateFile(i_path, desiredAccess, otherProgramsCanStillReadTheFile,
				useDefaultSecurity, onlySucceedIfFileExists, useDefaultAttributes, dontUseTemplateFile);
			if (fileHandle == INVALID_HANDLE_VALUE)
			{
				wereThereErrors = true;
				std::string windowsErrorMessage = eae6320::GetLastWindowsError();
				std::stringstream errorMessage;
				errorMessage << "Windows failed to open file \"" << i_path << "\": " << windowsErrorMessage;
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
		}

		// Get the file's size
		{
			LARGE_INTEGER fileSize_integer;
			if (GetFileSizeEx(fileHandle, &fileSize_integer) != FALSE)
			{
				assert(fileSize_integer.QuadPart <= SIZE_MAX);
				size = static_cast<size_t>(fileSize_integer.QuadPart);
			}
			else
			{
				wereThereErrors = true;
				std::string windowsErrorMessage = eae6320::GetLastWindowsError();
				std::stringstream errorMessage;
				errorMessage << "Windows failed to get the size of file \"" << i_path << "\": " << windowsErrorMessage;
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
		}

		// Read the file's contents into temporary memory and assign them to variables
		{
			temporaryBuffer = malloc(size);

			if (temporaryBuffer)
			{
				DWORD bytesReadCount;
				OVERLAPPED* readSynchronously = NULL;
				if (ReadFile(fileHandle, temporaryBuffer, static_cast<DWORD>(size),
					&bytesReadCount, readSynchronously) == FALSE)
				{
					wereThereErrors = true;
					std::string windowsErrorMessage = eae6320::GetLastWindowsError();
					std::stringstream errorMessage;
					errorMessage << "Windows failed to read the contents of file \"" << i_path << "\": " << windowsErrorMessage;
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
				*i_temporaryBuffer = temporaryBuffer;
			}
			else
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "Failed to allocate " << size << " bytes to read file \"" << i_path << "\"";
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
		}

	OnExit:

		if (fileHandle != INVALID_HANDLE_VALUE)
		{
			if (CloseHandle(fileHandle) == FALSE)
			{
				wereThereErrors = true;
				std::string windowsError = eae6320::GetLastWindowsError();
				std::stringstream errorMessage;
				errorMessage << "Windows failed to close file \"" << i_path << "\": " << windowsError;
				eae6320::UserOutput::Print(errorMessage.str());
			}
			fileHandle = INVALID_HANDLE_VALUE;
		}

		temporaryBuffer = NULL;

		return !wereThereErrors;
	}
}