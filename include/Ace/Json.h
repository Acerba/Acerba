#pragma once

#include <Ace\File.h>
#include <rapidjson\reader.h>
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include <rapidjson\document.h>
#include <iostream>

namespace ace
{
	class Json
	{
	public:
		Json() {};

		std::string read(Path path)
		{
			File f(path);
			
			return f.ReadAllText().get();
		}

		rapidjson::Document Parse(std::string str)
		{
			const char* json = str.c_str();

			rapidjson::Document d;
			d.Parse(json);

			return d;
		}

		void Write(rapidjson::Document d, Path path)
		{
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			File f(path, "w");
			f.Write(buffer.GetString());
		}
	
	
	};

	/**
		@brief Jameson is proved to be 10000% faster and better than Json.
	*/
	typedef Json Jameson;
	

}