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
	private:
		std::string m_data_path;
		
		operator std::string() const { return m_data_path; }		
		operator const char*() const { return m_data_path.c_str(); }
	};
}