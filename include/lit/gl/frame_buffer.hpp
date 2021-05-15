#pragma once

#include <memory>
#include <vector>
#include "texture.hpp"

namespace lit::gl {

    enum class GlFbAttachment {
        Depth = -1,
        RGB = (int) GlTextureInternalFormat::RGB,
        RGBA = (int) GlTextureInternalFormat::RGBA,
        RGB32 = (int) GlTextureInternalFormat::RGB32F,
        RGBA32 = (int) GlTextureInternalFormat::RGBA32F,
        RGB32I = (int) GlTextureInternalFormat::RGB32I,
        RGBA32I = (int) GlTextureInternalFormat::RGBA32I,
        RGB32UI = (int) GlTextureInternalFormat::RGB32UI,
        RGBA32UI = (int) GlTextureInternalFormat::RGBA32UI,
    };

    struct GlFrameBufferInfo {
        int width = 512;
        int height = 512;

        std::vector<GlFbAttachment> attachments;
    };

    class GlFrameBuffer {
    public:
        GlFrameBuffer() = default;

        ~GlFrameBuffer();

        void Init(const GlFrameBufferInfo &info);

        void Delete();

        void Resize(int width, int height);

        void Bind();

    private:
        GLuint m_frame_buffer_id = 0;
        GLuint m_depth_render_buffer = 0;
        GlFrameBufferInfo m_info;
        bool m_has_depth_attachment = false;
        std::vector<std::shared_ptr<GlTexture2D>> m_color_attachments;
    };

}
