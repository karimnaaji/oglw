#include "log.h"

namespace OGLW {

#define PRINT(L, MSG) \
printf("%s", L); \
va_list args; \
va_start(args, MSG); \
vfprintf(stderr, MSG, args); \
va_end(args); \
printf("\n");

void Log::setLevel(Log::Level _level) {
    m_level = _level;
}

void Log::debug(const char* msg, ...) {
    static const char* level = "[DEBUG] ";
    if (m_level >= Level::Debug) {
        PRINT(level, msg);
    }
}

void Log::info(const char* msg, ...) {
    static const char* level = "[INFO] ";
    if (m_level >= Level::Info) {
        PRINT(level, msg);
    }
}

void Log::warn(const char* msg, ...) {
    static const char* level = "[WARNING] ";
    if (m_level >= Level::Warning) {
        PRINT(level, msg);
    }
}

void Log::error(const char* msg, ...) {
    static const char* level = "[ERROR] ";
    if (m_level >= Level::Error) {
        PRINT(level, msg);
    }
}

} // OGLW
