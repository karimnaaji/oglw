#include "renderState.h"
#include "log.h"

namespace OGLW {
namespace RenderState {

Blending blending;
DepthTest depthTest;
StencilTest stencilTest;
Culling culling;
DepthWrite depthWrite;
DepthFunc depthFunc;
BlendingFunc blendingFunc;
StencilWrite stencilWrite;
StencilFunc stencilFunc;
StencilOp stencilOp;
ColorWrite colorWrite;
FrontFace frontFace;
CullFace cullFace;
ClearDepth clearDepth;
DepthRange depthRange;
ShaderProgram shaderProgram;
TextureUnit textureUnit;
Texture texture;
DrawBuffer drawBuffer;
ReadBuffer readBuffer;
GLint maxCombinedTextureUnits;

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

} // RenderState
} // OGLW
