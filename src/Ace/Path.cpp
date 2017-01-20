#include <Ace\Path.h>
#include <SDL_filesystem.h>
#include <iostream>

namespace ace
{
	Path::Path(std::string p)
	{
		char *base_path = SDL_GetBasePath();
		if (base_path)	
		{
			m_data_path = base_path + p;
			SDL_free(base_path);

			printf(m_data_path.c_str());
		}
		else
		{
			m_data_path = p;
		}
	}

	Path::~Path()
	{

	}

	//const char* Path::GetPath()
	//{
	//	return m_data_path;
	//}

	std::string Path::GetPath()
	{
		return m_data_path;
	}
}

