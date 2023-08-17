#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_wgpu.h>

#include <GLFW/glfw3.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgpu.h>
#include <iostream>
#include <webgpu/webgpu.h>
#include <webgpu/webgpu_cpp.h>

static WGPUDevice wgpu_device = nullptr;
static WGPUSurface wgpu_surface = nullptr;
static WGPUSwapChain wgpu_swap_chain = nullptr;
static int wgpu_swap_chain_width = 0;
static int wgpu_swap_chain_height = 0;

static void MainLoopStep(void *window);
static bool InitWGPU();
static void print_glfw_error(int error, const char *description);
static void print_wgpu_error(WGPUErrorType error_type, const char *message, void *);

void key_callback(GLFWwindow * /*window*/, int key, int scancode, int action, int mods)
{
    std::cout << "(key, scancode, action, mods)"
              << "(" << key << "," << scancode << "," << action << "," << mods << ")" << std::endl;
}

static void cursor_position_callback(GLFWwindow * /*window*/, double xpos, double ypos)
{
    std::cout << "(xpos, ypos)"
              << "(" << xpos << "," << ypos << ")" << std::endl;
}

static bool InitWGPU()
{
    WGPUInstanceDescriptor desc;
    desc.nextInChain = NULL;
    WGPUInstance instance = wgpuCreateInstance(&desc);

    wgpu_device = emscripten_webgpu_get_device();

    if (!wgpu_device)
        return false;

    wgpuDeviceSetUncapturedErrorCallback(wgpu_device, print_wgpu_error, nullptr);

    WGPUSurfaceDescriptorFromCanvasHTMLSelector html_surface_desc = {};
    html_surface_desc.selector = "#canvas";
    html_surface_desc.chain.next = nullptr;
    html_surface_desc.chain.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector;

    WGPUSurfaceDescriptor surface_desc = {};
    surface_desc.nextInChain = reinterpret_cast<WGPUChainedStruct *>(&html_surface_desc);

    // Use 'null' instance
    wgpu_surface = wgpuInstanceCreateSurface(instance, &surface_desc);

    return true;
}

extern "C" int __main__(int /*argc*/, char * /*argv*/[])
{
    glfwSetErrorCallback(print_glfw_error);
    if (!glfwInit())
        return 1;

    // Make sure GLFW does not initialize any graphics context.
    // This needs to be done explicitly later.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+WebGPU example", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    wgpu_device = emscripten_webgpu_get_device();

    // Initialize the WebGPU environment
    if (!InitWGPU())
    {
        if (window)
            glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
    glfwShowWindow(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOther(window, true);
    ImGui_ImplWGPU_Init(wgpu_device, 3, WGPUTextureFormat_RGBA8Unorm, WGPUTextureFormat_Undefined);

    emscripten_set_main_loop_arg(MainLoopStep, window, 0, false);

    return 0;
}

#ifndef KEEP_IN_MODULE
#define KEEP_IN_MODULE extern "C" __attribute__((used, visibility("default")))
#endif

KEEP_IN_MODULE void _glue_main_()
{
    __main__(0, nullptr);
}

EM_JS(void, webgpu_preint, (), {
    var entry = __glue_main_;
    if (entry)
    {
        /*
         * None of the WebGPU properties appear to survive Closure, including
         * Emscripten's own `preinitializedWebGPUDevice` (which from looking at
         *`library_html5` is probably designed to be inited in script before
         * loading the Wasm).
         */
        navigator["gpu"];
        if (navigator["gpu"])
        {
            navigator["gpu"]["requestAdapter"]().then(
                function(adapter) {
                    adapter["requestDevice"]().then(function(device) {
                        Module["preinitializedWebGPUDevice"] = device;
                        entry();
                    });
                },
                function() { console.error("No WebGPU adapter; not starting"); });
        }
        else
        {
            console.error("No support for WebGPU; not starting");
        }
    }
    else
    {
        console.error("Entry point not found; unable to start");
    }
});

int main(int, char **)
{

    webgpu_preint();

    return 0;
}

static void MainLoopStep(void *window)
{
    ImGuiIO& io = ImGui::GetIO();

    glfwPollEvents();

    int width, height;
    glfwGetFramebufferSize((GLFWwindow*)window, &width, &height);

    // React to changes in screen size
    if (width != wgpu_swap_chain_width && height != wgpu_swap_chain_height)
    {
        ImGui_ImplWGPU_InvalidateDeviceObjects();
        if (wgpu_swap_chain)
            wgpuSwapChainRelease(wgpu_swap_chain);
        wgpu_swap_chain_width = width;
        wgpu_swap_chain_height = height;
        WGPUSwapChainDescriptor swap_chain_desc = {};
        swap_chain_desc.usage = WGPUTextureUsage_RenderAttachment;
        swap_chain_desc.format = WGPUTextureFormat_RGBA8Unorm;
        swap_chain_desc.width = width;
        swap_chain_desc.height = height;
        swap_chain_desc.presentMode = WGPUPresentMode_Fifo;
        wgpu_swap_chain = wgpuDeviceCreateSwapChain(wgpu_device, wgpu_surface, &swap_chain_desc);
        ImGui_ImplWGPU_CreateDeviceObjects();
    }

    // Start the Dear ImGui frame
    ImGui_ImplWGPU_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Our state
    // (we use static, which essentially makes the variable globals, as a convenience to keep the example code easy to follow)
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                                // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");                     // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);            // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);                  // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color);       // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                                  // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);         // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();

    WGPURenderPassColorAttachment color_attachments = {};
    color_attachments.loadOp = WGPULoadOp_Clear;
    color_attachments.storeOp = WGPUStoreOp_Store;
    color_attachments.clearValue = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    color_attachments.view = wgpuSwapChainGetCurrentTextureView(wgpu_swap_chain);
    WGPURenderPassDescriptor render_pass_desc = {};
    render_pass_desc.colorAttachmentCount = 1;
    render_pass_desc.colorAttachments = &color_attachments;
    render_pass_desc.depthStencilAttachment = nullptr;

    WGPUCommandEncoderDescriptor enc_desc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(wgpu_device, &enc_desc);

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &render_pass_desc);
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass);
    wgpuRenderPassEncoderEnd(pass);

    WGPUCommandBufferDescriptor cmd_buffer_desc = {};
    WGPUCommandBuffer cmd_buffer = wgpuCommandEncoderFinish(encoder, &cmd_buffer_desc);
    WGPUQueue queue = wgpuDeviceGetQueue(wgpu_device);
    wgpuQueueSubmit(queue, 1, &cmd_buffer);
}

static void print_glfw_error(int error, const char *description)
{
    std::cout << "GLFW Error " << error << ":" << description << std::endl;
}

static void print_wgpu_error(WGPUErrorType error_type, const char *message, void *)
{
    const char *error_type_lbl = "";
    switch (error_type)
    {
    case WGPUErrorType_Validation:
        error_type_lbl = "Validation";
        break;
    case WGPUErrorType_OutOfMemory:
        error_type_lbl = "Out of memory";
        break;
    case WGPUErrorType_Unknown:
        error_type_lbl = "Unknown";
        break;
    case WGPUErrorType_DeviceLost:
        error_type_lbl = "Device lost";
        break;
    default:
        error_type_lbl = "Unknown";
    }
    std::cout << error_type_lbl << " Error: " << message << std::endl;
}