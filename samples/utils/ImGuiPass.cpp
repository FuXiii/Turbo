#include "ImGuiPass.h"
#include <TBuffer.h>
#include <TCommandBuffer.h>
#include <TFence.h>
#include <ReadFile.h>
#include <TPipelineDescriptorSet.h>

void ImGuiPassGlfwScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (ImGui::GetCurrentContext() != nullptr)
    {
        ImGui::GetIO().AddMouseWheelEvent(xoffset, yoffset);
    }
}

ImGuiPass::ImGuiPass(GLFWwindow *window)
{
    window != nullptr ? this->window = window : throw std::runtime_error("Error: ImGuiPass need a valid GLFWwindow window.");
    glfwSetScrollCallback(window, ImGuiPassGlfwScrollCallback);
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
            descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::COMBINED_IMAGE_SAMPLER, 1));
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