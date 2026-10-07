#include "engine/app.hpp"

int main()
{
    using namespace VK_RM; // Vulkan Raymarching (VK_RM)

    App app;

	app.Init();
    app.Run();
    app.Shutdown();

    return 0;
}