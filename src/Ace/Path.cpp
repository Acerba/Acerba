#include <Ace\Path.h>
#include <SDL_filesystem.h>
#include <iostream>

namespace ace
{
	///Sdt::string konstruktor
	Path::Path(std::string p)
	{
		char *base_path = SDL_GetBasePath();
		if (base_path)	
		{
			m_data_path = base_path + p;
			SDL_free(base_path);
		}
		else
		{
			m_data_path = p;
		}
	}

	///Const char konstruktor
	Path::Path(const char* c)
	{
		//This changes const char to std::string
		std::string p(c);

		char *base_path = SDL_GetBasePath();
		if (base_path)
		{
			m_data_path = base_path + p;
			SDL_free(base_path);
		}
		else
		{
			m_data_path = p;
		}
	}

	Path::~Path()
	{

	}

	///Returns path in string
	std::string Path::GetPath() const
	{
		return m_data_path;
	}
}

