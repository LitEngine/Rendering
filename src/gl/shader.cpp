#include <lit/gl/shader.hpp>
#include <iostream>

using namespace lit::gl;

void GlShader::Init(GlShaderType type, const std::string &source_code) {
    if (m_shader_id)
        return;
    m_type = type;
    const char *source_c_str = source_code.c_str();
    m_shader_id = glCreateShader((GLenum) type);
    glShaderSource(m_shader_id, 1, &source_c_str, nullptr);
    glCompileShader(m_shader_id);

    int success;
    char infoLog[512];
    glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(m_shader_id, 512, nullptr, infoLog);
        // todo: log
        std::cerr << infoLog << std::endl;
    }
}

void GlShader::Delete() {
    if (m_shader_id) {
        glDeleteShader(m_shader_id);
        m_shader_id = 0;
    }
}

GlShader::~GlShader() {
    Delete();
}
