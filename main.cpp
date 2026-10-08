#include "Engine.h"

int main()
{
    Engine engine;

    if (engine.Initialize(
        "2D Engine",
        1280,
        720,
        false,
        60))
    {
        engine.Run();
    }

    return 0;
}