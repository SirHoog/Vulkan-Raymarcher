include_guard(GLOBAL)

find_package(SDL3 CONFIG QUIET)

if(SDL3_FOUND)
	message(STATUS "SDL3 found.")
else()
	message(STATUS "SDL3 not found. Downloading...")

	include(FetchContent)
	FetchContent_Declare(
		SDL3
		GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
		GIT_TAG release-3.4.12
	)
	FetchContent_MakeAvailable(SDL3)
endif()