#include "renderTarget.h"
#include "log.h"
#include "renderState.h"

namespace OGLW {

RenderTarget::RenderTarget(RenderTargetSetup _setup) {
    m_renderBuffer = 0;
    m_fbo = 0;
    m_setup = _setup;
    m_isValid = _setup.isValid();
}

void RenderTarget::create(uint _width, uint _height) {
    if (m_fbo) {
        return;
    }

    if (!m_isValid) {
        WARN("Invalid render target\n");
    }

    GLint framebufferBound = 0;
    GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferBound));

    GL_CHECK(glGenFramebuffers(1, &m_fbo));
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

    if (!m_setup.useDepthTexture) {
        m_texture = std::make_unique<Texture>(_width, _height);
        m_texture->update(0);
        GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->getGlHandle(), 0));
    }

    if (m_setup.useDepth) {
        GLenum renderBufferTarget;
        GL_CHECK(glGenRenderbuffers(1, &m_renderBuffer));
        GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer));
        if (m_setup.useStencil) {
            renderBufferTarget = GL_DEPTH_STENCIL_ATTACHMENT;
            GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height));
        } else {
            renderBufferTarget = GL_DEPTH_ATTACHMENT;
            GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, _width, _height));
        }
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, renderBufferTarget, GL_RENDERBUFFER, m_renderBuffer));
    } else if (m_setup.useDepthTexture) {
        TextureOptions depthTextureOptions;

        depthTextureOptions.internalFormat = GL_DEPTH_COMPONENT32;
        depthTextureOptions.format = GL_DEPTH_COMPONENT;
        depthTextureOptions.type = GL_FLOAT;
        depthTextureOptions.filtering = { GL_NEAREST, GL_NEAREST };
        depthTextureOptions.wrapping = { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
        //depthTextureOptions.depthOptions.textureMode = GL_INTENSITY;
        //depthTextureOptions.depthOptions.compareMode = GL_TEXTURE_COMPARE_MODE;
        //depthTextureOptions.depthOptions.compareFunc = RenderState::depthFunc.get<0>();
        //depthTextureOptions.isDepthTexture = true;

        m_depthTexture = std::make_unique<Texture>(_width, _height, depthTextureOptions);
        m_depthTexture->update(0);
        GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->getGlHandle(), 0));
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    GL_CHECK(void(0));
    if (GL_FRAMEBUFFER_COMPLETE != status) {
        ERROR("Framebuffer incomplete\n");
        m_isValid = false;
    }

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, framebufferBound));
}

void RenderTarget::applyDefault(uint _width, uint _height, bool _clear) {
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL_CHECK(glViewport(0, 0, _width, _height));

    if (_clear) {
        GLenum clearBufferBits = GL_COLOR_BUFFER_BIT;

        clearBufferBits |= RenderState::depthWrite.compare(GL_TRUE) ? clearBufferBits : GL_DEPTH_BUFFER_BIT;
        clearBufferBits |= RenderState::stencilWrite.compare(GL_TRUE) ? clearBufferBits : GL_STENCIL_BUFFER_BIT;

        GL_CHECK(glClear(clearBufferBits));
    }

    // enable back color buffer read/write
    RenderState::drawBuffer(GL_BACK);
    RenderState::readBuffer(GL_BACK);
}

void RenderTarget::bindRenderTexture(GLuint _slot) {
    if (m_texture) {
        m_texture->bind(_slot);
    } else if (m_depthTexture) {
        m_depthTexture->bind(_slot);
    }
}

void RenderTarget::apply(uint _width, uint _height, uint _clearColor) {
    if (!m_isValid) {
        WARN("Invalid render target\n");
        return;
    }

    if (m_texture) {
        m_texture->resize(_width, _height);
        m_texture->update(0);
    } else if (m_depthTexture) {
        m_depthTexture->resize(_width, _height);
        m_depthTexture->update(0);
    }

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    GL_CHECK(glViewport(0, 0, _width, _height));
    GL_CHECK(glDisable(GL_SCISSOR_TEST));
    GLenum clearBufferBits = GL_COLOR_BUFFER_BIT;

    if (m_setup.useDepthTexture || m_setup.useDepth) {
        RenderState::depthWrite(GL_TRUE);
        GL_CHECK(glClearDepth(1.0));
    }

    if (m_setup.useDepth) {
        RenderState::stencilWrite(m_setup.useStencil);
        if (m_setup.useStencil) {
            GL_CHECK(glClearStencil(0));
        }
        clearBufferBits |= m_setup.useStencil ? GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT : GL_DEPTH_BUFFER_BIT;
    }

    if (m_setup.useDepthTexture) {
        // disable render to color buffer and clear only depth buffer
        clearBufferBits = GL_DEPTH_BUFFER_BIT;
        RenderState::drawBuffer(GL_NONE);
        RenderState::readBuffer(GL_NONE);
    } else {
        float r = (_clearColor >> 24) & 0xff;
        float g = (_clearColor >> 16) & 0xff;
        float b = (_clearColor >>  8) & 0xff;
        float a = (_clearColor >>  0) & 0xff;

        GL_CHECK(glClearColor(r / 255.0, g / 255.0, b / 255.0, a / 255.0));
        RenderState::drawBuffer(GL_BACK);
        RenderState::readBuffer(GL_BACK);
    }

    GL_CHECK(glClear(clearBufferBits));
}

RenderTarget::~RenderTarget() {
    if (m_fbo) {
        GL_CHECK(glDeleteFramebuffers(1, &m_fbo));
    }

    if (m_renderBuffer) {
        GL_CHECK(glDeleteRenderbuffers(1, &m_renderBuffer));
    }
}

} // OGLW
