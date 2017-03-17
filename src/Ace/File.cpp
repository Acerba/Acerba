#include <Ace/File.h>
#include <SDL_rwops.h>
#include <Ace/Log.h>

#include <iostream>

namespace ace
{
	struct File::FileImpl
	{
		SDL_RWops* rwops;

		//FileImpl konstruktor
		FileImpl(const char* path, const char* mode) : rwops(SDL_RWFromFile(path, mode))
		{
			//	TODO: Error handler loggerilla
			// Warning: if nullptr
			if (rwops == NULL)
			{
				Logger::LogError("File doesn't exist");
			}
		}

		//FileImpl destruktor
		~FileImpl()
		{
			if (rwops)
			{
				SDL_RWclose(rwops);
			}
		}
	};

	bool File::Exists(const Path& path)
	{
		SDL_RWops* file = SDL_RWFromFile(path.GetPath().c_str(), "r");
		
		if (file != nullptr)
		{
			SDL_RWclose(file);
			return true;
		}
		return false;
	}

	File::File(const Path& path, const char* mode) : m_fileImpl(new File::FileImpl(path.GetPath().c_str(), mode))
	{
		
	}

	File::~File()
	{

	}

	//Return buffer size
	UInt32 File::Size() const
	{
		return SDL_RWsize(m_fileImpl->rwops);
	}

	//Read data from file to buffer
	bool File::Read(UInt8* buffer, UInt32 size) const
	{
		SDL_RWread(m_fileImpl->rwops, buffer, size, 1);

		return true;
	}

	//Read all data from file to buffer
	std::shared_ptr<UInt8> File::ReadAll() const
	{
		UInt64 size = SDL_RWsize(m_fileImpl->rwops);
		UInt8* buffer = new UInt8[size + 1];
		SDL_RWread(m_fileImpl->rwops, buffer, size, 1);
		buffer[size] = '\0';
		return std::shared_ptr<UInt8>(buffer);
	}

	std::shared_ptr<char> File::ReadAllText() const
	{
		UInt64 size = SDL_RWsize(m_fileImpl->rwops);
		char* buffer = new char[size + 1];
		SDL_RWread(m_fileImpl->rwops, buffer, size, 1);
		buffer[size] = '\0';
		return std::shared_ptr<char>(buffer);
	}

	//Writing to end of a file
	bool File::Write(const char* text)
	{
		SDL_RWseek(m_fileImpl->rwops, 0, RW_SEEK_END);

		UInt32 text_len = SDL_strlen(text);

		if (SDL_RWwrite(m_fileImpl->rwops, text, text_len, 1) != text_len)
		{
			Logger::LogError("Couldn't fully write string");
		}
		else
		{
			printf("\nWrote %d 1-byte blocks \n", text_len);
		}

		SDL_RWseek(m_fileImpl->rwops, 0, RW_SEEK_SET);
		return true;
	}

	bool File::Write(UInt8* buffer, UInt32 size)
	{
		//Set writing to end of the file
		SDL_RWseek(m_fileImpl->rwops, 0, RW_SEEK_END);

		if (SDL_RWwrite(m_fileImpl->rwops, buffer, size, 1) != size)
		{
			Logger::LogError("Couldn't fully write string");
		}
		else
		{
			printf("\nWrote %d 1-byte blocks \n", size);
		}

		//Set "cursor" to beginning of file
		SDL_RWseek(m_fileImpl->rwops, 0, RW_SEEK_SET);
		return true;
	}
}