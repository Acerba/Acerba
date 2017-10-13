#pragma once

#include <Ace/Shader.h>
#include <Ace/Types.h>
#include <Ace/MaterialFlags.h>
#include <Ace/GraphicsObject.h>
#include <Ace/Texture.h>
#include <Ace/Uniform.h>

#include <vector>

namespace ace
{

	/**
	@brief Graphics Material
	*/
	struct Material : public GraphicsObject<struct MaterialImpl>
	{
        struct MaterialProperties
        {
            virtual ~MaterialProperties()
            {

            }

            inline virtual void Apply(const Material& material) const
            {

            }
        };

		MaterialFlags flags;
		
		static const UInt32 MAX_TEXTURES = 4;

		char textureNames[MAX_TEXTURES][16];
		Texture textures[MAX_TEXTURES];

		/**
			@brief Defautl Constructor
			Creates an empty material.
		*/
		Material(MaterialImpl* impl = nullptr, MaterialProperties* properties = nullptr);

		/**
			@brief Initialization Constructor
			Creates material from vertex and fragment shaders.
		*/
		Material(const Shader& vertex, const Shader& fragment, MaterialProperties* properties = nullptr);

		/**
			@brief Create Material from vertex and fragment shader.
			@return True if material is valid.
		*/
		bool Init(const Shader& vertex, const Shader& fragment);

		void AddTexture(const Texture& texture, const char* name, UInt8 id);

		static void Uniform(const char* name, const UInt32&);
		static void Uniform(const char* name, const Int32&);
		static void Uniform(const char* name, const float&);
         
		static void Uniform(const char* name, const Vector2&);
		static void Uniform(const char* name, const Vector3&);
		static void Uniform(const char* name, const Vector4&);
		static void Uniform(const char* name, const Color32&);
         
		static void Uniform(const char* name, const Matrix2&);
		static void Uniform(const char* name, const Matrix3&);
		static void Uniform(const char* name, const Matrix4&);
         
		static void Uniform(const char* name, const UInt32*, UInt32 count);
		static void Uniform(const char* name, const Int32*, UInt32 count);
		static void Uniform(const char* name, const float*, UInt32 count);
         
		static void Uniform(const char* name, const Vector2*, UInt32 count);
		static void Uniform(const char* name, const Vector3*, UInt32 count);
		static void Uniform(const char* name, const Vector4*, UInt32 count);
		static void Uniform(const char* name, const Color32*, UInt32 count);
         
		static void Uniform(const char* name, const Matrix2* , UInt32 count);
		static void Uniform(const char* name, const Matrix3* , UInt32 count);
		static void Uniform(const char* name, const Matrix4* , UInt32 count);

	protected:

        std::shared_ptr<MaterialProperties> m_properties;
		virtual void Init() const;

    public:

        /**
        @brief
        */
        inline void Apply() const
        {
            if (m_properties)
            {
                m_properties->Apply(*this);
            }
        }

        inline void SetProperties(MaterialProperties* properties)
        {
            m_properties.reset(properties);
        }
	};

    template <typename Properties>
    struct MaterialPropertyWrapper : public Material
    {
        MaterialPropertyWrapper(MaterialImpl* impl = nullptr, Material::MaterialProperties* properties = nullptr) : Material(impl, properties)
        {

        }

        inline Properties* operator->()
        {
            return static_cast<Properties*>(m_properties.get());
        }

        inline const Properties* operator->() const
        {
            return static_cast<Properties*>(m_properties.get());
        }
    };
}