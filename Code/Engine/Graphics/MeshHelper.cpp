// Header Files
//=============

#include "MeshHelper.h"

#include "../UserOutput/UserOutput.h"
#include "../../External/Lua/Includes.h"
#include <assert.h>
#include <iostream>
#include <sstream>

// Static Data Initialization
//===========================
namespace
{
	eae6320::Graphics::sVertex* vertices = NULL;
	unsigned int vertexSize = 0;
	uint32_t* indices = NULL;
	unsigned int primitiveSize = 0;
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
	bool LoadTableValues_vertices_array_position_xy(lua_State& io_luaState, unsigned int i_index);
	bool LoadTableValues_vertices_array_color(lua_State& io_luaState, unsigned int i_index);
	bool LoadTableValues_vertices_array_color_rgba(lua_State& io_luaState, unsigned int i_index);

	bool LoadTableValues_triangle_indexes(lua_State& io_luaState);
	bool LoadTableValues_triangle_indexes_array(lua_State& io_luaState, unsigned int i_count);
	bool LoadTableValues_triangle_indexes_array_abc(lua_State& io_luaState, unsigned int i_index);

	bool LoadMesh(eae6320::Graphics::Mesh& i_mesh, const char* const i_path, eae6320::Graphics::sVertex** i_vertices, uint32_t** i_indices, unsigned int& i_vertexSize, unsigned int& i_primitiveSize);
}

bool eae6320::Graphics::MeshHelper::ReadMeshFromFile(Mesh& i_mesh, const char* const i_path, const LoadMeshContext& i_loadMeshContext)
{
	bool wereThereErrors = false;

	sVertex* vertices = NULL;
	uint32_t* indices = NULL;
	unsigned int vertexSize = 0;
	unsigned int primitiveSize = 0;
	{
		if (!LoadMesh(i_mesh, i_path, &vertices, &indices, vertexSize, primitiveSize))
		{
			return false;
		}
	}

	{
		eae6320::Graphics::SetVertexBufferContext setVertexBufferContext =
		{
#if defined( EAE6320_PLATFORM_D3D )
			i_loadMeshContext.device,
#endif
			vertexSize
		};

		if (!eae6320::Graphics::MeshHelper::SetVertexBuffer(i_mesh, vertices, setVertexBufferContext))
		{
			wereThereErrors = true;
			goto OnExit;
		}
	}

	{
		eae6320::Graphics::SetIndexBufferContext setIndexBufferContext =
		{
#if defined( EAE6320_PLATFORM_D3D )
			i_loadMeshContext.device,
#endif
			primitiveSize
		};

		if (!eae6320::Graphics::MeshHelper::SetIndexBuffer(i_mesh, indices, setIndexBufferContext))
		{
			wereThereErrors = true;
			goto OnExit;
		}
	}

OnExit:
	free(vertices);
	free(indices);
	vertexSize = 0;
	primitiveSize = 0;
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
				eae6320::UserOutput::Print("Fail to get array length of 'vertices'");
				wereThereErrors = true;
				goto OnExit;
			}
			//QUESTION: opengl will not work if vertex count == 0 and index count != 0. Is a mesh possible to have 0 vertex?
			if (count == 0)
			{
				eae6320::UserOutput::Print("Array length of 'vertices' is 0");
				wereThereErrors = true;
				goto OnExit;
			}
			vertexSize = count;
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
				eae6320::UserOutput::Print("Fail to get value of 'vertices'");
				wereThereErrors = true;
				goto OnExit;
			}
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "The value at key '" << key << "' must be a table (instead of a " << luaL_typename(&io_luaState, -1);
			eae6320::UserOutput::Print(errorMessage.str());
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
				errorMessage << "The value at index " << i << " of 'vertices' must be a table (instead of a " << luaL_typename(&io_luaState, -1);
				eae6320::UserOutput::Print(errorMessage.str());
				return false;
			}
			if (!LoadTableValues_vertices_array_position(io_luaState, i))
			{
				lua_pop(&io_luaState, 1);
				std::stringstream errorMessage;
				errorMessage << "Fail to get value of 'position' for 'vertices' expected at index: " << i;
				eae6320::UserOutput::Print(errorMessage.str());
				return false;
			}
			if (!LoadTableValues_vertices_array_color(io_luaState, i))
			{
				lua_pop(&io_luaState, 1);
				std::stringstream errorMessage;
				errorMessage << "Fail to get value of 'color' for 'vertices' expected at index: " << i;
				eae6320::UserOutput::Print(errorMessage.str());
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
			errorMessage << "The value at key '" << key << "' must be a table (instead of a " << luaL_typename(&io_luaState, -1);
			eae6320::UserOutput::Print(errorMessage.str());
			wereThereErrors = true;
			goto OnExit;
		}

		if (!LoadTableValues_vertices_array_position_xy(io_luaState, i_index))
		{
			std::stringstream errorMessage;
			errorMessage << "Fail to get value of 'position' for 'vertices' at index: " << i_index;
			eae6320::UserOutput::Print(errorMessage.str());
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
			errorMessage << "The value at key '" << key << "' must be a table (instead of a " << luaL_typename(&io_luaState, -1);
			eae6320::UserOutput::Print(errorMessage.str());
			wereThereErrors = true;
			goto OnExit;
		}

		if (!LoadTableValues_vertices_array_color_rgba(io_luaState, i_index))
		{
			std::stringstream errorMessage;
			errorMessage << "Fail to get value of 'color' for 'vertices' at index: " << i_index;
			eae6320::UserOutput::Print(errorMessage.str());
			wereThereErrors = true;
			goto OnExit;
		}

	OnExit:

		lua_pop(&io_luaState, 1);
		return !wereThereErrors;
	}

	bool LoadTableValues_vertices_array_position_xy(lua_State& io_luaState, unsigned int i_index)
	{
		const int xyLength = luaL_len(&io_luaState, -1);
		const int LENGTH = 2;
		const char charMap[LENGTH] = { 'x','y' };

		float tempXY[LENGTH];
		if (xyLength != LENGTH)
		{
			std::stringstream errorMessage;
			errorMessage << "2 elements (x, y) in key 'position' expected for 'vertices' at index: " << i_index;
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
		for (int i = 1; i <= xyLength; i++)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);
			if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
			{
				std::stringstream errorMessage;
				errorMessage << "Number value expected for " << charMap[i - 1] << " in key 'position' for 'vertices' at index: " << i_index;
				eae6320::UserOutput::Print(errorMessage.str());
				lua_pop(&io_luaState, 1);
				return false;
			}
			tempXY[i - 1] = static_cast<float>(lua_tonumber(&io_luaState, -1));
			lua_pop(&io_luaState, 1);
		}

		vertices[i_index - 1].x = tempXY[0];
		vertices[i_index - 1].y = tempXY[1];

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
			errorMessage << "4 elements (r, g, b, a) in key 'color' expected for 'vertices' at index: " << i_index;
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
		for (int i = 1; i <= rgbaLength; i++)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);
			if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
			{
				std::stringstream errorMessage;
				errorMessage << "Number value expected for " << charMap[i - 1] << " in key 'color' for 'vertices' at index: " << i_index;
				eae6320::UserOutput::Print(errorMessage.str());
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
				eae6320::UserOutput::Print("Fail to get array length of 'triangle_indexes'");
				wereThereErrors = true;
				goto OnExit;
			}
			primitiveSize = count;
			const unsigned int vertexPerPrimitive = 3;
			indices = reinterpret_cast<uint32_t*>(malloc(sizeof(uint32_t) * vertexPerPrimitive * primitiveSize));
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
				eae6320::UserOutput::Print("Fail to get value of 'triangle_indexes'");
				wereThereErrors = true;
				goto OnExit;
			}
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "The value at key '" << key << "' must be a table (instead of a " << luaL_typename(&io_luaState, -1);
			eae6320::UserOutput::Print(errorMessage.str());
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
				errorMessage << "The value at index " << i << " of 'triangle_indexes' must be a table (instead of a " << luaL_typename(&io_luaState, -1);
				eae6320::UserOutput::Print(errorMessage.str());
				return false;
			}
			if (!LoadTableValues_triangle_indexes_array_abc(io_luaState, i))
			{
				lua_pop(&io_luaState, 1);
				std::stringstream errorMessage;
				errorMessage << "Fail to get value for 'triangle_indexes' expected at index: " << i;
				eae6320::UserOutput::Print(errorMessage.str());
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
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
		for (int i = 1; i <= abcLength; i++)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);
			if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
			{
				std::stringstream errorMessage;
				errorMessage << "Number value expected for 'triangle_indexes' at index: " << i_index;
				eae6320::UserOutput::Print(errorMessage.str());
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

	bool LoadMesh(eae6320::Graphics::Mesh& i_mesh, const char* const i_path, eae6320::Graphics::sVertex** i_vertices, uint32_t** i_indices, unsigned int& i_vertexSize, unsigned int& i_primitiveSize)
	{
		bool wereThereErrors = false;

		// Create a new Lua state
		lua_State* luaState = NULL;
		{
			luaState = luaL_newstate();
			if (!luaState)
			{
				eae6320::UserOutput::Print("Fail to create a new Lua state");
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
				eae6320::UserOutput::Print(lua_tostring(luaState, -1));
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
						eae6320::UserOutput::Print(errorMessage.str());
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
					eae6320::UserOutput::Print(errorMessage.str());
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

		*i_vertices = vertices;
		*i_indices = indices;
		i_vertexSize = vertexSize;
		i_primitiveSize = primitiveSize;

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

		// Verices and Indices need to assign to NULL
		vertices = NULL;
		vertexSize = 0;
		indices = NULL;
		primitiveSize = 0;

		return !wereThereErrors;
	}
}