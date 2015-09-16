#include "renderTarget.h"
#include "log.h"
#include "renderState.h"

namespace OGLW {

RenderTarget::RenderTarget(RenderTargetSetup _setup) {
    m_renderBuffer = 0;
    m_fbo = 0;
    m_setup = _setup;
}

void RenderTarget::create(unsigned int _width, unsigned int _height) {
    if (m_fbo != 0) {
        return;
    }

    GLint framebufferBound = 0;
    GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferBound));

    GL_CHECK(glGenFramebuffers(1, &m_fbo));
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    m_texture = std::make_unique<OGLW::Texture>(_width, _height);
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
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    GL_CHECK(void(0));
    if (GL_FRAMEBUFFER_COMPLETE != status) {
        ERROR("Framebuffer incomplete\n");
    }

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, framebufferBound));
}

void RenderTarget::apply(unsigned int _width, unsigned int _height) {
    m_texture->resize(_width, _height);
    m_texture->update(0);
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    GL_CHECK(glViewport(0, 0, _width, _height));
    GL_CHECK(glDisable(GL_SCISSOR_TEST));
    GLenum clearBufferBits = GL_COLOR_BUFFER_BIT;
    GL_CHECK(glClearColor(0.0, 0.0, 0.0, 0.0));

    if (m_setup.useDepth) {
        RenderState::depthWrite(m_setup.useDepth);
        RenderState::stencilWrite(m_setup.useStencil);
        GL_CHECK(glClearDepth(0.0));
        if (m_setup.useStencil) {
            GL_CHECK(glClearStencil(0));
        }
        clearBufferBits |= m_setup.useStencil ? GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT : GL_DEPTH_BUFFER_BIT;
    }

    GL_CHECK(glClear(clearBufferBits));
}

RenderTarget::~RenderTarget() {
    if (m_fbo != 0) {
        GL_CHECK(glDeleteFramebuffers(1, &m_fbo));
    }

    if (m_renderBuffer != 0) {
        GL_CHECK(glDeleteRenderbuffers(1, &m_renderBuffer));
    }
}

}