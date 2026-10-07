#include <print>

#include "app.hpp"

void App::Init()
{
	// SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
	// m_window = SDL_CreateWindow("Vulkan Raymarcher", m_screenWidth, m_screenHeight, WINDOW_FLAGS);
	// SDL_CHECK_NULL(m_window);

	// m_renderer.Init(m_window);
	// m_engine  .Init();
	// m_ui      .Init();
}
void App::Run()
{
	while (m_running)
	{
		Events();
		// Update
		// Render
	}
}
void App::Shutdown()
{
	// if (m_window != nullptr) SDL_DestroyWindow(m_window);
	// m_renderer.Shutdown();
	// SDL_Quit();
}

void App::Events()
{
	std::println("Test");

	// SDL_Event event;
	// while (SDL_PollEvent(&event))
	// {
	// 	switch (event.type)
	// 	{
	// 		case SDL_EVENT_QUIT:
	// 			m_running = false;
	// 
	// 			break;
	// 		case SDL_EVENT_KEY_DOWN:
	// 			if (event.key.key == SDLK_ESCAPE)
	// 				m_running = false;
	// 			
	// 			break;
	// 		case SDL_EVENT_WINDOW_RESIZED:
	// 			// ...
	// 
	// 			break;
	// 
	// 		default:
	// 			break;
	// 	}
	// }
}