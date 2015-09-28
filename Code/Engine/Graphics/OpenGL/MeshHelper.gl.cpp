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

namespace
{
	bool CreateVertexArray(eae6320::Graphics::Mesh& i_mesh);
	bool CleanUpBuffers(eae6320::Graphics::Mesh& i_mesh);
}

// Interface
//==========

bool eae6320::Graphics::MeshHelper::DrawMesh(const Mesh& i_mesh, const RenderContext& i_rendercontext)
{
	// Bind a specific vertex buffer to the device as a data source
	{
		glBindVertexArray(i_mesh.m_vertexArrayId);
		if (glGetError() != GL_NO_ERROR)
		{
			return false;
		}
	}

	// Render objects from the current streams
	{
		// We are using triangles as the "primitive" type,
		// and we have defined the vertex buffer as a triangle list
		// (meaning that every triangle is defined by three vertices)
		const GLenum mode = GL_TRIANGLES;
		// We'll use 32-bit indices in this class to keep things simple
		// (i.e. every index will be a 32 bit unsigned integer)
		const GLenum indexType = GL_UNSIGNED_INT;
		// It is possible to start rendering in the middle of an index buffer
		const GLvoid* const offset = 0;
		// We are drawing a square
		const GLsizei vertexCountPerTriangle = 3;
		const GLsizei vertexCountToRender = i_mesh.m_primitiveCount * vertexCountPerTriangle;
		glDrawElements(mode, vertexCountToRender, indexType, offset);
		if (glGetError() != GL_NO_ERROR)
		{
			return false;
		}
	}
	return true;
}

bool eae6320::Graphics::MeshHelper::SetVertexBuffer(Mesh& i_mesh, const BufferDataPtr& i_verftexBufferData, const SetVertexBufferContext& i_setVertexBufferContext)
{
	bool wereThereErrors = false;

	// If there is not, create a vertex array object and make it active
	if (i_mesh.m_vertexArrayId == 0)
	{
		if (!CreateVertexArray(i_mesh))
		{
			wereThereErrors = true;
			goto OnExit;
		}
	}

	// Create a vertex buffer object and make it active
	{
		const GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &i_mesh.m_vertexBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_ARRAY_BUFFER, i_mesh.m_vertexBufferId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to bind the vertex buffer: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to get an unused vertex buffer ID: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			goto OnExit;
		}
	}
	// Assign the data to the buffer
	{
		// Assign number of vertices which we get from load mesh file
		i_mesh.m_vertexCount = i_setVertexBufferContext.vertexCount;
		// Calculate buffersize by multiply vertexcount with size
		const GLsizeiptr bufferSize = static_cast<GLsizeiptr>(i_mesh.m_vertexCount * sizeof(sVertex));

		glBufferData(GL_ARRAY_BUFFER, bufferSize, i_verftexBufferData,
			// Our code will only ever write to the buffer
			GL_STATIC_DRAW);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to allocate the vertex buffer: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			goto OnExit;
		}
	}

	// Initialize the vertex format
	{
		const GLsizei stride = sizeof(sVertex);
		GLvoid* offset = 0;

		// Position (0)
		// 2 floats == 8 bytes
		// Offset = 0
		{
			const GLuint vertexElementLocation = 0;
			const GLint elementCount = 2;
			const GLboolean notNormalized = GL_FALSE;	// The given floats should be used as-is
			glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride, offset);
			const GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glEnableVertexAttribArray(vertexElementLocation);
				const GLenum errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					offset = reinterpret_cast<GLvoid*>(reinterpret_cast<uint8_t*>(offset) + (elementCount * sizeof(float)));
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to enable the POSITION vertex attribute: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to set the POSITION vertex attribute: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
		}
		// Color (1)
		// 4 uint8_ts == 4 bytes
		// Offset = 8
		{
			const GLuint vertexElementLocation = 1;
			const GLint elementCount = 4;
			// Each element will be sent to the GPU as an unsigned byte in the range [0,255]
			// but these values should be understood as representing [0,1] values
			// and that is what the shader code will interpret them as
			// (in other words, we could change the values provided here in C code
			// to be floats and sent GL_FALSE instead and the shader code wouldn't need to change)
			const GLboolean normalized = GL_TRUE;
			glVertexAttribPointer(vertexElementLocation, elementCount, GL_UNSIGNED_BYTE, normalized, stride, offset);
			const GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glEnableVertexAttribArray(vertexElementLocation);
				const GLenum errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					offset = reinterpret_cast<GLvoid*>(reinterpret_cast<uint8_t*>(offset) + (elementCount * sizeof(uint8_t)));
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to enable the COLOR0 vertex attribute: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to set the COLOR0 vertex attribute: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
		}
	}

OnExit:

	// Delete the buffer object
	// (the vertex array will hold a reference to it)
	if (i_mesh.m_vertexArrayId != 0 && i_mesh.m_vertexBufferId && i_mesh.m_indexBufferId != 0)
	{
		CleanUpBuffers(i_mesh);
	}

	return !wereThereErrors;
}

bool eae6320::Graphics::MeshHelper::SetIndexBuffer(Mesh& i_mesh, const BufferDataPtr& i_indexBufferData, const SetIndexBufferContext& i_setIndexBufferContext)
{
	bool wereThereErrors = false;

	// If there is not, create a vertex array object and make it active
	if (i_mesh.m_vertexArrayId == 0)
	{
		if (!CreateVertexArray(i_mesh))
		{
			wereThereErrors = true;
			goto OnExit;
		}
	}

	// Create an index buffer object and make it active
	{
		const GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &i_mesh.m_indexBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_mesh.m_indexBufferId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to bind the index buffer: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to get an unused index buffer ID: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			goto OnExit;
		}
	}
	// Allocate space and copy the triangle data into the index buffer
	{
		// Assign number of primitives which we get from load mesh file
		i_mesh.m_primitiveCount = i_setIndexBufferContext.primitiveCount;
		const unsigned int vertexPerPrimitive = 3;
		// Calculate buffersize by multiply primitivecount with vertex per primitive with size
		const GLsizeiptr bufferSize = static_cast<GLsizeiptr>(i_mesh.m_primitiveCount * vertexPerPrimitive * sizeof(uint32_t));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, i_indexBufferData,
			// Our code will only ever write to the buffer
			GL_STATIC_DRAW);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to allocate the index buffer: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			goto OnExit;
		}
	}

OnExit:
	return !wereThereErrors;
}

bool eae6320::Graphics::MeshHelper::CleanUp(Mesh& i_mesh, const CleanUpContext& i_cleanUpContext)
{
	if (i_mesh.m_vertexArrayId != 0)
	{
		const GLsizei arrayCount = 1;
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

namespace
{
	bool CreateVertexArray(eae6320::Graphics::Mesh& i_mesh)
	{
		const GLsizei arrayCount = 1;
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

	bool CleanUpBuffers(eae6320::Graphics::Mesh& i_mesh)
	{
		bool wereThereErrors = false;
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
					const GLsizei bufferCount = 1;
					glDeleteBuffers(bufferCount, &i_mesh.m_vertexBufferId);
					const GLenum errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to delete the vertex buffer: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
					}
					i_mesh.m_vertexBufferId = 0;
				}
				if (i_mesh.m_indexBufferId != 0)
				{
					// NOTE: See the same comment above about deleting the vertex buffer object here and gDEBugger
					// holds true for the index buffer
					const GLsizei bufferCount = 1;
					glDeleteBuffers(bufferCount, &i_mesh.m_indexBufferId);
					const GLenum errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "\nOpenGL failed to delete the index buffer: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
					}
					i_mesh.m_indexBufferId = 0;
				}
			}
			else
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to unbind the vertex array: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
			}
		}
		return !wereThereErrors;
	}
}