// Header Files
//=============

#include "MeshHelper.h"

#include "../UserOutput/UserOutput.h"
#include <assert.h>
#include <sstream>
#include "../Windows/Functions.h"

// Helper Function Declarations
//=============================

namespace
{
	bool ReadDataFromFile(const char* const i_path, void** i_temporaryBuffer);
}

// Interface
//==========

bool eae6320::Graphics::MeshHelper::LoadMeshFromFile(Mesh& i_mesh, const char* const i_path, const Context& i_context)
{
	bool wereThereErrors = false;

	sVertex* vertices = NULL;
	uint32_t* indices = NULL;
	uint32_t vertexSize = 0;
	uint32_t primitiveSize = 0;

	void* temporaryBuffer = NULL;
	{
		if (ReadDataFromFile(i_path, &temporaryBuffer))
		{
			char* tempPTR = reinterpret_cast<char*>(temporaryBuffer);
			vertexSize = *reinterpret_cast<uint32_t*>(tempPTR);
			tempPTR += sizeof(uint32_t);
			primitiveSize = *reinterpret_cast<uint32_t*>(tempPTR);
			tempPTR += sizeof(uint32_t);
			vertices = reinterpret_cast<sVertex*>(tempPTR);
			tempPTR += sizeof(sVertex) * vertexSize;
			indices = reinterpret_cast<uint32_t*>(tempPTR);
		}
		else
		{
			wereThereErrors = true;
			goto OnExit;
		}
	}

	{
		if (!eae6320::Graphics::MeshHelper::SetVertexBuffer(i_mesh, vertices, vertexSize, i_context))
		{
			wereThereErrors = true;
			goto OnExit;
		}
	}

	{
		if (!eae6320::Graphics::MeshHelper::SetIndexBuffer(i_mesh, indices, primitiveSize, i_context))
		{
			wereThereErrors = true;
			goto OnExit;
		}
	}

OnExit:

	vertices = NULL;
	vertexSize = 0;
	indices = NULL;
	primitiveSize = 0;

	if (temporaryBuffer)
	{
		free(temporaryBuffer);
	}
	temporaryBuffer = NULL;

	return !wereThereErrors;
}

bool eae6320::Graphics::MeshHelper::CleanUp(Mesh& i_mesh)
{
	return CleanUp(i_mesh, Context::Get());
}

bool eae6320::Graphics::MeshHelper::LoadMeshFromFile(Mesh& i_mesh, const char* const i_path)
{
	return LoadMeshFromFile(i_mesh, i_path, Context::Get());
}

// Helper Function Definitions
//============================

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