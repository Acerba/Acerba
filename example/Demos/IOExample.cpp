//IO demo
#include <Ace\Ace.h>

int main()
{
	//Initialize ACE-engine
	ace::Init();

	//First we load and print some text

	//For this demo we have one long path, so we shorten it
	//One for reading existing file
	ace::Path readTextPath = "assets/ReadTestTextFile.txt";
	//One for creating a new file and re-writing it
	ace::Path writeTextPath = "assets/WriteTestTextFile.txt";

	////////////////////
	/*	ATTENTION
	Usually the path would be: build/Bin/Debug in the debug mode
	In relase it would be: build/Bin/Relase
	*/
	////////////////////

	//First we load and print existing text file
	//Check if file exists (path/"filename")
	if (ace::File::Exists(readTextPath))
	{
		//Load existing text file (path/"filename")
		ace::File readTextFile(readTextPath);

		//Read textFile to buffer
		auto textBuffer = readTextFile.ReadAllText();

		//Print text file to command window from the buffer
		printf(textBuffer.get());
	}
	else
	{
		//Print error message
		printf("Error, file does not exist\n");
	}

	//File should not exist so we make a new one
	//	(path/"filename", write command)
	ace::File writeTextFile(writeTextPath, "w");

	//Make a text to write (two methods)
	//Make a constant char
	char* textToWrite = { "This is one way to write the file. \n" };

	//Write the text in const char, or..
	writeTextFile.Write(textToWrite);
	//write it straight
	writeTextFile.Write("This is also a way to write the file.\n");

	//Here we load an imagefile to memory
	//Check if file exists (You can use the method before to shorten this, or you can do it the hard way)
	if (ace::File::Exists("assets/TestImageFile.png"))
	{
		//Load image file
		ace::Image testImage(ace::File("assets/TestImageFile.png"));

		//But we have no window to show it, that will be done in other example/demo
	}

	// Shutdown ACE-engine
	ace::Quit();

	return 0;
}