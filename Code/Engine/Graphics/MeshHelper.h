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
			bool static SetVertexBuffer(Mesh& i_mesh, const BufferDataPtr& i_vertexBufferData, const unsigned int i_vertexCount, const Context& i_context);
			bool static SetIndexBuffer(Mesh& i_mesh, const BufferDataPtr& i_indexBufferData, const unsigned int i_primitiveCount, const Context& i_context);
		public:
			bool static DrawMesh(const Mesh& i_mesh, const Context& i_context);
			bool static CleanUp(Mesh& i_mesh, const Context& i_context);
			bool static LoadMeshFromFile(Mesh& i_mesh, const char* const i_path, const Context& i_context);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
