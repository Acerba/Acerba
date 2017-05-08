#include <Ace/Material.h>
#include <Ace/GraphicsDevice.h>
#include <Ace/GLMaterialImpl.h>

namespace ace
{
	Material::Material(MaterialImpl* impl) : Graphics(impl)
	{

	}

	Material::Material(const Shader& vertex, const Shader& fragment) : Graphics(nullptr)
	{
		Init(vertex, fragment);
	}

	bool Material::Init(const Shader& vertex, const Shader& fragment)
	{
		*this = GraphicsDevice::CreateMaterial(vertex, fragment);
		return (*this);
	}

	void Material::AddTexture(const Texture& texture, const char* name, UInt8 id)
	{
		textures[id] = texture;
		strcpy(textureNames[id], name);
	}

	void Material::Uniform(const char* name, const UInt32& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::UInt32, 1);
	}

	void Material::Uniform(const char* name, const Int32& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::Int32, 1);
	}

	void Material::Uniform(const char* name, const float& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::Float, 1);
	}
		 
	void Material::Uniform(const char* name, const Vector2& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::Vec2, 1);
	}
	
	void Material::Uniform(const char* name, const Vector3& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::Vec3, 1);
	}
	
	void Material::Uniform(const char* name, const Vector4& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::Vec4, 1);
	}
	
	void Material::Uniform(const char* name, const Color32& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value, UniformType::Vec4, 1);
	}
		 
	void Material::Uniform(const char* name, const Matrix2& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value.array, UniformType::Mat2, 1);
	}
	
	void Material::Uniform(const char* name, const Matrix3& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value.array, UniformType::Mat3, 1);
	}
	
	void Material::Uniform(const char* name, const Matrix4& value) const
	{
		GraphicsDevice::Uniform(*this, name, &value.array, UniformType::Mat4, 1);
	}
	


	void Material::Uniform(const char* name, const std::vector<UInt32>& values) const
	{
		GraphicsDevice::Uniform(*this, name, values.data(), UniformType::UInt32, values.size());
	}
	
	void Material::Uniform(const char* name, const std::vector<Int32>& values) const
	{
		GraphicsDevice::Uniform(*this, name, values.data(), UniformType::Int32, values.size());
	}
	
	void Material::Uniform(const char* name, const std::vector<float>& values) const
	{
		GraphicsDevice::Uniform(*this, name, values.data(), UniformType::Float, values.size());
	}
		
	void Material::Uniform(const char* name, const std::vector<Vector2>& values) const
	{
		GraphicsDevice::Uniform(*this, name, values.data(), UniformType::Vec2, values.size());
	}
	
	void Material::Uniform(const char* name, const std::vector<Vector3>& values) const
	{
		GraphicsDevice::Uniform(*this, name, values.data(), UniformType::Vec3, values.size());
	}
	
	void Material::Uniform(const char* name, const std::vector<Vector4>& values) const
	{
		GraphicsDevice::Uniform(*this, name, values.data(), UniformType::Vec4, values.size());
	}
	
	void Material::Uniform(const char* name, const std::vector<Color32>& values) const
	{
		GraphicsDevice::Uniform(*this, name, values.data(), UniformType::Vec4, values.size());
	}
		
	void Material::Uniform(const char* name, const std::vector<Matrix2>& values) const
	{
		GraphicsDevice::Uniform(*this, name, values.data(), UniformType::Mat2, values.size());
	}
	
	void Material::Uniform(const char* name, const std::vector<Matrix3>& values) const
	{
		GraphicsDevice::Uniform(*this, name, values.data(), UniformType::Mat3, values.size());
	}
	
	void Material::Uniform(const char* name, const std::vector<Matrix4>& values) const
	{
		GraphicsDevice::Uniform(*this, name, values.data(), UniformType::Mat4, values.size());
	}
}