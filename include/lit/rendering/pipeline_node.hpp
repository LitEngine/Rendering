#pragma once

#include <functional>

#include <lit/gl/program.hpp>
#include <lit/gl/frame_buffer.hpp>
#include "shaderx.hpp"

namespace lit::rendering {

    class PipelineNodeAny : protected gl::GlProgram {
    public:
        PipelineNodeAny() = default;

        virtual ~PipelineNodeAny() = default;

        void ResizeOutput(int width, int height);

        virtual void Run();

    protected:
        std::shared_ptr<ShaderX> m_vert_shader = nullptr;
        std::shared_ptr<ShaderX> m_frag_shader = nullptr;
        std::unique_ptr<gl::GlFrameBuffer> m_output = nullptr;
    };

    template<typename VertShaderT, typename FragShaderT>
    class PipelineNode : public PipelineNodeAny {
    public:
        explicit PipelineNode(std::function<void(VertShaderT &, FragShaderT &)> func);

        ~PipelineNode() override = default;

        void Run() override;

    private:
        gl::GlFrameBuffer m_frame_buffer;

        std::function<void(VertShaderT &, FragShaderT &)> m_func = nullptr;
    };

    template<typename VertShaderT, typename FragShaderT>
    void PipelineNode<VertShaderT, FragShaderT>::Run() {
        PipelineNodeAny::Run();
        m_func(
                *std::dynamic_pointer_cast<VertShaderT>(m_vert_shader),
                *std::dynamic_pointer_cast<FragShaderT>(m_frag_shader)
        );
    }

    template<typename VertShaderT, typename FragShaderT>
    PipelineNode<VertShaderT, FragShaderT>::PipelineNode(std::function<void(VertShaderT &, FragShaderT &)> func) {
        m_func = std::move(func);

        m_vert_shader = std::make_shared<VertShaderT>();
        m_frag_shader = std::make_shared<FragShaderT>();
        m_vert_shader->Init();
        m_frag_shader->Init();

        if (!m_frag_shader->GetOutputs().empty()) {
            m_output = std::make_unique<gl::GlFrameBuffer>();
            m_output->Init({512, 512, m_frag_shader->GetOutputs()});
        }

        Init(*m_vert_shader, *m_frag_shader);

        for (const auto &[name, res] : dynamic_cast<VertShaderT *>(m_vert_shader.get())->m_uniform_locations) {
            *res = GetUniformLocation(name);
        }

        for (const auto &[name, res] : dynamic_cast<FragShaderT *>(m_frag_shader.get())->m_uniform_locations) {
            *res = GetUniformLocation(name);
        }
    }

    void DrawQuad();

    void DrawCube();

}