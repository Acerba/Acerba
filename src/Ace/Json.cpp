#include <ace/Json.h>

namespace ace
{

	Json::Json()
	{
		
	}


	bool Json::Parse(const File& file)
	{
		auto* data = file.ReadAll().get();
        std::string str;

        //Would this work instead of loop
        //str = *data;
		
		str.reserve(file.Size());
		for (UInt32 i = 0u; i < file.Size(); ++i)
		{
			str.push_back(data[i]);
		}
		
		document.Parse(str.c_str());
		return !document.HasParseError();
	}

	bool Json::ParseString(const std::string& string)
	{
		document.Parse(string.c_str());
		return !document.HasParseError();
	}


	void Json::Write(Path path)
	{
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);

		File f(path, "w");
		f.Write(buffer.GetString());
	}
}
