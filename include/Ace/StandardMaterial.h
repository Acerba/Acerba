#pragma once

#include <Ace/Material.h>
#include <Ace/Color.h>

#include <Ace/Matrix2.h>
#include <Ace/Matrix4.h>

#include <Ace/GraphicsDevice.h>

namespace ace
{
    struct StandardMaterialProperties : public Material::MaterialProperties
    {
        Color32 color;
        Vector2 scale, position;

        float angle;

        math::Matrix4 model;

        mutable Texture diffuse;

        StandardMaterialProperties() : model(math::Matrix4::Identity()), color(0xFFFFFFFF), scale(1, 1), position(0, 0), angle(0)
        {

        }


        inline virtual void Apply(const Material& material) const
        {
            material.Uniform("Color", color);
            material.Uniform("Scale", scale);
            material.Uniform("Position", position);
            material.Uniform("Rotation", math::Matrix2::Rotation(angle));
            material.Uniform("Model", model);
            GraphicsDevice::SetMaterial(material);
            GraphicsDevice::SetTexture(diffuse, "Diffuse", 0);
        }
    };

	class StandardMaterial : public MaterialPropertyWrapper<StandardMaterialProperties>
	{
	public:

		StandardMaterial() : MaterialPropertyWrapper(nullptr, new StandardMaterialProperties())
		{
			Init();
			Apply();
		}

		~StandardMaterial()
		{
            
		}


	protected:

		virtual void Init() const
		{
			Shader vert, frag;
			vert.Create(
			"#version 100													\n"
			"attribute vec4 a_position;										\n"
			"attribute vec2 a_uv;											\n"
			"attribute vec4 a_color;										\n"
			"																\n"
			"varying vec4 o_c;												\n"
			"varying vec2 o_uv;												\n"
			"																\n"
			"uniform vec2 Position;											\n"
			"uniform vec2 Scale;											\n"
			"uniform mat2 Rotation; // 2D									\n"		
			"uniform mat4 Model;											\n"
			"uniform mat4 M[64];											\n"
			"uniform mat4 VP;												\n"
			"																\n"
			"void main()													\n"
			"{																\n"
			"	o_c = a_color;												\n"
			"	o_uv = a_uv;												\n"
			"																\n"
			"	vec4 pos = vec4(a_position.xyz, 1);							\n"
			"	pos.xy = Rotation * pos.xy;									\n"
			"	pos.x *= Scale.x;											\n"
			"   pos.y *= Scale.y;											\n"	
			"	pos.xy += Position.xy;										\n"
			"																\n"
            "	gl_Position = VP * M[int(a_position.w)] * Model * pos;	    \n"
			"}																\n"
			, ShaderType::Vertex);											
																			
																			
			frag.Create(													
			"#version 100													\n"
			"precision lowp float;											\n"
			"varying lowp vec4 o_c;											\n"
			"varying lowp vec2 o_uv;										\n"
			"																\n"
			"uniform sampler2D Diffuse;										\n"
			"uniform lowp vec4 Color;										\n"
			"																\n"
			"																\n"
			"void main()													\n"
			"{																\n"
			"	gl_FragColor = texture2D(Diffuse, o_uv) * o_c;				\n"
			"}																\n"
			, ShaderType::Fragment);

			InitImpl(GraphicsDevice::CreateMaterial(vert, frag));
			
			if (!(*this)->diffuse)
			{
                (*this)->diffuse.Create(Image(Color(255, 255, 255, 255)));
			}
		}
	};
}