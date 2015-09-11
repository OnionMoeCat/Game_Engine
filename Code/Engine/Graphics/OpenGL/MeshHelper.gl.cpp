// Header Files
//=============

#include "../MeshHelper.h"

#include "../../Windows/Includes.h"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <string>
#include <sstream>
#include "../../UserOutput/UserOutput.h"
#include "../../Windows/Functions.h"
#include "../../../External/OpenGlExtensions/OpenGlExtensions.h"

// Interface
//==========

bool eae6320::Graphics::MeshHelper::DrawMesh(const Mesh& i_mesh, const RENDERCONTEXT& i_rendercontext)
{
	// We are using triangles as the "primitive" type,
	// and we have defined the vertex buffer as a triangle list
	// (meaning that every triangle is defined by three vertices)
	const GLenum mode = i_rendercontext.mode;
	// We'll use 32-bit indices in this class to keep things simple
	// (i.e. every index will be a 32 bit unsigned integer)
	const GLenum indexType = i_rendercontext.indexType;
	// It is possible to start rendering in the middle of an index buffer
	const GLvoid* const offset = i_rendercontext.offset;
	// We are drawing a square
	const GLsizei vertexCountToRender = i_rendercontext.vertexCountToRender;
	glDrawElements(mode, vertexCountToRender, indexType, offset);
	return (glGetError() == GL_NO_ERROR);
}

bool eae6320::Graphics::MeshHelper::SetVertexBuffer(Mesh& i_mesh, const VERTEXBUFFERDATA& i_vertexBufferData, const SETVERTEXBUFFERCONTEXT& i_setVertexBufferContext)
{
	// Assign the data to the buffer
	glBufferData(GL_ARRAY_BUFFER, i_setVertexBufferContext.bufferSize, i_vertexBufferData.data,
		// Our code will only ever write to the buffer
		GL_STATIC_DRAW);
	const GLenum errorCode = glGetError();
	if (errorCode != GL_NO_ERROR)
	{
		std::stringstream errorMessage;
		errorMessage << "OpenGL failed to allocate the vertex buffer: " <<
			reinterpret_cast<const char*>(gluErrorString(errorCode));
		eae6320::UserOutput::Print(errorMessage.str());
		return false;
	}
	return true;
}

bool eae6320::Graphics::MeshHelper::CreateVertexBuffer(Mesh& i_mesh, const CREATEVERTEXBUFFERCONTEXT& i_createVertexBufferContext)
{
	// Create a vertex buffer object and make it active
	const GLsizei bufferCount = i_createVertexBufferContext.bufferCount;
	glGenBuffers(bufferCount, &i_mesh.m_vertexBufferId);
	const GLenum errorCode = glGetError();
	if (errorCode == GL_NO_ERROR)
	{
		glBindBuffer(GL_ARRAY_BUFFER, i_mesh.m_vertexBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to bind the vertex buffer: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
	}
	else
	{
		std::stringstream errorMessage;
		errorMessage << "OpenGL failed to get an unused vertex buffer ID: " <<
			reinterpret_cast<const char*>(gluErrorString(errorCode));
		eae6320::UserOutput::Print(errorMessage.str());
		return false;
	}
	return true;
}

bool eae6320::Graphics::MeshHelper::SetVertexDeclaration(Mesh& i_mesh, VERTEXDECLARATIONCONTEXTPTR i_vertexDeclarationContext, const unsigned int i_vertexElementsCount)
{
	for (unsigned int i = 0; i < i_vertexElementsCount; i++)
	{
		const GLsizei stride = i_vertexDeclarationContext.vertexElement[i].stride;
		const GLvoid* offset = i_vertexDeclarationContext.vertexElement[i].offset;
		const GLuint vertexElementLocation = i_vertexDeclarationContext.vertexElement[i].vertexElementLocation;
		const GLint elementCount = i_vertexDeclarationContext.vertexElement[i].elementCount;
		const GLboolean notNormalized = i_vertexDeclarationContext.vertexElement[i].notNormalized;	// The given floats should be used as-is
		const GLenum type = i_vertexDeclarationContext.vertexElement[i].type;

		glVertexAttribPointer(vertexElementLocation, elementCount, type, notNormalized, stride, offset);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glEnableVertexAttribArray(vertexElementLocation);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to enable the POSITION vertex attribute: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
				return false;
			}
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to set the POSITION vertex attribute: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
	}
	return true;
}
bool eae6320::Graphics::MeshHelper::CreateIndexBuffer(Mesh& i_mesh, const CREATEINDEXBUFFERCONTEXT& i_createIndexBufferData)
{
	// Create an index buffer object and make it active
	const GLsizei bufferCount = i_createIndexBufferData.bufferCount;
	glGenBuffers(bufferCount, &i_mesh.m_indexBufferId);
	const GLenum errorCode = glGetError();
	if (errorCode == GL_NO_ERROR)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_mesh.m_indexBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to bind the index buffer: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
	}
	else
	{
		std::stringstream errorMessage;
		errorMessage << "OpenGL failed to get an unused index buffer ID: " <<
			reinterpret_cast<const char*>(gluErrorString(errorCode));
		eae6320::UserOutput::Print(errorMessage.str());
		return false;
	}
	return true;
}
bool eae6320::Graphics::MeshHelper::SetIndexBuffer(Mesh& i_mesh, const INDEXBUFFERDATA& i_indexBufferData, const SETINDEXBUFFERCONTEXT& i_setIndexBufferContext)
{
	const GLsizeiptr bufferSize = i_setIndexBufferContext.bufferSize;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, i_indexBufferData.data,
		// Our code will only ever write to the buffer
		GL_STATIC_DRAW);
	const GLenum errorCode = glGetError();
	if (errorCode != GL_NO_ERROR)
	{
		std::stringstream errorMessage;
		errorMessage << "OpenGL failed to allocate the index buffer: " <<
			reinterpret_cast<const char*>(gluErrorString(errorCode));
		eae6320::UserOutput::Print(errorMessage.str());
		return false;
	}
	return true;
}
bool eae6320::Graphics::MeshHelper::CleanUp(Mesh& i_mesh, const CLEANUPCONTEXT& i_cleanUpContext)
{
	// Delete the buffer object
	// (the vertex array will hold a reference to it)
	if (i_mesh.m_vertexArrayId != 0)
	{
		// Unbind the vertex array
		// (this must be done before deleting the vertex buffer)
		glBindVertexArray(0);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			if (i_mesh.m_vertexBufferId != 0)
			{
				// NOTE: If you delete the vertex buffer object here, as I recommend,
				// then gDEBugger won't know about it and you won't be able to examine the data.
				// If you find yourself in a situation where you want to see the buffer object data in gDEBugger
				// comment out this block of code temporarily
				// (doing this will cause a memory leak so make sure to restore the deletion code after you're done debugging).
				const GLsizei vertexBufferCount = i_cleanUpContext.vertexBufferCount;
				glDeleteBuffers(vertexBufferCount, &i_mesh.m_vertexBufferId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to delete the vertex buffer: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					return false;
				}
				i_mesh.m_vertexBufferId = 0;
			}
			if (i_mesh.m_indexBufferId != 0)
			{
				// NOTE: See the same comment above about deleting the vertex buffer object here and gDEBugger
				// holds true for the index buffer
				const GLsizei indexBufferCount = i_cleanUpContext.indexBufferCount;
				glDeleteBuffers(indexBufferCount, &i_mesh.m_indexBufferId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					std::stringstream errorMessage;
					errorMessage << "\nOpenGL failed to delete the index buffer: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					return false;
				}
				i_mesh.m_indexBufferId = 0;
			}
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to unbind the vertex array: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
	}
	//QUESTION: if vertexArrayId is 0, it just return true?
	return true;
}
bool eae6320::Graphics::MeshHelper::Initialize(Mesh& i_mesh, const INITIALIZECONTEXT& i_numVertexArray)
{
	// Create a vertex array object and make it active
	const GLsizei arrayCount = i_numVertexArray.arrayCount;
	glGenVertexArrays(arrayCount, &i_mesh.m_vertexArrayId);
	const GLenum errorCode = glGetError();
	if (errorCode == GL_NO_ERROR)
	{
		glBindVertexArray(i_mesh.m_vertexArrayId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to bind the vertex array: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
	}
	else
	{
		std::stringstream errorMessage;
		errorMessage << "OpenGL failed to get an unused vertex array ID: " <<
			reinterpret_cast<const char*>(gluErrorString(errorCode));
		eae6320::UserOutput::Print(errorMessage.str());
		return false;
	}
	return true;
}

bool eae6320::Graphics::MeshHelper::Finalize(Mesh& i_mesh, const FINALIZECONTEXT& i_finalizeContext)
{
	if (i_mesh.m_vertexArrayId != 0)
	{
		const GLsizei arrayCount = i_finalizeContext.arrayCount;
		glDeleteVertexArrays(arrayCount, &i_mesh.m_vertexArrayId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to delete the vertex array: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			UserOutput::Print(errorMessage.str());
			return false;
		}
		i_mesh.m_vertexArrayId = 0;
	}
	//QUESTION: if vertexArrayId is 0, it just return true?
	return true;
}

bool eae6320::Graphics::MeshHelper::LockVertexBuffer(Mesh& i_mesh, VERTEXBUFFERDATA& o_vertexBufferData, const LOCKVERTEXBUFFERCONTEXT& i_lockVertexBufferContext)
{
	return true;
}

bool eae6320::Graphics::MeshHelper::UnlockVertexBuffer(Mesh& i_mesh)
{
	return true;
}

bool eae6320::Graphics::MeshHelper::LockIndexBuffer(Mesh& i_mesh, INDEXBUFFERDATA& o_vertexBufferData, const LOCKINDEXBUFFERCONTEXT& i_lockVertexBufferContext)
{
	return true;
}

bool eae6320::Graphics::MeshHelper::UnlockIndexBuffer(Mesh& i_mesh)
{
	return true;
}