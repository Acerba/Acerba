#pragma once

#include <Ace/Shader.h>
#include <Ace/Types.h>
#include <Ace/MaterialFlags.h>

#include <memory>
#include <vector>

namespace ace
{
	enum class UniformType
	{
		Int32,
		UInt32,
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat2,
		Mat3,
		Mat4,
	};

	struct Material
	{
		struct MaterialImpl;
		std::shared_ptr<MaterialImpl> impl;

		MaterialFlags flags;

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


		void Uniform(const char* name, const UInt32&);
		void Uniform(const char* name, const Int32&);
		void Uniform(const char* name, const float&);

		void Uniform(const char* name, const Vector2&);
		void Uniform(const char* name, const Vector3&);
		void Uniform(const char* name, const Vector4&);
		void Uniform(const char* name, const Color32&);

		void Uniform(const char* name, const Matrix2&);
		void Uniform(const char* name, const Matrix3&);
		void Uniform(const char* name, const Matrix4&);



		void Uniform(const char* name, const std::vector<UInt32>&);
		void Uniform(const char* name, const std::vector<Int32>&);
		void Uniform(const char* name, const std::vector<float>&);

		void Uniform(const char* name, const std::vector<Vector2>&);
		void Uniform(const char* name, const std::vector<Vector3>&);
		void Uniform(const char* name, const std::vector<Vector4>&);
		void Uniform(const char* name, const std::vector<Color32>&);

		void Uniform(const char* name, const std::vector<Matrix2>&);
		void Uniform(const char* name, const std::vector<Matrix3>&);
		void Uniform(const char* name, const std::vector<Matrix4>&);

	};
}