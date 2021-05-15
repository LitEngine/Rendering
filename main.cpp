#include <iostream>
#include <vector>
#include <lit/application/application.hpp>
#include <lit/rendering/pipeline.hpp>

using namespace lit::application;
using namespace lit::rendering;
using namespace glm;

class MyVertShader : public VertShader {
    GLSL_START("MyVertShader")

    in vec3 i_vert;

    void main() {
        gl_Position = vec4(i_vert, 1);
    }

    GLSL_END()
};

class MyFragShader : public FragShader {
    GLSL_FINAL_NODE();

    GLSL_IN(vec3, pos);

    GLSL_OUT_FRAG(vec4, color, RGBA);

    GLSL_UNI(float, scale);

    GLSL_START("MyFragShader")

    void main() {
        color = vec4(gl_FragCoord.xyz() / scale, 1.0);
    }

    GLSL_END()
};

template<typename V, typename F>
std::shared_ptr<PipelineNodeAny> makeNode(std::function<void(V &, F &)> func) {
    return std::dynamic_pointer_cast<PipelineNodeAny>(std::make_shared<PipelineNode<V, F>>(func));
}

class Renderer : public WindowRenderer {
    bool Init(SDL_Window *window, SDL_GLContext context) override {
        m_window = window;

        auto g = makeNode<MyVertShader, MyFragShader>([&](MyVertShader &vert, MyFragShader &frag) {
            frag.u_scale = 900.0f;
            DrawQuad();
        });

        m_pipeline = std::make_shared<Pipeline>(std::vector<std::shared_ptr<PipelineNodeAny>>({g}));

        return true;
    }

    void Redraw() override {
        int width, height;
        SDL_GetWindowSize(m_window, &width, &height);
        m_pipeline->Run(width, height);
    }

    std::shared_ptr<Pipeline> m_pipeline;
    SDL_Window *m_window;
};

void GLAPIENTRY
MessageCallback(GLenum, GLenum type, GLuint, GLenum severity, GLsizei, const GLchar *message, const void *) {
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
    std::cout << message << std::endl;
}

int main(int argc, char **argv) {

    Application app;
    app.Init();

    WindowInfo game_window;
    game_window.title = "Test window";
    game_window.maximized = false;
    game_window.width = 1280;
    game_window.height = 720;

    auto renderer = std::make_shared<Renderer>();

    app.CreateWindow(game_window, {renderer}, {});

    while (app.AnyWindowAlive()) {
        app.PollEvents();
        app.Redraw();
    }

    return 0;
}