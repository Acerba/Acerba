#pragma once

#include <string>

namespace ace
{
	class Path
	{
	public:
		Path(std::string p, bool isAbsolute = false);
		Path(const char* c);
		~Path();
		
		std::string GetPath() const;

        inline operator std::string() const { return m_data_path; }
        inline operator const char*() const { return m_data_path.c_str(); }

        std::string GetBasePath() const;

	private:
		std::string m_data_path;
	};
}