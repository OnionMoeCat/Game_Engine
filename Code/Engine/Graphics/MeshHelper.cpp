// Header Files
//=============

#include "MeshHelper.h"

#include "../UserOutput/UserOutput.h"
#include "../Utils/FileUtils.h"

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
		if (eae6320::Utils::FileUtils::ReadDataFromFile(i_path, &temporaryBuffer))
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