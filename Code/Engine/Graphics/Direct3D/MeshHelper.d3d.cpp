// Header Files
//=============

#include "../MeshHelper.h"

#include "../../Windows/Includes.h"

#include <d3d9.h>
#include "../../UserOutput/UserOutput.h"

// Helper Function Declarations
//=============================

namespace
{
	HRESULT GetVertexProcessingUsage(DWORD& o_usage, IDirect3DDevice9* i_device);
}

// Interface
//==========

bool eae6320::Graphics::MeshHelper::DrawMesh(const Mesh& i_mesh, const RenderContext& i_rendercontext)
{
	// We are using triangles as the "primitive" type,
	// and we have defined the vertex buffer as a triangle list
	// (meaning that every triangle is defined by three vertices)
	const D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLELIST;
	// It's possible to start rendering primitives in the middle of the stream
	const unsigned int indexOfFirstVertexToRender = 0;
	const unsigned int indexOfFirstIndexToUse = 0;
	// We are drawing a square
	const unsigned int vertexCountToRender = i_mesh.m_vertexCount;	// How vertices from the vertex buffer will be used?
	const unsigned int primitiveCountToRender = i_mesh.m_primitiveCount;	// How many triangles will be drawn?
	HRESULT result = i_rendercontext.device->DrawIndexedPrimitive(primitiveType,
		indexOfFirstVertexToRender, indexOfFirstVertexToRender, vertexCountToRender,
		indexOfFirstIndexToUse, primitiveCountToRender);
	return (SUCCEEDED(result));
}

bool eae6320::Graphics::MeshHelper::SetVertexBuffer(Mesh& i_mesh, const BufferDataPtr& i_vertexBufferData, const SetVertexBufferContext& i_setVertexBufferContext)
{
	// The usage tells Direct3D how this vertex buffer will be used
	DWORD usage = 0;
	{
		// The type of vertex processing should match what was specified when the device interface was created with CreateDevice()
		const HRESULT result = GetVertexProcessingUsage(usage, i_setVertexBufferContext.device);
		if (FAILED(result))
		{
			return false;
		}
		// Our code will only ever write to the buffer
		usage |= D3DUSAGE_WRITEONLY;
	}

	unsigned int bufferSize;
	// Create a vertex buffer
	{
		// Assign number of vertices which we get from load mesh file
		i_mesh.m_vertexCount = i_setVertexBufferContext.vertexCount;
		// Calculate buffersize by multiply count with size
		bufferSize = i_mesh.m_vertexCount * sizeof(sVertex);
		// We will define our own vertex format
		const DWORD useSeparateVertexDeclaration = 0;
		// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
		const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
		HANDLE* const notUsed = NULL;
		const HRESULT result = i_setVertexBufferContext.device->CreateVertexBuffer(bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,
			&i_mesh.m_vertexBuffer, notUsed);
		if (FAILED(result))
		{
			eae6320::UserOutput::Print("Direct3D failed to create a vertex buffer");
			return false;
		}
	}
	
	// Fill the vertex buffer with the triangle's vertices
	{
		// Before the vertex buffer can be changed it must be "locked"
		sVertex* vertexData;
		{
			const unsigned int lockEntireBuffer = 0;
			const DWORD useDefaultLockingBehavior = 0;
			const HRESULT result = i_mesh.m_vertexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
				reinterpret_cast<void**>(&vertexData), useDefaultLockingBehavior);
			if (FAILED(result))
			{
				eae6320::UserOutput::Print("Direct3D failed to lock the vertex buffer");
				return false;
			}
		}
		// Fill the buffer
		{
			memcpy(vertexData, i_vertexBufferData, bufferSize);
		}
		// The buffer must be "unlocked" before it can be used
		{
			const HRESULT result = i_mesh.m_vertexBuffer->Unlock();
			if (FAILED(result))
			{
				eae6320::UserOutput::Print("Direct3D failed to unlock the vertex buffer");
				return false;
			}
		}
	}

	return true;
}

bool eae6320::Graphics::MeshHelper::SetIndexBuffer(Mesh& i_mesh, const BufferDataPtr& i_indexBufferData, const SetIndexBufferContext& i_setIndexBufferContext)
{
	// The usage tells Direct3D how this vertex buffer will be used
	DWORD usage = 0;
	{
		// The type of vertex processing should match what was specified when the device interface was created with CreateDevice()
		const HRESULT result = GetVertexProcessingUsage(usage, i_setIndexBufferContext.device);
		if (FAILED(result))
		{
			return false;
		}
		// Our code will only ever write to the buffer
		usage |= D3DUSAGE_WRITEONLY;
	}

	// Create an index buffer
	unsigned int bufferSize;
	{
		// Assign number of primitives which we get from load mesh file
		i_mesh.m_primitiveCount = i_setIndexBufferContext.primitiveCount;
		const unsigned int vertexPerPrimitive = 3;
		// Calculate buffersize by multiply primitivecount with number of vertex per primitive with size
		bufferSize = i_mesh.m_primitiveCount * vertexPerPrimitive * sizeof(uint32_t);
		// We'll use 32-bit indices in this class to keep things simple
		// (i.e. every index will be a 32 bit unsigned integer)
		const D3DFORMAT format = D3DFMT_INDEX32;
		// Place the index buffer into memory that Direct3D thinks is the most appropriate
		const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
		HANDLE* notUsed = NULL;
		const HRESULT result = i_setIndexBufferContext.device->CreateIndexBuffer(bufferSize, usage, format, useDefaultPool,
			&i_mesh.m_indexBuffer, notUsed);
		if (FAILED(result))
		{
			eae6320::UserOutput::Print("Direct3D failed to create an index buffer");
			return false;
		}
	}
	// Fill the index buffer with the triangles' connectivity data
	{
		// Before the index buffer can be changed it must be "locked"
		uint32_t* indexData;
		{
			const unsigned int lockEntireBuffer = 0;
			const DWORD useDefaultLockingBehavior = 0;
			const HRESULT result = i_mesh.m_indexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
				reinterpret_cast<void**>(&indexData), useDefaultLockingBehavior);
			if (FAILED(result))
			{
				eae6320::UserOutput::Print("Direct3D failed to lock the index buffer");
				return false;
			}
		}
		// Fill the buffer
		{
			memcpy(indexData, i_indexBufferData, bufferSize);
		}
		// The buffer must be "unlocked" before it can be used
		{
			const HRESULT result = i_mesh.m_indexBuffer->Unlock();
			if (FAILED(result))
			{
				eae6320::UserOutput::Print("Direct3D failed to unlock the index buffer");
				return false;
			}
		}
	}

	return true;
}

bool eae6320::Graphics::MeshHelper::SetVertexDeclaration(Mesh& i_mesh, const VertexDeclarationSpec* i_vertexDeclarationSpec, const VertexDeclarationContext& i_vertexDeclarationContext)
{
	HRESULT result = i_vertexDeclarationContext.device->CreateVertexDeclaration(i_vertexDeclarationSpec, &i_mesh.m_vertexDeclaration);
	if (SUCCEEDED(result))
	{
		result = i_vertexDeclarationContext.device->SetVertexDeclaration(i_mesh.m_vertexDeclaration);
		if (FAILED(result))
		{
			eae6320::UserOutput::Print("Direct3D failed to set the vertex declaration");
			return false;
		}
	}
	else
	{
		eae6320::UserOutput::Print("Direct3D failed to create a Direct3D9 vertex declaration");
		return false;
	}
	return true;
}

bool eae6320::Graphics::MeshHelper::CleanUp(Mesh& i_mesh, const CleanUpContext& i_cleanUpContext)
{
	if (i_mesh.m_vertexBuffer)
	{
		i_mesh.m_vertexBuffer->Release();
		i_mesh.m_vertexBuffer = NULL;
	}
	if (i_mesh.m_indexBuffer)
	{
		i_mesh.m_indexBuffer->Release();
		i_mesh.m_indexBuffer = NULL;
	}
	if (i_mesh.m_vertexDeclaration)
	{
		i_cleanUpContext.device->SetVertexDeclaration(NULL);
		i_mesh.m_vertexDeclaration->Release();
		i_mesh.m_vertexDeclaration = NULL;
	}
	return true;
}

// Helper Function Definitions
//============================

namespace
{
	HRESULT GetVertexProcessingUsage(DWORD& o_usage, IDirect3DDevice9* i_device)
	{
		D3DDEVICE_CREATION_PARAMETERS deviceCreationParameters;
		const HRESULT result = i_device->GetCreationParameters(&deviceCreationParameters);
		if (SUCCEEDED(result))
		{
			DWORD vertexProcessingType = deviceCreationParameters.BehaviorFlags &
				(D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_SOFTWARE_VERTEXPROCESSING);
			o_usage = (vertexProcessingType != D3DCREATE_SOFTWARE_VERTEXPROCESSING) ? 0 : D3DUSAGE_SOFTWAREPROCESSING;
		}
		else
		{
			eae6320::UserOutput::Print("Direct3D failed to get the device's creation parameters");
		}
		return result;
	}
}


