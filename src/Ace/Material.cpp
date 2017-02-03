#include <Ace/Material.h>
#include <Ace/GraphicsDevice.h>

namespace ace
{
	Material::Material() : impl(nullptr)
	{

	}

	Material::Material(const Shader& vertex, const Shader& fragment)
	{
		Init(vertex, fragment);
	}

	bool Material::Init(const Shader& vertex, const Shader& fragment)
	{
		*this = GraphicsDevice::CreateMaterial(vertex, fragment);
		return impl != nullptr;
	}
}