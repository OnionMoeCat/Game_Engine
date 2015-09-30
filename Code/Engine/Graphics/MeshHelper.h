/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_MESHHELPER_H
#define EAE6320_MESHHELPER_H

// Header Files
//=============
#include "Mesh.h"
#include "CrossPlatformDefine.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct MeshHelper
		{
		private:
			bool static SetVertexBuffer(Mesh& i_mesh, const BufferDataPtr& i_vertexBufferData, const SetVertexBufferContext& i_setVertexBufferContext);
			bool static SetIndexBuffer(Mesh& i_mesh, const BufferDataPtr& i_indexBufferData, const SetIndexBufferContext& i_setIndexBufferContext);
		public:
			bool static DrawMesh(const Mesh& i_mesh, const RenderContext& i_renderContext);
			bool static CleanUp(Mesh& i_mesh, const CleanUpMeshContext& i_cleanUpContext);
			bool static LoadMeshFromFile(Mesh& i_mesh, const char* const i_path, const LoadMeshContext& i_loadMeshContext);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
