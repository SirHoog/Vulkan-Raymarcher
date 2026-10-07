#pragma once

#include <SDL3/SDL_pixels.h>

namespace VK_RM
{
	namespace UTILS
	{
		namespace COLORS
		{
			inline constexpr SDL_Color WHITE      { 255, 255, 255, 255 }; // White
			inline constexpr SDL_Color BLACK      {   0,   0,   0, 255 }; // Black
			inline constexpr SDL_Color GRAY       { 130, 130, 130, 255 }; // Gray
			inline constexpr SDL_Color LIGHTGRAY  { 200, 200, 200, 255 }; // Light Gray
			inline constexpr SDL_Color DARKGRAY   {  80,  80,  80, 255 }; // Dark Gray
			inline constexpr SDL_Color TRANSPARENT{   0,   0,   0,   0 }; // Transparent
			inline constexpr SDL_Color RED        { 230,  41,  55, 255 }; // Red
			inline constexpr SDL_Color ORANGE     { 255, 161,   0, 255 }; // Orange
			inline constexpr SDL_Color YELLOW     { 253, 249,   0, 255 }; // Yellow
			inline constexpr SDL_Color GREEN      {   0, 228,  48, 255 }; // Green
			inline constexpr SDL_Color BLUE       {   0, 121, 241, 255 }; // Blue
			inline constexpr SDL_Color VIOLET     { 135,  60, 190, 255 }; // Violet
			inline constexpr SDL_Color PURPLE     { 200, 122, 255, 255 }; // Purple
			inline constexpr SDL_Color PINK       { 255, 109, 194, 255 }; // Pink
			inline constexpr SDL_Color MAROON     { 190,  33,  55, 255 }; // Maroon
			inline constexpr SDL_Color GOLD       { 255, 203,   0, 255 }; // Gold
			inline constexpr SDL_Color BEIGE      { 211, 176, 131, 255 }; // Beige
			inline constexpr SDL_Color LIME       {   0, 158,  47, 255 }; // Lime
			inline constexpr SDL_Color DARKGREEN  {   0, 117,  44, 255 }; // Dark Green
			inline constexpr SDL_Color SKYBLUE    { 102, 191, 255, 255 }; // Sky Blue
			inline constexpr SDL_Color DARKBLUE   {   0,  82, 172, 255 }; // Dark Blue
			inline constexpr SDL_Color DARKPURPLE { 112,  31, 126, 255 }; // Dark Purple
			inline constexpr SDL_Color MAGENTA    { 255,   0, 255, 255 }; // Magenta
			inline constexpr SDL_Color BROWN      { 127, 106,  79, 255 }; // Brown
			inline constexpr SDL_Color DARKBROWN  {  76,  63,  47, 255 }; // Dark Brown
		}
	}
}