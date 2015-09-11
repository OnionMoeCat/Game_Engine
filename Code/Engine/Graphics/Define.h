/*
This file contains the function declarations for graphics
*/

#ifndef EAE6320_DEFINE_H
#define EAE6320_DEFINE_H

// Header Files
//=============
#include "../Windows/Includes.h"

#if defined( EAE6320_PLATFORM_D3D )
#include <d3d9.h>
#elif defined( EAE6320_PLATFORM_GL )
#include <gl/GL.h>
#include <gl/GLU.h>
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#endif

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{

#if defined( EAE6320_PLATFORM_D3D )
		struct InitializeContext_D3D
		{
		};
#define INITIALIZECONTEXT InitializeContext_D3D
#elif defined( EAE6320_PLATFORM_GL )
		struct InitializeContext_GL
		{
			GLsizei arrayCount;
		};
#define INITIALIZECONTEXT InitializeContext_GL
#endif

#if defined( EAE6320_PLATFORM_D3D )
		struct RenderContext_D3D
		{
			IDirect3DDevice9* device;
			const D3DPRIMITIVETYPE primitiveType;
			const unsigned int indexOfFirstVertexToRender;
			const unsigned int indexOfFirstIndexToUse;
			const unsigned int vertexCountToRender;
			const unsigned int primitiveCountToRender;
		};
#define RENDERCONTEXT RenderContext_D3D
#elif defined( EAE6320_PLATFORM_GL )
		struct RenderContext_GL
		{
			const GLenum mode;
			const GLenum indexType;
			const GLvoid* const offset;
			const GLsizei vertexCountToRender;
		};
#define RENDERCONTEXT RenderContext_GL
#endif

#if defined( EAE6320_PLATFORM_D3D )
		struct VertexBufferData_D3D
		{
			void* data;
		};
#define VERTEXBUFFERDATA VertexBufferData_D3D
#elif defined( EAE6320_PLATFORM_GL )
		struct VertexBufferData_GL
		{
			GLvoid* data;
		};
#define VERTEXBUFFERDATA VertexBufferData_GL
#endif

#if defined( EAE6320_PLATFORM_D3D )
		struct CreateVertexBufferContext_D3D
		{
			IDirect3DDevice9* device;
			const DWORD usage;
			const unsigned int bufferSize;
			const DWORD useSeparateVertexDeclaration;
			const D3DPOOL useDefaultPool;
			HANDLE* const notUsed;
		};
#define CREATEVERTEXBUFFERCONTEXT CreateVertexBufferContext_D3D
#elif defined( EAE6320_PLATFORM_GL )
		struct CreateVertexBufferContext_GL
		{
			const GLsizei bufferCount;
		};
#define CREATEVERTEXBUFFERCONTEXT CreateVertexBufferContext_GL
#endif

#if defined( EAE6320_PLATFORM_D3D )
		struct LockVertexBufferContext_D3D
		{
			const unsigned int lockEntireBuffer;
			const DWORD useDefaultLockingBehavior;
		};
#define LOCKVERTEXBUFFERCONTEXT LockVertexBufferContext_D3D
#elif defined( EAE6320_PLATFORM_GL )
		struct LockVertexBufferContext_GL
		{
		};
#define LOCKVERTEXBUFFERCONTEXT LockVertexBufferContext_GL
#endif

#if defined( EAE6320_PLATFORM_D3D )
		struct SetVertexBufferContext_D3D
		{
		};
#define SETVERTEXBUFFERCONTEXT SetVertexBufferContext_D3D
#elif defined( EAE6320_PLATFORM_GL )
		struct SetVertexBufferContext_GL
		{
			const unsigned int bufferSize;
		};
#define SETVERTEXBUFFERCONTEXT SetVertexBufferContext_GL
#endif

#if defined( EAE6320_PLATFORM_D3D )
		struct IndexBufferData_D3D
		{
			void* data;
		};
#define INDEXBUFFERDATA IndexBufferData_D3D
#elif defined( EAE6320_PLATFORM_GL )
		struct IndexBufferData_GL
		{
			GLvoid* data;
		};
#define INDEXBUFFERDATA IndexBufferData_GL
#endif

#if defined( EAE6320_PLATFORM_D3D )
		struct CreateIndexBufferContext_D3D
		{
			IDirect3DDevice9* device;
			const DWORD usage;
			const unsigned int bufferSize;
			const D3DFORMAT format;
			const D3DPOOL useDefaultPool;
			HANDLE* notUsed;
		};
#define CREATEINDEXBUFFERCONTEXT CreateIndexBufferContext_D3D
#elif defined( EAE6320_PLATFORM_GL )
		struct CreateIndexBufferContext_GL
		{
			const GLsizei bufferCount;
	};
#define CREATEINDEXBUFFERCONTEXT CreateIndexBufferContext_GL
#endif

#if defined( EAE6320_PLATFORM_D3D )
		struct LockIndexBufferContext_D3D
		{
			const unsigned int lockEntireBuffer;
			const DWORD useDefaultLockingBehavior;
		};
#define LOCKINDEXBUFFERCONTEXT LockIndexBufferContext_D3D
#elif defined( EAE6320_PLATFORM_GL )
		struct LockIndexBufferContext_GL
		{
		};
#define LOCKINDEXBUFFERCONTEXT LockIndexBufferContext_GL
#endif

#if defined( EAE6320_PLATFORM_D3D )
		struct SetIndexBufferContext_D3D
		{
		};
#define SETINDEXBUFFERCONTEXT SetIndexBufferContext_D3D
#elif defined( EAE6320_PLATFORM_GL )
		struct SetIndexBufferContext_GL
		{
			const GLsizeiptr bufferSize;
		};
#define SETINDEXBUFFERCONTEXT SetIndexBufferContext_GL
#endif

#if defined( EAE6320_PLATFORM_D3D )
		struct VertexDeclarationContextPTR_D3D
		{
			IDirect3DDevice9* device;
			D3DVERTEXELEMENT9* vertexElement;
		};
#define VERTEXDECLARATIONCONTEXT D3DVERTEXELEMENT9
#define VERTEXDECLARATIONCONTEXTPTR VertexDeclarationContextPTR_D3D
#elif defined( EAE6320_PLATFORM_GL )
		struct VertexDeclarationContext_GL
		{
			const GLsizei stride;
			const GLvoid* offset;
			const GLuint vertexElementLocation;
			const GLint elementCount;
			const GLboolean notNormalized;
			const GLenum type;
		};
		struct VertexDeclarationContextPTR_GL
		{
			VertexDeclarationContext_GL* vertexElement;
		};
#define VERTEXDECLARATIONCONTEXT VertexDeclarationContext_GL
#define VERTEXDECLARATIONCONTEXTPTR VertexDeclarationContextPTR_GL
#endif

#if defined( EAE6320_PLATFORM_D3D )
		struct CleanUpContext_D3D
		{
		};
#define CLEANUPCONTEXT CleanUpContext_D3D
#elif defined( EAE6320_PLATFORM_GL )
		struct CleanUpContext_GL
		{
			const unsigned int vertexBufferCount;
			const unsigned int indexBufferCount;
		};
#define CLEANUPCONTEXT CleanUpContext_GL
#endif

#if defined( EAE6320_PLATFORM_D3D )
		struct FinalizeContext_D3D
		{
			IDirect3DDevice9* device;
		};
#define FINALIZECONTEXT FinalizeContext_D3D
#elif defined( EAE6320_PLATFORM_GL )
		struct FinalizeContext_GL
		{
			GLsizei arrayCount;
		};
#define FINALIZECONTEXT FinalizeContext_GL
#endif
	}
}

#endif	// EAE6320_DEFINE_H
