#pragma once

#include <string>
#include <GL/glew.h>

namespace lit::gl {

    enum class GlShaderType : GLenum {
        NONE = 0,
        VERT = GL_VERTEX_SHADER,
        FRAG = GL_FRAGMENT_SHADER,
        COMP = GL_COMPUTE_SHADER
    };

    class GlShader {
    public:

        GlShader() = default;

        ~GlShader();

        void Init(GlShaderType type, const std::string & source_code);

        void Delete();

        friend class GlProgram;

    protected:
        GLuint m_shader_id;
        GlShaderType m_type;
    };
}