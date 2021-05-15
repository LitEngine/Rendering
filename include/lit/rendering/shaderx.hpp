#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <experimental/source_location>

#include <lit/gl/shader.hpp>
#include <lit/gl/frame_buffer.hpp>

#define GLM_FORCE_SWIZZLE

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtx/extended_min_max.hpp>
#include <vector>

#define GLSL_START(name) std::string GetName() const override { return name; }
#define GLSL_END() source_location GetLocation() const override { return source_location::current(); }

#define uniform
#define in
#define out

#define GLSL_ARRAY(...) { __VA_ARGS__ }

#define layout(...)

#define GLSL_FINAL_NODE() bool IsFinalNode() const override { return true; }

#define GLSL_IN(type, name) \
    void _helper_ ## name() { \
        int location = m_in_declarations.size();                    \
        m_in_declarations.push_back(::format("layout(location=%d) in %s %s;", location, #type, #name)); \
    } \
    type name = (_helper_ ## name(),type());

#define GLSL_OUT(type, name) \
    void _helper_ ## name() { \
        int location = m_out_declarations.size();                    \
        m_out_declarations.push_back(::format("layout(location=%d) out %s %s;", location, #type, #name)); \
    } \
    type name = (_helper_ ## name(), type());

#define GLSL_OUT_FRAG(type, name, texture) \
    void _helper_ ## name() { \
        int location = m_out_declarations.size(); \
        m_outputs.push_back(lit::gl::GlFbAttachment::texture);                           \
        m_out_declarations.push_back(::format("layout(location=%d) out %s %s;", location, #type, #name)); \
    } \
    type name = (_helper_ ## name(), type());

#define GLSL_UNI(type, name) \
    void _helper_ ## name() { \
        m_uniform_declarations.push_back(::format("uniform %s %s;", #type, #name)); \
        m_uniform_locations.emplace_back(#name, &(u_ ## name.m_location)); \
    }                        \
    public:                  \
    Uniform<type> u_ ## name = Uniform<type>();                                     \
    private:                 \
    type name = (_helper_ ## name(), type());               \

#define GLSL_DEPTH(condition)\
    void _helper_ ## name() { \
        int location = m_out_declarations.size();                    \
        m_outputs.push_back(lit::gl::GlFbAttachment::Depth);                           \
        m_out_declarations.push_back(::format("layout(depth_%s) out float gl_FragDepth;", location, #condition)); \
    } \
    float gl_FragDepth;

template<typename ... Args>
std::string format(const std::string &format, Args ... args) {
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
    auto size = static_cast<size_t>( size_s );
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1);
}

namespace lit::rendering {
    template<typename T>
    void SetUniform(GLint location, T val);

    template<typename T>
    struct Uniform {
        T m_val = T();
        GLint m_location = -1;

        Uniform() = default;

        Uniform<T>& operator=(const T & val) {
            if (val != m_val) {
                m_val = val;
                SetUniform(m_location, m_val);
            }
            return *this;
        }
    };

    class ShaderX : public gl::GlShader {
    public:

        explicit ShaderX(gl::GlShaderType type);

        virtual ~ShaderX() = default;

        void Init();

        std::vector<gl::GlFbAttachment> GetOutputs() const;

        template<typename V, typename F>
        friend class PipelineNode;

    protected:
        using source_location = std::experimental::source_location;

        std::string GetGlslCode() const;

        virtual bool IsFinalNode() const;

        virtual source_location GetLocation() const;

        virtual std::string GetName() const;

        std::vector<std::pair<std::string, GLint *>> m_uniform_locations;
        std::vector<gl::GlFbAttachment> m_attachments;

        std::vector<gl::GlFbAttachment> m_outputs;
        std::vector<std::string> m_in_declarations;
        std::vector<std::string> m_out_declarations;
        std::vector<std::string> m_uniform_declarations;
    };

    class VertShader : public ShaderX {
    public:
        VertShader() : ShaderX(gl::GlShaderType::VERT) {}

        ~VertShader() override = default;

    protected:
        glm::vec4 gl_Position{};
    };

    class FragShader : public ShaderX {
    public:
        FragShader() : ShaderX(gl::GlShaderType::FRAG) {}

        ~FragShader() override = default;

    protected:
        glm::vec4 gl_FragCoord{};
    };

    class CompShader : public ShaderX {
    public:
        CompShader() : ShaderX(gl::GlShaderType::COMP) {}

        ~CompShader() override = default;
    };

}

namespace glm {

    template<typename T, typename S, int DIM, typename R = decltype(T() -
                                                                    S()), typename = std::enable_if_t<!std::is_same<T, S>::value>>
    vec<DIM, R> operator-(const vec<DIM, T> &lhs, const vec<DIM, S> &rhs) {
        return vec<DIM, R>(lhs) - vec<DIM, R>(rhs);
    }

    template<typename T, typename S, int DIM, typename R = decltype(T() +
                                                                    S()), typename = std::enable_if_t<!std::is_same<T, S>::value>>
    vec<DIM, R> operator+(const vec<DIM, T> &lhs, const vec<DIM, S> &rhs) {
        return vec<DIM, R>(lhs) + vec<DIM, R>(rhs);
    }

    template<typename T, typename S, int DIM, typename R = decltype(T() *
                                                                    S()), typename = std::enable_if_t<!std::is_same<T, S>::value>>
    vec<DIM, R> operator*(const vec<DIM, T> &lhs, const vec<DIM, S> &rhs) {
        return vec<DIM, R>(lhs) * vec<DIM, R>(rhs);
    }

    template<typename T, typename S, int DIM, typename R = decltype(T() *
                                                                    S()), typename = std::enable_if_t<!std::is_same<T, S>::value>, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
    vec<DIM, R> operator*(const vec<DIM, T> &lhs, const S &rhs) {
        return vec<DIM, R>(lhs) * vec<DIM, R>(rhs);
    }
}
