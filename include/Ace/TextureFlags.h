namespace ace
{
	enum class FilteringModes
	{
		Nearest,
		Linear,
	};

	enum class WrapModes
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	struct TextureFlags
	{
		FilteringModes filteringModes;
		WrapModes wrapModes;
		bool mipmaps;
	};
}