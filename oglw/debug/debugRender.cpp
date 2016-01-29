#include "debugRender.h"

#include "gl/renderState.h"
#include "core/log.h"

#define DEBUG_DRAW_IMPLEMENTATION
#include "debug_draw.hpp"

namespace OGLW {

void DebugRender::init() {
    static const std::string shaderBundle = R"END(
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
    m_lineMesh.shader->loadBundleSource(shaderBundle);
    m_lineMesh.layout = std::unique_ptr<VertexLayout>(new VertexLayout({
        {"position", 3, GL_FLOAT, false, 0, AttributeLocation::position},
        {"colorPointSize", 4, GL_FLOAT, false, 0, AttributeLocation::color},
    }));

    m_lineMesh.vao = std::make_unique<Vao>();
    GL_CHECK(glGenBuffers(1, &m_lineMesh.vertexBuffer));
    m_lineMesh.vao->init(m_lineMesh.vertexBuffer, 0, *m_lineMesh.layout, m_lineMesh.layout->getLocations());
    m_lineMesh.shader->bindVertexLayout(*m_lineMesh.layout);

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

}

dd::GlyphTextureHandle DebugRender::createGlyphTexture(int _width,
    int _height,
    const void* _pixels)
{

}

void DebugRender::destroyGlyphTexture(dd::GlyphTextureHandle _glyphTex) {

}

DebugRender::~DebugRender() {

}

} // OGLW
