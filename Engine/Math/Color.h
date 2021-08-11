#pragma once
#include <iostream>
#include <string>
#include <SDL.h>

namespace nc
{
	struct Color
	{
		float r, g, b; 

		Color() : r{0}, g{0}, b{0} {}
		Color(float r, float g, float b) : r{r}, g{g}, b{b} {} 
		Color(std::uint32_t rgb) // DWORD ( x | b | g | r )
		{
			r = (rgb & 0xff) / 255.0f;          // 0 - 255
			g = ((rgb >> 8) & 0xff) / 255.0f;   // 0 - 255
			b = ((rgb >> 16) & 0xff) / 255.0f;  // 0 - 255
		}

		Color operator + (const Color& color) { return {r + color.r, g + color.g, b + color.b}; }
		Color operator - (const Color& color) { return {r - color.r, g - color.g, b - color.b}; }
		Color operator * (float s) const { return { r * s, g * s, b * s }; } 
		operator std::uint32_t() const { return ToRGB(); }

		friend std::istream& operator >> (std::istream& stream, Color& c);

		std::uint32_t ToRGB() const
		{
			std::uint8_t red = static_cast<std::uint8_t>(r) * 255;
			std::uint8_t green = static_cast<std::uint8_t>(g) * 255;
			std::uint8_t blue = static_cast<std::uint8_t>(b) * 255;

			return (red | (green << 8) | (blue << 16)); 
		}

		operator SDL_Color() const
		{
			SDL_Color color; 
			color.r = static_cast<Uint8>(r * 255); 
			color.g = static_cast<Uint8>(g * 255); 
			color.b = static_cast<Uint8>(b * 255); 
			color.a = 255; 

			return color; 
		}

		static const Color white; 
		static const Color red;
		static const Color green;
		static const Color blue;
		static const Color purple;
		static const Color cyan;
		static const Color orange;
		static const Color yellow;
		static const Color black;
	};
}