//
// Created by telep on 10/10/2026.
//

#ifndef ENGINE2D_LOGS_H
#define ENGINE2D_LOGS_H
#pragma once

#include <string_view>
#include <fstream>

enum class LogLevel
{
    Info,
    Warning,
    Error
};

class Log
{
public:
    static bool Init(std::string_view filename = "logs.txt");
    static void Shutdown();

    static void Info(std::string_view msg);
    static void Warn(std::string_view msg);
    static void Error(std::string_view msg);
    static void SDLError(std::string_view action);

private:
    static void Write(LogLevel level, std::string_view msg);
    static std::ofstream logFile;
};
#endif //ENGINE2D_LOGS_H