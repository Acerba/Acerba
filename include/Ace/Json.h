#pragma once

#include <Ace/File.h>
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>

namespace ace
{
	class Json
	{
	public:

        typedef rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> Document;
		//rapidjson::Document document;

        Document document;

		Json();

		// TODO: ParseString method.

		bool ParseString(const std::string& string);
		
		bool Parse(const File& file);
		
		void Write(Path path);

	};

}