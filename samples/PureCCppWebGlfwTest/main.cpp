#include <GLFW/glfw3.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgpu.h>
#include <iostream>
#include <webgpu/webgpu.h>
#include <webgpu/webgpu_cpp.h>

static void print_glfw_error(int error, const char *description);
static void MainLoopStep(void *window);

void key_callback(GLFWwindow */*window*/, int key, int scancode, int action,
                  int mods) {
  std::cout << "(key, scancode, action, mods)"
            << "(" << key << "," << scancode << "," << action << "," << mods
            << ")" << std::endl;
}

static void cursor_position_callback(GLFWwindow */*window*/, double xpos,
                                     double ypos) {
  std::cout << "(xpos, ypos)"
            << "(" << xpos << "," << ypos << ")" << std::endl;
}

int main(int, char **) {
  glfwSetErrorCallback(print_glfw_error);
  if (!glfwInit())
    return 1;

  // Make sure GLFW does not initialize any graphics context.
  // This needs to be done explicitly later.
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow *window = glfwCreateWindow(
      1280, 720, "Dear ImGui GLFW+WebGPU example", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return 1;
  }

  // Initialize the WebGPU environment
  // if (!InitWGPU())
  //{
  //    if (window)
  //        glfwDestroyWindow(window);
  //    glfwTerminate();
  //    return 1;
  //}
  glfwShowWindow(window);

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);

  // Setup Dear ImGui context
  // IMGUI_CHECKVERSION();
  // ImGui::CreateContext();
  // ImGuiIO& io = ImGui::GetIO(); (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // For an Emscripten build we are disabling file-system access, so let's not
  // attempt to do a fopen() of the imgui.ini file. You may manually call
  // LoadIniSettingsFromMemory() to load settings from your own storage.
  // io.IniFilename = nullptr;

  // Setup Dear ImGui style
  // ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  // ImGui_ImplGlfw_InitForOther(window, true);
  // ImGui_ImplWGPU_Init(wgpu_device, 3, WGPUTextureFormat_RGBA8Unorm,
  // WGPUTextureFormat_Undefined);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can
  // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
  // them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
  // need to select the font among multiple.
  // - If the file cannot be loaded, the function will return a nullptr. Please
  // handle those errors in your application (e.g. use an assertion, or display
  // an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored
  // into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
  // ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype
  // for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // - Emscripten allows preloading a file or folder to be accessible at
  // runtime. See Makefile for details.
  // io.Fonts->AddFontDefault();
#ifndef IMGUI_DISABLE_FILE_FUNCTIONS
  // io.Fonts->AddFontFromFileTTF("fonts/segoeui.ttf", 18.0f);
  // io.Fonts->AddFontFromFileTTF("fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("fonts/Cousine-Regular.ttf", 15.0f);
  // io.Fonts->AddFontFromFileTTF("fonts/ProggyTiny.ttf", 10.0f);
  // ImFont* font = io.Fonts->AddFontFromFileTTF("fonts/ArialUni.ttf", 18.0f,
  // nullptr, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != nullptr);
#endif

  // This function will directly return and exit the main function.
  // Make sure that no required objects get cleaned up.
  // This way we can use the browsers 'requestAnimationFrame' to control the
  // rendering.
  emscripten_set_main_loop_arg(MainLoopStep, window, 0, false);

  return 0;
}

static void print_glfw_error(int error, const char *description) {
  printf("GLFW Error %d: %s\n", error, description);
}

static void MainLoopStep(void *window) {
  glfwPollEvents();
  int width, height;
  glfwGetFramebufferSize((GLFWwindow *)window, &width, &height);

  //static uint32_t index = 0;
  //index++;

  //std::cout << index << " (width, height):"
  //          << "(" << width << "," << height << ")" << std::endl;
}