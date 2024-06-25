#include "../build/imgui/imgui.h"
#include "../build/imgui/imgui_impl_glfw.h"
#include "../build/imgui/imgui_impl_opengl2.h"
#include "avl.h"

#include <GLFW/glfw3.h>

int main(int, char**)
{

    // 初始化avl树
    avl::init(20, avl::root);
    int key = 0;

    if (!glfwInit())
        return 1;


    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "hello world", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 渲染avl树
        if (ImGui::Begin("avl tree")) {
            ImGui::SetWindowFontScale(2.0f);
            ImVec2 winPos = ImGui::GetWindowPos();
            ImGui::Text("window pos: (%.2f, %.2f)", winPos.x, winPos.y);
            ImGui::InputInt("input integer", &key);
            if (ImGui::Button("insert")) {
                avl::add(key, avl::root);
            }
            if (ImGui::Button("remove")) {
                avl::remove(key, avl::root);
            }
            if (ImGui::Button("search")) {
                avl::search(key, avl::root);
            }
            avl::dfs(avl::root, {winPos.x + 600, winPos.y + 200}); 
        }
        ImGui::End();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}