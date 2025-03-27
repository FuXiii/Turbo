#include "ImGuiPass.h"
#include <TBuffer.h>
#include <TCommandBuffer.h>
#include <TFence.h>
#include <ReadFile.h>
#include <TPipelineDescriptorSet.h>

ImGuiKey GlfwKeyToImGuiKey(int key)
{
    // clang-format off
    switch (key) {
        case GLFW_KEY_SPACE              :{return ImGuiKey_::ImGuiKey_Space;}break;
        case GLFW_KEY_APOSTROPHE         :{return ImGuiKey_::ImGuiKey_Apostrophe;}break;
        case GLFW_KEY_COMMA              :{return ImGuiKey_::ImGuiKey_Comma;}break;
        case GLFW_KEY_MINUS              :{return ImGuiKey_::ImGuiKey_Minus;}break;
        case GLFW_KEY_PERIOD             :{return ImGuiKey_::ImGuiKey_Period;}break;
        case GLFW_KEY_SLASH              :{return ImGuiKey_::ImGuiKey_Slash;}break;
        case GLFW_KEY_0                  :{return ImGuiKey_::ImGuiKey_0;}break;
        case GLFW_KEY_1                  :{return ImGuiKey_::ImGuiKey_1;}break;
        case GLFW_KEY_2                  :{return ImGuiKey_::ImGuiKey_2;}break;
        case GLFW_KEY_3                  :{return ImGuiKey_::ImGuiKey_3;}break;
        case GLFW_KEY_4                  :{return ImGuiKey_::ImGuiKey_4;}break;
        case GLFW_KEY_5                  :{return ImGuiKey_::ImGuiKey_5;}break;
        case GLFW_KEY_6                  :{return ImGuiKey_::ImGuiKey_6;}break;
        case GLFW_KEY_7                  :{return ImGuiKey_::ImGuiKey_7;}break;
        case GLFW_KEY_8                  :{return ImGuiKey_::ImGuiKey_8;}break;
        case GLFW_KEY_9                  :{return ImGuiKey_::ImGuiKey_9;}break;
        case GLFW_KEY_SEMICOLON          :{return ImGuiKey_::ImGuiKey_Semicolon;}break;
        case GLFW_KEY_EQUAL              :{return ImGuiKey_::ImGuiKey_Equal;}break;
        case GLFW_KEY_A                  :{return ImGuiKey_::ImGuiKey_A;}break;
        case GLFW_KEY_B                  :{return ImGuiKey_::ImGuiKey_B;}break;
        case GLFW_KEY_C                  :{return ImGuiKey_::ImGuiKey_C;}break;
        case GLFW_KEY_D                  :{return ImGuiKey_::ImGuiKey_D;}break;
        case GLFW_KEY_E                  :{return ImGuiKey_::ImGuiKey_E;}break;
        case GLFW_KEY_F                  :{return ImGuiKey_::ImGuiKey_F;}break;
        case GLFW_KEY_G                  :{return ImGuiKey_::ImGuiKey_G;}break;
        case GLFW_KEY_H                  :{return ImGuiKey_::ImGuiKey_H;}break;
        case GLFW_KEY_I                  :{return ImGuiKey_::ImGuiKey_I;}break;
        case GLFW_KEY_J                  :{return ImGuiKey_::ImGuiKey_J;}break;
        case GLFW_KEY_K                  :{return ImGuiKey_::ImGuiKey_K;}break;
        case GLFW_KEY_L                  :{return ImGuiKey_::ImGuiKey_L;}break;
        case GLFW_KEY_M                  :{return ImGuiKey_::ImGuiKey_M;}break;
        case GLFW_KEY_N                  :{return ImGuiKey_::ImGuiKey_N;}break;
        case GLFW_KEY_O                  :{return ImGuiKey_::ImGuiKey_O;}break;
        case GLFW_KEY_P                  :{return ImGuiKey_::ImGuiKey_P;}break;
        case GLFW_KEY_Q                  :{return ImGuiKey_::ImGuiKey_Q;}break;
        case GLFW_KEY_R                  :{return ImGuiKey_::ImGuiKey_R;}break;
        case GLFW_KEY_S                  :{return ImGuiKey_::ImGuiKey_S;}break;
        case GLFW_KEY_T                  :{return ImGuiKey_::ImGuiKey_T;}break;
        case GLFW_KEY_U                  :{return ImGuiKey_::ImGuiKey_U;}break;
        case GLFW_KEY_V                  :{return ImGuiKey_::ImGuiKey_V;}break;
        case GLFW_KEY_W                  :{return ImGuiKey_::ImGuiKey_W;}break;
        case GLFW_KEY_X                  :{return ImGuiKey_::ImGuiKey_X;}break;
        case GLFW_KEY_Y                  :{return ImGuiKey_::ImGuiKey_Y;}break;
        case GLFW_KEY_Z                  :{return ImGuiKey_::ImGuiKey_Z;}break;
        case GLFW_KEY_LEFT_BRACKET       :{return ImGuiKey_::ImGuiKey_LeftBracket;}break;
        case GLFW_KEY_BACKSLASH          :{return ImGuiKey_::ImGuiKey_Backslash;}break;
        case GLFW_KEY_RIGHT_BRACKET      :{return ImGuiKey_::ImGuiKey_RightBracket;}break;
        case GLFW_KEY_GRAVE_ACCENT       :{return ImGuiKey_::ImGuiKey_GraveAccent;}break;
        case GLFW_KEY_WORLD_1            :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_WORLD_2            :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_ESCAPE             :{return ImGuiKey_::ImGuiKey_Escape;}break;
        case GLFW_KEY_ENTER              :{return ImGuiKey_::ImGuiKey_Enter;}break;
        case GLFW_KEY_TAB                :{return ImGuiKey_::ImGuiKey_Tab;}break;
        case GLFW_KEY_BACKSPACE          :{return ImGuiKey_::ImGuiKey_Backspace;}break;
        case GLFW_KEY_INSERT             :{return ImGuiKey_::ImGuiKey_Insert;}break;
        case GLFW_KEY_DELETE             :{return ImGuiKey_::ImGuiKey_Delete;}break;
        case GLFW_KEY_RIGHT              :{return ImGuiKey_::ImGuiKey_RightArrow;}break;
        case GLFW_KEY_LEFT               :{return ImGuiKey_::ImGuiKey_LeftArrow;}break;
        case GLFW_KEY_DOWN               :{return ImGuiKey_::ImGuiKey_DownArrow ;}break;
        case GLFW_KEY_UP                 :{return ImGuiKey_::ImGuiKey_UpArrow;}break;
        case GLFW_KEY_PAGE_UP            :{return ImGuiKey_::ImGuiKey_PageUp;}break;
        case GLFW_KEY_PAGE_DOWN          :{return ImGuiKey_::ImGuiKey_PageUp;}break;
        case GLFW_KEY_HOME               :{return ImGuiKey_::ImGuiKey_Home;}break;
        case GLFW_KEY_END                :{return ImGuiKey_::ImGuiKey_End;}break;
        case GLFW_KEY_CAPS_LOCK          :{return ImGuiKey_::ImGuiKey_CapsLock;}break;
        case GLFW_KEY_SCROLL_LOCK        :{return ImGuiKey_::ImGuiKey_ScrollLock;}break;
        case GLFW_KEY_NUM_LOCK           :{return ImGuiKey_::ImGuiKey_NumLock;}break;
        case GLFW_KEY_PRINT_SCREEN       :{return ImGuiKey_::ImGuiKey_PrintScreen;}break;
        case GLFW_KEY_PAUSE              :{return ImGuiKey_::ImGuiKey_Pause;}break;
        case GLFW_KEY_F1                 :{return ImGuiKey_::ImGuiKey_F1;}break;
        case GLFW_KEY_F2                 :{return ImGuiKey_::ImGuiKey_F2;}break;
        case GLFW_KEY_F3                 :{return ImGuiKey_::ImGuiKey_F3;}break;
        case GLFW_KEY_F4                 :{return ImGuiKey_::ImGuiKey_F4;}break;
        case GLFW_KEY_F5                 :{return ImGuiKey_::ImGuiKey_F5;}break;
        case GLFW_KEY_F6                 :{return ImGuiKey_::ImGuiKey_F6;}break;
        case GLFW_KEY_F7                 :{return ImGuiKey_::ImGuiKey_F7;}break;
        case GLFW_KEY_F8                 :{return ImGuiKey_::ImGuiKey_F8;}break;
        case GLFW_KEY_F9                 :{return ImGuiKey_::ImGuiKey_F9;}break;
        case GLFW_KEY_F10                :{return ImGuiKey_::ImGuiKey_F10;}break;
        case GLFW_KEY_F11                :{return ImGuiKey_::ImGuiKey_F11;}break;
        case GLFW_KEY_F12                :{return ImGuiKey_::ImGuiKey_F12;}break;
        case GLFW_KEY_F13                :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_F14                :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_F15                :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_F16                :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_F17                :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_F18                :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_F19                :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_F20                :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_F21                :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_F22                :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_F23                :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_F24                :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_F25                :{return ImGuiKey_::ImGuiKey_None;}break;
        case GLFW_KEY_KP_0               :{return ImGuiKey_::ImGuiKey_Keypad0;}break;
        case GLFW_KEY_KP_1               :{return ImGuiKey_::ImGuiKey_Keypad1;}break;
        case GLFW_KEY_KP_2               :{return ImGuiKey_::ImGuiKey_Keypad2;}break;
        case GLFW_KEY_KP_3               :{return ImGuiKey_::ImGuiKey_Keypad3;}break;
        case GLFW_KEY_KP_4               :{return ImGuiKey_::ImGuiKey_Keypad4;}break;
        case GLFW_KEY_KP_5               :{return ImGuiKey_::ImGuiKey_Keypad5;}break;
        case GLFW_KEY_KP_6               :{return ImGuiKey_::ImGuiKey_Keypad6;}break;
        case GLFW_KEY_KP_7               :{return ImGuiKey_::ImGuiKey_Keypad7;}break;
        case GLFW_KEY_KP_8               :{return ImGuiKey_::ImGuiKey_Keypad8;}break;
        case GLFW_KEY_KP_9               :{return ImGuiKey_::ImGuiKey_Keypad9;}break;
        case GLFW_KEY_KP_DECIMAL         :{return ImGuiKey_::ImGuiKey_KeypadDecimal;}break;
        case GLFW_KEY_KP_DIVIDE          :{return ImGuiKey_::ImGuiKey_KeypadDivide;}break;
        case GLFW_KEY_KP_MULTIPLY        :{return ImGuiKey_::ImGuiKey_KeypadMultiply;}break;
        case GLFW_KEY_KP_SUBTRACT        :{return ImGuiKey_::ImGuiKey_KeypadSubtract;}break;
        case GLFW_KEY_KP_ADD             :{return ImGuiKey_::ImGuiKey_KeypadAdd;}break;
        case GLFW_KEY_KP_ENTER           :{return ImGuiKey_::ImGuiKey_KeypadEnter;}break;
        case GLFW_KEY_KP_EQUAL           :{return ImGuiKey_::ImGuiKey_KeypadEqual;}break;
        case GLFW_KEY_LEFT_SHIFT         :{return ImGuiKey_::ImGuiKey_LeftShift;}break;
        case GLFW_KEY_LEFT_CONTROL       :{return ImGuiKey_::ImGuiKey_LeftCtrl;}break;
        case GLFW_KEY_LEFT_ALT           :{return ImGuiKey_::ImGuiKey_LeftAlt;}break;
        case GLFW_KEY_LEFT_SUPER         :{return ImGuiKey_::ImGuiKey_LeftSuper;}break;
        case GLFW_KEY_RIGHT_SHIFT        :{return ImGuiKey_::ImGuiKey_RightShift;}break;
        case GLFW_KEY_RIGHT_CONTROL      :{return ImGuiKey_::ImGuiKey_RightCtrl;}break;
        case GLFW_KEY_RIGHT_ALT          :{return ImGuiKey_::ImGuiKey_RightAlt;}break;
        case GLFW_KEY_RIGHT_SUPER        :{return ImGuiKey_::ImGuiKey_RightSuper;}break;
        case GLFW_KEY_MENU               :{return ImGuiKey_::ImGuiKey_Menu;}break;
        default:{}break;
    }
    // clang-format on
    return ImGuiKey_::ImGuiKey_None;
}

void ImGuiPassGlfwScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (ImGui::GetCurrentContext() != nullptr)
    {
        ImGui::GetIO().AddMouseWheelEvent(xoffset, yoffset);
    }
}

void ImGuiPassGlfwCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (ImGui::GetCurrentContext() != nullptr)
    {
        ImGui::GetIO().AddInputCharacter(key);
        if (action == GLFW_RELEASE)
        {
            ImGui::GetIO().AddKeyEvent(GlfwKeyToImGuiKey(key), false);
        }
        else
        {
            ImGui::GetIO().AddKeyEvent(GlfwKeyToImGuiKey(key), true);
        }
    }
}

ImGuiPass::ImGuiPass(GLFWwindow *window)
{
    window != nullptr ? this->window = window : throw std::runtime_error("Error: ImGuiPass need a valid GLFWwindow window.");
    glfwSetScrollCallback(window, ImGuiPassGlfwScrollCallback);
    glfwSetKeyCallback(window, ImGuiPassGlfwCallback);
}

ImGuiPass::~ImGuiPass()
{
}

void ImGuiPass::Init(Turbo::Core::TCommandBuffer *commandBuffer, Turbo::Core::TImageView *imageView)
{
    if (Turbo::Core::TReferenced::Valid(commandBuffer) && Turbo::Core::TReferenced::Valid(imageView))
    {
        {
            ImGui::CreateContext();
            ImGui::StyleColorsDark();
        }

        auto command_pool = commandBuffer->GetCommandBufferPool();
        auto queue = command_pool->GetDeviceQueue();

        {
            this->device = queue->GetDevice();
        }

        {
            std::vector<Turbo::Core::TDescriptorSize> descriptor_sizes;
            descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptor::TType::COMBINED_IMAGE_SAMPLER, 1));
            this->descriptorPool = new Turbo::Core::TDescriptorPool(device, descriptor_sizes.size() * 1000, descriptor_sizes);
        }

        {
            Turbo::Core::TSubpass subpass(Turbo::Core::TPipelineType::Graphics);
            subpass.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);

            std::vector<Turbo::Core::TSubpass> subpasses;
            subpasses.push_back(subpass);

            Turbo::Core::TAttachment color_attachment(imageView->GetFormat(), imageView->GetImage()->GetSampleCountBits(), Turbo::Core::TLoadOp::LOAD, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL, Turbo::Core::TImageLayout::PRESENT_SRC_KHR);

            std::vector<Turbo::Core::TAttachment> attachemts;
            attachemts.push_back(color_attachment);

            this->renderPass = new Turbo::Core::TRenderPass(this->device, attachemts, subpasses);
        }

        {
            this->sampler = new Turbo::Core::TSampler(this->device);
        }

        {

            Turbo::Core::TRefPtr<Turbo::Core::TVertexShader> imgui_vertex_shader = new Turbo::Core::TVertexShader(this->device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(std::string(TURBO_ASSET_ROOT) + "/shaders/imgui_meta.vert"));
            Turbo::Core::TRefPtr<Turbo::Core::TFragmentShader> imgui_fragment_shader = new Turbo::Core::TFragmentShader(this->device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(std::string(TURBO_ASSET_ROOT) + "/shaders/imgui_meta.frag"));

            Turbo::Core::TVertexBinding imgui_vertex_binding(0, sizeof(ImDrawVert), Turbo::Core::TVertexRate::VERTEX);
            imgui_vertex_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32_SFLOAT, IM_OFFSETOF(ImDrawVert, pos));  // position
            imgui_vertex_binding.AddAttribute(1, Turbo::Core::TFormatType::R32G32_SFLOAT, IM_OFFSETOF(ImDrawVert, uv));   // uv
            imgui_vertex_binding.AddAttribute(2, Turbo::Core::TFormatType::R8G8B8A8_UNORM, IM_OFFSETOF(ImDrawVert, col)); // color

            std::vector<Turbo::Core::TVertexBinding> imgui_vertex_bindings;
            imgui_vertex_bindings.push_back(imgui_vertex_binding);

            this->pipeline = new Turbo::Core::TGraphicsPipeline(this->renderPass, 0, imgui_vertex_bindings, imgui_vertex_shader, imgui_fragment_shader, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_NONE, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, false, false, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, 0, 0, false, Turbo::Core::TLogicOp::NO_OP, true, Turbo::Core::TBlendFactor::SRC_ALPHA, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendOp::ADD, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendFactor::ZERO, Turbo::Core::TBlendOp::ADD);
        }

        {
            unsigned char *imgui_font_pixels;
            int imgui_font_width, imgui_font_height;
            ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&imgui_font_pixels, &imgui_font_width, &imgui_font_height);
            size_t imgui_upload_size = imgui_font_width * imgui_font_height * 4 * sizeof(char);

            Turbo::Core::TRefPtr<Turbo::Core::TImage> imgui_font_image = new Turbo::Core::TImage(this->device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R8G8B8A8_UNORM, imgui_font_width, imgui_font_height, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_SAMPLED | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
            this->font = new Turbo::Core::TImageView(imgui_font_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, imgui_font_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
            {
                Turbo::Core::TRefPtr<Turbo::Core::TBuffer> imgui_font_buffer = new Turbo::Core::TBuffer(this->device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, imgui_upload_size);
                memcpy(imgui_font_buffer->Map(), imgui_font_pixels, imgui_upload_size);
                imgui_font_buffer->Unmap();

                Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> imgui_copy_command_buffer = command_pool->Allocate();
                imgui_copy_command_buffer->Begin();
                imgui_copy_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::HOST_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::HOST_WRITE_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, imgui_font_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                imgui_copy_command_buffer->CmdCopyBufferToImage(imgui_font_buffer, imgui_font_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, 0, imgui_font_width, imgui_font_height, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1, 0, 0, 0, imgui_font_width, imgui_font_height, 1);
                imgui_copy_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::FRAGMENT_SHADER_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TAccessBits::SHADER_READ_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::SHADER_READ_ONLY_OPTIMAL, imgui_font_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                imgui_copy_command_buffer->End();

                Turbo::Core::TRefPtr<Turbo::Core::TFence> imgui_font_copy_fence = new Turbo::Core::TFence(this->device);
                queue->Submit(imgui_copy_command_buffer, imgui_font_copy_fence);

                imgui_font_copy_fence->WaitUntil();
            }

            this->pipelineDescriptorSet = this->descriptorPool->Allocate(this->pipeline->GetPipelineLayout());
            this->pipelineDescriptorSet->BindData(0, 0, std::make_pair(this->font, this->sampler));

            ImGui::GetIO().Fonts->TexID = (ImTextureID)(imgui_font_image->GetVkImage());
        }
    }
}

void ImGuiPass::DrawImGui(float deltaTime, Turbo::Core::TCommandBuffer *commandBuffer, Turbo::Core::TImageView *imageView)
{
    ImGuiIO &io = ImGui::GetIO();
    io.DeltaTime = deltaTime;

    io.DisplaySize = ImVec2(imageView->GetImage()->GetWidth(), imageView->GetImage()->GetHeight());

    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
    {
        io.MouseDown[i] = glfwGetMouseButton(window, i) != 0;
    }

    if (glfwGetWindowAttrib(window, GLFW_FOCUSED) != 0)
    {
        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        io.MousePos = ImVec2(mouse_x, mouse_y);
    }

    {
        static Turbo::Core::TRefPtr<Turbo::Core::TBuffer> imgui_vertex_buffer;
        static Turbo::Core::TRefPtr<Turbo::Core::TBuffer> imgui_index_buffer;
        // if(false)
        {
            ImDrawData *draw_data = ImGui::GetDrawData();

            std::vector<Turbo::Core::TImageView *> image_views;
            image_views.push_back(imageView);
            this->framebuffer = new Turbo::Core::TFramebuffer(this->renderPass, image_views);

            Turbo::Core::TViewport viewport(0, 0, io.DisplaySize.x, io.DisplaySize.y, 0, 1);
            Turbo::Core::TScissor scissor(0, 0, viewport.GetWidth(), viewport.GetHeight());

            commandBuffer->CmdBeginRenderPass(this->renderPass, this->framebuffer);
            commandBuffer->CmdSetViewport(viewport);
            commandBuffer->CmdSetScissor(scissor);
            if (draw_data->TotalVtxCount > 0)
            {
                size_t vertex_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
                size_t index_size = draw_data->TotalIdxCount * sizeof(ImDrawIdx);

                imgui_vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, vertex_size);
                imgui_index_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_INDEX_BUFFER, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, index_size);

                ImDrawVert *vtx_dst = (ImDrawVert *)imgui_vertex_buffer->Map();
                ImDrawIdx *idx_dst = (ImDrawIdx *)imgui_index_buffer->Map();
                for (int n = 0; n < draw_data->CmdListsCount; n++)
                {
                    const ImDrawList *cmd_list = draw_data->CmdLists[n];
                    memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
                    memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
                    vtx_dst += cmd_list->VtxBuffer.Size;
                    idx_dst += cmd_list->IdxBuffer.Size;
                }
                imgui_vertex_buffer->Unmap();
                imgui_index_buffer->Unmap();

                commandBuffer->CmdBindPipeline(this->pipeline);
                commandBuffer->CmdBindPipelineDescriptorSet(this->pipelineDescriptorSet);

                std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> imgui_vertex_buffers;
                imgui_vertex_buffers.push_back(imgui_vertex_buffer);
                commandBuffer->CmdBindVertexBuffers(imgui_vertex_buffers);
                commandBuffer->CmdBindIndexBuffer(imgui_index_buffer, 0, sizeof(ImDrawIdx) == 2 ? Turbo::Core::TIndexType::UINT16 : Turbo::Core::TIndexType::UINT32);

                float scale[2];
                scale[0] = 2.0f / draw_data->DisplaySize.x;
                scale[1] = 2.0f / draw_data->DisplaySize.y;
                float translate[2];
                translate[0] = -1.0f - draw_data->DisplayPos.x * scale[0];
                translate[1] = -1.0f - draw_data->DisplayPos.y * scale[1];

                commandBuffer->CmdPushConstants(sizeof(float) * 0, sizeof(float) * 2, scale);
                commandBuffer->CmdPushConstants(sizeof(float) * 2, sizeof(float) * 2, translate);

                ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
                ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

                int global_vtx_offset = 0;
                int global_idx_offset = 0;

                for (int n = 0; n < draw_data->CmdListsCount; n++)
                {
                    const ImDrawList *cmd_list = draw_data->CmdLists[n];
                    for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
                    {
                        const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];
                        if (pcmd->UserCallback != NULL)
                        {
                            // User callback, registered via ImDrawList::AddCallback()
                            // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                            if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                            {
                                commandBuffer->CmdBindPipeline(this->pipeline);
                                commandBuffer->CmdBindPipelineDescriptorSet(this->pipelineDescriptorSet);

                                commandBuffer->CmdBindVertexBuffers(imgui_vertex_buffers);
                                commandBuffer->CmdBindIndexBuffer(imgui_index_buffer, 0, sizeof(ImDrawIdx) == 2 ? Turbo::Core::TIndexType::UINT16 : Turbo::Core::TIndexType::UINT32);

                                float __scale[2];
                                __scale[0] = 2.0f / draw_data->DisplaySize.x;
                                __scale[1] = 2.0f / draw_data->DisplaySize.y;
                                float __translate[2];
                                __translate[0] = -1.0f - draw_data->DisplayPos.x * scale[0];
                                __translate[1] = -1.0f - draw_data->DisplayPos.y * scale[1];

                                commandBuffer->CmdPushConstants(sizeof(float) * 0, sizeof(float) * 2, __scale);
                                commandBuffer->CmdPushConstants(sizeof(float) * 2, sizeof(float) * 2, __translate);
                            }
                            else
                            {
                                pcmd->UserCallback(cmd_list, pcmd);
                            }
                        }
                        else
                        {
                            ImVec4 clip_rect;
                            clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                            clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                            clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                            clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                            // if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                            {
                                // Negative offsets are illegal for vkCmdSetScissor
                                if (clip_rect.x < 0.0f)
                                    clip_rect.x = 0.0f;
                                if (clip_rect.y < 0.0f)
                                    clip_rect.y = 0.0f;

                                VkRect2D scissor;
                                scissor.offset.x = (int32_t)(clip_rect.x);
                                scissor.offset.y = (int32_t)(clip_rect.y);
                                scissor.extent.width = (uint32_t)(clip_rect.z - clip_rect.x);
                                scissor.extent.height = (uint32_t)(clip_rect.w - clip_rect.y);

                                Turbo::Core::TScissor imgui_scissor(scissor.offset.x, scissor.offset.y, scissor.extent.width, scissor.extent.height);
                                std::vector<Turbo::Core::TScissor> imgui_scissors;
                                imgui_scissors.push_back(imgui_scissor);
                                commandBuffer->CmdSetScissor(imgui_scissors);

                                // Draw
                                commandBuffer->CmdDrawIndexed(pcmd->ElemCount, 1, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset, 0);
                            }
                        }
                    }
                    global_idx_offset += cmd_list->IdxBuffer.Size;
                    global_vtx_offset += cmd_list->VtxBuffer.Size;
                }
            }
            commandBuffer->CmdEndRenderPass();
        }
    }
}

std::string ImGuiPass::ToString() const
{
    return "ImGuiPass";
}