#pragma once

#include "texture.h"
#include <memory>
#include "glTypes.h"

namespace OGLW {

struct RenderTargetSetup {
    bool useDepth = false;

    // Using the depth texture as default render target 
    // would disable color draw buffer read and write
    bool useDepthTexture = false;
    bool useStencil = false;

    bool isValid() {
        return !((useDepth || useStencil) && useDepthTexture);
    }
};

class RenderTarget {

public:  
    RenderTarget(RenderTargetSetup _setup = {});
    ~RenderTarget();

    // creates the render target and initialize the renderTexture/renderBuffer
    void create(uint _width, uint _height);
    // apply a render target for anything going to be rendered in the viewport
    void apply(uint _width, uint _height);
    // get the render target texture handle
    const std::unique_ptr<Texture>& getRenderTexture() const { return m_texture; }
    // apply the default render target
    static void applyDefault(uint _width, uint _height, bool _clear = false);
    // bind the render texture to the specified slot
    void bindRenderTexture(GLuint _slot);

private:
    // the render texture
    std::unique_ptr<Texture> m_texture;
    // the render texture
    std::unique_ptr<Texture> m_depthTexture;
    // the GL framebuffer handle
    GLuint m_fbo;
    // the render buffer depth/stencil
    GLuint m_renderBuffer;
    // the render target setup options
    RenderTargetSetup m_setup;
    // whether the render target is valid
    bool m_isValid;

};

} // OGLW
