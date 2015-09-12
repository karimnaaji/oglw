#pragma once

#include <string>
#include <memory>
#include "glm/mat4x4.hpp"
#include "gl/shader.h"
#include "gl/textureCube.h"
#include "gl/mesh.h"

namespace OGLW {

class Skybox {
public:
    Skybox(std::string _file);
    ~Skybox() {}

    void draw(const glm::mat4& _mvp, const glm::vec3& _camPosition);

private:
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<TextureCube> m_texture;
    std::unique_ptr<RawMesh> m_mesh;

    std::string m_file;
};

}