#include "utils.h"
#include "core/log.h"

namespace OGLW {

bool stringFromPath(const std::string& path, std::string* into) {
    std::ifstream file;
    std::string buffer;

    file.open(path.c_str());
    if (!file.is_open()) {
        WARN("Can't load string resource %s", path.c_str());
        return false;
    }
    while (!file.eof()) {
        getline(file, buffer);
        (*into) += buffer + "\n";
    }

    file.close();
    return true;
}

uchar* bytesFromPath(const char* _path, uint* _size) {
    std::ifstream resource(_path, std::ifstream::ate | std::ifstream::binary);

    if (!resource.is_open()) {
        WARN("Failed to read file at path %s ", _path);
        *_size = 0;
        return nullptr;
    }

    *_size = resource.tellg();

    resource.seekg(std::ifstream::beg);

    char* cdata = (char*)malloc(sizeof(char) * (*_size));

    resource.read(cdata, *_size);
    resource.close();

    return reinterpret_cast<uchar*>(cdata);
}

}
