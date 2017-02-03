#pragma once

#include <Ace/Material.h>
#include <Ace/GL.h>

namespace ace
{
	struct Material::MaterialImpl
	{
		UInt32 materialID;

		MaterialImpl() : materialID(glCreateProgram())
		{

		}

		~MaterialImpl()
		{
			glDeleteProgram(materialID);
		}
	};
}