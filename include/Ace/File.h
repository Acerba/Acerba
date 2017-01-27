#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Path.h>
#include <memory>

namespace ace
{
	class File
	{
		struct FileImpl;
	public:
		File(const Path& path, const char* mode = "r");
		~File();

		UInt32 Size() const;
		bool Read(UInt8* buffer, UInt32 size) const;
		std::shared_ptr<UInt8> ReadAll() const;
		bool Write(UInt8* buffer, UInt32 size, const char* text);

	private:
		std::shared_ptr<FileImpl> m_fileImpl;

	};
}