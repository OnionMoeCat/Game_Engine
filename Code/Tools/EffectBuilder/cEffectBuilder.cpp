// Header Files
//=============

#include "cEffectBuilder.h"
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
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	uint8_t renderStates;

	namespace RenderStates
	{
		const uint8_t ALPHA = 1 << 0;
		const uint8_t DEPTHTEST = 1 << 1;
		const uint8_t DEPTHWRITE = 1 << 2;
		const bool DEFAULTALPHA = false;
		const bool DEFAULTDEPTHTEST = true;
		const bool DEFAULTDEPTHWRITE = true;
	}
}

// Helper Function Declarations
//=============================

namespace
{
	bool LoadTableValues(lua_State& io_luaState);

	bool WriteBufferToFile(const char* const i_path, HANDLE i_fileHandle, const void* i_buffer, DWORD i_bytesToWrite);

	bool LoadEffect(const char* const i_path);
	bool WriteEffect(const char* const i_path);

	bool BuildEffectFromFile(const char* const i_source, const char* const i_target);
}

// Interface
//==========

// Build
//------

bool eae6320::cEffectBuilder::Build( const std::vector<std::string>& )
{
	bool wereThereErrors = false;

	// Copy the source to the target
	{
		const bool dontFailIfTargetAlreadyExists = false;
		const bool updateTheTargetFileTime = true;
		std::string errorMessage;
		if ( !BuildEffectFromFile( m_path_source, m_path_target ) )
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
		// Get the value of "vertex_shader_path"
		{
			const char* const key = "vertex_shader_path";
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

			if (lua_type(&io_luaState, -1) != LUA_TSTRING)
			{
				std::stringstream errorMessage;
				errorMessage << "The value for \"" << key << "\" must be a string "
					"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			{
				vertexShaderPath = lua_tostring(&io_luaState, -1);
				lua_pop(&io_luaState, 1);
			}
		}

		// Get the value of "vertex_shader_path"
		{
			const char* const key = "fragment_shader_path";
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

			if (lua_type(&io_luaState, -1) != LUA_TSTRING)
			{
				std::stringstream errorMessage;
				errorMessage << "The value for \"" << key << "\" must be a string "
					"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				lua_pop(&io_luaState, 1);
				return false;
			}

			{
				fragmentShaderPath = lua_tostring(&io_luaState, -1);
				lua_pop(&io_luaState, 1);
			}
		}

		renderStates = 0;

		// Get the value of "alpha_transparency"
		{
			const char* const key = "alpha_transparency";
			lua_pushstring(&io_luaState, key);

			{
				const int currentIndexOfTable = -2;
				lua_gettable(&io_luaState, currentIndexOfTable);
			}

			bool isBitOn = RenderStates::DEFAULTALPHA;

			if (!lua_isnil(&io_luaState, -1))
			{
				if (!lua_isboolean(&io_luaState, -1))
				{
					std::stringstream errorMessage;
					errorMessage << "The value for \"" << key << "\" must be a boolean "
						"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
					eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					lua_pop(&io_luaState, 1);
					return false;
				}

				isBitOn = lua_toboolean(&io_luaState, -1) != 0;
			}

			if (isBitOn)
			{
				renderStates |= RenderStates::ALPHA;
			}
			lua_pop(&io_luaState, 1);
		}

		// Get the value of "depth_testing"
		{
			const char* const key = "depth_testing";
			lua_pushstring(&io_luaState, key);

			{
				const int currentIndexOfTable = -2;
				lua_gettable(&io_luaState, currentIndexOfTable);
			}

			bool isBitOn = RenderStates::DEFAULTDEPTHTEST;

			if (!lua_isnil(&io_luaState, -1))
			{
				if (lua_type(&io_luaState, -1) != LUA_TBOOLEAN)
				{
					std::stringstream errorMessage;
					errorMessage << "The value for \"" << key << "\" must be a boolean "
						"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
					eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					lua_pop(&io_luaState, 1);
					return false;
				}

				isBitOn = lua_toboolean(&io_luaState, -1) != 0;
			}

			if (isBitOn)
			{
				renderStates |= RenderStates::DEPTHTEST;
			}
			lua_pop(&io_luaState, 1);
		}

		// Get the value of "depth_writing"
		{
			const char* const key = "depth_writing";
			lua_pushstring(&io_luaState, key);

			{
				const int currentIndexOfTable = -2;
				lua_gettable(&io_luaState, currentIndexOfTable);
			}

			bool isBitOn = RenderStates::DEFAULTDEPTHWRITE;

			if (!lua_isnil(&io_luaState, -1))
			{
				if (lua_type(&io_luaState, -1) != LUA_TBOOLEAN)
				{
					std::stringstream errorMessage;
					errorMessage << "The value for \"" << key << "\" must be a boolean "
						"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
					eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					lua_pop(&io_luaState, 1);
					return false;
				}

				isBitOn = lua_toboolean(&io_luaState, -1) != 0;
			}

			if (isBitOn)
			{
				renderStates |= RenderStates::DEPTHWRITE;
			}
			lua_pop(&io_luaState, 1);
		}

		return true;
	}

	bool LoadEffect(const char* const i_path)
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

	bool WriteEffect(const char* const i_path)
	{
		bool wereThereErrors = false;
		//Side Effect: Will cause a error: File_ALREADY_EXISTS. But it should be good because we only get error when something really fail. 
		//QUESTION: Is it really ok?
		HANDLE fileHandle = CreateFile(i_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		
		if (fileHandle == INVALID_HANDLE_VALUE)
		{
			std::stringstream errorMessage;
			errorMessage << "Fail to create file \"" << i_path << "\": " << eae6320::GetLastWindowsError();
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			return false;
		}

		unsigned int tempStrlen = static_cast<unsigned int>(vertexShaderPath.length());
		//Output error if strlength can not be hold in a uint8_t
		if (tempStrlen + 1 > 0xFF)
		{
			unsigned int outputLen = 50;
			std::stringstream errorMessage;
			errorMessage << "File name length exceeds range of uint8_t. File name length: " << tempStrlen << " File name (first "<< outputLen << "): " << vertexShaderPath.substr(0, outputLen);
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
		if (!WriteBufferToFile(i_path, fileHandle, vertexShaderPath.c_str(), bytesToWrite))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		tempStrlen = static_cast<unsigned int>(fragmentShaderPath.length());
		//Output error if strlength can not be hold in a uint8_t
		if (tempStrlen + 1 > 0xFF)
		{
			unsigned int outputLen = 50;
			std::stringstream errorMessage;
			errorMessage << "File name length exceeds range of uint8_t. File name length: " << tempStrlen << " File name (first " << outputLen << "): " << fragmentShaderPath.substr(0, outputLen);
			eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			return false;
		}

		strlength = static_cast<uint8_t>(tempStrlen + 1);
		bytesToWrite = 1;
		if (!WriteBufferToFile(i_path, fileHandle, &strlength, bytesToWrite))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		bytesToWrite = strlength;
		if (!WriteBufferToFile(i_path, fileHandle, fragmentShaderPath.c_str(), bytesToWrite))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		bytesToWrite = sizeof(uint8_t);
		if (!WriteBufferToFile(i_path, fileHandle, &renderStates, bytesToWrite))
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

	bool BuildEffectFromFile(const char* const i_source, const char* const i_target)
	{
		bool wereThereErrors = false;

		if (!LoadEffect(i_source))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		if (!WriteEffect(i_target))
		{
			wereThereErrors = true;
			goto OnExit;
		}

	OnExit:

		// Clean up global strings
		vertexShaderPath.clear();
		fragmentShaderPath.clear();
		renderStates = 0;

		return !wereThereErrors;
	}
}