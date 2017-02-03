#pragma once

#include <Ace/Shader.h>

#include <memory>

namespace ace
{
	struct Material
	{
		struct MaterialImpl;
		std::shared_ptr<MaterialImpl> impl;

		/**
			@brief Defautl Constructor
			Creates an empty material.
		*/
		Material();

		/**
			@brief Initialization Constructor
			Create material from vertex and fragment shaders.
		*/
		Material(const Shader& vertex, const Shader& fragment);

		/**
			@brief Create Material from vertex and fragment shader.
			@return True if material is valid.
		*/
		bool Init(const Shader& vertex, const Shader& fragment);
	};
}