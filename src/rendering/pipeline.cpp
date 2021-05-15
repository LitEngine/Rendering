#include <lit/rendering/pipeline.hpp>
#include <utility>

using namespace lit::rendering;

Pipeline::Pipeline(std::vector<std::shared_ptr<PipelineNodeAny>> nodes) : m_nodes(std::move(nodes)) {}

void Pipeline::Run(int width, int height) {
    if (width != m_width || height != m_height) {
        m_width = width;
        m_height = height;
        for (const auto &node : m_nodes) {
            node->ResizeOutput(width, height);
        }
    }

    for (auto &node : m_nodes) {
        node->Run();
    }
}
