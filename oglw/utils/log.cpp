#include "log.h"

namespace OGLW {

#define PRINT(L, MSG, ARGS) \
va_list args; \
va_start(args, msg); \
printf((L + std::string(MSG)).c_str(), ARGS); \
printf("\n"); \
va_end(args); \

void Log::setLevel(Log::Level _level) {
    m_level = _level;
}

void Log::debug(const char* msg, ...) {
    static std::string level = "[DEBUG] ";
    if (m_level >= Level::Debug) {
        PRINT(level, msg, args);
    }
}

void Log::info(const char* msg, ...) {
    static std::string level = "[INFO] ";
    if (m_level >= Level::Info) {
        PRINT(level, msg, args);
    }
}

void Log::warn(const char* msg, ...) {
    static std::string level = "[WARNING] ";
    if (m_level >= Level::Warning) {
        PRINT(level, msg, args);
    }
}

void Log::error(const char* msg, ...) {
    static std::string level = "[ERROR] ";
    if (m_level >= Level::Error) {
        PRINT(level, msg, args);
    }
}



} // OGLW