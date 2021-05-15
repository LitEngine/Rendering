#include <lit/rendering/shaderx.hpp>

bool replace(std::string &str, const std::string &from, const std::string &to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

bool replace(std::string &str, size_t start, const std::string &from, const std::string &to) {
    size_t start_pos = str.find(from, start);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

bool replace_define(std::string &code, const std::string &define) {
    size_t pos = code.find(define);
    if (pos == std::string::npos)
        return false;
    size_t end = code.find(')', pos) + 2;
    return replace(code, code.substr(pos, end - pos), "");
}

using namespace glm;
using namespace lit::rendering;

ShaderX::ShaderX(lit::gl::GlShaderType type) : GlShader() {
    this->m_type = type;
}

void ShaderX::Init() {
    gl::GlShader::Init(m_type, GetGlslCode());
}

std::string ShaderX::GetGlslCode() const {
    std::ifstream fin(GetLocation().file_name());

    std::string code((std::istreambuf_iterator<char>(fin)),
                     std::istreambuf_iterator<char>());

    size_t pos = code.find(GetName()) - 16;
    size_t pos_start = code.find(std::string("GLSL") + "_START", pos) + 14 + GetName().length();
    size_t pos_end = code.find(std::string("GLSL") + "_END", pos) - 5;

    if (pos_start == std::string::npos || pos_end == std::string::npos) {
        return "";
    }

    code = code.substr(pos_start, pos_end - pos_start);

    replace(code, "GLSL_ARRAY", "int[9]"); // todo:

    std::stringstream ss(code);
    std::string line;

    code = "";

    code = "#version 450\n\n"; // todo: parameter

    if (!m_in_declarations.empty()) {
        for (const auto &str : m_in_declarations) code += str + "\n";
        code += "\n";
    }
    if (!m_out_declarations.empty()) {
        for (const auto &str : m_out_declarations) code += str + "\n";
        code += "\n";
    }
    if (!m_uniform_declarations.empty()) {
        for (const auto &str : m_uniform_declarations) code += str + "\n";
        code += "\n";
    }

    std::string prev = "";
    while (std::getline(ss, line, '\n')) {
        if (line.length() > 4) line = line.substr(4);
        if (prev.empty() && line.empty()) continue;
        code += line + "\n";
        prev = line;
    }

    for (char a : "xyz") {
        for (char b: "xyz") {
            for (char c : "xyz") {
                std::string r = std::string() + a + b + c;
                while (replace(code, r + "()", r)) {}
            }
        }
    }

    return code;
}

ShaderX::source_location ShaderX::GetLocation() const {
    return source_location::current();
}

std::string ShaderX::GetName() const {
    return "";
}

std::vector<lit::gl::GlFbAttachment> ShaderX::GetOutputs() const {
    if (IsFinalNode())
        return {};
    return m_outputs;
}

bool ShaderX::IsFinalNode() const {
    return false;
}

template<>
void lit::rendering::SetUniform<int>(GLint location, int val) {
    glUniform1i(location, val);
}

template<>
void lit::rendering::SetUniform<float>(GLint location, float val) {
    glUniform1f(location, val);
}

template<>
void lit::rendering::SetUniform<glm::ivec3>(GLint location, glm::ivec3 val) {
    glUniform3i(location, val.x, val.y, val.z);
}

template<>
void lit::rendering::SetUniform<glm::vec3>(GLint location, glm::vec3 val) {
    glUniform3f(location, val.x, val.y, val.z);
}
