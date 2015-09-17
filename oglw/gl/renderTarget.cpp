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
    m_texture = std::make_unique<Texture>(_width, _height);
    m_texture->update(0);
    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->getGlHandle(), 0));

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
        TextureOptions depthTextureOptions = {
            GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT,
            {GL_NEAREST, GL_NEAREST},
            {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE}
        };
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

void RenderTarget::applyDefault(uint _width, uint _height) {
    GLenum clearBufferBits = GL_COLOR_BUFFER_BIT;

    clearBufferBits |= RenderState::depthWrite.compare(GL_TRUE) ? clearBufferBits : GL_DEPTH_BUFFER_BIT;
    clearBufferBits |= RenderState::stencilWrite.compare(GL_TRUE) ? clearBufferBits : GL_STENCIL_BUFFER_BIT;

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL_CHECK(glViewport(0, 0, _width, _height));
    GL_CHECK(glClear(clearBufferBits));

    // enable back color buffer read/write
    RenderState::drawBuffer(GL_BACK);
    RenderState::readBuffer(GL_BACK);
}

void RenderTarget::bindRenderTexture(GLuint _slot, GLuint _depthTextureSlot) {
    if (m_depthTexture && _slot == _depthTextureSlot) {
        WARN("Binding render target textures to same slots\n");
    }

    m_texture->bind(_slot);

    if (m_depthTexture) {
        m_depthTexture->bind(_depthTextureSlot);
    }
}

void RenderTarget::apply(uint _width, uint _height) {
    if (!m_isValid) {
        WARN("Invalid render target\n");
        return;
    }

    m_texture->resize(_width, _height);
    m_texture->update(0);

    if (m_depthTexture) {
        m_depthTexture->resize(_width, _height);
        m_depthTexture->update(0);
    }

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    GL_CHECK(glViewport(0, 0, _width, _height));
    GL_CHECK(glDisable(GL_SCISSOR_TEST));
    GLenum clearBufferBits = GL_COLOR_BUFFER_BIT;
    GL_CHECK(glClearColor(0.0, 0.0, 0.0, 0.0));

    if (m_setup.useDepth) {
        RenderState::depthWrite(m_setup.useDepth);
        RenderState::stencilWrite(m_setup.useStencil);
        GL_CHECK(glClearDepth(1.0));
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
