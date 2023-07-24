#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_wgpu.h>
#include <imgui_internal.h>

#include <GLFW/glfw3.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgpu.h>
#include <iostream>
#include <webgpu/webgpu.h>
#include <webgpu/webgpu_cpp.h>

#include <TextEditor.h>

#include <string>
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
    // std::cout << "(key, scancode, action, mods)"
    //           << "(" << key << "," << scancode << "," << action << "," << mods << ")" << std::endl;
}

static void cursor_position_callback(GLFWwindow * /*window*/, double xpos, double ypos)
{
    // std::cout << "(xpos, ypos)"
    //           << "(" << xpos << "," << ypos << ")" << std::endl;
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

void drop_callback(GLFWwindow *window, int count, const char **paths)
{
    int i;
    for (i = 0; i < count; i++)
        std::cout << paths[i] << std::endl;
}

std::string IMGUI_CLIPBOARD_TEXT;

void SET_CLIPBOARD_TEXT_FUN(void *user_data, const char *text)
{
    std::cout << "SET_CLIPBOARD_TEXT_FUN:" << text << std::endl;
    IMGUI_CLIPBOARD_TEXT = std::string(text);
}

const char *GET_CLIPBOARD_TEXT_FUN(void *user_data)
{
    return IMGUI_CLIPBOARD_TEXT.c_str();
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
    glfwSetDropCallback(window, drop_callback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    ImGuiContext &g = *ImGui::GetCurrentContext();

    g.IO.SetClipboardTextFn = &SET_CLIPBOARD_TEXT_FUN;
    g.IO.GetClipboardTextFn = &GET_CLIPBOARD_TEXT_FUN;

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

std::string clipboard_str__;

void GetClipboardStr(size_t size, char *str)
{
    clipboard_str__ = std::string(str);
    free(str);
}

KEEP_IN_MODULE void _get_clipboard_str_(size_t size, char *str)
{
    GetClipboardStr(size, str);
}

// EM_JS(void, test_clip, (), { navigator.clipboard.readText().then((clipText) => (console.log(clipText))); })

// clang-format off
EM_JS(void, get_clipboard_str, (), {
    navigator.permissions.query({name : 'clipboard-read'}).then(result => {
        const hasFocus = document.hasFocus(); // 这个是重点，可判断是否为当前dom页面
        if (hasFocus && (result.state === 'granted' || result.state === 'prompt'))
        {
            const clipboard = navigator.clipboard.readText();
            clipboard.then(clipText => {
                // if (clipText !== clipboardContent && clipText) {
                // console.log(clipText);
                __get_clipboard_str_(clipText.length, stringToNewUTF8(clipText));
                //}
            })
        }
    });
})
// clang-format on

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

const char *CLIENT_ITEMS[] = {"Vulkan", "OpenGL"};
int CLIENT_ITEMS_CURRENT_INDEX = 0;

const char *TARGET_CLIENT_ITEMS[] = {"Vulkan 1.0", "Vulkan 1.1", "Vulkan 1.2", "Vulkan 1.3", "OpenGL 4.5"};
int TARGET_CLIENT_ITEMS_CURRENT_INDEX = 0;

const char *TARGET_LANGUAGE_ITEMS[] = {"SPIR-V 1.0", "SPIR-V 1.1", "SPIR-V 1.2", "SPIR-V 1.3",
                                       "SPIR-V 1.4", "SPIR-V 1.5", "SPIR-V 1.6"};
int TARGET_LANGUAGE_ITEMS_CURRENT_INDEX = 0;

static void MainLoopStep(void *window)
{
    ImGuiIO &io = ImGui::GetIO();

    glfwPollEvents();

    int width, height;
    glfwGetFramebufferSize((GLFWwindow *)window, &width, &height);

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
    // (we use static, which essentially makes the variable globals, as a convenience to keep the example code easy to
    // follow)
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to
    // learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button(
                "Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window",
                     &show_another_window); // Pass a pointer to our bool variable (the window will have a closing
                                            // button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    {
        static bool is_init = false;
        static TextEditor editor;
        // static const char* fileToEdit = "E:/Echo/SDK/ImGuiColorTextEdit/TextEditor.cpp";
        static const char *fileToEdit = "E:/Echo/README.txt";

        if (!is_init)
        {
            auto lang = TextEditor::LanguageDefinition::GLSL();

            // set your own known preprocessor symbols...
            static const char *ppnames[] = {"NULL",
                                            "PM_REMOVE",
                                            "ZeroMemory",
                                            "DXGI_SWAP_EFFECT_DISCARD",
                                            "D3D_FEATURE_LEVEL",
                                            "D3D_DRIVER_TYPE_HARDWARE",
                                            "WINAPI",
                                            "D3D11_SDK_VERSION",
                                            "assert"};
            // ... and their corresponding values
            static const char *ppvalues[] = {
                "#define NULL ((void*)0)",
                "#define PM_REMOVE (0x0001)",
                "Microsoft's own memory zapper function\n(which is a macro actually)\nvoid ZeroMemory(\n\t[in] PVOID  "
                "Destination,\n\t[in] SIZE_T Length\n); ",
                "enum DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD = 0",
                "enum D3D_FEATURE_LEVEL",
                "enum D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE  = ( D3D_DRIVER_TYPE_UNKNOWN + 1 )",
                "#define WINAPI __stdcall",
                "#define D3D11_SDK_VERSION (7)",
                " #define assert(expression) (void)(                                                  \n"
                "    (!!(expression)) ||                                                              \n"
                "    (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \n"
                " )"};

            for (int i = 0; i < sizeof(ppnames) / sizeof(ppnames[0]); ++i)
            {
                TextEditor::Identifier id;
                id.mDeclaration = ppvalues[i];
                lang.mPreprocIdentifiers.insert(std::make_pair(std::string(ppnames[i]), id));
            }

            // set your own identifiers
            static const char *identifiers[] = {"HWND",
                                                "HRESULT",
                                                "LPRESULT",
                                                "D3D11_RENDER_TARGET_VIEW_DESC",
                                                "DXGI_SWAP_CHAIN_DESC",
                                                "MSG",
                                                "LRESULT",
                                                "WPARAM",
                                                "LPARAM",
                                                "UINT",
                                                "LPVOID",
                                                "ID3D11Device",
                                                "ID3D11DeviceContext",
                                                "ID3D11Buffer",
                                                "ID3D11Buffer",
                                                "ID3D10Blob",
                                                "ID3D11VertexShader",
                                                "ID3D11InputLayout",
                                                "ID3D11Buffer",
                                                "ID3D10Blob",
                                                "ID3D11PixelShader",
                                                "ID3D11SamplerState",
                                                "ID3D11ShaderResourceView",
                                                "ID3D11RasterizerState",
                                                "ID3D11BlendState",
                                                "ID3D11DepthStencilState",
                                                "IDXGISwapChain",
                                                "ID3D11RenderTargetView",
                                                "ID3D11Texture2D",
                                                "TextEditor"};
            static const char *idecls[] = {"typedef HWND_* HWND",
                                           "typedef long HRESULT",
                                           "typedef long* LPRESULT",
                                           "struct D3D11_RENDER_TARGET_VIEW_DESC",
                                           "struct DXGI_SWAP_CHAIN_DESC",
                                           "typedef tagMSG MSG\n * Message structure",
                                           "typedef LONG_PTR LRESULT",
                                           "WPARAM",
                                           "LPARAM",
                                           "UINT",
                                           "LPVOID",
                                           "ID3D11Device",
                                           "ID3D11DeviceContext",
                                           "ID3D11Buffer",
                                           "ID3D11Buffer",
                                           "ID3D10Blob",
                                           "ID3D11VertexShader",
                                           "ID3D11InputLayout",
                                           "ID3D11Buffer",
                                           "ID3D10Blob",
                                           "ID3D11PixelShader",
                                           "ID3D11SamplerState",
                                           "ID3D11ShaderResourceView",
                                           "ID3D11RasterizerState",
                                           "ID3D11BlendState",
                                           "ID3D11DepthStencilState",
                                           "IDXGISwapChain",
                                           "ID3D11RenderTargetView",
                                           "ID3D11Texture2D",
                                           "class TextEditor"};
            for (int i = 0; i < sizeof(identifiers) / sizeof(identifiers[0]); ++i)
            {
                TextEditor::Identifier id;
                id.mDeclaration = std::string(idecls[i]);
                lang.mIdentifiers.insert(std::make_pair(std::string(identifiers[i]), id));
            }
            editor.SetLanguageDefinition(lang);

            TextEditor::ErrorMarkers markers;
            markers.insert(
                std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
            markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
            editor.SetErrorMarkers(markers);

            {
                // std::ifstream t(fileToEdit);
                // if (t.good())
                //{
                //     std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
                //     editor.SetText(str);
                // }
            }

            is_init = true;
        }

        auto cpos = editor.GetCursorPosition();
        ImGui::Begin("Text Editor Demo", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
        ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save"))
                {
                    auto textToSave = editor.GetText();
                    /// save text....
                }
                if (ImGui::MenuItem("Quit", "Alt-F4"))
                {
                    // break;
                }
                ImGui::EndMenu();
            }

            // const char *clip_text = glfwGetClipboardString((GLFWwindow *)window);

            // test_clip();
            get_clipboard_str();
            // std::cout << clipboard_str__ << std::endl;
            ImGui::SetClipboardText(clipboard_str__.c_str());
            // std::cout << clipboard_str__ << std::endl;

            if (ImGui::BeginMenu("Edit"))
            {
                bool ro = editor.IsReadOnly();
                if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                    editor.SetReadOnly(ro);
                ImGui::Separator();

                if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
                    editor.Undo();
                if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
                    editor.Redo();

                ImGui::Separator();

                if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
                    editor.Copy();
                if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
                    editor.Cut();
                if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
                    editor.Delete();
                if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro))
                    editor.Paste(clipboard_str__.c_str());

                ImGui::Separator();

                if (ImGui::MenuItem("Select all", nullptr, nullptr))
                    editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Dark palette"))
                    editor.SetPalette(TextEditor::GetDarkPalette());
                if (ImGui::MenuItem("Light palette"))
                    editor.SetPalette(TextEditor::GetLightPalette());
                if (ImGui::MenuItem("Retro blue palette"))
                    editor.SetPalette(TextEditor::GetRetroBluePalette());
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Text("ImGui Version: %s", ImGui::GetVersion());
        ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
                    editor.IsOverwrite() ? "Ovr" : "Ins", editor.CanUndo() ? "*" : " ",
                    editor.GetLanguageDefinition().mName.c_str(), fileToEdit);

        editor.Render("TextEditor");
        ImGui::End();
    }

    // Rendering
    ImGui::Render();

    WGPURenderPassColorAttachment color_attachments = {};
    color_attachments.loadOp = WGPULoadOp_Clear;
    color_attachments.storeOp = WGPUStoreOp_Store;
    color_attachments.clearValue = {clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                                    clear_color.z * clear_color.w, clear_color.w};
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