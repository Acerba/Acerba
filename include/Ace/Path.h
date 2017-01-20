#pragma once

#include <string>					//Use if operator changer is needed

namespace ace
{
	class Path
	{
	public:
		Path(std::string p);
		~Path();
		/// Returns root path
		//const char* GetPath();
		std::string GetPath();
	private:
		//const char *m_data_path;
		//operator const char* () const	{ return path.c_str(); }	//This would change from string to const char*
		std::string m_data_path;									//This is the root path
		
		operator std::string() const { return m_data_path; }		//This would return string word
		operator const char*() const { return m_data_path.c_str(); }
	};
}