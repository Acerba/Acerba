#pragma once

#include <Ace/File.h>

#include <memory>

namespace ace
{
	enum class ShaderType
	{
		Vertex,
		Fragment,
	};

	struct Shader
	{
		struct ShaderImpl;

		ShaderType type;
		std::shared_ptr<ShaderImpl> impl;

		/**
			@brief Default Constructor
			Creates an empty shader.
		*/
		Shader();

		/**
			@brief Initialization Constructor
			Create Shader from a file.
		*/
		Shader(const File& file, ShaderType type);

		/**
			@brief Load Shader from a file.
			@return True if shader is valid.
		*/
		bool Load(const File& file, ShaderType type);

		/**
			@brief Create Shader from text.
			@return True if shader is valid.
		*/
		bool Init(const char* src, ShaderType type);
	};
}