#include "logs.h"
#include <iostream>
#include <SDL3/SDL.h>

std::ofstream Log::logFile;

bool Log::Init(std::string_view filename)
{
    logFile.open(filename.data(), std::ios::out | std::ios::trunc);
    if (!logFile.is_open())
    {
        std::cerr << "[ERROR] Nie udalo sie otworzyc pliku logow: " << filename << '\n';
        return false;
    }

    Info("System logowania zostal uruchomiony.");
    return true;
}

void Log::Shutdown()
{
    if (logFile.is_open())
    {
        Info("System logowania zakonczyl prace.");
        logFile.close();
    }
}

void Log::Write(LogLevel level, std::string_view msg)
{
    const char* prefix = "[INFO] ";
    if (level == LogLevel::Warning) prefix = "[WARN] ";
    if (level == LogLevel::Error)   prefix = "[ERROR] ";
    if (level == LogLevel::Error)
        std::cerr << prefix << msg << '\n';
    else
        std::cout << prefix << msg << '\n';

    if (logFile.is_open())
    {
        logFile << prefix << msg << '\n';
        logFile.flush();
    }
}

void Log::Info(std::string_view msg)
{
    Write(LogLevel::Info, msg);
}

void Log::Warn(std::string_view msg)
{
    Write(LogLevel::Warning, msg);
}

void Log::Error(std::string_view msg)
{
    Write(LogLevel::Error, msg);
}

void Log::SDLError(std::string_view action)
{
    std::string fullMsg = std::string(action) + " -> SDL: " + SDL_GetError();
    Write(LogLevel::Error, fullMsg);
}