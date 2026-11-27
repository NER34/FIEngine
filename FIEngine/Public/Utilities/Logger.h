#pragma once

#include "spdlog/spdlog.h"

#if DEBUG_ENABLED
    #define LOG_INFO(...) spdlog::info(__VA_ARGS__)
    #define LOG_WARN(...) spdlog::warn(__VA_ARGS__)
    #define LOG_ERROR(...) spdlog::error(__VA_ARGS__)
#else
    #define LOG_INFO(...) {}
    #define LOG_WARN(...) {}
    #define LOG_ERROR(...) {}
#endif
