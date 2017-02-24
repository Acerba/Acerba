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
		Clamp,
	};

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