#include <lit/gl/texture.hpp>

using namespace lit::gl;

GlTexture3D::~GlTexture3D() {
    Delete();
}

void GlTexture2D::Init(const GlTexture2DInfo &textureInfo) {
    if (m_texture_id) {
        Delete();
    }

    m_info = textureInfo;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int) m_info.min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int) m_info.mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // todo:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, (int) m_info.internal_format, m_info.width, m_info.height, 0,
                 (int) m_info.data_format,
                 (int) m_info.data_type, m_info.data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GlTexture2D::~GlTexture2D() {
    Delete();
}

void GlTexture2D::Delete() {
    if (m_texture_id) {
        glDeleteTextures(1, &m_texture_id);
        m_texture_id = 0;
    }
}

void GlTexture2D::Resize(int width, int height) {
    m_info.width = width;
    m_info.height = height;
    Delete();
    Init(m_info);
}

void GlTexture2D::Bind(int texture_index) {
    if (m_texture_id) {
        glActiveTexture(GL_TEXTURE0 + texture_index);
        glBindTexture(GL_TEXTURE_2D, m_texture_id);
    }
}

void GlTexture3D::Init(const GlTexture3DInfo &textureInfo) {
    if (m_texture_id) {
        Delete();
    }

    m_info = textureInfo;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_3D, m_texture_id);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, (int) m_info.min_filter);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, (int) m_info.mag_filter);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // todo:
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexImage3D(GL_TEXTURE_3D, 0, (int) m_info.internal_format, m_info.width, m_info.height, m_info.depth, 0,
                 (int) m_info.data_format,
                 (int) m_info.data_type, m_info.data);
    glBindTexture(GL_TEXTURE_3D, 0);
}

void GlTexture3D::Delete() {
    if (m_texture_id) {
        glDeleteTextures(1, &m_texture_id);
        m_texture_id = 0;
    }
}

void GlTexture3D::Resize(int width, int height, int depth) {
    m_info.width = width;
    m_info.height = height;
    m_info.depth = depth;
    Delete();
    Init(m_info);
}

void GlTexture3D::Bind(int texture_index) {
    if (m_texture_id) {
        glActiveTexture(GL_TEXTURE0 + texture_index);
        glBindTexture(GL_TEXTURE_3D, m_texture_id);
    }
}
