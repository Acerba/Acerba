#pragma once

namespace ace
{
	class Rect
	{
	public:
		/**
			@brief Empty constructor
		*/
		Rect();

		/**
			@brief Constructor for Rect
			@param[in] x Position x
			@paraim[in] y Position y
			@param width Width as float
			@param height Height as float
		*/
		Rect(float x, float y, float width, float height);

		~Rect();

		/**
			@brief Rect parameters: Position x & y, Size width & height in float
		*/
		float x;
		float y;
		float width;
		float height;
	private:


	};
}