#pragma once

#include "pipeline_node.hpp"

namespace lit::rendering {

    class Pipeline {
    public:
        explicit Pipeline(std::vector<std::shared_ptr<PipelineNodeAny>> nodes);

        void Run(int width, int height);

    private:
        std::vector<std::shared_ptr<PipelineNodeAny>> m_nodes;

        int m_width = 128;
        int m_height = 128;
    };
}
