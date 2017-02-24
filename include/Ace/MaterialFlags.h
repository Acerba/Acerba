namespace ace
{
	enum class BlendModes
	{
		Zero,
		One,
		SrcColor,
		OneMinusSrcColor,
		DstColor,
		OneMinusDstColor,
		SrcAlpha,
		OneMinusSrcAlpha,
		DstAlpha,
		OneMinusDstAlpha
	};
	
	enum class TestFlags
	{
		Always,
		Never,
		Less,
		Equal,
		LEqual,
		Greater,
		NotEqual,
		GEqual
	};

	enum class StencilOps
	{
		Keep,
		Zero,
		Replace,
		Incr,
		IncrWrap,
		Decr,
		DecrWrap,
		Invert
	};

	enum class CullingMode
	{
		Back,
		Front,
		Both
	};

	struct MaterialFlags
	{
		BlendModes blendModesSrc;
		BlendModes blendModesDst;
		TestFlags depthFlags;
		CullingMode cullingMode;

		MaterialFlags() : blendModesSrc(BlendModes::SrcAlpha), blendModesDst(BlendModes::OneMinusSrcAlpha), depthFlags(TestFlags::LEqual), cullingMode(CullingMode::Back)
		{

		}
	};

}