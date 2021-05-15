#pragma once

#include "shader.hpp"

namespace lit::gl {

    class GlProgram {
    public:
        GlProgram() = default;

        ~GlProgram();

        template<typename ...Args>
        void Init(const Args &...args) {
            if (m_program_id)
                return;
            m_program_id = glCreateProgram(); // todo: log error
            AttachShader(args...);
            glLinkProgram(m_program_id);
        }

        void AttachShader() {}

        template<typename T, typename ...Args>
        void AttachShader(const T &shader, Args ... args) {
            glAttachShader(m_program_id, shader.m_shader_id);
            AttachShader(args...);
        }

        int GetUniformLocation(const std::string &name);

        void Delete();

        void Use();

    protected:
        GLuint m_program_id = 0;
    };

}
