#include <Ace/Path.h>
#include <Ace/IntTypes.h>

#include <SDL_filesystem.h>


namespace ace
{
	//std::string constructor
	Path::Path(std::string p, bool isAbsolute)
	{
		if (!isAbsolute)
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
		else
		{
			m_data_path = p;
		}
	}

	//Const char constructor
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

	//Returns path as std::string
	std::string Path::GetPath() const
	{
		return m_data_path;
	}

    std::string Path::GetBasePath() const
    {
        const char* path = m_data_path.c_str();
        UInt16 fileTypePos = 0;

        // Find '.' in the path
        while (*(path + fileTypePos++) != '.')
        {

        }

        // Find file name
        while (*(path + fileTypePos) != '/' && *(path + fileTypePos) != '\\')
        {
            --fileTypePos;
        }

        return std::string(path, path + (fileTypePos + 1));
    }
}

