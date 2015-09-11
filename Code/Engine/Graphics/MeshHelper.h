/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_MESHHELPER_H
#define EAE6320_MESHHELPER_H

// Header Files
//=============
#include "Mesh.h"
#include "Define.h"
// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct MeshHelper
		{
			bool static DrawMesh(const Mesh& i_mesh, const RENDERCONTEXT& i_renderContext);
			bool static CreateVertexBuffer(Mesh& i_mesh, const CREATEVERTEXBUFFERCONTEXT& i_createVertexBufferContext);
			bool static LockVertexBuffer(Mesh& i_mesh, VERTEXBUFFERDATA& o_vertexBufferData, const LOCKVERTEXBUFFERCONTEXT& i_lockVertexBufferContext);
			bool static SetVertexBuffer(Mesh& i_mesh, const VERTEXBUFFERDATA& i_vertexBufferData, const SETVERTEXBUFFERCONTEXT& i_setVertexBufferContext);
			bool static UnlockVertexBuffer(Mesh& i_mesh);
			bool static CreateIndexBuffer(Mesh& i_mesh, const CREATEINDEXBUFFERCONTEXT& i_createIndexBufferData);
			bool static LockIndexBuffer(Mesh& i_mesh, INDEXBUFFERDATA& o_vertexBufferData, const LOCKINDEXBUFFERCONTEXT& i_lockVertexBufferContext);
			bool static SetIndexBuffer(Mesh& i_mesh, const INDEXBUFFERDATA& i_indexBufferData, const SETINDEXBUFFERCONTEXT& i_setIndexBufferContext);
			bool static UnlockIndexBuffer(Mesh& i_mesh);
			bool static SetVertexDeclaration(Mesh& i_mesh, VERTEXDECLARATIONCONTEXTPTR i_vertexDeclarationContext, const unsigned int i_vertexElementsCount);
			bool static CleanUp(Mesh& i_mesh, const CLEANUPCONTEXT& i_cleanUpContext);
			bool static Initialize(Mesh& i_mesh, const INITIALIZECONTEXT& i_numVertexArray);
			bool static Finalize(Mesh& i_mesh, const FINALIZECONTEXT& i_finalizeContext);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
