#pragma once

#define GLSL_UNIFORM(type, name) \
    type name; \
public:                             \
    GlUniform<type> u_ ## name;   \
private: \
    void _add_location_ptr ## name() {m_uniform_locations.emplace_back(#name, &(u_ ## name).m_location);} \
    int _helper_ ## name = (_add_location_ptr ## name(),0);

template<typename T>
struct GlUniform {
    GLint m_location = -1;
    T m_value = T();

    GlUniform() = default;

    GlUniform &operator=(const T &val) {
        if (m_value == val && m_value != T())
            return *this;
        m_value = val;
        if (std::is_same<T, int>::value) {
            glUniform1i(m_location, m_value);
        }
        if (std::is_same<T, float>::value) {
            glUniform1f(m_location, m_value);
        }
        /*if (std::is_same<T, glm::ivec3>::value) {
            glUniform3i(m_location, m_value.x, m_value.y, m_value.z);
        }
        if (std::is_same<T, glm::vec3>::value) {
            glUniform3f(m_location, m_value.x, m_value.y, m_value.z);
        }*/
        return *this;
    }
};
