include_guard(GLOBAL)

find_package(glm CONFIG QUIET)

if(glm_FOUND)
	message(STATUS "GLM found.")
else()
	message(STATUS "GLM not found. Downloading...")

	include(FetchContent)
	FetchContent_Declare(
		glm
		GIT_REPOSITORY https://github.com/g-truc/glm.git
		GIT_TAG 1.0.3
	)
	FetchContent_MakeAvailable(glm)
endif()