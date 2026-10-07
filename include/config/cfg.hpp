#pragma once

#include <glm/glm.hpp>
#include <SDL3/SDL_video.h>

#include "utils/colors.hpp"

namespace VK_RM
{
	// Global
	using glm::vec3;
	using glm::vec2;

	namespace AppCfg
	{
		inline constexpr Uint32          INIT_SCREEN_WIDTH  = 800;
		inline constexpr Uint32          INIT_SCREEN_HEIGHT = 600;
		inline constexpr SDL_Color       BACKGROUND_COLOR   = UTILS::COLORS::SKYBLUE;
		inline constexpr SDL_WindowFlags WINDOW_FLAGS       = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE;
	}
	namespace RendererCfg
	{
		inline constexpr Uint32 MAX_FRAMES_IN_FLIGHT = 2;
	}
	namespace RayCfg
	{
		static constexpr Uint32 RAYMARCH_STEPS   = 128;
		static constexpr Uint32 BVH_LEAVES       = 4;
		static constexpr float  CLIP_DISTANCE    = 60.0f;
		static constexpr float  UNION_SMOOTHNESS = 0.25f;
	}
	namespace CamCfg
	{
		static constexpr float CAMERA_SPEED         = 5.0f;
		static constexpr float CAMERA_SENSITIVITY   = 0.1f;
		inline constexpr vec3  INIT_CAMERA_POSITION = vec3(0.0f);
		inline constexpr vec3  INIT_CAMERA_TARGET   = vec3(0.0f);
		inline constexpr vec3  CAMERA_UP            = vec3(0.0f, 0.0f, 1.0f); // Z-up
		inline constexpr float CAMERA_FOV           = 60.0f;
	}
}