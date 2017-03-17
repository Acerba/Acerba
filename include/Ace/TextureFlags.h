namespace ace
{
	/**
		@brief Filtering Modes
	*/
	enum class FilteringModes : UInt8
	{
		Nearest,	/** Nearest Filtering  */
		Linear,		/** Linear Filtering   */
	};


	/**
		@brief Wrap modes
	*/
	enum class WrapModes : UInt8
	{
		Repeat,		/** Repeat Texture UV  */
		Clamp,		/** Clamp Texture UV   */
	};

	/**
		@brief texture Flags
	*/
	struct TextureFlags
	{
		FilteringModes minFiltering;
		FilteringModes magFiltering;
		WrapModes wrapModes;
		bool mipmaps;

		TextureFlags() : minFiltering(FilteringModes::Nearest), magFiltering(FilteringModes::Nearest), wrapModes(WrapModes::Repeat), mipmaps(false)
		{

		}
	};

}