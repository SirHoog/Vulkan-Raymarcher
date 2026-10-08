#include <set>

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1

#include "rendering/renderer.hpp"

using namespace VK_RM;

void Renderer::Init(SDL_Window* _window)
{
	window = _window;

	VK_CHECK(volkInitialize());
	InitInstance();
	InitSurface();
	InitPhysicalDevice();
	InitLogicalDevice();
	InitVMA();
	InitSwapchain();
	InitImageViews();
	InitShaderModules();
	InitPipelineLayout();
	InitGraphicsPipeline();
	InitSync();
	InitCommandBuffers();
}
void Renderer::Shutdown()
{
	if (m_alloc)    vmaDestroyAllocator(m_alloc);
	if (m_surface)  vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	if (m_logical)  vkDestroyDevice    (m_logical,  nullptr);
	if (m_instance) vkDestroyInstance  (m_instance, nullptr);

	volkFinalize();
}

void Renderer::InitInstance()
{
	Uint32 extensionCount;
	auto extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);
	
	if (!extensions) FatalError("Failed to get Vulkan instance extensions. {}", SDL_GetError());

	std::vector<const char*> requestedExtensions = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
	std::vector<const char*> requestedLayers     = { "VK_LAYER_KHRONOS_validation" };

	for (Uint32 i = 0; i < extensionCount; i++)
		requestedExtensions.push_back(extensions[i]);

	VkApplicationInfo appInfo
	{
		.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName   = "Vulkan Raymarcher",
		.applicationVersion = VK_MAKE_VERSION(VK_RM_MAJOR, VK_RM_MINOR, VK_RM_PATCH),
		.pEngineName        = "VK-RM",
		.engineVersion      = VK_MAKE_VERSION(VK_RM_MAJOR, VK_RM_MINOR, VK_RM_PATCH),
		.apiVersion         = VK_API_VERSION_1_4
	};
	VkDebugUtilsMessengerCreateInfoEXT debugInfo
	{
		.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
						   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
						   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
						   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = DebugCallback
	};
	VkInstanceCreateInfo createInfo
	{
		.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext                   = &debugInfo,
		.pApplicationInfo        = &appInfo,
		.enabledLayerCount       = static_cast<Uint32>(requestedLayers.size()),
		.ppEnabledLayerNames     = requestedLayers.data(),
		.enabledExtensionCount   = static_cast<Uint32>(requestedExtensions.size()),
		.ppEnabledExtensionNames = requestedExtensions.data()
	};

	VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_instance));
	volkLoadInstance(m_instance);
}
void Renderer::InitSurface()
{
	if (!SDL_Vulkan_CreateSurface(window, m_instance, nullptr, &m_surface))
		FatalError("Failed to create Vulkan surface. {}", SDL_GetError());
}
void Renderer::InitPhysicalDevice()
{
	#pragma region Enumerate physical devices
	std::vector<VkPhysicalDevice> devices;
	Uint32 devicesCount;

	VK_CHECK(vkEnumeratePhysicalDevices(m_instance, &devicesCount, nullptr)); devices.resize(devicesCount);
	VK_CHECK(vkEnumeratePhysicalDevices(m_instance, &devicesCount, devices.data()));

	if (devicesCount == 0) FatalError("No Vulkan-compatible GPUs found.");
	#pragma endregion
	#pragma region Select the best GPU
	std::string topName = "";
	Uint32 topScore = 0;

	std::println("Physical devices found:");

	for (Uint32 i = 0; i < devicesCount; i++)
	{
		VkPhysicalDevice device = devices[i];
		VkPhysicalDeviceProperties properties = {};

		vkGetPhysicalDeviceProperties(device, &properties);

		std::println("({}) {}", i + 1, properties.deviceName);

		#pragma region Find queue family indices
		Uint32 present  = UINT32_MAX;
		Uint32 graphics = UINT32_MAX;
		Uint32 queueFamilyCount;
		std::vector<VkQueueFamilyProperties2> queueFamilies;

		// Get queue family properties
		vkGetPhysicalDeviceQueueFamilyProperties2(device, &queueFamilyCount, nullptr);

		queueFamilies.resize(queueFamilyCount);

		for (auto& queueFamily : queueFamilies)
		{
			queueFamily.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
			queueFamily.pNext = nullptr;
		}

		vkGetPhysicalDeviceQueueFamilyProperties2(device, &queueFamilyCount, queueFamilies.data());

		for (Uint32 i = 0; i < queueFamilyCount; i++)
		{
			VkQueueFamilyProperties& queueFamily = queueFamilies[i].queueFamilyProperties;

			VkBool32 presentSupport = VK_FALSE;

			VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport));

			// Select earliest queue family for each type of queue
			if (present == UINT32_MAX && presentSupport)
				present = i;

			if (graphics == UINT32_MAX && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				graphics = i;
		}
		#pragma endregion
		#pragma region Qualifications
		// Queue family support
		if (present  == UINT32_MAX ||
			graphics == UINT32_MAX)  continue;

		// Extension support
		Uint32 extensionCount = 0;
		std::vector<VkExtensionProperties> extensions;
		std::set<std::string> requested = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		VK_CHECK(vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr)); extensions.resize(extensionCount);
		VK_CHECK(vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data()));

		for (const auto& extension : extensions)
			requested.erase(extension.extensionName);

		if (!requested.empty()) continue;
		#pragma endregion
		#pragma region Scoring
		Uint32 score = 0;

		// Discrete > Integrated > CPU/Other
		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 5000;
		else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
			score += 1000;

		// More VRAM and higher resolution
		VkPhysicalDeviceMemoryProperties memory;
		VkDeviceSize maxLocalVram = 0;

		vkGetPhysicalDeviceMemoryProperties(device, &memory);

		for (Uint32 i = 0; i < memory.memoryHeapCount; i++)
			if (memory.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
				maxLocalVram = std::max(maxLocalVram, memory.memoryHeaps[i].size);

		score += static_cast<Uint32>(maxLocalVram / (1024 * 1024 * 10)); // 1 pt per 10 MB of VRAM
		score += properties.limits.maxImageDimension2D;                  // 1 pt per pixel of resolution
		#pragma endregion

		if (score > topScore)
		{
			topName    = properties.deviceName;
			topScore   = score;
			m_present  = present;
			m_graphics = graphics;
			m_physical = device;
		}
	}
	#pragma endregion

	if (m_physical == VK_NULL_HANDLE)
		FatalError("No suitable physical device found");

	std::println("Selected device: {}", topName);
}
void Renderer::InitLogicalDevice()
{
	#pragma region Query feature support
	VkPhysicalDeviceVulkan14Features supported14{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES, .pNext = nullptr };
	VkPhysicalDeviceVulkan13Features supported13{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES, .pNext = &supported14 };
	VkPhysicalDeviceVulkan12Features supported12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES, .pNext = &supported13 };
	VkPhysicalDeviceFeatures2        supported  { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,          .pNext = &supported12 };

	vkGetPhysicalDeviceFeatures2(m_physical, &supported);

	if (!supported13.dynamicRendering ||
		!supported13.synchronization2 ||
		!supported12.timelineSemaphore)
		FatalError("Vulkan feature requirements not met.");

	VkPhysicalDeviceVulkan14Features features14
	{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES,
		.pNext = nullptr
	};
	VkPhysicalDeviceVulkan13Features features13
	{
		.sType            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.pNext            = &features14,
		.synchronization2 = VK_TRUE,
		.dynamicRendering = VK_TRUE
	};
	VkPhysicalDeviceVulkan12Features features12
	{
		.sType             = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		.pNext             = &features13,
		.timelineSemaphore = VK_TRUE
	};
	VkPhysicalDeviceFeatures2 features{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, .pNext = &features12 };
	#pragma endregion
	#pragma region Create unique queue families
	std::vector<VkDeviceQueueCreateInfo> queueInfos;
	float queuePriority = 1.0f;

	std::set<Uint32> uniqueQueueFamilies = { m_graphics, m_present };

	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueInfo
		{
			.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.pNext            = nullptr,
			.queueFamilyIndex = queueFamily,
			.queueCount       = 1,
			.pQueuePriorities = &queuePriority
		};
		queueInfos.push_back(queueInfo);
	}
	#pragma endregion
	#pragma region Create logical device
	const std::vector<const char*> extensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	VkDeviceCreateInfo createInfo
	{
		.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext                   = &features,
		.queueCreateInfoCount    = static_cast<Uint32>(queueInfos.size()),
		.pQueueCreateInfos       = queueInfos.data(),
		.enabledExtensionCount   = static_cast<Uint32>(extensions.size()),
		.ppEnabledExtensionNames = extensions.data(),
		.pEnabledFeatures        = nullptr
	};

	VK_CHECK(vkCreateDevice(m_physical, &createInfo, nullptr, &m_logical));
	volkLoadDevice(m_logical);
	#pragma endregion
}
void Renderer::InitVMA()
{
	VmaVulkanFunctions vmaFuncs{};
	VmaAllocatorCreateInfo vmaCreateInfo
	{
		.physicalDevice   = m_physical,
		.device           = m_logical,
		.pVulkanFunctions = &vmaFuncs,
		.instance         = m_instance,
		.vulkanApiVersion = VK_API_VERSION_1_4
	};

	vmaImportVulkanFunctionsFromVolk(&vmaCreateInfo, &vmaFuncs);
	VK_CHECK(vmaCreateAllocator(&vmaCreateInfo, &m_alloc));
}
void Renderer::InitSwapchain()
{

}
void Renderer::InitImageViews()
{

}
void Renderer::InitShaderModules()
{

}
void Renderer::InitPipelineLayout()
{

}
void Renderer::InitGraphicsPipeline()
{

}
void Renderer::InitSync()
{

}
void Renderer::InitCommandBuffers()
{

}

void Renderer::DestroySwapchain()
{

}

VkShaderModule Renderer::LoadShaderModule(const std::string& path, const std::string& name, const std::string& entry)
{
	return VK_NULL_HANDLE;
}