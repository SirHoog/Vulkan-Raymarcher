#include "app/app.hpp"

using namespace VK_RM;
using namespace AppCfg;

void App::Init()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
		FatalError("Failed to initialize SDL. {}", SDL_GetError());

	m_window = SDL_CreateWindow("Vulkan Raymarcher", INIT_SCREEN_WIDTH, INIT_SCREEN_HEIGHT, WINDOW_FLAGS);
	
	if (!m_window)
		FatalError("Failed to create SDL window. {}", SDL_GetError());

	m_renderer.Init(m_window);
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