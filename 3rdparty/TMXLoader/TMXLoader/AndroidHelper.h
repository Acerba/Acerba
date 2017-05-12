#pragma once

#if defined(__ANDROID__) || defined(ANDROID)

#include <sstream>
#include <string>
#include <cstdlib>

	//inline int atoi(const char* buffer)
	//{
	//	std::stringstream reader(buffer);
	//		
	//	int val;
	//	reader >> val;
	//	
	//	return val;
	//}

namespace std
{
	template <typename T>
	inline std::string to_string(T value)
	{
		std::ostringstream os ;
		os << value ;
		return os.str() ;
	}
	
	inline unsigned int stoi(const std::string& buffer, int = 0, int base = 10)
	{
		std::stringstream reader;
		
		if(base == 16)
		{
			reader << std::hex << buffer;
		}
		else
		{
			reader << buffer;
		}
		
		unsigned int val;
		reader >> val;
		
		return val;
	}
	
	
	inline unsigned long stoul(const std::string& buffer, int = 0, int base = 10)
	{
		std::stringstream reader;
		
		if(base == 16)
		{
			reader << std::hex << buffer;
		}
		else
		{
			reader << buffer;
		}
		
		unsigned long val;
		reader >> val;
		
		return val;
	}
}

#endif