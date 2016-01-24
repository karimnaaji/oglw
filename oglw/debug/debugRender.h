#include "debug_draw.hpp"
#include "gl/gl.h"
#include "gl/shader.h"
#include "gl/vertexLayout.h"
#include "core/camera.h"
#include <memory>

namespace OGLW {

class DebugRender : public dd::RenderInterface {
public:
    void init();
    virtual void beginDraw() override;
    virtual void endDraw() override;
    virtual ~DebugRender() override;

    virtual void drawPointList(const dd::DrawVertex* _points, int _count, bool _depthEnabled) override;
    virtual void drawLineList(const dd::DrawVertex* _lines, int _count, bool _depthEnabled) override;
    virtual void drawGlyphList(const dd::DrawVertex* _glyphs, int _count, dd::GlyphTextureHandle _glyphTex) override;
    virtual dd::GlyphTextureHandle createGlyphTexture(int _width, int _height, const void* _pixels) override;
    virtual void destroyGlyphTexture(dd::GlyphTextureHandle _glyphTex) override;
    void setMVP(glm::mat4 _mvp) { m_mvp = _mvp; }

private:
    std::unordered_map<std::string, GLuint> m_layoutLocations;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexLayout> m_layout;
    GLint m_boundBuffer;
    glm::mat4 m_mvp;
};

} // OGLW
