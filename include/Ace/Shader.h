#pragma once

#include <Ace/File.h>

#include <Ace/GraphicsObject.h>

namespace ace
{
	/**
		@brief Shader Type
	*/
	enum class ShaderType
	{
		Vertex,
		Fragment,
	};

	struct Shader : public GraphicsObject<struct ShaderImpl>
	{
		ShaderType type;

		/**
			@brief Default Constructor
			Creates an empty shader.
		*/
		Shader(ShaderImpl* impl = nullptr);

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
		bool Create(const char* src, ShaderType type);

	protected:
		virtual void Init() const;
	};
}