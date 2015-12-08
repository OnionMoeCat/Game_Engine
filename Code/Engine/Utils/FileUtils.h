#ifndef EAE6320_FILEUTILS_H
#define EAE6320_FILEUTILS_H

namespace eae6320
{
	namespace Utils
	{
		namespace FileUtils
		{
			bool ReadDataFromFile(const char* const i_path, void** i_temporaryBuffer);
		}
	}
}

#endif	// EAE6320_UTILS_H
