#include <algorithm>
#include <lit/gl/frame_buffer.hpp>

using namespace lit::gl;

GlFrameBuffer::~GlFrameBuffer() {
    Delete();
}

void GlFrameBuffer::Init(const GlFrameBufferInfo &info) {
    if (m_frame_buffer_id)
        Delete();

    m_info = info;

    glGenFramebuffers(1, &m_frame_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer_id);

    for (auto &attachment : m_info.attachments) {
        if (attachment == GlFbAttachment::Depth) {
            m_has_depth_attachment = true;
            continue;
        }

        GlTexture2DInfo texture_info;
        texture_info.width = m_info.width;
        texture_info.height = m_info.height;
        texture_info.internal_format = (GlTextureInternalFormat) attachment;

        auto index = m_color_attachments.size();
        m_color_attachments.push_back(std::make_shared<GlTexture2D>());
        m_color_attachments[index]->Init(texture_info);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D,
                               m_color_attachments[index]->m_texture_id, 0);
    }

    if (m_has_depth_attachment) {
        glGenRenderbuffers(1, &m_depth_render_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depth_render_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, m_info.width, m_info.height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_render_buffer);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GlFrameBuffer::Delete() {
    if (m_frame_buffer_id) {
        if (m_depth_render_buffer) {
            glDeleteRenderbuffers(1, &m_depth_render_buffer);
            m_depth_render_buffer = 0;
        }
        std::for_each(m_color_attachments.begin(), m_color_attachments.end(), [](auto t) { t->Delete(); });
        m_color_attachments.clear();
        m_frame_buffer_id = 0;
        glDeleteFramebuffers(1, &m_frame_buffer_id);
    }
}

void GlFrameBuffer::Resize(int width, int height) {
    m_info.width = width;
    m_info.height = height;
    Delete();
    Init(m_info);
}

void GlFrameBuffer::Bind() {
    if (m_frame_buffer_id) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer_id);
        if (m_has_depth_attachment) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        if (!m_color_attachments.empty()) {
            size_t num = m_color_attachments.size();
            auto *color_attachments = new GLenum[num];

            for (size_t i = 0; i < num; i++) {
                color_attachments[i] = GL_COLOR_ATTACHMENT0 + i;
            }

            glDrawBuffers((int) num, color_attachments);

            delete[] color_attachments;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
