#pragma once

#include <Ace/Buffer.h>
#include <Ace/Rect.h>
#include <array>

namespace ace
{

    class Sprite
    {
    public:

		static const UInt8 size = 4u;

		std::array<Vertex, size> vertexData;

		Sprite();

		Sprite(float deg);

        /**
	   @Brief Sprite constructor.
	   @param[in] data Array of 4 vertices by reference.
        */
        Sprite(const Vertex(&data)[4]);

        Sprite(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4);
		
		// TODO: Update all Verticies

		/**
		@Brief Rotate sprite
		@param[in] deg Degrees to rotate in float
		*/
		void Rotate(float deg);

		/**
		@Brief Scale sprite
		@param[in] scale New size in Vector2
		*/
		void Scale(const Vector2& scale);

		/**
		@Brief Move sprite from current location to new location
		@param[in] pos New position in Vector3
		*/
		void Move(const Vector3& pos);

		/**
		@Brief Colorize Sprite
		@param[in] color New sprite color in Color 
		*/
		void Colorize(const Color32& color);

		/**
		@brief Set UV for sprite
		@param[in] uv Rectangle shape 
		*/
		void UVRect(const Rect& uv);
		// Set UV Rect. (Better method name?) 
		
		
		/**
		@brief FlipUV to opposite direction
		@param[in] axis True = x-axis, false = y-axis
		*/
		void FlipUV(const bool axis = true);
		

		// TODO: Update Vertex by Index.
		// void SetUV(UInt8, const Vector2& uv);
		// void SetColor(UInt8 index, const Color& color);
		// void SetPosition(UInt8 index, const Vector2& pos);
		// etc.

		// TODO: Get methods? (Low priority)

		/**
			@Brief Set Vertex UV
			@param[in] color
		*/
		void SetUV(UInt8 index, const Vector2& uv);

		/**
			@Brief Set Vertex Color
			@param[in] color
		*/
		void SetColor(UInt8 index, const Color32& color);
	
		/**
			@Brief Set Vertex Position
			@param[in] color
		*/
		void SetPosition(UInt8 index, const Vector2& position);

		void SetInstanceID(UInt8 id);
    };

}