// Header Files
//=============

#include "../MeshHelper.h"

#include "../../Windows/Includes.h"

#include <cassert>
#include <cstdint>
#include <d3d9.h>
#include <d3dx9shader.h>
#include <sstream>
#include "../../UserOutput/UserOutput.h"

// Interface
//==========

bool eae6320::Graphics::MeshHelper::DrawMesh(const Mesh& i_mesh, const RENDERCONTEXT& i_rendercontext)
{
	// We are using triangles as the "primitive" type,
	// and we have defined the vertex buffer as a triangle list
	// (meaning that every triangle is defined by three vertices)
	const D3DPRIMITIVETYPE primitiveType = i_rendercontext.primitiveType;
	// It's possible to start rendering primitives in the middle of the stream
	const unsigned int indexOfFirstVertexToRender = i_rendercontext.indexOfFirstVertexToRender;
	const unsigned int indexOfFirstIndexToUse = i_rendercontext.indexOfFirstIndexToUse;
	// We are drawing a square
	const unsigned int vertexCountToRender = i_rendercontext.vertexCountToRender;	// How vertices from the vertex buffer will be used?
	const unsigned int primitiveCountToRender = i_rendercontext.primitiveCountToRender;	// How many triangles will be drawn?
	HRESULT result = i_rendercontext.device->DrawIndexedPrimitive(primitiveType,
		indexOfFirstVertexToRender, indexOfFirstVertexToRender, vertexCountToRender,
		indexOfFirstIndexToUse, primitiveCountToRender);
	return (SUCCEEDED(result));
}

bool eae6320::Graphics::MeshHelper::CreateVertexBuffer(Mesh& i_mesh, const CREATEVERTEXBUFFERCONTEXT& i_createVertexBufferContext)
{
	// The usage tells Direct3D how this vertex buffer will be used
	const DWORD usage = i_createVertexBufferContext.usage;
	// We are drawing one square
	const unsigned int bufferSize = i_createVertexBufferContext.bufferSize;
	// We will define our own vertex format
	const DWORD useSeparateVertexDeclaration = i_createVertexBufferContext.useSeparateVertexDeclaration;
	// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
	const D3DPOOL useDefaultPool = i_createVertexBufferContext.useDefaultPool;
	HANDLE* const notUsed = i_createVertexBufferContext.notUsed;
	const HRESULT result = i_createVertexBufferContext.device->CreateVertexBuffer(bufferSize, usage, 
		useSeparateVertexDeclaration, useDefaultPool, &i_mesh.m_vertexBuffer, notUsed);
	if (FAILED(result))
	{
		eae6320::UserOutput::Print("Direct3D failed to create a vertex buffer");
		return false;
	}
	return true;
}

bool eae6320::Graphics::MeshHelper::LockVertexBuffer(Mesh& i_mesh, VERTEXBUFFERDATA& o_vertexBufferData, const LOCKVERTEXBUFFERCONTEXT& i_lockVertexBufferContext)
{
	// Before the vertex buffer can be changed it must be "locked"
	const unsigned int lockEntireBuffer = i_lockVertexBufferContext.lockEntireBuffer;
	const DWORD useDefaultLockingBehavior = i_lockVertexBufferContext.useDefaultLockingBehavior;
	const HRESULT result = i_mesh.m_vertexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
		&o_vertexBufferData.data, useDefaultLockingBehavior);
	if (FAILED(result))
	{
		eae6320::UserOutput::Print("Direct3D failed to lock the vertex buffer");
		return false;
	}
	return true;
}

bool eae6320::Graphics::MeshHelper::SetVertexBuffer(Mesh& i_mesh, const VERTEXBUFFERDATA& i_vertexBufferData, const SETVERTEXBUFFERCONTEXT& i_setVertexBufferContext)
{
	return true;
}

bool eae6320::Graphics::MeshHelper::UnlockVertexBuffer(Mesh& i_mesh)
{
	// The buffer must be "unlocked" before it can be used
	const HRESULT result = i_mesh.m_vertexBuffer->Unlock();
	if (FAILED(result))
	{
		eae6320::UserOutput::Print("Direct3D failed to unlock the vertex buffer");
		return false;
	}
	return true;
}

bool eae6320::Graphics::MeshHelper::CreateIndexBuffer(Mesh& i_mesh, const CREATEINDEXBUFFERCONTEXT& i_createIndexBufferData)
{
	// The usage tells Direct3D how this vertex buffer will be used
	const DWORD usage = i_createIndexBufferData.usage;
	// We are drawing a square
	const unsigned int bufferSize = i_createIndexBufferData.bufferSize;
	// We'll use 32-bit indices in this class to keep things simple
	// (i.e. every index will be a 32 bit unsigned integer)
	const D3DFORMAT format = i_createIndexBufferData.format;
	// Place the index buffer into memory that Direct3D thinks is the most appropriate
	const D3DPOOL useDefaultPool = i_createIndexBufferData.useDefaultPool;
	HANDLE* notUsed = i_createIndexBufferData.notUsed;
	const HRESULT result = i_createIndexBufferData.device->CreateIndexBuffer(bufferSize, usage, format, useDefaultPool,
		&i_mesh.m_indexBuffer, notUsed);
	if (FAILED(result))
	{
		eae6320::UserOutput::Print("Direct3D failed to create an index buffer");
		return false;
	}
	return true;
}

bool eae6320::Graphics::MeshHelper::LockIndexBuffer(Mesh& i_mesh, INDEXBUFFERDATA& o_vertexBufferData, const LOCKINDEXBUFFERCONTEXT& i_lockIndexBufferContext)
{
	const unsigned int lockEntireBuffer = i_lockIndexBufferContext.lockEntireBuffer;
	const DWORD useDefaultLockingBehavior = i_lockIndexBufferContext.useDefaultLockingBehavior;
	const HRESULT result = i_mesh.m_indexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
		&o_vertexBufferData.data, useDefaultLockingBehavior);
	if (FAILED(result))
	{
		eae6320::UserOutput::Print("Direct3D failed to lock the index buffer");
		return false;
	}
	return true;
}

bool eae6320::Graphics::MeshHelper::SetIndexBuffer(Mesh& i_mesh, const INDEXBUFFERDATA& i_indexBufferData, const SETINDEXBUFFERCONTEXT& i_setIndexBufferContext)
{
	return true;
}

bool eae6320::Graphics::MeshHelper::UnlockIndexBuffer(Mesh& i_mesh)
{
	const HRESULT result = i_mesh.m_indexBuffer->Unlock();
	if (FAILED(result))
	{
		eae6320::UserOutput::Print("Direct3D failed to unlock the index buffer");
		return false;
	}
	return true;
}

bool eae6320::Graphics::MeshHelper::SetVertexDeclaration(Mesh& i_mesh, VERTEXDECLARATIONCONTEXTPTR i_vertexDeclarationContext, const unsigned int i_vertexElementsCount)
{
	// Initialize the vertex format
	HRESULT result = i_vertexDeclarationContext.device->CreateVertexDeclaration(i_vertexDeclarationContext.vertexElement, &i_mesh.m_vertexDeclaration);
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

bool eae6320::Graphics::MeshHelper::CleanUp(Mesh& i_mesh, const CLEANUPCONTEXT& i_cleanUpContext)
{
	return true;
}

bool eae6320::Graphics::MeshHelper::Initialize(Mesh& i_mesh, const INITIALIZECONTEXT& i_numVertexArray)
{
	return true;
}

bool eae6320::Graphics::MeshHelper::Finalize(Mesh& i_mesh, const FINALIZECONTEXT& i_finalizeContext)
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
		i_finalizeContext.device->SetVertexDeclaration(NULL);
		i_mesh.m_vertexDeclaration->Release();
		i_mesh.m_vertexDeclaration = NULL;
	}
	return true;
}