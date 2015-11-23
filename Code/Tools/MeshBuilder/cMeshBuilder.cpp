// Header Files
//=============

#include "cMeshBuilder.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include "../../Engine/Windows/Functions.h"
#include "../../External/Lua/Includes.h"
#include "../../Engine/Graphics/Includes.h"

// Static Data Initialization
//===========================
namespace
{
	eae6320::Graphics::sVertex* vertices = NULL;
	uint32_t vertexSize = 0;
	uint32_t* indices = NULL;
	uint32_t primitiveSize = 0;
}

// Helper Function Declarations
//=============================

namespace
{
	bool LoadTableValues(lua_State& io_luaState);

	bool LoadTableValues_vertices(lua_State& io_luaState);
	bool LoadTableValues_vertices_array(lua_State& io_luaState, unsigned int i_count);
	bool LoadTableValues_array_count(lua_State& io_luaState, unsigned int& o_count);
	bool LoadTableValues_vertices_array_position(lua_State& io_lua_state, unsigned int i_index);
	bool LoadTableValues_vertices_array_position_xyz(lua_State& io_luaState, unsigned int i_index);
	bool LoadTableValues_vertices_array_color(lua_State& io_luaState, unsigned int i_index);
	bool LoadTableValues_vertices_array_color_rgba(lua_State& io_luaState, unsigned int i_index);

	bool LoadTableValues_triangle_indexes(lua_State& io_luaState);
	bool LoadTableValues_triangle_indexes_array(lua_State& io_luaState, unsigned int i_count);
	bool LoadTableValues_triangle_indexes_array_abc(lua_State& io_luaState, unsigned int i_index);

	bool WriteBufferToFile(const char* const i_path, HANDLE i_fileHandle, const void* i_buffer, DWORD i_bytesToWrite);

	bool LoadMesh(const char* const i_path);
	bool WriteMesh(const char* const i_path);

	bool BuildMeshFromFile(const char* const i_source, const char* const i_target);
}

// Interface
//==========

// Build
//------

bool eae6320::cMeshBuilder::Build( const std::vector<std::string>& )
{
	bool wereThereErrors = false;

	// Copy the source to the target
	{
		std::string errorMessage;
		if ( !BuildMeshFromFile( m_path_source, m_path_target ) )
		{
			wereThereErrors = true;
			std::stringstream decoratedErrorMessage;
			decoratedErrorMessage << "Windows failed to copy \"" << m_path_source << "\" to \"" << m_path_target << "\"";
			eae6320::OutputErrorMessage( decoratedErrorMessage.str().c_str(), __FILE__ );
		}
	}
	
	return !wereThereErrors;
}

// Helper Function Definitions
//============================

namespace
{
	bool LoadTableValues(lua_State& io_luaState)
	{
		if (!LoadTableValues_vertices(io_luaState))
		{
			return false;
		}
		if (!LoadTableValues_triangle_indexes(io_luaState))
		{
			return false;
		}

		return true;
	}

	bool LoadTableValues_vertices(lua_State& io_luaState)
	{
		bool wereThereErrors = false;

		const char* const key = "vertices";
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);
		if (lua_istable(&io_luaState, -1))
		{
			unsigned int count;
			if (!LoadTableValues_array_count(io_luaState, count))
			{
				eae6320::OutputErrorMessage("Fail to get array length of \"vertices\"", __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}
			//QUESTION: opengl will not work if vertex count == 0 and index count != 0. Is a mesh possible to have 0 vertex?
			if (count == 0)
			{
				eae6320::OutputErrorMessage("Array length of \"vertices\" is 0", __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}
			vertexSize = static_cast<uint32_t>(count);
			vertices = reinterpret_cast<eae6320::Graphics::sVertex*>(malloc(sizeof(eae6320::Graphics::sVertex) * vertexSize));
			if (!LoadTableValues_vertices_array(io_luaState, count))
			{
				free(vertices);
				vertexSize = 0;
				vertices = NULL;
				if (indices != NULL)
				{
					free(indices);
					primitiveSize = 0;
					indices = NULL;
				}
				eae6320::OutputErrorMessage("Fail to get value of \"vertices\"", __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "The value at key \"" << key << "\" must be a table (instead of a " << luaL_typename(&io_luaState, -1);
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			wereThereErrors = true;
			goto OnExit;
		}

	OnExit:

		// Pop the textures table
		lua_pop(&io_luaState, 1);

		return !wereThereErrors;
	}

	bool LoadTableValues_vertices_array(lua_State& io_luaState, unsigned int i_count)
	{
		for (unsigned int i = 1; i <= i_count; ++i)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);
			if (!lua_istable(&io_luaState, -1))
			{
				lua_pop(&io_luaState, 1);
				std::stringstream errorMessage;
				errorMessage << "The value at index " << i << " of \"vertices\" must be a table (instead of a " << luaL_typename(&io_luaState, -1);
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}
			if (!LoadTableValues_vertices_array_position(io_luaState, i))
			{
				lua_pop(&io_luaState, 1);
				std::stringstream errorMessage;
				errorMessage << "Fail to get value of \"position\" for \"vertices\" expected at index: " << i;
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}
			if (!LoadTableValues_vertices_array_color(io_luaState, i))
			{
				lua_pop(&io_luaState, 1);
				std::stringstream errorMessage;
				errorMessage << "Fail to get value of \"color\" for \"vertices\" expected at index: " << i;
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}
			lua_pop(&io_luaState, 1);
		}

		return true;
	}

	bool LoadTableValues_array_count(lua_State& io_luaState, unsigned int& o_count)
	{
		o_count = luaL_len(&io_luaState, -1);
		return true;
	}

	bool LoadTableValues_vertices_array_position(lua_State& io_luaState, unsigned int i_index)
	{
		bool wereThereErrors = false;

		char* key = "position";
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);

		if (!lua_istable(&io_luaState, -1))
		{
			std::stringstream errorMessage;
			errorMessage << "The value at key \"" << key << "\" must be a table (instead of a " << luaL_typename(&io_luaState, -1);
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			wereThereErrors = true;
			goto OnExit;
		}

		if (!LoadTableValues_vertices_array_position_xyz(io_luaState, i_index))
		{
			std::stringstream errorMessage;
			errorMessage << "Fail to get value of \"position\" for \"vertices\" at index: " << i_index;
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			wereThereErrors = true;
			goto OnExit;
		}

	OnExit:

		lua_pop(&io_luaState, 1);
		return !wereThereErrors;
	}

	bool LoadTableValues_vertices_array_color(lua_State& io_luaState, unsigned int i_index)
	{
		bool wereThereErrors = false;

		char* key = "color";
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);

		if (!lua_istable(&io_luaState, -1))
		{
			std::stringstream errorMessage;
			errorMessage << "The value at key \"" << key << "\" must be a table (instead of a " << luaL_typename(&io_luaState, -1);
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			wereThereErrors = true;
			goto OnExit;
		}

		if (!LoadTableValues_vertices_array_color_rgba(io_luaState, i_index))
		{
			std::stringstream errorMessage;
			errorMessage << "Fail to get value of \"color\" for \"vertices\" at index: " << i_index;
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			wereThereErrors = true;
			goto OnExit;
		}

	OnExit:

		lua_pop(&io_luaState, 1);
		return !wereThereErrors;
	}

	bool LoadTableValues_vertices_array_position_xyz(lua_State& io_luaState, unsigned int i_index)
	{
		const int xyzLength = luaL_len(&io_luaState, -1);
		const int LENGTH = 3;
		const char charMap[LENGTH] = { 'x','y','z' };

		float tempXYZ[LENGTH];
		if (xyzLength != LENGTH)
		{
			std::stringstream errorMessage;
			errorMessage << "3 elements (x, y, z) in key \"position\" expected for \"vertices\" at index: " << i_index;
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			return false;
		}
		for (int i = 1; i <= xyzLength; i++)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);
			if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
			{
				std::stringstream errorMessage;
				errorMessage << "Number value expected for " << charMap[i - 1] << " in key \"position\" for \"vertices\" at index: " << i_index;
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}
			tempXYZ[i - 1] = static_cast<float>(lua_tonumber(&io_luaState, -1));
			lua_pop(&io_luaState, 1);
		}

		vertices[i_index - 1].x = tempXYZ[0];
		vertices[i_index - 1].y = tempXYZ[1];
		vertices[i_index - 1].z = tempXYZ[2];

		return true;
	}

	bool LoadTableValues_vertices_array_color_rgba(lua_State& io_luaState, unsigned int i_index)
	{
		const int colorScale = 255;
		const int rgbaLength = luaL_len(&io_luaState, -1);
		const int LENGTH = 4;
		const char charMap[LENGTH] = { 'r','g','b','a' };

		uint8_t tempRGBA[LENGTH];
		if (rgbaLength != LENGTH)
		{
			std::stringstream errorMessage;
			errorMessage << "4 elements (r, g, b, a) in key \"color\" expected for \"vertices\" at index: " << i_index;
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			return false;
		}
		for (int i = 1; i <= rgbaLength; i++)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);
			if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
			{
				std::stringstream errorMessage;
				errorMessage << "Number value expected for " << charMap[i - 1] << " in key \"color\" for \"vertices\" at index: " << i_index;
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}
			// round to nearest integer
			tempRGBA[i - 1] = static_cast<uint8_t>(std::round(lua_tonumber(&io_luaState, -1) * colorScale));
			lua_pop(&io_luaState, 1);
		}

		vertices[i_index - 1].r = tempRGBA[0];
		vertices[i_index - 1].g = tempRGBA[1];
		vertices[i_index - 1].b = tempRGBA[2];
		vertices[i_index - 1].a = tempRGBA[3];

		return true;
	}

	bool LoadTableValues_triangle_indexes(lua_State& io_luaState)
	{
		bool wereThereErrors = false;

		const char* const key = "triangle_indexes";
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);
		if (lua_istable(&io_luaState, -1))
		{
			unsigned int count;
			if (!LoadTableValues_array_count(io_luaState, count))
			{
				eae6320::OutputErrorMessage("Fail to get array length of \"triangle_indexes\"", __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}
			primitiveSize = static_cast<uint32_t>(count);
			const unsigned int vertexPerPrimitive = 3;
			indices = reinterpret_cast<uint32_t*>(malloc(sizeof(uint32_t) * vertexPerPrimitive * primitiveSize));
			if (indices == NULL)
			{
				wereThereErrors = true;
				goto OnExit;
			}
			if (!LoadTableValues_triangle_indexes_array(io_luaState, count))
			{
				free(indices);
				primitiveSize = 0;
				indices = NULL;
				if (vertices != NULL)
				{
					free(vertices);
					vertexSize = 0;
					vertices = NULL;
				}
				eae6320::OutputErrorMessage("Fail to get value of \"triangle_indexes\"", __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "The value at key \"" << key << "\" must be a table (instead of a " << luaL_typename(&io_luaState, -1);
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			wereThereErrors = true;
			goto OnExit;
		}

	OnExit:

		// Pop the textures table
		lua_pop(&io_luaState, 1);

		return !wereThereErrors;
	}

	bool LoadTableValues_triangle_indexes_array(lua_State& io_luaState, unsigned int i_count)
	{
		for (unsigned int i = 1; i <= i_count; ++i)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);
			if (!lua_istable(&io_luaState, -1))
			{
				lua_pop(&io_luaState, 1);
				std::stringstream errorMessage;
				errorMessage << "The value at index " << i << " of \"triangle_indexes\" must be a table (instead of a " << luaL_typename(&io_luaState, -1);
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}
			if (!LoadTableValues_triangle_indexes_array_abc(io_luaState, i))
			{
				lua_pop(&io_luaState, 1);
				std::stringstream errorMessage;
				errorMessage << "Fail to get value for \"triangle_indexes\" expected at index: " << i;
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}
			lua_pop(&io_luaState, 1);
		}

		return true;
	}

	bool LoadTableValues_triangle_indexes_array_abc(lua_State& io_luaState, unsigned int i_index)
	{
		const int abcLength = luaL_len(&io_luaState, -1);
		const int LENGTH = 3;
		uint32_t tempABC[LENGTH];

		if (abcLength != LENGTH)
		{
			std::stringstream errorMessage;
			errorMessage << "3 elements expected at triangle_indexes index: " << i_index;
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			return false;
		}
		for (int i = 1; i <= abcLength; i++)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);
			if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
			{
				std::stringstream errorMessage;
				errorMessage << "Number value expected for \"triangle_indexes\" at index: " << i_index;
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}
			tempABC[i - 1] = static_cast<uint32_t>(lua_tonumber(&io_luaState, -1));
			lua_pop(&io_luaState, 1);
		}
#if defined( EAE6320_PLATFORM_D3D )
		indices[(i_index - 1) * LENGTH + 0] = tempABC[0];
		indices[(i_index - 1) * LENGTH + 1] = tempABC[2];
		indices[(i_index - 1) * LENGTH + 2] = tempABC[1];
#elif defined( EAE6320_PLATFORM_GL )
		indices[(i_index - 1) * LENGTH + 0] = tempABC[0];
		indices[(i_index - 1) * LENGTH + 1] = tempABC[1];
		indices[(i_index - 1) * LENGTH + 2] = tempABC[2];
#endif
		return true;
	}

	bool LoadMesh(const char* const i_path)
	{
		bool wereThereErrors = false;

		// Create a new Lua state
		lua_State* luaState = NULL;
		{
			luaState = luaL_newstate();
			if (!luaState)
			{
				eae6320::OutputErrorMessage("Fail to create a new Lua state", __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}
		}

		// Load the asset file as a "chunk",
		// meaning there will be a callable function at the top of the stack
		{
			const int luaResult = luaL_loadfile(luaState, i_path);
			if (luaResult != LUA_OK)
			{
				wereThereErrors = true;
				eae6320::OutputErrorMessage(lua_tostring(luaState, -1), __FILE__);
				// Pop the error message
				lua_pop(luaState, 1);
				goto OnExit;
			}
		}
		// Execute the "chunk", which should load the asset
		// into a table at the top of the stack
		{
			const int argumentCount = 0;
			const int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
			const int noMessageHandler = 0;
			const int luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
			if (luaResult == LUA_OK)
			{
				// A well-behaved asset file will only return a single value
				const int returnedValueCount = lua_gettop(luaState);
				if (returnedValueCount == 1)
				{
					// A correct asset file _must_ return a table
					if (!lua_istable(luaState, -1))
					{
						std::stringstream errorMessage;
						errorMessage << "Asset files must return a table (instead of a " << luaL_typename(luaState, -1);
						eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
						wereThereErrors = true;
						// Pop the returned non-table value
						lua_pop(luaState, 1);
						goto OnExit;
					}
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "Asset files must return a table (instead of a " << returnedValueCount << " values)";
					eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					wereThereErrors = true;
					// Pop every value that was returned
					lua_pop(luaState, returnedValueCount);
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				// Pop the error message
				lua_pop(luaState, 1);
				goto OnExit;
			}
		}

		// If this code is reached the asset file was loaded successfully,
		// and its table is now at index -1
		if (!LoadTableValues(*luaState))
		{
			wereThereErrors = true;
			lua_pop(luaState, 1);
			goto OnExit;
		}

		// Pop the table
		lua_pop(luaState, 1);

	OnExit:

		if (luaState)
		{
			// If I haven't made any mistakes
			// there shouldn't be anything on the stack,
			// regardless of any errors encountered while loading the file:
			assert(lua_gettop(luaState) == 0);

			lua_close(luaState);
			luaState = NULL;
		}

		return !wereThereErrors;
	}

	bool WriteBufferToFile(const char* const i_path, HANDLE i_fileHandle, const void* i_buffer, DWORD i_bytesToWrite)
	{
		DWORD bytesWritten = 0;
		BOOL errorFlag = WriteFile(i_fileHandle, i_buffer, i_bytesToWrite, &bytesWritten, NULL);

		if (errorFlag == FALSE)
		{
			std::stringstream errorMessage;
			errorMessage << "Fail to write file \"" << i_path << "\": " << eae6320::GetLastWindowsError();
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			return false;
		}
		else
		{
			if (bytesWritten != i_bytesToWrite)
			{
				std::stringstream errorMessage;
				errorMessage << "Fail to write all bytes to file \"" << i_path << "\": " << eae6320::GetLastWindowsError();
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}
		}
		return true;
	}

	bool WriteMesh(const char* const i_path)
	{
		bool wereThereErrors = false;
		//Side Effect: Wiil cause a error: File_ALREADY_EXISTS. But it should be good because we only get error when something really fail. 
		//QUESTION: Is it really ok?
		HANDLE fileHandle = CreateFile(i_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		
		if (fileHandle == INVALID_HANDLE_VALUE)
		{
			std::stringstream errorMessage;
			errorMessage << "Fail to create file \"" << i_path << "\": " << eae6320::GetLastWindowsError();
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			return false;
		}

		DWORD bytesToWrite = (DWORD)sizeof(uint32_t);
		if (!WriteBufferToFile(i_path, fileHandle, &vertexSize, bytesToWrite))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		bytesToWrite = (DWORD)sizeof(uint32_t);
		if (!WriteBufferToFile(i_path, fileHandle, &primitiveSize, bytesToWrite))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		bytesToWrite = (DWORD)(sizeof(eae6320::Graphics::sVertex) * vertexSize);
		if (!WriteBufferToFile(i_path, fileHandle, vertices, bytesToWrite))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		const unsigned int vertexPerPrimitive = 3;
		bytesToWrite = (DWORD)(sizeof(uint32_t) * primitiveSize * vertexPerPrimitive);
		if (!WriteBufferToFile(i_path, fileHandle, indices, bytesToWrite))
		{
			wereThereErrors = true;
			goto OnExit;
		}

	OnExit:

		if (fileHandle != INVALID_HANDLE_VALUE)
		{
			if (!CloseHandle(fileHandle))
			{
				std::stringstream errorMessage;
				errorMessage << "Fail to close file \"" << i_path << "\": " << eae6320::GetLastWindowsError();
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}
			fileHandle = INVALID_HANDLE_VALUE;
		}

		return !wereThereErrors;
	}

	bool BuildMeshFromFile(const char* const i_source, const char* const i_target)
	{
		bool wereThereErrors = false;

		if (!LoadMesh(i_source))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		if (!WriteMesh(i_target))
		{
			wereThereErrors = true;
			goto OnExit;
		}

	OnExit:

		// Verices and Indices need to assign to NULL
		if (vertices != NULL)
		{
			free(vertices);
		}
		vertices = NULL;
		vertexSize = 0;
		if (indices != NULL)
		{
			free(indices);
		}
		indices = NULL;
		primitiveSize = 0;

		return !wereThereErrors;
	}
}