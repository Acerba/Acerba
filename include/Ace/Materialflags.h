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
		Lequal,
		Greater,
		NotEqual,
		Gequal
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
		BlendModes blendModesDst;
		BlendModes blendModesSrc;
		TestFlags depthFlags;
		CullingMode cullingMode;
	};

}