#include "Engine.h"

int main(int argc, char *argv[])
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