include_guard(GLOBAL)

find_package(slang CONFIG QUIET)

if(slang_FOUND)
	message(STATUS "Slang found.")
else()
	message(STATUS "Slang not found. Downloading...")

	include(FetchContent)
	FetchContent_Declare(
		slang
		GIT_REPOSITORY https://github.com/shader-slang/slang.git
		GIT_TAG master
	)
	FetchContent_MakeAvailable(slang)
endif()

# Slang shader compilation for CPU target
find_program(SLANG_COMPILER slangc REQUIRED)

function(compile_slang)
	# Parse slangc flags
	set(OPTIONS "")
	set(ONE_VALUE_ARGS SRC OUT TARGET ENTRY STAGE PROFILE)
	set(MULTI_VALUE_ARGS "")
	cmake_parse_arguments(ARG "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN})

	# Required flags
	foreach(req SRC OUT TARGET ENTRY STAGE PROFILE)
		if(NOT ARG_${req})
			message(FATAL_ERROR "compile_slang: '${req}' flag is required")
		endif()
	endforeach()

	# Extra slangc flags
	set(EXTRA_FLAGS ${ARG_UNPARSED_ARGUMENTS})

	# slangc compile command
	set(CMD
		"${SLANG_COMPILER}"
		"${ARG_SRC}"
		-o "${ARG_OUT}"
		-target "${ARG_TARGET}"
		-entry "${ARG_ENTRY}"
		-stage "${ARG_STAGE}"
		-profile "${ARG_PROFILE}"
		"${EXTRA_FLAGS}"
	)

	add_custom_command(
		OUTPUT "${ARG_OUT}"
		COMMAND ${CMD}
		DEPENDS "${ARG_SRC}"
		COMMENT "Compiling Slang shader ${ARG_SRC} to ${ARG_OUT}..."
		VERBATIM
	)
	set_property(GLOBAL APPEND PROPERTY OUTPUTS "${ARG_OUT}")
endfunction()