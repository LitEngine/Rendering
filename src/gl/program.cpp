#include <lit/gl/program.hpp>

using namespace lit::gl;

GlProgram::~GlProgram() {
    Delete();
}

int GlProgram::GetUniformLocation(const std::string &name) {
    return glGetUniformLocation(m_program_id, name.c_str());
}

void GlProgram::Use() {
    if (m_program_id) {
        glUseProgram(m_program_id);
    }
}

void GlProgram::Delete() {
    if (m_program_id) {
        glDeleteProgram(m_program_id);
    }
}
