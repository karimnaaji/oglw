#include "debugRender.h"

#include "gl/renderState.h"

#define DEBUG_DRAW_IMPLEMENTATION
#include "debug_draw.hpp"

namespace OGLW {

void DebugRenderInterface::init() {
    static const std::string shaderBundle = R"END(
        #pragma begin:vertex
        #version 150
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
        #version 150
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
        {"color", 3, GL_FLOAT, false, 0, AttributeLocation::color},
    }));
}

void DebugRenderInterface::beginDraw() {
    GL_CHECK(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*) &m_boundBuffer));
    //RenderState::depthTest(GL_FALSE);
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_boundBuffer));
	GL_CHECK(glBindVertexArray(0));
}

void DebugRenderInterface::endDraw() {
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_boundBuffer));
}

void DebugRenderInterface::drawPointList(const dd::DrawVertex* _points,
    int _count,
    bool _depthEnabled)
{

}

void DebugRenderInterface::drawLineList(const dd::DrawVertex* _lines,
    int _count,
    bool _depthEnabled)
{

}

void DebugRenderInterface::drawGlyphList(const dd::DrawVertex* _glyphs,
    int _count,
    dd::GlyphTextureHandle _glyphTex)
{

}

dd::GlyphTextureHandle DebugRenderInterface::createGlyphTexture(int _width,
    int _height,
    const void* _pixels)
{

}

void DebugRenderInterface::destroyGlyphTexture(dd::GlyphTextureHandle _glyphTex) {

}

DebugRenderInterface::~DebugRenderInterface() {

}

} // OGLW
