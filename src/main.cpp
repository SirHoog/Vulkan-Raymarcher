#include "app.hpp"

int main()
{
    App app;

	app.Init();
    app.Run();
    app.Shutdown();

    return 0;
}