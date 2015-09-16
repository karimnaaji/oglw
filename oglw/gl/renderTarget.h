#pragma once

#include "texture.h"
#include <memory>
#include "glTypes.h"

namespace OGLW {

struct RenderTargetSetup {
    bool useDepth = false;
    bool useStencil = false;
};

class RenderTarget {

public:  
    RenderTarget(RenderTargetSetup _setup = {});
    ~RenderTarget();

    // creates the render target and initialize the renderTexture/renderBuffer
    void create(unsigned int _width, unsigned int _height);
    // apply a render target for anything going to be rendered in the viewport
    void apply(unsigned int _width, unsigned int _height);
    // get the render target texture handle
    const std::unique_ptr<Texture>& getRenderTexture() const { return m_texture; }

private:
    // the render texture
    std::unique_ptr<Texture> m_texture;
    // the GL framebuffer handle
    GLuint m_fbo;
    // the render buffer depth/stencil
    GLuint m_renderBuffer;
    // the render target setup options
    RenderTargetSetup m_setup;

};

}