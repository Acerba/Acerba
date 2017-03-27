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

		rapidjson::Document document;

		Json();

		//TODO copy constructer
		
		std::string Read(Path path);
		
		void Parse(std::string str);
		
		void Write(Path path);
	
	private:
		rapidjson::Document *ptr;


	};

	/**
		@brief Jameson is proved to be 10000% faster and better than Json.
	*/
	typedef Json Jameson;
	

}