#pragma once

#include <vector>
#include <string>

namespace OGLW {
namespace _Trace {

struct GLCall {
    std::string fname;
    std::string stmt;
    int line;
};

void glTrace(const char* stmt, const char* fname, int line);

void printGLTrace();

} // _Trace
} // OGLW