#include <ace/Json.h>

namespace ace
{

	Json::Json()
	{
		
	}


	std::string Json::Read(Path path)
	{
		File f(path);

		return f.ReadAllText().get();
	}


	void Json::Parse(std::string str)
	{
		const char* json = str.c_str();

		document.Parse(json);
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
