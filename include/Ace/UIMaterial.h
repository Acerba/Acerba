#pragma once

#include <Ace/Material.h>
#include <Ace/Color.h>

#include <Ace/Matrix2.h>
#include <Ace/Matrix4.h>

#include <Ace/GraphicsDevice.h>
#include <Ace/StandardMaterial.h>

namespace ace
{
	class UIMaterial final : public MaterialPropertyWrapper<StandardMaterialProperties>
	{
	public:

		UIMaterial() : MaterialPropertyWrapper(nullptr, new StandardMaterialProperties())
		{
			Init();
			Apply();
		}

		~UIMaterial()
		{
            
		}


	protected:

		virtual void Init() const final override
		{
			Shader vert, frag;
			vert.Create(
			"#version 100													\n"
			"attribute vec4 a_position;										\n"
			"attribute vec3 a_uv;											\n"
			"attribute vec4 a_color;										\n"
			"varying vec4 o_c;												\n"
			"varying vec2 o_uv;												\n"
			"uniform vec2 Position;											\n"
			"uniform vec2 Scale;											\n"
			"uniform mat2 Rotation; // 2D									\n"		
			"uniform mat4 Model;											\n"			
			"uniform mat4 VP;												\n"
			"uniform mat4 V;												\n"
			"uniform mat4 Screen;											\n"
			"void main()													\n"
			"{																\n"
			"	o_c = a_color;												\n"
			"	o_uv = a_uv.xy;												\n"
			"	vec4 pos = vec4(a_position.xyz, 1);							\n"
			"	pos.xy = Rotation * pos.xy;									\n"
			"	pos.x *= Scale.x;											\n"
			"   pos.y *= Scale.y;											\n"	
			"	pos.xy += Position.xy;										\n"
            "	gl_Position = Screen * Model * pos;							\n"
			"}																\n"
			, ShaderType::Vertex);											
																			
																			
			frag.Create(													
			"#version 100													\n"
			"precision lowp float;											\n"
			"varying lowp vec4 o_c;											\n"
			"varying lowp vec2 o_uv;										\n"
			"uniform sampler2D Diffuse;										\n"
			"uniform lowp vec4 Color;										\n"
			"void main()													\n"
			"{																\n"
			"	gl_FragColor = texture2D( Diffuse, o_uv.st) * o_c * Color;	\n"
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