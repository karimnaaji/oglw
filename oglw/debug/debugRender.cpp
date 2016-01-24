#include "debugRender.h"

#include "gl/renderState.h"
#include "core/log.h"

#define DEBUG_DRAW_IMPLEMENTATION
#include "debug_draw.hpp"

namespace OGLW {

void DebugRender::init() {
    m_layoutLocations = {
        {"position", 0},
        {"colorPointSize", 1}
    };
    static const std::string shaderBundle = R"END(
        #pragma begin:vertex
        #version 330
        layout(location = 0) in vec3 position;
        layout(location = 1) in vec4 colorPointSize;

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


    m_shader = std::make_unique<Shader>();
    m_shader->loadBundleSource(shaderBundle);
    m_layout = std::unique_ptr<VertexLayout>(new VertexLayout({
        {"position", 3, GL_FLOAT, false, 0, AttributeLocation::position},
        {"color", 4, GL_FLOAT, false, 0, AttributeLocation::color},
    }));

    dd::initialize(this);
}

void DebugRender::beginDraw() {
    INFO("DebugRender::beginDraw\n");
    GL_CHECK(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*) &m_boundBuffer));
    RenderState::depthTest(GL_FALSE);
    RenderState::blending(GL_FALSE);
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_boundBuffer));
	GL_CHECK(glBindVertexArray(0));
}

void DebugRender::endDraw() {
    INFO("DebugRender::endDraw\n");
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_boundBuffer));
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
    INFO("DRAWLINELIST %d\n", _count);

    m_shader->setUniform("mvp", m_mvp);
    m_layout->enable(m_layoutLocations, 0, (void*)_lines);
    // TODO: use vao
    GL_CHECK(glDrawArrays(GL_LINES, 0, _count));
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
