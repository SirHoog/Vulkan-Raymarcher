cmake_minimum_required(VERSION 3.30)

set(VK_RM_MAJOR 0)
set(VK_RM_MINOR 1)
set(VK_RM_PATCH 0)

project(
	VK_RM # Vulkan Raymarcher
	VERSION ${VK_RM_MAJOR}.${VK_RM_MINOR}.${VK_RM_PATCH}
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/deps.cmake)

set(RES_DIR "${PROJECT_SOURCE_DIR}/res")
set(SHADERS_DIR "${RES_DIR}/shaders")

add_executable(VK_RM
	src/main.cpp
	src/engine/app.cpp
	src/rendering/renderer.cpp
)
target_include_directories(VK_RM PRIVATE include)
target_link_libraries(VK_RM PRIVATE
	SDL3::SDL3
	Vulkan::Vulkan
	Vulkan::volk
	GPUOpen::VulkanMemoryAllocator
	slang::slang
)
target_compile_definitions(VK_RM PRIVATE
	VK_RM_MAJOR=${VK_RM_MAJOR}
	VK_RM_MINOR=${VK_RM_MINOR}
	VK_RM_PATCH=${VK_RM_PATCH}

	RES_DIR="${RES_DIR}"
	SHADERS_DIR="${SHADERS_DIR}"
)
target_sources(VK_RM PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/res/shaders/main.slang
)