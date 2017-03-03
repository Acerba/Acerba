namespace ace
{
	/**
		@brief BlendModes
	*/
	enum class BlendModes
	{
		Zero,					/** Zero Blend				*/
		One,					/** One Blend				*/
		SrcColor,				/** Src Color				*/
		OneMinusSrcColor,		/** One Minus Src Color		*/
		DstColor,				/** Dst Color				*/
		OneMinusDstColor,		/** One Minus Dst Color		*/
		SrcAlpha,				/** Src Alpha				*/
		OneMinusSrcAlpha,		/** One Minus Src Alpha		*/
		DstAlpha,				/** Dst Alpha				*/
		OneMinusDstAlpha		/** One Minus Dst Alpha		*/
	};						
	
	/**
		@brief TestFlags for Depth and Stencil
	*/
	enum class TestFlags
	{											 
		Always,					/** Always			*/
		Never,					/** Never			*/
		Less,					/** Less			*/
		Equal,					/** Equal			*/
		LEqual,					/** Less Equal		*/
		Greater,				/** Greater			*/
		NotEqual,				/** Not Equal		*/
		GEqual					/** Greater Equal	*/
	};

	/**
		@brief Stencil Operators
	*/
	enum class StencilOps
	{											
		Keep,					/**	Keep			*/
		Zero,					/**	Zero			*/
		Replace,				/**	Replace			*/
		Incr,					/**	Increment		*/
		IncrWrap,				/**	Increment Wrap	*/
		Decr,					/**	Decrement		*/
		DecrWrap,				/**	Decrement Wrap	*/
		Invert					/**	Invert			*/
	};

	/**
		@brief Culling Mode
	*/
	enum class CullingMode
	{						   
		Back,					/**	Back Culling	 */
		Front,					/**	Front Culling	 */
		Both					/**	No Culling		 */
	};

	/**
		@brief Material Flags
	*/
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