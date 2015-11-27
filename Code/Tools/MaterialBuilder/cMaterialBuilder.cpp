// Header Files
//=============

#include "cMaterialBuilder.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include "../../Engine/Windows/Functions.h"
#include "../../External/Lua/Includes.h"
#include "../../Engine/Graphics/Uniform.h"

// Static Data Initialization
//===========================
namespace
{
	
	std::string effectPath;
	uint32_t uniformSize = 0;
	eae6320::Graphics::Uniform* uniforms = NULL;
	char** uniformNames = NULL;

	uint32_t textureSize = 0;
	char** textureNames = NULL;
	char** texturePaths = NULL;
}

// Helper Function Declarations
//=============================

namespace
{
	bool LoadTableValues(lua_State& io_luaState);

	bool LoadTableValues_paths(lua_State& io_luaState);
	bool LoadTableValues_eachpath(lua_State& io_luaState);
	bool LoadTableValues_array_count(lua_State& io_luaState, unsigned int& o_count);

	bool LoadTableValues_uniforms(lua_State& io_luaState);
	bool LoadTableValues_eachuniform(lua_State& io_luaState, unsigned int i_count);
	bool LoadTableValues_eachuniform_parameters(lua_State& io_luaState, unsigned int i_index);
	bool LoadTableValues_eachuniform_parameters_values(lua_State& io_luaState, unsigned int i_index);

	bool LoadTableValues_textures(lua_State& io_luaState);
	bool LoadTableValues_eachtexture(lua_State& io_luaState, unsigned int i_count);
	bool LoadTableValues_eachtexture_parameters(lua_State& io_luaState, unsigned int i_index);

	bool WriteBufferToFile(const char* const i_path, HANDLE i_fileHandle, const void* i_buffer, DWORD i_bytesToWrite);

	bool LoadMaterial(const char* const i_path);
	bool WriteMaterial(const char* const i_path);

	bool BuildMaterialFromFile(const char* const i_source, const char* const i_target);
}

// Interface
//==========

bool eae6320::cMaterialBuilder::Build(const std::vector<std::string>&)
{
	bool wereThereErrors = false;

	// Copy the source to the target
	{
		std::string errorMessage;
		if (!BuildMaterialFromFile(m_path_source, m_path_target))
		{
			wereThereErrors = true;
			std::stringstream decoratedErrorMessage;
			decoratedErrorMessage << "Windows failed to copy \"" << m_path_source << "\" to \"" << m_path_target << "\"";
			eae6320::OutputErrorMessage(decoratedErrorMessage.str().c_str(), __FILE__);
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
		if (!LoadTableValues_paths(io_luaState))
		{
			return false;
		}
		if (!LoadTableValues_uniforms(io_luaState))
		{
			return false;
		}
		if (!LoadTableValues_textures(io_luaState))
		{
			return false;
		}

		return true;
	}

	bool LoadTableValues_paths(lua_State& io_luaState)
	{
		{
			const char* const key = "paths";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);

			if (lua_isnil(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "No value for \"" << key << "\" was found in the asset table"
					"\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			if (lua_istable(&io_luaState, -1))
			{
				if (!LoadTableValues_eachpath(io_luaState))
				{
					std::stringstream errorMessage;
					errorMessage << "Fail to get value of " << key;
					eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					lua_pop(&io_luaState, 1);
					return false;
				}
			}
		}

		lua_pop(&io_luaState, 1);

		return true;
	}

	bool LoadTableValues_eachpath(lua_State& io_luaState)
	{
		// Get the value of "effect_path"
		{
			const char* const key = "effect_path";
			lua_pushstring(&io_luaState, key);

			{
				const int currentIndexOfTable = -2;
				lua_gettable(&io_luaState, currentIndexOfTable);
			}

			if (lua_isnil(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "No value for \"" << key << "\" was found in the asset table"
					"\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			if (!lua_isstring(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "The value for \"" << key << "\" must be a string "
					"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}
			
			{
				effectPath = lua_tostring(&io_luaState, -1);
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

	bool LoadTableValues_uniforms(lua_State& io_luaState)
	{
		bool wereThereErrors = false;

		const char* const key = "uniforms";
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);
		if (lua_istable(&io_luaState, -1))
		{
			unsigned int count;
			if (!LoadTableValues_array_count(io_luaState, count))
			{
				eae6320::OutputErrorMessage("Fail to get array length of \"uniforms\"", __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

			uniformSize = static_cast<uint32_t>(count);
			uniforms = reinterpret_cast<eae6320::Graphics::Uniform*>(malloc(sizeof(eae6320::Graphics::Uniform) * uniformSize));
			uniformNames = reinterpret_cast<char**>(malloc(sizeof(char*) * uniformSize));
			if (uniforms == NULL)
			{
				wereThereErrors = true;
				goto OnExit;
			}
			if (uniformNames == NULL)
			{
				wereThereErrors = true;
				goto OnExit;
			}
			if (!LoadTableValues_eachuniform(io_luaState, count))
			{
				uniformSize = 0;
				free(uniforms);
				uniforms = NULL;
				free(uniformNames);
				uniformNames = NULL;

				eae6320::OutputErrorMessage("Fail to get value of \"uniforms\"", __FILE__);
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

	bool LoadTableValues_eachuniform(lua_State& io_luaState, unsigned int i_count)
	{
		for (unsigned int i = 1; i <= i_count; ++i)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);

			if (lua_isnil(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "No value was found in the asset table"
					"\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			if (!lua_istable(&io_luaState, -1))
			{
				lua_pop(&io_luaState, 1);
				std::stringstream errorMessage;
				errorMessage << "The value at index " << i << " of \"uniforms\" must be a table (instead of a " << luaL_typename(&io_luaState, -1);
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}
			if (!LoadTableValues_eachuniform_parameters(io_luaState, i))
			{
				lua_pop(&io_luaState, 1);
				std::stringstream errorMessage;
				errorMessage << "Fail to get value for \"uniforms\" expected at index: " << i;
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}
			lua_pop(&io_luaState, 1);
		}

		return true;
	}

	bool LoadTableValues_eachuniform_parameters(lua_State& io_luaState, unsigned int i_index)
	{
		{
			const char* const key = "name";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);

			if (lua_isnil(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "No value for \"" << key << "\" was found in the asset table"
					"\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			if (!lua_isstring(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "The value at key \"" << key << "\" must be a string (instead of a " << luaL_typename(&io_luaState, -1);
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			{
				const char* tempStr = lua_tostring(&io_luaState, -1);
				uniformNames[i_index - 1] = _strdup(tempStr);
			}

			lua_pop(&io_luaState, 1);

		}

		{
			const char* const key = "values";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);

			if (lua_isnil(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "No value for \"" << key << "\" was found in the asset table"
					"\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			if (!lua_istable(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "The value at key \"" << key << "\" must be a string (instead of a " << luaL_typename(&io_luaState, -1);
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			if (!LoadTableValues_eachuniform_parameters_values(io_luaState, i_index))
			{
				lua_pop(&io_luaState, 1);
				std::stringstream errorMessage;
				errorMessage << "Fail to get value for \"values\" expected at index: " << i_index;
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}

			lua_pop(&io_luaState, 1);

		}

		{
			const char* const key = "shader_type";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);

			if (lua_isnil(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "No value for \"" << key << "\" was found in the asset table"
					"\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			if (!lua_isstring(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "The value at key \"" << key << "\" must be a string (instead of a " << luaL_typename(&io_luaState, -1);
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			{
				std::string temp = lua_tostring(&io_luaState, -1);
				if (temp == "vertex")
				{
					uniforms[i_index - 1].shaderType = eae6320::Graphics::ShaderTypes::eShaderType::Vertex;
				}
				else if (temp == "fragment")
				{
					uniforms[i_index - 1].shaderType = eae6320::Graphics::ShaderTypes::eShaderType::Fragment;
				}
				else
				{
					uniforms[i_index - 1].shaderType = eae6320::Graphics::ShaderTypes::eShaderType::Unknown;
				}
			}

			lua_pop(&io_luaState, 1);

		}

		return true;
	}

	bool LoadTableValues_eachuniform_parameters_values(lua_State& io_luaState, unsigned int i_index)
	{
		const int valueLength = luaL_len(&io_luaState, -1);
		const int MAXLENGTH = 4;

		uniforms[i_index - 1].valueCountToSet = valueLength;

		if (valueLength > MAXLENGTH)
		{
			std::stringstream errorMessage;
			errorMessage << "less equal than 4 elements (r, g, b, a) in key \"color\" expected for \"vertices\" at index: " << i_index;
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			return false;
		}
		for (int i = 1; i <= valueLength; i++)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);
			if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
			{
				std::stringstream errorMessage;
				errorMessage << "Number value expected for index " << i << " in key \"color\" for \"vertices\" at index: " << i_index;
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}
			// round to nearest integer
			uniforms[i_index - 1].values[i - 1] = static_cast<float>(lua_tonumber(&io_luaState, -1));
			lua_pop(&io_luaState, 1);
		}

		return true;
	}

	bool LoadTableValues_textures(lua_State& io_luaState)
	{
		bool wereThereErrors = false;

		const char* const key = "textures";
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);
		if (lua_istable(&io_luaState, -1))
		{
			unsigned int count;
			if (!LoadTableValues_array_count(io_luaState, count))
			{
				eae6320::OutputErrorMessage("Fail to get array length of \"textures\"", __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

			textureSize = static_cast<uint32_t>(count);
			textureNames = reinterpret_cast<char**>(malloc(sizeof(char*) * textureSize));
			texturePaths = reinterpret_cast<char**>(malloc(sizeof(char*) * textureSize));
			if (uniforms == NULL)
			{
				wereThereErrors = true;
				goto OnExit;
			}
			if (uniformNames == NULL)
			{
				wereThereErrors = true;
				goto OnExit;
			}
			if (!LoadTableValues_eachtexture(io_luaState, count))
			{
				textureSize = 0;
				free(textureNames);
				textureNames = NULL;
				free(texturePaths);
				texturePaths = NULL;

				eae6320::OutputErrorMessage("Fail to get value of \"uniforms\"", __FILE__);
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

	bool LoadTableValues_eachtexture(lua_State& io_luaState, unsigned int i_count)
	{
		for (unsigned int i = 1; i <= i_count; ++i)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);

			if (lua_isnil(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "No value was found in the asset table"
					"\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			if (!lua_istable(&io_luaState, -1))
			{
				lua_pop(&io_luaState, 1);
				std::stringstream errorMessage;
				errorMessage << "The value at index " << i << " of \"textures\" must be a table (instead of a " << luaL_typename(&io_luaState, -1);
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}
			if (!LoadTableValues_eachtexture_parameters(io_luaState, i))
			{
				lua_pop(&io_luaState, 1);
				std::stringstream errorMessage;
				errorMessage << "Fail to get value for \"textures\" expected at index: " << i;
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}
			lua_pop(&io_luaState, 1);
		}

		return true;
	}

	bool LoadTableValues_eachtexture_parameters(lua_State& io_luaState, unsigned int i_index)
	{
		{
			const char* const key = "name";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);

			if (lua_isnil(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "No value for \"" << key << "\" was found in the asset table"
					"\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			if (!lua_isstring(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "The value at key \"" << key << "\" must be a string (instead of a " << luaL_typename(&io_luaState, -1);
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			{
				const char* tempStr = lua_tostring(&io_luaState, -1);
				textureNames[i_index - 1] = _strdup(tempStr);
			}

			lua_pop(&io_luaState, 1);

		}

		{
			const char* const key = "path";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);

			if (lua_isnil(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "No value for \"" << key << "\" was found in the asset table"
					"\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			if (!lua_isstring(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "The value at key \"" << key << "\" must be a string (instead of a " << luaL_typename(&io_luaState, -1);
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			{
				const char* tempStr = lua_tostring(&io_luaState, -1);
				texturePaths[i_index - 1] = _strdup(tempStr);
			}

			lua_pop(&io_luaState, 1);

		}

		{
			const char* const key = "shader_type";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);

			if (lua_isnil(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "No value for \"" << key << "\" was found in the asset table"
					"\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			if (!lua_isstring(&io_luaState, -1))
			{
				std::stringstream errorMessage;
				errorMessage << "The value at key \"" << key << "\" must be a string (instead of a " << luaL_typename(&io_luaState, -1);
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			{
				std::string temp = lua_tostring(&io_luaState, -1);
				if (temp == "vertex")
				{
					uniforms[i_index - 1].shaderType = eae6320::Graphics::ShaderTypes::eShaderType::Vertex;
				}
				else if (temp == "fragment")
				{
					uniforms[i_index - 1].shaderType = eae6320::Graphics::ShaderTypes::eShaderType::Fragment;
				}
				else
				{
					uniforms[i_index - 1].shaderType = eae6320::Graphics::ShaderTypes::eShaderType::Unknown;
				}
			}

			lua_pop(&io_luaState, 1);

		}

		return true;
	}

	bool LoadMaterial(const char* const i_path)
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

	bool WriteMaterial(const char* const i_path)
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

		unsigned int tempStrlen = static_cast<unsigned int>(effectPath.length());
		//Output error if strlength can not be hold in a uint8_t
		if (tempStrlen + 1 > 0xFF)
		{
			unsigned int outputLen = 50;
			std::stringstream errorMessage;
			errorMessage << "File name length exceeds range of uint8_t. File name length: " << tempStrlen << " File name (first " << outputLen << "): " << effectPath.substr(0, outputLen);
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			return false;
		}

		uint8_t strlength = static_cast<uint8_t>(tempStrlen + 1);
		DWORD bytesToWrite = 1;
		if (!WriteBufferToFile(i_path, fileHandle, &strlength, bytesToWrite))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		bytesToWrite = strlength;
		if (!WriteBufferToFile(i_path, fileHandle, effectPath.c_str(), bytesToWrite))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		bytesToWrite = (DWORD)sizeof(uint32_t);
		if (!WriteBufferToFile(i_path, fileHandle, &uniformSize, bytesToWrite))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		for (size_t i = 0; i < uniformSize; i++)
		{
			bytesToWrite = (DWORD)sizeof(eae6320::Graphics::Uniform);
			if (!WriteBufferToFile(i_path, fileHandle, &uniforms[i], bytesToWrite))
			{
				wereThereErrors = true;
				goto OnExit;
			}
		}

		for (size_t i = 0; i < uniformSize; i++)
		{
			tempStrlen = static_cast<unsigned int>(strlen(uniformNames[i]));
			//Output error if strlength can not be hold in a uint8_t
			if (tempStrlen + 1 > 0xFF)
			{
				unsigned int outputLen = 50;
				std::stringstream errorMessage;
				errorMessage << "Uniform name length exceeds range of uint8_t. File name length: " << tempStrlen << " File name (first " << outputLen << "): " << effectPath.substr(0, outputLen);
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}

			strlength = static_cast<uint8_t>(tempStrlen + 1);
			DWORD bytesToWrite = 1;
			if (!WriteBufferToFile(i_path, fileHandle, &strlength, bytesToWrite))
			{
				wereThereErrors = true;
				goto OnExit;
			}

			bytesToWrite = strlength;
			if (!WriteBufferToFile(i_path, fileHandle, uniformNames[i], bytesToWrite))
			{
				wereThereErrors = true;
				goto OnExit;
			}
		}

		bytesToWrite = (DWORD)sizeof(uint32_t);
		if (!WriteBufferToFile(i_path, fileHandle, &textureSize, bytesToWrite))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		for (size_t i = 0; i < textureSize; i++)
		{
			tempStrlen = static_cast<unsigned int>(strlen(textureNames[i]));
			//Output error if strlength can not be hold in a uint8_t
			if (tempStrlen + 1 > 0xFF)
			{
				unsigned int outputLen = 50;
				std::stringstream errorMessage;
				errorMessage << "Texture name length exceeds range of uint8_t. Variable name length: " << tempStrlen << " File name (first " << outputLen << "): " << effectPath.substr(0, outputLen);
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}

			strlength = static_cast<uint8_t>(tempStrlen + 1);
			DWORD bytesToWrite = 1;
			if (!WriteBufferToFile(i_path, fileHandle, &strlength, bytesToWrite))
			{
				wereThereErrors = true;
				goto OnExit;
			}

			bytesToWrite = strlength;
			if (!WriteBufferToFile(i_path, fileHandle, textureNames[i], bytesToWrite))
			{
				wereThereErrors = true;
				goto OnExit;
			}

			tempStrlen = static_cast<unsigned int>(strlen(texturePaths[i]));
			//Output error if strlength can not be hold in a uint8_t
			if (tempStrlen + 1 > 0xFF)
			{
				unsigned int outputLen = 50;
				std::stringstream errorMessage;
				errorMessage << "Texture path length exceeds range of uint8_t. File name length: " << tempStrlen << " File name (first " << outputLen << "): " << effectPath.substr(0, outputLen);
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				return false;
			}

			strlength = static_cast<uint8_t>(tempStrlen + 1);
			DWORD bytesToWrite = 1;
			if (!WriteBufferToFile(i_path, fileHandle, &strlength, bytesToWrite))
			{
				wereThereErrors = true;
				goto OnExit;
			}

			bytesToWrite = strlength;
			if (!WriteBufferToFile(i_path, fileHandle, texturePaths[i], bytesToWrite))
			{
				wereThereErrors = true;
				goto OnExit;
			}
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

	bool BuildMaterialFromFile(const char* const i_source, const char* const i_target)
	{
		bool wereThereErrors = false;

		if (!LoadMaterial(i_source))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		if (!WriteMaterial(i_target))
		{
			wereThereErrors = true;
			goto OnExit;
		}

	OnExit:

		// Clean up global strings
		effectPath.clear();
		if (uniforms != NULL)
		{
			free(uniforms);
		}

		if (uniformNames != NULL)
		{
			for (size_t i = 0; i < uniformSize; i++)
			{
				if (uniformNames[i] != NULL)
				{
					free(uniformNames[i]);
				}
			}
			free(uniformNames);
		}

		uniforms = NULL;
		uniformSize = 0;

		if (textureNames != NULL)
		{
			for (size_t i = 0; i < textureSize; i++)
			{
				if (textureNames[i] != NULL)
				{
					free(textureNames[i]);
				}
			}
			free(textureNames);
		}

		textureNames = NULL;

		if (texturePaths != NULL)
		{
			for (size_t i = 0; i < textureSize; i++)
			{
				if (texturePaths[i] != NULL)
				{
					free(texturePaths[i]);
				}
			}
			free(texturePaths);
		}

		texturePaths = NULL;

		textureSize = 0;
		return !wereThereErrors;
	}
}