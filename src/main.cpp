#include "engine.h"




int main(int argc, char *argv[])
{
    Engine engine;

    if (engine.Initialize(
        "2D Engine",
        1920,
        1080,
        false,
        60))
    {
        engine.Run();
    }

    return 0;
}