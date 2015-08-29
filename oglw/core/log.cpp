#include "log.h"

namespace OGLW {

#define VPRINT(MSG) \
va_list args; \
va_start(args, MSG); \
vfprintf(stderr, MSG, args); \
va_end(args);

void Log::setLevel(Log::Level _level) {
    m_level = _level;
}

void Log::debug(const char* msg, ...) {
    if (m_level >= Level::Debug) {
        VPRINT(msg);
    }
}

void Log::info(const char* msg, ...) {
    if (m_level >= Level::Info) {
        VPRINT(msg);
    }
}

void Log::warn(const char* msg, ...) {
    if (m_level >= Level::Warning) {
        VPRINT(msg);
    }
}

void Log::error(const char* msg, ...) {
    if (m_level >= Level::Error) {
        VPRINT(msg);
    }
}

} // OGLW
