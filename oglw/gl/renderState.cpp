#include "renderState.h"

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

void initialize() {
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
}

} // RenderState
} // OGLW
