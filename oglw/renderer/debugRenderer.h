#pragma once

#include "debug_draw.hpp"
#include "gl/gl.h"
#include "glm/glm.hpp"
#include <memory>

namespace OGLW {

// Forwards
class Shader;
class VertexLayout;
class Texture;
class Vao;

class DebugRenderer : public dd::RenderInterface {
public:
    void init();
    virtual void beginDraw() override;
    virtual void endDraw() override;
    virtual ~DebugRenderer() override;

    virtual void drawPointList(const dd::DrawVertex* _points, int _count, bool _depthEnabled) override;
    virtual void drawLineList(const dd::DrawVertex* _lines, int _count, bool _depthEnabled) override;
    virtual void drawGlyphList(const dd::DrawVertex* _glyphs, int _count, dd::GlyphTextureHandle _glyphTex) override;
    virtual dd::GlyphTextureHandle createGlyphTexture(int _width, int _height, const void* _pixels) override;
    virtual void destroyGlyphTexture(dd::GlyphTextureHandle _glyphTex) override;
    // Sets the model view projection matrix for the renderer
    void setMVP(glm::mat4 _mvp) { m_mvp = _mvp; }

private:

    struct Mesh {
        GLuint vertexBuffer;
        std::unique_ptr<Texture> texture;
        std::unique_ptr<Shader> shader;
        std::unique_ptr<VertexLayout> layout;
        std::unique_ptr<Vao> vao;
    };

    Mesh m_lineMesh;
    Mesh m_textMesh;

    GLint m_boundBuffer;
    glm::mat4 m_mvp;
};

} // OGLW
