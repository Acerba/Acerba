#include <ace/Json.h>

namespace ace
{

	Json::Json()
	{
		
	}


	bool Json::Parse(const File& file)
	{
		File f = file;
		auto data = f.ReadAll();
		auto* d = data.get();
		std::string str;
		
		str.reserve(f.Size());
		for (UInt32 i = 0u; i < f.Size(); ++i)
		{
			str.push_back(d[i]);
		}
		
		document.Parse(str.c_str());
		return !document.HasParseError();
	}

	bool Json::ParseString(const std::string& string)
	{
		std::string str = string;

		document.Parse(str.c_str);
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
