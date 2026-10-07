include_guard(GLOBAL)

find_package(Vulkan QUIET COMPONENTS volk)

if(Vulkan_FOUND)
	message(STATUS "Vulkan found.")
else()
	message(FATAL_ERROR "Vulkan not found. Please install Vulkan SDK or add it to PATH.")
endif()

find_package(VulkanMemoryAllocator CONFIG QUIET)

if(VulkanMemoryAllocator_FOUND)
	message(STATUS "VMA found.")
else()
    message(STATUS "VMA not found. Downloading...")

	include(FetchContent)
	FetchContent_Declare(
		VulkanMemoryAllocator
		GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
		GIT_TAG v3.4.0
	)
	FetchContent_MakeAvailable(VulkanMemoryAllocator)
endif()