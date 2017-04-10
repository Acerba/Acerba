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

		bool Parse(const bool isFile, std::string& str);
		
		void Write(Path path);

	};

}