#pragma once

#include <vulkan/vulkan.h>

#include "colors.hpp"

inline constexpr VkClearColorValue SDL_to_Vulkan_Color(SDL_Color color)
{
	return VkClearColorValue({
		color.r / 255.0f,
		color.g / 255.0f,
		color.b / 255.0f,
		color.a / 255.0f
	});
}