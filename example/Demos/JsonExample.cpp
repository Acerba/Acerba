//JSON demo
#include <Ace\Ace.h>
#include <string>
#include <iostream>
int main(int, char**)
{
	ace::Init();
	ace::Path jsonpath = "assets/example.json";
	ace::File jsonfile = jsonpath;
	ace::Json json;
	//const char* jsonexample = " { \"hello\" : \"world\" }";
	//json.ParseString(jsonexample);
	if (!json.Parse(jsonfile)) std::cout << "JSON File has error\n";

	if (json.document.HasMember("Red")) 
	{
		for (auto& itr : json.document.GetObject().MemberBegin()->value.GetArray())
		{
			std::cout << itr.GetInt() << " ";
		}
	}
	
	//printf("hello = %s\n", json.document["hello"].GetString());
}