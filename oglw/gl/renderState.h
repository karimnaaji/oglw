#pragma once

#include "gl/gl.h"
#include <tuple>
#include <limits>

namespace OGLW {

namespace RenderState {

void push();
void pop();
void initialize();
GLuint getTextureUnit(GLuint _unit);
void activeTextureUnit(GLuint _unit);
void bindTexture(GLenum _target, GLuint _textureId);

template <typename T>
class State {
public:
    void init(const typename T::Type& _default) {
        T::set(_default);
        m_current = _default;
    }

    inline void operator()(const typename T::Type& _value) {
        if (m_current != _value) {
            m_current = _value;
            T::set(m_current);
        }
    }

    inline typename T::Type get() {
        return m_current;
    }

private:
    typename T::Type m_current;
};

template <GLenum N>
struct BoolSwitch {
    using Type = int;
    inline static void set(const Type& _type) {
        if (_type) {
            GL_CHECK(glEnable(N));
        } else {
            GL_CHECK(glDisable(N));
        }
    }
};

// http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
// Generate integer sequece for getting values from 'params' tuple.
template<int ...> struct seq {};
template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};
template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };

template <typename F, F fn, typename ...Args>
struct StateWrap {

    using Type = std::tuple<Args...>;
    Type params;

    void init(Args... _param, bool _force = true) {
        params = std::make_tuple(_param...);
        if (_force) {
            call(typename gens<sizeof...(Args)>::type());
        }
    }

    inline void operator()(Args... _args) {
        auto _params = std::make_tuple(_args...);

        if (_params != params) {
            params = _params;
            call(typename gens<sizeof...(Args)>::type());
        }
    }

    inline bool compare(Args... _args) {
        auto _params = std::make_tuple(_args...);
        return _params == params;
    }

    template<int S>
    inline auto get() {
        return std::get<S>(params);
    }

    template<int ...S>
    inline void call(seq<S...>) {
        GL_CHECK(fn(std::get<S>(params) ...));
    }
};

#define FUN(X) decltype((X)), X

using DepthTest = State<BoolSwitch<GL_DEPTH_TEST>>;
using StencilTest = State<BoolSwitch<GL_STENCIL_TEST>>;
using Blending = State<BoolSwitch<GL_BLEND>>;
using Culling = State<BoolSwitch<GL_CULL_FACE>>;
using DepthWrite = StateWrap<FUN(glDepthMask), GLboolean>;
using DepthFunc = StateWrap<FUN(glDepthFunc), GLenum>;
using BlendingFunc = StateWrap<FUN(glBlendFunc), GLenum, GLenum>;
using StencilWrite = StateWrap<FUN(glStencilMask), GLuint>;
using StencilFunc = StateWrap<FUN(glStencilFunc), GLenum, GLint, GLuint>;
using StencilOp = StateWrap<FUN(glStencilOp), GLenum, GLenum, GLenum>;
using ColorWrite = StateWrap<FUN(glColorMask), GLboolean, GLboolean, GLboolean, GLboolean>;
using FrontFace = StateWrap<FUN(glFrontFace), GLenum>;
using CullFace = StateWrap<FUN(glCullFace), GLenum>;
using ClearDepth = StateWrap<FUN(glClearDepth), GLclampd>;
using DepthRange = StateWrap<FUN(glDepthRange), GLclampd, GLclampd>;
using ShaderProgram = StateWrap<FUN(glUseProgram), GLuint>;
using TextureUnit = StateWrap<FUN(activeTextureUnit), GLuint>;
using Texture = StateWrap<FUN(bindTexture), GLenum, GLuint>;
using DrawBuffer = StateWrap<FUN(glDrawBuffer), GLenum>;
using ReadBuffer = StateWrap<FUN(glReadBuffer), GLenum>;

extern DepthTest depthTest;
extern DepthWrite depthWrite;
extern Blending blending;
extern BlendingFunc blendingFunc;
extern DepthFunc depthFunc;
extern StencilTest stencilTest;
extern StencilWrite stencilWrite;
extern StencilFunc stencilFunc;
extern StencilOp stencilOp;
extern ColorWrite colorWrite;
extern FrontFace frontFace;
extern CullFace cullFace;
extern Culling culling;
extern ClearDepth clearDepth;
extern DepthRange depthRange;
extern ShaderProgram shaderProgram;
extern TextureUnit textureUnit;
extern Texture texture;
extern DrawBuffer drawBuffer;
extern ReadBuffer readBuffer;

} // RenderState
} // OGLW
