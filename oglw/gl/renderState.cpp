#include "renderState.h"
#include "log.h"
#include <stack>

namespace OGLW {
namespace RenderState {

#define RENDER_STATES           \
Blending blending;              \
DepthTest depthTest;            \
StencilTest stencilTest;        \
Culling culling;                \
DepthWrite depthWrite;          \
DepthFunc depthFunc;            \
BlendingFunc blendingFunc;      \
StencilWrite stencilWrite;      \
StencilFunc stencilFunc;        \
StencilOp stencilOp;            \
ColorWrite colorWrite;          \
FrontFace frontFace;            \
CullFace cullFace;              \
ClearDepth clearDepth;          \
DepthRange depthRange;          \
ShaderProgram shaderProgram;    \
TextureUnit textureUnit;        \
Texture texture;                \
DrawBuffer drawBuffer;          \
ReadBuffer readBuffer;

RENDER_STATES

struct RenderStateDescriptor {
    RENDER_STATES
};

#undef RENDER_STATES

GLint maxCombinedTextureUnits;

std::stack<RenderStateDescriptor> savedRenderStates;

GLuint getTextureUnit(GLuint _unit) {
    if (_unit >= maxCombinedTextureUnits) {
        WARN("Trying to access unavailable texture unit");
    }

    return GL_TEXTURE0 + _unit;
}

void activeTextureUnit(GLuint _unit) {
    glActiveTexture(getTextureUnit(_unit));
}

void bindTexture(GLenum _target, GLuint _textureId) {
    glBindTexture(_target, _textureId);
}

void initialize() {
    GL_CHECK(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxCombinedTextureUnits));
    RenderState::depthTest.init(true);
    RenderState::clearDepth(1.0);
    RenderState::depthRange(0.0, 1.0);
    RenderState::depthFunc.init(GL_LEQUAL);
    RenderState::depthWrite.init(GL_TRUE);
    RenderState::depthWrite.init(true);
    RenderState::culling.init(true);
    RenderState::cullFace.init(GL_BACK);
    RenderState::frontFace.init(GL_CCW);
    RenderState::blending.init(false);
    RenderState::drawBuffer.init(GL_BACK);
    RenderState::readBuffer.init(GL_BACK);
    RenderState::shaderProgram.init(std::numeric_limits<unsigned int>::max(), false);
    RenderState::texture.init(GL_TEXTURE_2D, std::numeric_limits<unsigned int>::max(), false);
    RenderState::texture.init(GL_TEXTURE_CUBE_MAP, std::numeric_limits<unsigned int>::max(), false);
}

void push() {
    savedRenderStates.push({
        blending, depthTest, stencilTest, culling,
        depthWrite, depthFunc, blendingFunc, stencilWrite,
        stencilFunc, stencilOp, colorWrite, frontFace,
        cullFace, clearDepth, depthRange, shaderProgram,
        textureUnit, texture, drawBuffer, readBuffer
    });
}

void pop() {
    if (savedRenderStates.size() == 0) {
        return;
    }

    RenderStateDescriptor desc = savedRenderStates.top();

    RenderState::depthTest(desc.depthTest.get());
    RenderState::clearDepth(desc.clearDepth.get<0>());
    RenderState::depthRange(desc.depthRange.get<0>(), desc.depthRange.get<1>());
    RenderState::depthFunc(desc.depthFunc.get<0>());
    RenderState::depthWrite(desc.depthWrite.get<0>());
    RenderState::culling(desc.culling.get());
    RenderState::cullFace(desc.cullFace.get<0>());
    RenderState::frontFace(desc.frontFace.get<0>());
    RenderState::blending(desc.blending.get());
    RenderState::drawBuffer(desc.drawBuffer.get<0>());
    RenderState::readBuffer(desc.readBuffer.get<0>());

    if (desc.shaderProgram.get<0>() != std::numeric_limits<unsigned int>::max()) {
        RenderState::shaderProgram(desc.shaderProgram.get<0>());
    }

    if (desc.texture.get<1>() != std::numeric_limits<unsigned int>::max()) {
        RenderState::texture(desc.texture.get<0>(), desc.texture.get<1>());
    }

    savedRenderStates.pop();
}

} // RenderState
} // OGLW
