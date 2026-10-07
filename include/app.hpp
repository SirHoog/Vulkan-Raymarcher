#pragma once

struct App
{
	void Init();
	void Run();
	void Shutdown();

private:
	// SDL_Window* m_window = nullptr;
	// Renderer    m_renderer;
	// Engine      m_engine;
	// UI          m_ui;

	bool m_running = true;

	void Events();
};