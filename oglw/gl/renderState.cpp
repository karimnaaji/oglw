#include "renderState.h"
    
namespace OGLW {
namespace RenderState {

Blending blending;
DepthTest depthTest;
StencilTest stencilTest;
Culling culling;
DepthWrite depthWrite;
BlendingFunc blendingFunc;
StencilWrite stencilWrite;
StencilFunc stencilFunc;
StencilOp stencilOp;
ColorWrite colorWrite;
FrontFace frontFace;
CullFace cullFace;

void initialize() {
    RenderState::depthTest.init(GL_TRUE);
    RenderState::depthWrite.init(GL_TRUE);
    RenderState::culling.init(GL_TRUE);
    RenderState::cullFace.init(GL_BACK);
    RenderState::frontFace.init(GL_CCW);
    RenderState::blending.init(GL_FALSE);
}

} // RenderState
} // OGLW