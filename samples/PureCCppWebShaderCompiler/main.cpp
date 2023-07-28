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

#include <spirv_cpp.hpp>
#include <spirv_glsl.hpp>
#include <spirv_hlsl.hpp>
#include <spirv_reflect.hpp>

/*
    // spirv_cross::CompilerGLSL glsl(siprv_code);
    // spirv_cross::CompilerGLSL::Options opts = glsl.get_common_options();
    // opts.vulkan_semantics = true;
    // glsl.set_common_options(opts);
    // Log(glsl.compile());

    // spirv_cross::CompilerCPP cpp(siprv_code);
    // spirv_cross::CompilerCPP::Options opts = cpp.get_common_options();
    // opts.vulkan_semantics = true;
    // cpp.set_common_options(opts);
    // Log(cpp.compile());

    // spirv_cross::CompilerReflection reflection(siprv_code);
    // spirv_cross::CompilerCPP::Options opts = reflection.get_common_options();
    // opts.vulkan_semantics = true;
    // reflection.set_common_options(opts);
    // Log(reflection.compile());
*/

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
    // int i;
    // for (i = 0; i < count; i++)
    //     std::cout << paths[i] << std::endl;
}

std::string IMGUI_CLIPBOARD_TEXT;

void SET_CLIPBOARD_TEXT_FUN(void *user_data, const char *text)
{
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
    GLFWwindow *window = glfwCreateWindow(1920, 1080, "Dear ImGui GLFW+WebGPU example", nullptr, nullptr);
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
        const hasFocus = document.hasFocus();
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

// clang-format off
EM_JS(void, write_clipboard_str, (const char* str), {navigator.clipboard.writeText(UTF8ToString(str))})
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

struct CustomeExampleAppConsole
{
    char InputBuf[256];
    ImVector<char *> Items;
    ImVector<const char *> Commands;
    ImVector<char *> History;
    int HistoryPos; // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter Filter;
    bool AutoScroll;
    bool ScrollToBottom;

    CustomeExampleAppConsole()
    {
        // IMGUI_DEMO_MARKER("Examples/Console");
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;

        // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
        Commands.push_back("HELP");
        Commands.push_back("HISTORY");
        Commands.push_back("CLEAR");
        Commands.push_back("CLASSIFY");
        AutoScroll = true;
        ScrollToBottom = false;
    }
    ~CustomeExampleAppConsole()
    {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            free(History[i]);
    }

    // Portable helpers
    static int Stricmp(const char *s1, const char *s2)
    {
        int d;
        while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
        {
            s1++;
            s2++;
        }
        return d;
    }
    static int Strnicmp(const char *s1, const char *s2, int n)
    {
        int d = 0;
        while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
        {
            s1++;
            s2++;
            n--;
        }
        return d;
    }
    static char *Strdup(const char *s)
    {
        IM_ASSERT(s);
        size_t len = strlen(s) + 1;
        void *buf = malloc(len);
        IM_ASSERT(buf);
        return (char *)memcpy(buf, (const void *)s, len);
    }
    static void Strtrim(char *s)
    {
        char *str_end = s + strlen(s);
        while (str_end > s && str_end[-1] == ' ')
            str_end--;
        *str_end = 0;
    }

    void ClearLog()
    {
        for (int i = 0; i < Items.Size; i++)
            free(Items[i]);
        Items.clear();
    }

    void AddLog(const char *fmt, ...) IM_FMTARGS(2)
    {
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
    }

    void Draw(const char *title, bool *p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(1060, 100), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
        // So e.g. IsItemHovered() will return true when hovering the title bar.
        // Here we create a context menu only available from the title bar.
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Close Console"))
                *p_open = false;
            ImGui::EndPopup();
        }

        // ImGui::TextWrapped(
        //     "This example implements a console with basic coloring, completion (TAB key) and history (Up/Down keys).
        //     A more elaborate " "implementation may want to store entries along with extra data such as timestamp,
        //     emitter, etc.");
        // ImGui::TextWrapped("Enter 'HELP' for help.");

        // TODO: display items starting from the bottom

        // if (ImGui::SmallButton("Add Debug Text"))  { AddLog("%d some text", Items.Size); AddLog("some more text");
        // AddLog("display very important message here!"); } ImGui::SameLine(); if (ImGui::SmallButton("Add Debug
        // Error")) { AddLog("[error] something went wrong"); } ImGui::SameLine(); if (ImGui::SmallButton("Clear")) {
        // ClearLog(); } ImGui::SameLine(); bool copy_to_clipboard = ImGui::SmallButton("Copy");
        bool copy_to_clipboard = false;
        // static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

        // ImGui::Separator();

        // Options menu
        // if (ImGui::BeginPopup("Options"))
        //{
        //    ImGui::Checkbox("Auto-scroll", &AutoScroll);
        //    ImGui::EndPopup();
        //}

        //// Options, Filter
        // if (ImGui::Button("Options"))
        //     ImGui::OpenPopup("Options");
        // ImGui::SameLine();
        // Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        // ImGui::Separator();

        // Reserve enough left-over height for 1 separator + 1 input text
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false,
                              ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::Selectable("Clear"))
                    ClearLog();
                ImGui::EndPopup();
            }

            // Display every line as a separate entry so we can change their color or add custom widgets.
            // If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
            // NB- if you have thousands of entries this approach may be too inefficient and may require user-side
            // clipping to only process visible items. The clipper will automatically measure the height of your first
            // item and then "seek" to display only items in the visible area. To use the clipper we can replace your
            // standard loop:
            //      for (int i = 0; i < Items.Size; i++)
            //   With:
            //      ImGuiListClipper clipper;
            //      clipper.Begin(Items.Size);
            //      while (clipper.Step())
            //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            // - That your items are evenly spaced (same height)
            // - That you have cheap random access to your elements (you can access them given their index,
            //   without processing all the ones before)
            // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
            // We would need random-access on the post-filtered list.
            // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
            // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
            // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
            // to improve this example code!
            // If your items are of variable height:
            // - Split them into same height items would be simpler and facilitate random-seeking into your list.
            // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            if (copy_to_clipboard)
                ImGui::LogToClipboard();
            for (int i = 0; i < Items.Size; i++)
            {
                const char *item = Items[i];
                if (!Filter.PassFilter(item))
                    continue;

                // Normally you would store more information in your item than just a string.
                // (e.g. make Items[] an array of structure, store color/type etc.)
                ImVec4 color;
                bool has_color = false;
                if (strstr(item, "[error]"))
                {
                    color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                    has_color = true;
                }
                else if (strstr(item, "[success]"))
                {
                    color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                    has_color = true;
                }
                else if (strncmp(item, "# ", 2) == 0)
                {
                    color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
                    has_color = true;
                }
                if (has_color)
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(item);
                if (has_color)
                    ImGui::PopStyleColor();
            }
            if (copy_to_clipboard)
                ImGui::LogFinish();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the
            // frame. Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            ScrollToBottom = false;

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
        // ImGui::Separator();
        //
        //// Command-line
        // bool reclaim_focus = false;
        // ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue |
        // ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion |
        // ImGuiInputTextFlags_CallbackHistory; if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf),
        // input_text_flags, &TextEditCallbackStub, (void*)this))
        //{
        //     char* s = InputBuf;
        //     Strtrim(s);
        //     if (s[0])
        //         ExecCommand(s);
        //     strcpy(s, "");
        //     reclaim_focus = true;
        // }
        //
        //// Auto-focus on window apparition
        // ImGui::SetItemDefaultFocus();
        // if (reclaim_focus)
        //     ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        ImGui::End();
    }

    void ExecCommand(const char *command_line)
    {
        AddLog("# %s\n", command_line);

        // Insert into history. First find match and delete it so it can be pushed to the back.
        // This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.Size - 1; i >= 0; i--)
            if (Stricmp(History[i], command_line) == 0)
            {
                free(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        History.push_back(Strdup(command_line));

        // Process command
        if (Stricmp(command_line, "CLEAR") == 0)
        {
            ClearLog();
        }
        else if (Stricmp(command_line, "HELP") == 0)
        {
            AddLog("Commands:");
            for (int i = 0; i < Commands.Size; i++)
                AddLog("- %s", Commands[i]);
        }
        else if (Stricmp(command_line, "HISTORY") == 0)
        {
            int first = History.Size - 10;
            for (int i = first > 0 ? first : 0; i < History.Size; i++)
                AddLog("%3d: %s\n", i, History[i]);
        }
        else
        {
            AddLog("Unknown command: '%s'\n", command_line);
        }

        // On command input, we scroll to bottom even if AutoScroll==false
        ScrollToBottom = true;
    }

    // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
    static int TextEditCallbackStub(ImGuiInputTextCallbackData *data)
    {
        CustomeExampleAppConsole *console = (CustomeExampleAppConsole *)data->UserData;
        return console->TextEditCallback(data);
    }

    int TextEditCallback(ImGuiInputTextCallbackData *data)
    {
        // AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag)
        {
        case ImGuiInputTextFlags_CallbackCompletion: {
            // Example of TEXT COMPLETION

            // Locate beginning of current word
            const char *word_end = data->Buf + data->CursorPos;
            const char *word_start = word_end;
            while (word_start > data->Buf)
            {
                const char c = word_start[-1];
                if (c == ' ' || c == '\t' || c == ',' || c == ';')
                    break;
                word_start--;
            }

            // Build a list of candidates
            ImVector<const char *> candidates;
            for (int i = 0; i < Commands.Size; i++)
                if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
                    candidates.push_back(Commands[i]);

            if (candidates.Size == 0)
            {
                // No match
                AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
            }
            else if (candidates.Size == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0]);
                data->InsertChars(data->CursorPos, " ");
            }
            else
            {
                // Multiple matches. Complete as much as we can..
                // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                int match_len = (int)(word_end - word_start);
                for (;;)
                {
                    int c = 0;
                    bool all_candidates_matches = true;
                    for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                        if (i == 0)
                            c = toupper(candidates[i][match_len]);
                        else if (c == 0 || c != toupper(candidates[i][match_len]))
                            all_candidates_matches = false;
                    if (!all_candidates_matches)
                        break;
                    match_len++;
                }

                if (match_len > 0)
                {
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                }

                // List matches
                AddLog("Possible matches:\n");
                for (int i = 0; i < candidates.Size; i++)
                    AddLog("- %s\n", candidates[i]);
            }

            break;
        }
        case ImGuiInputTextFlags_CallbackHistory: {
            // Example of HISTORY
            const int prev_history_pos = HistoryPos;
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                if (HistoryPos == -1)
                    HistoryPos = History.Size - 1;
                else if (HistoryPos > 0)
                    HistoryPos--;
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                if (HistoryPos != -1)
                    if (++HistoryPos >= History.Size)
                        HistoryPos = -1;
            }

            // A better implementation would preserve the data on the current input line along with cursor position.
            if (prev_history_pos != HistoryPos)
            {
                const char *history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, history_str);
            }
        }
        }
        return 0;
    }
};

static CustomeExampleAppConsole console;

static void Log(const std::string &str)
{
    console.AddLog(str.c_str());
}

static void LogSuccess(const std::string &str)
{
    std::string error_str = "[success] \n" + str;
    console.AddLog(error_str.c_str());
}

static void LogError(const std::string &str)
{
    std::string error_str = "[error] \n" + str;
    console.AddLog(error_str.c_str());
}

static void ClearLog()
{
    console.ClearLog();
}

const char *CLIENT_ITEMS[] = {"Vulkan", "OpenGL"};
int CLIENT_ITEMS_CURRENT_INDEX = 0;

const char *TARGET_CLIENT_ITEMS[] = {"Vulkan 1.0", "Vulkan 1.1", "Vulkan 1.2", "Vulkan 1.3", "OpenGL 4.5"};
int TARGET_CLIENT_ITEMS_CURRENT_INDEX = 0;

const char *TARGET_LANGUAGE_ITEMS[] = {"SPIR-V 1.0", "SPIR-V 1.1", "SPIR-V 1.2", "SPIR-V 1.3",
                                       "SPIR-V 1.4", "SPIR-V 1.5", "SPIR-V 1.6"};
int TARGET_LANGUAGE_ITEMS_CURRENT_INDEX = 0;

const char *SHADER_TYPE_ITEMS[] = {"Vertex",
                                   "Tessellation Control",
                                   "Tessellation Evaluation",
                                   "Geometry",
                                   "Fragment",
                                   "Compute",
                                   "Ray Generation",
                                   "Intersection",
                                   "Any-Hit",
                                   "Closest Hit",
                                   "Miss",
                                   "Callable",
                                   "Task",
                                   "Mesh"};
int SHADER_TYPE_ITEMS_CURRENT_INDEX = 0;

const char *LANGUAGE_ITEMS[] = {"GLSL", "HLSL", "SPIR-V Binary", "SPIR-V Disassemble"};
int LANGUAGE_ITEMS_CURRENT_INDEX = 0;

static TextEditor codeEditor;

static TextEditor codeViewEditor;
static bool isCodeViewEditorShow = false;

namespace CodeEditorLanguage
{
enum Language
{
    GLSL = 0,
    HLSL = 1,
    SPIRV_BINARY = 2,
    SPIRV_DISASSEMBLE = 3,
};
} // namespace CodeEditorLanguage

void ChangeEditorLanguage(TextEditor &editor, CodeEditorLanguage::Language language)
{
    TextEditor::LanguageDefinition lang;
    switch (language)
    {
    case CodeEditorLanguage::Language::GLSL: {
        lang = TextEditor::LanguageDefinition::GLSL();
    }
    break;
    case CodeEditorLanguage::Language::HLSL: {
        lang = TextEditor::LanguageDefinition::HLSL();
    }
    break;
    case CodeEditorLanguage::Language::SPIRV_BINARY: {
        lang = TextEditor::LanguageDefinition::SpirVBinary();
    }
    break;
    case CodeEditorLanguage::Language::SPIRV_DISASSEMBLE: {
        lang = TextEditor::LanguageDefinition::SpirVDisassemble();
    }
    break;
    }

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
}

#include <SPIRV/GlslangToSpv.h>
#include <SPIRV/SpvTools.h>
#include <StandAlone/DirStackFileIncluder.h>
#include <glslang/Include/BaseTypes.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>

std::vector<uint32_t> ShaderCodeToSpirV(glslang::EShSource language, EShLanguage shaderType, glslang::EShClient client,
                                        glslang::EShTargetClientVersion targetClientVersion,
                                        glslang::EShTargetLanguageVersion targetLanguageVersion,
                                        const std::string &code)
{
    std::vector<uint32_t> shader_spir;

    bool is_initialize_process = glslang::InitializeProcess();
    if (!is_initialize_process)
    {
        LogError("glslang::InitializeProcess failed");
        return shader_spir;
    }

    EShLanguage esh_language = shaderType;

    glslang::TShader shader_glslang(esh_language);

    glslang::EShSource esh_source = language;

    const char *code_c_str = code.c_str();
    shader_glslang.setStrings(&code_c_str, 1);
    shader_glslang.setEnvInput(esh_source, esh_language, client, 100);

    glslang::EShTargetClientVersion esh_target_client_version =
        targetClientVersion; // glslang::EShTargetClientVersion::EShTargetVulkan_1_0;
    glslang::EShTargetLanguageVersion esh_target_language_version =
        targetLanguageVersion; // glslang::EShTargetLanguageVersion::EShTargetSpv_1_0;

    shader_glslang.setEnvClient(glslang::EShClient::EShClientVulkan, esh_target_client_version);
    shader_glslang.setEnvTarget(glslang::EShTargetLanguage::EShTargetSpv, esh_target_language_version);
    EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

    if (!shader_glslang.parse(GetDefaultResources(), 100, false, messages))
    {
        std::string log_messgae(shader_glslang.getInfoLog());
        LogError(log_messgae);
        glslang::FinalizeProcess();
        return shader_spir;
    }

    glslang::TProgram program;
    program.addShader(&shader_glslang);
    if (!program.link(messages))
    {
        std::string log_messgae(program.getInfoLog());
        LogError(log_messgae);
        glslang::FinalizeProcess();
        return shader_spir;
    }

    glslang::GlslangToSpv(*(program.getIntermediate(esh_language)), shader_spir);

    glslang::FinalizeProcess();

    return shader_spir;
}
#include <sstream>

#include <spirv-tools/libspirv.h>

// SPIR-V Binary To SPIR-V Disassemble
std::string SpirVBinaryToSpirVDisassemble(const std::vector<uint32_t> &spirv, spv_target_env requested_context)
{
    std::string result;
    spv_context context = spvContextCreate(requested_context);
    spv_text text;
    spv_diagnostic diagnostic = nullptr;
    spvBinaryToText(context, spirv.data(), spirv.size(),
                    SPV_BINARY_TO_TEXT_OPTION_FRIENDLY_NAMES | SPV_BINARY_TO_TEXT_OPTION_INDENT, &text, &diagnostic);

    if (diagnostic == nullptr)
    {
        std::string spirv_text_code = std::string(text->str);
        result = std::string(text->str);
    }
    else
    {
        LogError(std::string(diagnostic->error));
    }

    spvDiagnosticDestroy(diagnostic);
    spvContextDestroy(context);

    return result;
}

// SPIR-V Disassemble To SPIR-V Binary
std::vector<uint32_t> SpirVDisassembleToSpirVBinary(const std::string &sprivDisassemble,
                                                    spv_target_env requested_context)
{
    std::vector<uint32_t> result;

    spv_binary binary;
    spv_context context = spvContextCreate(requested_context);
    spv_diagnostic diagnostic = nullptr;
    spv_result_t spv_result =
        spvTextToBinary(context, sprivDisassemble.c_str(), sprivDisassemble.length(), &binary, &diagnostic);

    if (diagnostic == nullptr)
    {
        for (uint32_t binary_index = 0; binary_index < binary->wordCount; binary_index++)
        {
            uint32_t bin = *(binary->code + binary_index);
            result.push_back(bin);
        }
    }
    else
    {
        LogError(std::string(diagnostic->error));
    }

    spvDiagnosticDestroy(diagnostic);
    spvContextDestroy(context);

    return result;
}

std::vector<uint32_t> SpirVBinaryToSpirVBinary(const std::vector<uint32_t> &spirv, spv_target_env requested_context)
{
    std::vector<uint32_t> result;

    std::string disassemble = SpirVBinaryToSpirVDisassemble(spirv, requested_context);

    if (!disassemble.empty())
    {
        result = SpirVDisassembleToSpirVBinary(disassemble, requested_context);
    }

    return result;
}

// FIXME: SPIR-V Disassemble To SPIR-V Disassemble maybe use SPIR-V Binary for intermediate layer
std::string SpirVDisassembleToSpirVDisassemble(const std::string &sprivDisassemble, spv_target_env requested_context)
{
    std::string result;

    std::vector<uint32_t> assemble = SpirVDisassembleToSpirVBinary(sprivDisassemble, requested_context);

    if (!assemble.empty())
    {
        result = SpirVBinaryToSpirVDisassemble(assemble, requested_context);
    }

    return result;
}

std::vector<uint32_t> GLSLToSpirVBinary(EShLanguage shaderType, glslang::EShClient client,
                                        glslang::EShTargetClientVersion targetClientVersion,
                                        glslang::EShTargetLanguageVersion targetLanguageVersion,
                                        const std::string &glsl)
{
    std::vector<uint32_t> result;

    result = ShaderCodeToSpirV(glslang::EShSource::EShSourceGlsl, shaderType, client, targetClientVersion,
                               targetLanguageVersion, glsl);

    return result;
}

std::vector<uint32_t> HLSLToSpirVBinary(EShLanguage shaderType, glslang::EShClient client,
                                        glslang::EShTargetClientVersion targetClientVersion,
                                        glslang::EShTargetLanguageVersion targetLanguageVersion,
                                        const std::string &hlsl)
{
    std::vector<uint32_t> result;

    result = ShaderCodeToSpirV(glslang::EShSource::EShSourceHlsl, shaderType, client, targetClientVersion,
                               targetLanguageVersion, hlsl);

    return result;
}

std::string SpirVBinaryToGLSL(
    const std::vector<unsigned int> &spirv,
    glslang::EShTargetClientVersion targetClientVersion = glslang::EShTargetClientVersion::EShTargetVulkan_1_0)
{
    spirv_cross::CompilerGLSL glsl(spirv);
    spirv_cross::CompilerGLSL::Options opts = glsl.get_common_options();

    switch (targetClientVersion)
    {
    case glslang::EShTargetClientVersion::EShTargetVulkan_1_0: {
    }
    case glslang::EShTargetClientVersion::EShTargetVulkan_1_1: {
    }
    case glslang::EShTargetClientVersion::EShTargetVulkan_1_2: {
    }
    case glslang::EShTargetClientVersion::EShTargetVulkan_1_3: {
        opts.vulkan_semantics = true;
    }
    break;
    case glslang::EShTargetClientVersion::EShTargetOpenGL_450: {
        opts.vulkan_semantics = false;
    }
    break;
    default: {
    }
    break;
    }

    glsl.set_common_options(opts);
    return glsl.compile();
}

std::string SpirVBinaryToHLSL(
    const std::vector<unsigned int> &spirv,
    glslang::EShTargetClientVersion targetClientVersion = glslang::EShTargetClientVersion::EShTargetVulkan_1_0)
{
    spirv_cross::CompilerHLSL hlsl(spirv);
    spirv_cross::CompilerGLSL::Options opts = hlsl.get_common_options();

    switch (targetClientVersion)
    {
    case glslang::EShTargetClientVersion::EShTargetVulkan_1_0: {
    }
    case glslang::EShTargetClientVersion::EShTargetVulkan_1_1: {
    }
    case glslang::EShTargetClientVersion::EShTargetVulkan_1_2: {
    }
    case glslang::EShTargetClientVersion::EShTargetVulkan_1_3: {
        opts.vulkan_semantics = true;
    }
    break;
    case glslang::EShTargetClientVersion::EShTargetOpenGL_450: {
        opts.vulkan_semantics = false;
    }
    break;
    default: {
    }
    break;
    }

    hlsl.set_common_options(opts);
    return hlsl.compile();
}

std::string SpirVBinaryToHexArrayString(const std::vector<uint32_t> &spirvCode)
{
    std::string hex_spirv_code_result;
    {
        std::stringstream ss;

        ss << "static const uint32_t spirv_code[] = {\n    ";
        for (size_t code_index = 0; code_index < spirvCode.size(); code_index++)
        {
            std::stringstream code_num_ss;
            code_num_ss << std::hex << spirvCode[code_index];

            std::string code_num_str = code_num_ss.str();
            if (code_num_str.length() < 8)
            {
                size_t zero_add_count = 8 - code_num_str.length();
                std::string zeros;
                for (size_t zero_add_index = 0; zero_add_index < zero_add_count; zero_add_index++)
                {
                    zeros += "0";
                }

                code_num_str = zeros + code_num_str;
            }

            if (code_index != spirvCode.size() - 1)
            {
                ss << "0x" << code_num_str;
                ss << ", ";
            }
            else
            {
                ss << "0x" << code_num_str;
            }

            // if (code_index != 0 && (code_index % 9 == 0))
            if (((code_index + 1) % 9) == 0)
            {
                ss << "\n    ";
            }
        }
        ss << "};";
        hex_spirv_code_result = ss.str();
    }

    return hex_spirv_code_result;
}

std::vector<uint32_t> HexArrayStringToSpirVBinary(const std::string &ArrayString)
{
    std::vector<uint32_t> result;

    // const std::regex txt_regex("/0x[a-zA-Z0-9]+/g");
    const std::regex hex_regex("0x[a-zA-Z0-9]+");

    auto words_begin = std::sregex_iterator(ArrayString.begin(), ArrayString.end(), hex_regex);
    auto words_end = std::sregex_iterator();
    auto word_count = std::distance(words_begin, words_end);

    for (std::sregex_iterator i = words_begin; i != words_end; ++i)
    {
        std::smatch match = *i;
        std::string match_str = match.str();

        uint32_t bin = 0;
        std::stringstream ss;
        ss << match_str;
        ss >> std::hex >> bin;

        result.push_back(bin);
    }

    return result;
}

//================================= UI =======================================
CodeEditorLanguage::Language GetCurrentLanguage()
{
    CodeEditorLanguage::Language result = CodeEditorLanguage::Language::GLSL;

    switch (LANGUAGE_ITEMS_CURRENT_INDEX)
    {
    case 0: {
        result = CodeEditorLanguage::Language::GLSL;
    }
    break;
    case 1: {
        result = CodeEditorLanguage::Language::HLSL;
    }
    break;
    case 2: {
        result = CodeEditorLanguage::Language::SPIRV_BINARY;
    }
    break;
    case 3: {
        result = CodeEditorLanguage::Language::SPIRV_DISASSEMBLE;
    }
    break;
    }

    return result;
}

EShLanguage GetCurrentShaderType()
{
    EShLanguage shader_stage = EShLanguage::EShLangVertex;

    if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 0 /*Vertex*/)
    {
        shader_stage = EShLanguage::EShLangVertex;
    }
    else if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 1 /*Tessellation Control*/)
    {
        shader_stage = EShLanguage::EShLangTessControl;
    }
    else if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 2 /*Tessellation Evaluation*/)
    {
        shader_stage = EShLanguage::EShLangTessEvaluation;
    }
    else if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 3 /*Geometry*/)
    {
        shader_stage = EShLanguage::EShLangGeometry;
    }
    else if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 4 /*Fragment*/)
    {
        shader_stage = EShLanguage::EShLangFragment;
    }
    else if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 5 /*Compute*/)
    {
        shader_stage = EShLanguage::EShLangCompute;
    }
    else if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 6 /*Ray Generation*/)
    {
        shader_stage = EShLanguage::EShLangRayGen;
    }
    else if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 7 /*Intersection*/)
    {
        shader_stage = EShLanguage::EShLangIntersect;
    }
    else if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 8 /*Any-Hit*/)
    {
        shader_stage = EShLanguage::EShLangAnyHit;
    }
    else if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 9 /*Closest Hit*/)
    {
        shader_stage = EShLanguage::EShLangClosestHit;
    }
    else if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 1 /*"Miss*/)
    {
        shader_stage = EShLanguage::EShLangMiss;
    }
    else if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 1 /*"Callable*/)
    {
        shader_stage = EShLanguage::EShLangCallable;
    }
    else if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 1 /*"Task*/)
    {
        shader_stage = EShLanguage::EShLangTask;
    }
    else if (SHADER_TYPE_ITEMS_CURRENT_INDEX == 1 /*"Mesh*/)
    {
        shader_stage = EShLanguage::EShLangMesh;
    }

    return shader_stage;
}

glslang::EShClient GetCurrentCLient()
{
    glslang::EShClient client = glslang::EShClient::EShClientVulkan;
    switch (CLIENT_ITEMS_CURRENT_INDEX)
    {
    case 0: {
        client = glslang::EShClient::EShClientVulkan;
    }
    break;
    case 1: {
        client = glslang::EShClient::EShClientOpenGL;
    }
    break;
    }

    return client;
}

glslang::EShTargetClientVersion GetCurrentTargetClientVersion()
{
    glslang::EShTargetClientVersion target_client_version = glslang::EShTargetClientVersion::EShTargetVulkan_1_0;
    switch (TARGET_CLIENT_ITEMS_CURRENT_INDEX)
    {
    case 0: {
        target_client_version = glslang::EShTargetClientVersion::EShTargetVulkan_1_0;
    }
    break;
    case 1: {
        target_client_version = glslang::EShTargetClientVersion::EShTargetVulkan_1_1;
    }
    break;
    case 2: {
        target_client_version = glslang::EShTargetClientVersion::EShTargetVulkan_1_2;
    }
    break;
    case 3: {
        target_client_version = glslang::EShTargetClientVersion::EShTargetVulkan_1_3;
    }
    break;
    case 4: {
        target_client_version = glslang::EShTargetClientVersion::EShTargetOpenGL_450;
    }
    break;
    }

    return target_client_version;
}

glslang::EShTargetLanguageVersion GetCurrentTargetLanguageVersion()
{
    glslang::EShTargetLanguageVersion target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_0;
    switch (TARGET_LANGUAGE_ITEMS_CURRENT_INDEX)
    {
    case 0: {
        target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_0;
    }
    break;
    case 1: {
        target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_1;
    }
    break;
    case 2: {
        target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_2;
    }
    break;
    case 3: {
        target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_3;
    }
    break;
    case 4: {
        target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_4;
    }
    break;
    case 5: {
        target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_5;
    }
    break;
    case 6: {
        target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_6;
    }
    break;
    }

    return target_language_version;
}
//================================= UI =======================================

spv_target_env GetCurrentSpirVTargetEnvFromTargetLanguageVersion()
{
    spv_target_env requested_context = spv_target_env::SPV_ENV_UNIVERSAL_1_0;

    glslang::EShTargetLanguageVersion target_language_version = GetCurrentTargetLanguageVersion();
    switch (target_language_version)
    {
    case glslang::EShTargetLanguageVersion::EShTargetSpv_1_0: {
        requested_context = spv_target_env::SPV_ENV_UNIVERSAL_1_0;
    }
    break;
    case glslang::EShTargetLanguageVersion::EShTargetSpv_1_1: {
        requested_context = spv_target_env::SPV_ENV_UNIVERSAL_1_1;
    }
    break;
    case glslang::EShTargetLanguageVersion::EShTargetSpv_1_2: {
        requested_context = spv_target_env::SPV_ENV_UNIVERSAL_1_2;
    }
    break;
    case glslang::EShTargetLanguageVersion::EShTargetSpv_1_3: {
        requested_context = spv_target_env::SPV_ENV_UNIVERSAL_1_3;
    }
    break;
    case glslang::EShTargetLanguageVersion::EShTargetSpv_1_4: {
        requested_context = spv_target_env::SPV_ENV_UNIVERSAL_1_4;
    }
    break;
    case glslang::EShTargetLanguageVersion::EShTargetSpv_1_5: {
        requested_context = spv_target_env::SPV_ENV_UNIVERSAL_1_5;
    }
    break;
    case glslang::EShTargetLanguageVersion::EShTargetSpv_1_6: {
        requested_context = spv_target_env::SPV_ENV_UNIVERSAL_1_6;
    }
    break;
    default: {
    }
    break;
    }

    return requested_context;
}

/*All language to SPIR-V Binary in here*/
std::vector<uint32_t> CompileEditorCodeToSpirVBinary()
{
    std::string code_str = codeEditor.GetText();

    std::vector<uint32_t> siprv_code;

    if (!code_str.empty())
    {
        CodeEditorLanguage::Language code_editor_language = GetCurrentLanguage();

        EShLanguage shader_stage = GetCurrentShaderType();

        glslang::EShClient client = GetCurrentCLient();

        glslang::EShTargetClientVersion target_client_version = GetCurrentTargetClientVersion();

        glslang::EShTargetLanguageVersion target_language_version = GetCurrentTargetLanguageVersion();

        switch (code_editor_language)
        {
        case CodeEditorLanguage::Language::GLSL: {
        }
        case CodeEditorLanguage::Language::HLSL: {
            if (code_editor_language == CodeEditorLanguage::Language::GLSL)
            {
                siprv_code =
                    GLSLToSpirVBinary(shader_stage, client, target_client_version, target_language_version, code_str);
            }
            else if (code_editor_language == CodeEditorLanguage::Language::HLSL)
            {
                siprv_code =
                    HLSLToSpirVBinary(shader_stage, client, target_client_version, target_language_version, code_str);
            }
        }
        break;
        case CodeEditorLanguage::Language::SPIRV_DISASSEMBLE: {
            siprv_code = SpirVDisassembleToSpirVBinary(code_str, GetCurrentSpirVTargetEnvFromTargetLanguageVersion());
        }
        break;
        case CodeEditorLanguage::Language::SPIRV_BINARY: {
            std::vector<uint32_t> temp_siprv_code = HexArrayStringToSpirVBinary(code_str);
            siprv_code = SpirVBinaryToSpirVBinary(temp_siprv_code, GetCurrentSpirVTargetEnvFromTargetLanguageVersion());
        }
        break;
        }
    }
    else
    {
        LogError("Shader code is empty.");
    }

    return siprv_code;
}

void OuputSpirVBinaryCallback(const std::vector<uint32_t> &spirvCode)
{
    std::string hex_spirv_code_result = SpirVBinaryToHexArrayString(spirvCode);

    isCodeViewEditorShow = true;
    codeViewEditor.SetText(hex_spirv_code_result);

    // LogSuccess(hex_spirv_code_result);
    LogSuccess("shader code successfully compile to SPIR-V binary code");
}

void OuputSpirVDisassembleCallback(const std::vector<uint32_t> &spirvCode)
{
    spv_target_env requested_context = GetCurrentSpirVTargetEnvFromTargetLanguageVersion();

    std::string spirv_disassemble_str = SpirVBinaryToSpirVDisassemble(spirvCode, requested_context);
    // std::cout << spirv_disassemble_str << std::endl;
    if (!spirv_disassemble_str.empty())
    {
        isCodeViewEditorShow = true;
        codeViewEditor.SetText(spirv_disassemble_str);
    }

    LogSuccess("shader code successfully compile to SPIR-V disassemble code");
}

void ToSpirVDisassembleCallback()
{
    std::vector<uint32_t> siprv_code = CompileEditorCodeToSpirVBinary();

    if (siprv_code.size() > 0)
    {
        OuputSpirVDisassembleCallback(siprv_code);
    }
    else
    {
        isCodeViewEditorShow = false;
        codeViewEditor.SetText("");
    }
}

void ToSpirVBinaryCallback()
{
    std::vector<uint32_t> siprv_code = CompileEditorCodeToSpirVBinary();

    if (siprv_code.size() > 0)
    {
        OuputSpirVBinaryCallback(siprv_code);
    }
    else
    {
        isCodeViewEditorShow = false;
        codeViewEditor.SetText("");
    }
}

void CodeEditorLoop()
{
    ImGuiIO &io = ImGui::GetIO();

    static bool is_init = false;
    // static const char* fileToEdit = "E:/Echo/SDK/ImGuiColorTextEdit/TextEditor.cpp";
    static const char *fileToEdit = "";

    if (!is_init)
    {
        ChangeEditorLanguage(codeEditor, CodeEditorLanguage::Language::GLSL);

        // TextEditor::ErrorMarkers markers;
        // markers.insert(
        //     std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found:
        //     \"TextEditor.h\""));
        // markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
        // codeEditor.SetErrorMarkers(markers);

        {
            // std::ifstream t(fileToEdit);
            // if (t.good())
            //{
            //     std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            //     codeEditor.SetText(str);
            // }
        }

        is_init = true;
    }

    auto cpos = codeEditor.GetCursorPosition();
    ImGui::Begin("Shader Code Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
    ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImGui::SetWindowPos(ImVec2(200, 100), ImGuiCond_FirstUseEver);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save"))
            {
                auto textToSave = codeEditor.GetText();
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
            bool ro = codeEditor.IsReadOnly();
            if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                codeEditor.SetReadOnly(ro);
            ImGui::Separator();

            if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && codeEditor.CanUndo()))
                codeEditor.Undo();
            if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && codeEditor.CanRedo()))
                codeEditor.Redo();

            ImGui::Separator();

            if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, codeEditor.HasSelection()))
            {
                codeEditor.Copy();
            }
            if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && codeEditor.HasSelection()))
                codeEditor.Cut();
            if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && codeEditor.HasSelection()))
                codeEditor.Delete();
            if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro))
                codeEditor.Paste(clipboard_str__.c_str());

            ImGui::Separator();

            if (ImGui::MenuItem("Select all", nullptr, nullptr))
                codeEditor.SetSelection(TextEditor::Coordinates(),
                                        TextEditor::Coordinates(codeEditor.GetTotalLines(), 0));

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Dark palette"))
                codeEditor.SetPalette(TextEditor::GetDarkPalette());
            if (ImGui::MenuItem("Light palette"))
                codeEditor.SetPalette(TextEditor::GetLightPalette());
            if (ImGui::MenuItem("Retro blue palette"))
                codeEditor.SetPalette(TextEditor::GetRetroBluePalette());
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Convert"))
        {
            if (ImGui::BeginMenu("SPIR-V")) // <-- Append!
            {
                ImGui::Combo("Client", &CLIENT_ITEMS_CURRENT_INDEX, CLIENT_ITEMS, IM_ARRAYSIZE(CLIENT_ITEMS));
                ImGui::Separator();
                ImGui::Combo("Target Client", &TARGET_CLIENT_ITEMS_CURRENT_INDEX, TARGET_CLIENT_ITEMS,
                             IM_ARRAYSIZE(TARGET_CLIENT_ITEMS));
                ImGui::Combo("Target Language", &TARGET_LANGUAGE_ITEMS_CURRENT_INDEX, TARGET_LANGUAGE_ITEMS,
                             IM_ARRAYSIZE(TARGET_LANGUAGE_ITEMS));

                ImGui::Separator();
                if (ImGui::Button("To SPIR-V Binary"))
                {
                    ToSpirVBinaryCallback();
                }
                ImGui::Separator();
                if (ImGui::Button("To SPIR-V Disassemble"))
                {
                    ToSpirVDisassembleCallback();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    {
        struct funcs
        {
            static bool IsLegacyNativeDupe(ImGuiKey key)
            {
                return key < 512 && ImGui::GetIO().KeyMap[key] != -1;
            }
        }; // Hide Native<>ImGuiKey duplicates when both exists in the array
        ImGuiKey start_key = (ImGuiKey)0;
        // ImGui::Text("Keys down:");
        std::vector<ImGuiKey> keys;
        for (ImGuiKey key = start_key; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1))
        {
            if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key))
                continue;
            // ImGui::SameLine();
            // ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key);
            keys.push_back(key);
        }

        // static previous_paste_time = io.DeltaTime;
        ImGuiContext &g = *ImGui::GetCurrentContext();

        static float second = 0;
        second += io.DeltaTime;
        // second += g.FramerateSecPerFrameAccum / g.FramerateSecPerFrameCount;

        static float trigger_paste_delta_time = 0.1;
        static float next_trigger_paste_time = 0;

        static float trigger_copy_delta_time = 0.1;
        static float next_trigger_copy_time = 0;

        // ImGui::Text(std::to_string(second).c_str());

        if (keys.size() == 3)
        {
            if (keys[0] == ImGuiKey::ImGuiKey_LeftCtrl && keys[1] == ImGuiKey::ImGuiKey_V &&
                keys[2] == ImGuiKey::ImGuiKey_ReservedForModCtrl)
            {
                if (second > next_trigger_paste_time)
                {
                    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
                    {
                        codeEditor.Paste(clipboard_str__.c_str());
                    }
                }
                next_trigger_paste_time = second + trigger_paste_delta_time;
            }

            if (keys[0] == ImGuiKey::ImGuiKey_LeftCtrl && keys[1] == ImGuiKey::ImGuiKey_C &&
                keys[2] == ImGuiKey::ImGuiKey_ReservedForModCtrl)
            {
                if (second > next_trigger_copy_time)
                {
                    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
                    {
                        std::string selected_code_str = codeEditor.GetSelectedText();
                        // std::cout << "selected_code_str:" << selected_code_str << std::endl;
                        write_clipboard_str(selected_code_str.c_str());
                    }
                }
                next_trigger_copy_time = second + trigger_copy_delta_time;
            }
        }
    }

    if (ImGui::Combo("Language", &LANGUAGE_ITEMS_CURRENT_INDEX, LANGUAGE_ITEMS, IM_ARRAYSIZE(LANGUAGE_ITEMS)))
    {
        if (LANGUAGE_ITEMS_CURRENT_INDEX == 0)
        {
            ChangeEditorLanguage(codeEditor, CodeEditorLanguage::Language::GLSL);
        }
        else if (LANGUAGE_ITEMS_CURRENT_INDEX == 1)
        {
            ChangeEditorLanguage(codeEditor, CodeEditorLanguage::Language::HLSL);
        }
        else if (LANGUAGE_ITEMS_CURRENT_INDEX == 2)
        {
            ChangeEditorLanguage(codeEditor, CodeEditorLanguage::Language::SPIRV_BINARY);
        }
        else if (LANGUAGE_ITEMS_CURRENT_INDEX == 3)
        {
            ChangeEditorLanguage(codeEditor, CodeEditorLanguage::Language::SPIRV_DISASSEMBLE);
        }
        // Log(std::to_string(LANGUAGE_ITEMS_CURRENT_INDEX));
    }

    // FIXME: If Language is [SPIR-V Binary] or [SPIR-V Disassemble] we don't need the [Shader Type] item
    if (LANGUAGE_ITEMS_CURRENT_INDEX != 2 && LANGUAGE_ITEMS_CURRENT_INDEX != 3)
    {
        ImGui::Combo("Shader Type", &SHADER_TYPE_ITEMS_CURRENT_INDEX, SHADER_TYPE_ITEMS,
                     IM_ARRAYSIZE(SHADER_TYPE_ITEMS));
    }
    ImGui::Text("ImGui Version: %s", ImGui::GetVersion());
    ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, codeEditor.GetTotalLines(),
                codeEditor.IsOverwrite() ? "Ovr" : "Ins", codeEditor.CanUndo() ? "*" : " ",
                codeEditor.GetLanguageDefinition().mName.c_str(), fileToEdit);

    codeEditor.Render("TextEditor");
    ImGui::End();
}

void CodeViewerEditorLoop()
{
    if (!isCodeViewEditorShow)
    {
        return;
    }

    ImGuiIO &io = ImGui::GetIO();

    static bool is_init = false;
    // static const char* fileToEdit = "E:/Echo/SDK/ImGuiColorTextEdit/TextEditor.cpp";
    static const char *fileToEdit = "";

    if (!is_init)
    {
        ChangeEditorLanguage(codeViewEditor, CodeEditorLanguage::Language::GLSL);

        // TextEditor::ErrorMarkers markers;
        // markers.insert(
        //     std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found:
        //     \"TextEditor.h\""));
        // markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
        // codeViewEditor.SetErrorMarkers(markers);

        {
            // std::ifstream t(fileToEdit);
            // if (t.good())
            //{
            //     std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            //     codeViewEditor.SetText(str);
            // }
        }

        codeViewEditor.SetReadOnly(true);

        is_init = true;
    }
    // std::cout << isCodeViewEditorShow << std::endl;

    auto cpos = codeViewEditor.GetCursorPosition();
    ImGui::Begin("Code Viewer", &isCodeViewEditorShow, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
    ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImGui::SetWindowPos(ImVec2(770, 20), ImGuiCond_FirstUseEver);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save"))
            {
                auto textToSave = codeViewEditor.GetText();
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
            bool ro = codeViewEditor.IsReadOnly();
            // if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
            //     codeViewEditor.SetReadOnly(ro);
            // ImGui::Separator();

            if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && codeViewEditor.CanUndo()))
                codeViewEditor.Undo();
            if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && codeViewEditor.CanRedo()))
                codeViewEditor.Redo();

            ImGui::Separator();

            if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, codeViewEditor.HasSelection()))
                codeViewEditor.Copy();
            if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && codeViewEditor.HasSelection()))
                codeViewEditor.Cut();
            if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && codeViewEditor.HasSelection()))
                codeViewEditor.Delete();
            if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro))
                codeViewEditor.Paste(clipboard_str__.c_str());

            ImGui::Separator();

            if (ImGui::MenuItem("Select all", nullptr, nullptr))
                codeViewEditor.SetSelection(TextEditor::Coordinates(),
                                            TextEditor::Coordinates(codeViewEditor.GetTotalLines(), 0));

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Dark palette"))
                codeViewEditor.SetPalette(TextEditor::GetDarkPalette());
            if (ImGui::MenuItem("Light palette"))
                codeViewEditor.SetPalette(TextEditor::GetLightPalette());
            if (ImGui::MenuItem("Retro blue palette"))
                codeViewEditor.SetPalette(TextEditor::GetRetroBluePalette());
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    {
        struct funcs
        {
            static bool IsLegacyNativeDupe(ImGuiKey key)
            {
                return key < 512 && ImGui::GetIO().KeyMap[key] != -1;
            }
        }; // Hide Native<>ImGuiKey duplicates when both exists in the array
        ImGuiKey start_key = (ImGuiKey)0;
        // ImGui::Text("Keys down:");
        std::vector<ImGuiKey> keys;
        for (ImGuiKey key = start_key; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1))
        {
            if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key))
                continue;
            // ImGui::SameLine();
            // ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key);
            keys.push_back(key);
        }

        // static previous_paste_time = io.DeltaTime;
        ImGuiContext &g = *ImGui::GetCurrentContext();

        static float second = 0;
        second += io.DeltaTime;
        // second += g.FramerateSecPerFrameAccum / g.FramerateSecPerFrameCount;

        static float trigger_paste_delta_time = 0.1;
        static float next_trigger_paste_time = 0;

        static float trigger_copy_delta_time = 0.1;
        static float next_trigger_copy_time = 0;

        // ImGui::Text(std::to_string(second).c_str());

        if (keys.size() == 3)
        {
            if (keys[0] == ImGuiKey::ImGuiKey_LeftCtrl && keys[1] == ImGuiKey::ImGuiKey_V &&
                keys[2] == ImGuiKey::ImGuiKey_ReservedForModCtrl)
            {
                if (second > next_trigger_paste_time)
                {
                    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
                    {
                        // codeViewEditor.Paste(clipboard_str__.c_str());
                    }
                }
                next_trigger_paste_time = second + trigger_paste_delta_time;
            }

            if (keys[0] == ImGuiKey::ImGuiKey_LeftCtrl && keys[1] == ImGuiKey::ImGuiKey_C &&
                keys[2] == ImGuiKey::ImGuiKey_ReservedForModCtrl)
            {
                if (second > next_trigger_copy_time)
                {
                    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
                    {
                        std::string selected_code_str = codeViewEditor.GetSelectedText();
                        // std::cout << "selected_code_str:" << selected_code_str << std::endl;
                        write_clipboard_str(selected_code_str.c_str());
                    }
                }
                next_trigger_copy_time = second + trigger_copy_delta_time;
            }
        }
    }

    ImGui::Text("ImGui Version: %s", ImGui::GetVersion());
    ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1,
                codeViewEditor.GetTotalLines(), codeViewEditor.IsOverwrite() ? "Ovr" : "Ins",
                codeViewEditor.CanUndo() ? "*" : " ", codeViewEditor.GetLanguageDefinition().mName.c_str(), fileToEdit);

    codeViewEditor.Render("TextEditor");
    ImGui::End();
}

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

    // ImGui::ShowDemoWindow();

    static bool show_console_window = true;

    console.Draw("Console", &show_console_window);

    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    CodeEditorLoop();
    CodeViewerEditorLoop();

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