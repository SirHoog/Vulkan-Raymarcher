#pragma once

#include <print>
#include <format>
#include <string_view>

#include <Volk/volk.h>
#include <slang/slang.h>

// Helper functions/macros for error handling and debugging

namespace VK_RM
{
	inline const char* ResultString(VkResult result)
	{
		switch (result)
		{
			case VK_SUCCESS:                     return "VK_SUCCESS";
			case VK_NOT_READY:                   return "VK_NOT_READY";
			case VK_TIMEOUT:                     return "VK_TIMEOUT";
			case VK_EVENT_SET:                   return "VK_EVENT_SET";
			case VK_EVENT_RESET:                 return "VK_EVENT_RESET";
			case VK_INCOMPLETE:                  return "VK_INCOMPLETE";
			case VK_SUBOPTIMAL_KHR:              return "VK_SUBOPTIMAL_KHR";
			case VK_ERROR_OUT_OF_HOST_MEMORY:    return "VK_ERROR_OUT_OF_HOST_MEMORY";
			case VK_ERROR_OUT_OF_DEVICE_MEMORY:  return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
			case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
			case VK_ERROR_DEVICE_LOST:           return "VK_ERROR_DEVICE_LOST";
			case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
			case VK_ERROR_FEATURE_NOT_PRESENT:   return "VK_ERROR_FEATURE_NOT_PRESENT";
			case VK_ERROR_OUT_OF_DATE_KHR:       return "VK_ERROR_OUT_OF_DATE_KHR";
			default:                             return "VK_UNKNOWN_ERROR_OR_EXTENSION_ERROR";
		}
	};

	#if defined(_MSC_VER)
		#define VK_DEBUG_BREAK() __debugbreak()
	#else
		#define VK_DEBUG_BREAK() __builtin_trap()
	#endif

	template<class... Args>
	inline void FatalError(std::format_string<Args...> fmt, Args&&... args)
	{
		std::println(stderr, "Fatal Error:\n{})", std::format(fmt, std::forward<Args>(args)...));
		VK_DEBUG_BREAK();
		std::abort();
	};
	template<class... Args>
	inline void Warning(std::format_string<Args...> fmt, Args&&... args)
	{
		std::println(stderr, "Warning:\n{})", std::format(fmt, std::forward<Args>(args)...));
	};

	inline VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
	{
		if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			Warning   ("Validation Layer: {}", callbackData->pMessage);
		if (severity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			FatalError("Validation Layer: {}", callbackData->pMessage);

		return false;
	}

	// CAUTION: Minimize macro definitions
	// TODO: Convert these macros to inline functions

	#define VK_CHECK(EXPRESSION)                                                         \
		do {                                                                             \
			VkResult res = (EXPRESSION);                                                 \
			if (res != VK_SUCCESS)                                                       \
			{                                                                            \
				constexpr std::string_view fmt = "- Expression: {}\n"                    \
												 "- Result:"  " {}\n"                    \
												 "- File:"    " {}\n"                    \
												 "- Line:"    " {}\n";                   \
																						 \
				/* Negative = fatal errors, positive = warnings */                       \
				if (res < 0)                                                             \
					FatalError(fmt, #EXPRESSION, ResultString(res), __FILE__, __LINE__); \
				else                                                                     \
					Warning   (fmt, #EXPRESSION, ResultString(res), __FILE__, __LINE__); \
			}                                                                            \
		} while(0)
	#define SLANG_CHECK(EXPRESSION)                                \
		if (SLANG_FAILED(EXPRESSION))                              \
			FatalError("- Expression: {}\n- File: {}\n- Line: {}", \
						 #EXPRESSION,     __FILE__,   __LINE__);
}