#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Path.h>
#include <memory>

namespace ace
{
	class File
	{
		struct FileImpl;
	public:
		/**
			@return True if file exists
		*/
		static bool Exists(const Path& path);

		/**
			@brief File constructor
			@param[in] path Relative path
			@param[in] mode r, w, a, r+, w+, a+
		*/
		File(const Path& path, const char* mode = "r");
		~File();

		/**
			@return File size
		*/
		UInt32 Size() const;
		/**
			@brief Read data from file to buffer
			@param[in, out] buffer Pointer to buffer where data will be saved to
			@param[in] size Size of the data in bytes
			@return True if succesful
		*/
		bool Read(UInt8* buffer, UInt32 size) const;

		/**
			@brief Read all data from file to buffer
			@return Buffer with data in it
		*/
		std::shared_ptr<UInt8> ReadAll() const;

		/**
			@brief Read all text data from file to buffer
			@return Buffer with data in it
		*/
		std::shared_ptr<char> ReadAllText() const;

		//TODO: Error handler loggerilla, sekä muu palautus kuin true
		/**
			@brief Write text to end of a file
			@param[in,out] buffer Pointer to file where data will be taken from
			@param[in,out] size Size of the file in buffer
			@param[in,out] text Text to write in to the file
			@return True if succesful
		*/
		bool Write(const char* text);

		/**
			@brief Write certain amount of text from known buffer to a file
			@param buffer Buffer that contains writable data
			@param size Size of the data to write
			@return True if succesful
		*/
		bool Write(UInt8* buffer, UInt32 size);


	private:
		std::shared_ptr<FileImpl> m_fileImpl;

	};
}