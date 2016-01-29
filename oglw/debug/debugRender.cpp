#include "debugRender.h"

#include "gl/renderState.h"
#include "core/log.h"

#define DEBUG_DRAW_IMPLEMENTATION
#include "debug_draw.hpp"

namespace OGLW {

void DebugRender::init() {
    static const std::string lineShaderBundle = R"END(
        #pragma begin:vertex
        #version 330
        in vec3 position;
        in vec4 colorPointSize;
        out vec4 fColor;
        uniform mat4 mvp;
        void main() {
            gl_Position = mvp * vec4(position, 1.0);
            gl_PointSize = colorPointSize.w;
            fColor = vec4(colorPointSize.xyz, 1.0);
        }
        #pragma end:vertex
        #pragma begin:fragment
        #version 330
        in vec4 fColor;
        out vec4 outColor;

        void main() {
            outColor = fColor;
        }
        #pragma end:fragment
    )END";

    m_lineMesh.shader = std::make_unique<Shader>();
    m_lineMesh.shader->loadBundleSource(lineShaderBundle);
    m_lineMesh.layout = std::unique_ptr<VertexLayout>(new VertexLayout({
        {"position", 3, GL_FLOAT, false, 0, AttributeLocation::position},
        {"colorPointSize", 4, GL_FLOAT, false, 0, AttributeLocation::color},
    }));

    m_lineMesh.vao = std::make_unique<Vao>();
    GL_CHECK(glGenBuffers(1, &m_lineMesh.vertexBuffer));
    m_lineMesh.vao->init(m_lineMesh.vertexBuffer, 0, *m_lineMesh.layout, m_lineMesh.layout->getLocations());
    m_lineMesh.shader->bindVertexLayout(*m_lineMesh.layout);

    static const std::string textShaderBundle = R"END(
        #pragma begin:vertex
        #version 330
        in vec2 position;
        in vec2 uvs;
        in vec3 color;
        uniform vec2 resolution;
        out vec2 fUVs;
        out vec4 fColor;
        void main() {
            float x = ((2.0f * (position.x - 0.5f)) / resolution.x) - 1.0f;
            float y = 1.0f - ((2.0f * (position.y - 0.5f)) / resolution.y);
            gl_Position = vec4(x, y, 0.0, 1.0);
            fUVs = uvs;
            fColor = vec4(color, 1.0);
        }
        #pragma end:vertex
        #pragma begin:fragment
        #version 330
        in vec2 fUVs;
        in vec4 fColor;
        uniform sampler2D tex;
        out vec4 outColor;
        void main() {
            outColor = fColor * texture(tex, fUVs).rrrr;
        }
        #pragma end:fragment
    )END";

    m_textMesh.shader = std::make_unique<Shader>();
    m_textMesh.shader->loadBundleSource(textShaderBundle);
    m_textMesh.layout = std::unique_ptr<VertexLayout>(new VertexLayout({
        {"position", 2, GL_FLOAT, false, 0, AttributeLocation::position},
        {"uvs", 2, GL_FLOAT, false, 0, AttributeLocation::uv},
        {"color", 3, GL_FLOAT, false, 0, AttributeLocation::color},
    }));

    m_textMesh.vao = std::make_unique<Vao>();
    GL_CHECK(glGenBuffers(1, &m_textMesh.vertexBuffer));
    m_textMesh.vao->init(m_textMesh.vertexBuffer, 0, *m_textMesh.layout, m_textMesh.layout->getLocations());
    m_textMesh.shader->bindVertexLayout(*m_textMesh.layout);

    dd::initialize(this);
}

void DebugRender::beginDraw() {
    INFO("DebugRender::beginDraw\n");
    RenderState::depthTest(GL_FALSE);
    RenderState::blending(GL_FALSE);
}

void DebugRender::endDraw() {
    INFO("DebugRender::endDraw\n");
}

void DebugRender::drawPointList(const dd::DrawVertex* _points,
    int _count,
    bool _depthEnabled)
{

}

void DebugRender::drawLineList(const dd::DrawVertex* _lines,
    int _count,
    bool _depthEnabled)
{
    m_lineMesh.shader->setUniform("mvp", m_mvp);
    m_lineMesh.vao->bind();

    // buffer orphaning
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_lineMesh.vertexBuffer));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, _count * sizeof(dd::DrawVertex), NULL, GL_DYNAMIC_DRAW));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, _count * sizeof(dd::DrawVertex), _lines, GL_DYNAMIC_DRAW));

    GL_CHECK(glDrawArrays(GL_LINES, 0, _count));

    m_lineMesh.vao->unbind();
}

void DebugRender::drawGlyphList(const dd::DrawVertex* _glyphs,
    int _count,
    dd::GlyphTextureHandle _glyphTex)
{
    m_textMesh.texture->bind(0);
    m_textMesh.shader->setUniform("resolution", glm::vec2(1000, 1000));
    m_textMesh.shader->setUniform("tex", 0);
    m_textMesh.vao->bind();

    RenderState::blending(GL_TRUE);
    RenderState::blendingFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // buffer orphaning
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_textMesh.vertexBuffer));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, _count * sizeof(dd::DrawVertex), NULL, GL_DYNAMIC_DRAW));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, _count * sizeof(dd::DrawVertex), _glyphs, GL_DYNAMIC_DRAW));

    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, _count));

    m_textMesh.vao->unbind();
}

dd::GlyphTextureHandle DebugRender::createGlyphTexture(int _width,
    int _height,
    const void* _pixels)
{
    TextureOptions options;
    options.format = GL_RED;
    options.internalFormat = GL_R8;
    m_textMesh.texture = std::make_unique<Texture>(_width, _height, options);
    m_textMesh.texture->setData(reinterpret_cast<const GLuint*>(_pixels), _width * _height);
    m_textMesh.texture->update(0);

    return reinterpret_cast<dd::GlyphTextureHandle>(m_textMesh.texture->getGlHandle());
}

void DebugRender::destroyGlyphTexture(dd::GlyphTextureHandle _glyphTex) {
    m_textMesh.texture.reset();
}

DebugRender::~DebugRender() {
    // No-op
}

} // OGLW
