#pragma once

#include <Volk/volk.h>
#include <vk_mem_alloc.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <SDL3/SDL_hints.h>

#include <slang/slang.h>
#include <slang/slang-com-ptr.h>

#include "config/cfg.hpp"
#include "utils/debug.hpp"

namespace VK_RM
{
	struct Renderer
	{
		SDL_Window* window = nullptr;

		Renderer() = default;

		void Init(SDL_Window* _window);
		void Shutdown();

		void BeginFrame();
		void EndFrame();

		void BeginGraphics();
		void EndGraphics();

	private:
		// Vulkan handles
		VkInstance       m_instance         = VK_NULL_HANDLE;
		VkSurfaceKHR     m_surface          = VK_NULL_HANDLE;
		VkPhysicalDevice m_physical         = VK_NULL_HANDLE;
		VkDevice         m_logical          = VK_NULL_HANDLE;
		VmaAllocator     m_alloc            = VK_NULL_HANDLE;
		VkSwapchainKHR   m_swapchain        = VK_NULL_HANDLE;
		VkPipelineLayout m_pipelineLayout   = VK_NULL_HANDLE;
		VkPipeline       m_graphicsPipeline = VK_NULL_HANDLE;

		VkShaderModule m_vertModule = VK_NULL_HANDLE;
		VkShaderModule m_fragModule = VK_NULL_HANDLE;

		//Slang::ComPtr<slang::IGlobalSession> m_globalSession;

		// Queue family indices
		Uint32 m_present  = UINT32_MAX;
		Uint32 m_graphics = UINT32_MAX;

		//// Swapchain details
		//VkSurfaceFormatKHR m_format     {};
		//VkPresentModeKHR   m_presentMode{};
		//VkExtent2D         m_extent     {};

		//std::array<FrameData, RendererCfg::MAX_FRAMES_IN_FLIGHT> m_frames{};
		//Uint32 m_frameIndex = 0; // Current frame
		//FrameSync m_frameSync{};

		//std::vector<ImageData> m_images{};
		//Uint32 m_imageIndex = 0; // Current swapchain image

		// Initialization
		void InitInstance();
		void InitSurface();
		void InitPhysicalDevice();
		void InitLogicalDevice();
		void InitVMA();
		void InitSwapchain();
		void InitImageViews();
		void InitShaderModules();
		void InitPipelineLayout();
		void InitGraphicsPipeline();
		void InitSync();
		void InitCommandBuffers();

		// Shutdown
		void DestroySwapchain();

		VkShaderModule LoadShaderModule(const std::string& path, const std::string& name, const std::string& entry);
	};
}