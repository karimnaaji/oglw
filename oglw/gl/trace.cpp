#include "trace.h"
#include "log.h"

namespace OGLW {
namespace _Trace {

static std::vector<GLCall> glCalls;

void glTrace(const char* stmt, const char* fname, int line) {
    glCalls.push_back({std::string(stmt), std::string(fname), line});
}

void printGLTrace() {
    for (auto glCall : glCalls) {
        INFO("%s:%d %s\n", glCall.fname.c_str(), glCall.line, glCall.stmt.c_str());
    }
}

}
}