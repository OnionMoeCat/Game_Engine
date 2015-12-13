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

bool eae6320::Graphics::MeshHelper::DrawMesh(const Mesh& i_mesh, const Context& i_context)
{
	{
		if (i_context.device == NULL)
		{
			return false;
		}
	}
	// Bind a specific vertex buffer to the device as a data source
	{
		// There can be multiple streams of data feeding the display adaptor at the same time
		const unsigned int streamIndex = 0;
		// It's possible to start streaming data in the middle of a vertex buffer
		const unsigned int bufferOffset = 0;
		// The "stride" defines how large a single vertex is in the stream of data
		const unsigned int bufferStride = sizeof(sVertex);
		HRESULT result = i_context.device->SetStreamSource(streamIndex, i_mesh.m_vertexBuffer, bufferOffset, bufferStride);
		if (FAILED(result))
		{
			return false;
		}
	}
	// Bind a specific index buffer to the device as a data source
	{
		HRESULT result = i_context.device->SetIndices(i_mesh.m_indexBuffer);
		if (FAILED(result))
		{
			return false;
		}
	}

	// Render objects from the current streams
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
		HRESULT result = i_context.device->DrawIndexedPrimitive(primitiveType,
			indexOfFirstVertexToRender, indexOfFirstVertexToRender, vertexCountToRender,
			indexOfFirstIndexToUse, primitiveCountToRender);
		if (FAILED(result))
		{
			return false;
		}
	}
	return true;
}

bool eae6320::Graphics::MeshHelper::SetIndexBuffer(Mesh& i_mesh, const BufferDataPtr& i_indexBufferData, const unsigned int i_primitiveCount, const Context& i_context)
{
	{
		if (i_context.device == NULL)
		{
			return false;
		}
	}
	// The usage tells Direct3D how this vertex buffer will be used
	DWORD usage = 0;
	{
		// The type of vertex processing should match what was specified when the device interface was created with CreateDevice()
		const HRESULT result = GetVertexProcessingUsage(usage, i_context.device);
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
		i_mesh.m_primitiveCount = i_primitiveCount;
		const unsigned int vertexPerPrimitive = 3;
		// Calculate buffersize by multiply primitivecount with number of vertex per primitive with size
		bufferSize = i_mesh.m_primitiveCount * vertexPerPrimitive * sizeof(uint32_t);
		// We'll use 32-bit indices in this class to keep things simple
		// (i.e. every index will be a 32 bit unsigned integer)
		const D3DFORMAT format = D3DFMT_INDEX32;
		// Place the index buffer into memory that Direct3D thinks is the most appropriate
		const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
		HANDLE* notUsed = NULL;
		const HRESULT result = i_context.device->CreateIndexBuffer(bufferSize, usage, format, useDefaultPool,
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

bool eae6320::Graphics::MeshHelper::SetVertexBuffer(Mesh& i_mesh, const BufferDataPtr& i_vertexBufferData, const unsigned int i_vertexCount, const Context& i_context)
{
	{
		if (i_context.device == NULL)
		{
			return false;
		}
	}
	// The usage tells Direct3D how this vertex buffer will be used
	DWORD usage = 0;
	{
		// The type of vertex processing should match what was specified when the device interface was created with CreateDevice()
		const HRESULT result = GetVertexProcessingUsage(usage, i_context.device);
		if (FAILED(result))
		{
			return false;
		}
		// Our code will only ever write to the buffer
		usage |= D3DUSAGE_WRITEONLY;
	}

	// Initialize the vertex format
	{
		// These elements must match the VertexFormat::sVertex layout struct exactly.
		// They instruct Direct3D how to match the binary data in the vertex buffer
		// to the input elements in a vertex shader
		// (by using D3DDECLUSAGE enums here and semantics in the shader,
		// so that, for example, D3DDECLUSAGE_POSITION here matches with POSITION in shader code).
		// Note that OpenGL uses arbitrarily assignable number IDs to do the same thing.
		D3DVERTEXELEMENT9 vertexElements[] =
		{
			// Stream 0

			// POSITION
			// 3 floats == 12 bytes
			// Offset = 0
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

			// COLOR0
			// D3DCOLOR == 4 bytes
			// Offset = 12
			{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

			// TEXCOORD
			// 2 float == 8 bytes
			// Offset = 16
			{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },

			// The following marker signals the end of the vertex declaration
			D3DDECL_END()
		};
		HRESULT result = i_context.device->CreateVertexDeclaration(vertexElements, &i_mesh.m_vertexDeclaration);
		if (SUCCEEDED(result))
		{
			result = i_context.device->SetVertexDeclaration(i_mesh.m_vertexDeclaration);
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
	}

	unsigned int bufferSize;
	// Create a vertex buffer
	{
		// Assign number of vertices which we get from load mesh file
		i_mesh.m_vertexCount = i_vertexCount;
		// Calculate buffersize by multiply count with size
		bufferSize = i_mesh.m_vertexCount * sizeof(sVertex);
		// We will define our own vertex format
		const DWORD useSeparateVertexDeclaration = 0;
		// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
		const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
		HANDLE* const notUsed = NULL;
		const HRESULT result = i_context.device->CreateVertexBuffer(bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,
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


bool eae6320::Graphics::MeshHelper::CleanUp(Mesh& i_mesh, const Context& i_context)
{
	{
		if (i_context.device == NULL)
		{
			return false;
		}
	}
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


