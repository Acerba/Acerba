#pragma once

#include <Ace/Shader.h>
#include <Ace/Types.h>
#include <Ace/MaterialFlags.h>
#include <Ace/GraphicsObject.h>
#include <Ace/Texture.h>

#include <vector>

namespace ace
{
	/**
	@brief Uniform Types
	*/
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

	/**
	@brief Graphics Material
	*/
	struct Material : public GraphicsObject<struct MaterialImpl>
	{
		
		MaterialFlags flags;
		
		static const UInt32 MAX_TEXTURES = 4;

		char textureNames[MAX_TEXTURES][16];
		Texture textures[MAX_TEXTURES];

		/**
			@brief Defautl Constructor
			Creates an empty material.
		*/
		Material(MaterialImpl* impl = nullptr);

		/**
			@brief Initialization Constructor
			Creates material from vertex and fragment shaders.
		*/
		Material(const Shader& vertex, const Shader& fragment);

		/**
			@brief Create Material from vertex and fragment shader.
			@return True if material is valid.
		*/
		bool Init(const Shader& vertex, const Shader& fragment);

		void AddTexture(const Texture& texture, const char* name, UInt8 id);

		void Uniform(const char* name, const UInt32&) const;
		void Uniform(const char* name, const Int32&) const;
		void Uniform(const char* name, const float&) const;

		void Uniform(const char* name, const Vector2&) const;
		void Uniform(const char* name, const Vector3&) const;
		void Uniform(const char* name, const Vector4&) const;
		void Uniform(const char* name, const Color32&) const;

		void Uniform(const char* name, const Matrix2&) const;
		void Uniform(const char* name, const Matrix3&) const;
		void Uniform(const char* name, const Matrix4&) const;

		void Uniform(const char* name, const UInt32*, UInt32 count) const;
		void Uniform(const char* name, const Int32*, UInt32 count) const;
		void Uniform(const char* name, const float*, UInt32 count) const;

		void Uniform(const char* name, const Vector2*, UInt32 count) const;
		void Uniform(const char* name, const Vector3*, UInt32 count) const;
		void Uniform(const char* name, const Vector4*, UInt32 count) const;
		void Uniform(const char* name, const Color32*, UInt32 count) const;

		void Uniform(const char* name, const Matrix2* , UInt32 count) const;
		void Uniform(const char* name, const Matrix3* , UInt32 count) const;
		void Uniform(const char* name, const Matrix4* , UInt32 count) const;

		/**
			@brief
		*/
		inline virtual void Apply()
		{

		}

	protected:
		virtual void Init() const;
	};
}