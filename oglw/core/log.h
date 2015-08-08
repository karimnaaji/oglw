#pragma once
#include <memory>
#include <iostream>
#include <cstdarg>

namespace OGLW {

class Log {

public:
    enum Level {
        Silent,
        Debug,
        Warning,
        Info,
        Error,
        Verbose
    };

    static auto& GetInstance() {
        static std::unique_ptr<Log> log = std::make_unique<Log>();
        return log;
    }

    void setLevel(Level _level);
    void debug(const char* msg, ...);
    void info(const char* msg, ...);
    void warn(const char* msg, ...);
    void error(const char* msg, ...);

private:
    Level m_level = Level::Verbose;

};

/// Shortcuts
#define DBG(...) do { OGLW::Log::GetInstance()->debug(__VA_ARGS__); } while(0)
#define INFO(...) do { OGLW::Log::GetInstance()->info(__VA_ARGS__); } while(0)
#define WARN(...) do { OGLW::Log::GetInstance()->warn(__VA_ARGS__); } while(0)
#define ERROR(...) do { OGLW::Log::GetInstance()->error(__VA_ARGS__); exit(-1); } while(0)
    
} // OGLW
