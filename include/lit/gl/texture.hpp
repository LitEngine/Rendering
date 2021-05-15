#pragma once

#include <GL/glew.h>

namespace lit::gl {

    enum class GlTextureMinFilter : GLint {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        NearestMipMapNearest = GL_NEAREST_MIPMAP_NEAREST,
        LinearMipMapNearest = GL_LINEAR_MIPMAP_NEAREST,
        NearestMipMapLinear = GL_NEAREST_MIPMAP_LINEAR,
        LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR
    };

    enum class GlTextureMagFilter : GLint {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR
    };

    enum class GlTextureInternalFormat : GLint {
        R8UI = GL_R8UI,
        RGB = GL_RGB,
        RGBA = GL_RGBA,
        RGB32F = GL_RGB32F,
        RGBA32F = GL_RGBA32F,
        RGB32I = GL_RGB32I,
        RGBA32I = GL_RGBA32I,
        RGB32UI = GL_RGB32UI,
        RGBA32UI = GL_RGBA32UI,
    };

    enum class GlTextureDataFormat : GLint {
        RedInteger = GL_RED_INTEGER,
        Red = GL_RED,
        RGB = GL_RGB,
        RGBA = GL_RGBA
    };

    enum class GlTextureDataType : GLint {
        UnsignedByte = GL_UNSIGNED_BYTE,
        Float = GL_FLOAT
    };

    struct GlTexture2DInfo {
        GlTextureMinFilter min_filter = GlTextureMinFilter::Nearest;
        GlTextureMagFilter mag_filter = GlTextureMagFilter::Nearest;
        GlTextureInternalFormat internal_format = GlTextureInternalFormat::RGB;
        GlTextureDataFormat data_format = GlTextureDataFormat::RGB;
        GlTextureDataType data_type = GlTextureDataType::UnsignedByte;
        void *data = nullptr;
        int width = 512;
        int height = 512;
    };

    struct GlTexture3DInfo : public GlTexture2DInfo {
        int depth = 512;
    };

    class GlTexture2D {
    public:
        GlTexture2D() = default;

        ~GlTexture2D();

        void Init(const GlTexture2DInfo &textureInfo);

        void Delete();

        void Resize(int width, int height);

        void Bind(int texture_index);

        friend class GlFrameBuffer;

    private:
        GLuint m_texture_id = 0;
        GlTexture2DInfo m_info;
    };

    class GlTexture3D {
    public:
        GlTexture3D() = default;

        ~GlTexture3D();

        void Init(const GlTexture3DInfo &textureInfo);

        void Delete();

        void Resize(int width, int height, int depth);

        void Bind(int texture_index);

    private:
        GLuint m_texture_id = 0;
        GlTexture3DInfo m_info;
    };

}
