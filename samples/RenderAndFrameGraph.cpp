#include "render/include/TBuffer.h"
#include "render/include/TDomain.h"
#include "render/include/TImage.h"
#include "render/include/TShader.h"
#include <chrono>
#include <framegraph/include/TFrameGraph.hpp>
#include <fstream>
#include <iostream>
#include <render/include/TContext.h>
#include <render/include/TPipeline.h>
#include <render/include/TRenderPass.h>
#include <render/include/TResourceAllocator.h>
#include <sstream>
#include <stdint.h>
#include <vector>

#include <core/include/TCommandBuffer.h>
#include <core/include/TDevice.h>
#include <core/include/TDeviceQueue.h>
#include <core/include/TFence.h>
#include <core/include/TImage.h>
#include <core/include/TImageView.h>
#include <core/include/TInstance.h>
#include <core/include/TPipeline.h>
#include <core/include/TSurface.h>
#include <core/include/TSwapchain.h>
#include <core/include/TVulkanLoader.h>
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

int main()
{
    /*
       [Clear Pass] → Color Texture → [Present Pass]

       1. 在ClearPass创建一个Texture并使用Clear刷新颜色
       2. 在PresentPass中将之前刷新的Texture结果拷贝至要显示的纹理中
       3. 显示
       4. 重复1-3

       注：该例子仅供参考，此示例仅为试验测试目的
    */
    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);

    if (!glfwInit())
        return 0;

    int window_width = 512;
    int window_height = 512;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(window_width, window_height, "Turbo", NULL, NULL);
    VkSurfaceKHR vk_surface_khr = VK_NULL_HANDLE;
    VkInstance vk_instance = context.GetInstance()->GetVkInstance();
    VkResult surface_result = glfwCreateWindowSurface(vk_instance, window, NULL, &vk_surface_khr);

    if (vk_surface_khr == VK_NULL_HANDLE)
    {
        std::cout << "vk_surface_khr is VK_NULL_HANDLE::" << surface_result << std::endl;
        return 0;
    }

    Turbo::Extension::TSurface *surface = new Turbo::Extension::TSurface(context.GetDevice(), vk_surface_khr);

    uint32_t max_image_count = surface->GetMaxImageCount();
    uint32_t min_image_count = surface->GetMinImageCount();
    uint32_t swapchain_image_count = max_image_count <= min_image_count ? min_image_count : max_image_count - 1;

    Turbo::Extension::TSwapchain *swapchain = new Turbo::Extension::TSwapchain(surface, swapchain_image_count, Turbo::Core::TFormatType::B8G8R8A8_SRGB, 1, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, true);
    std::vector<Turbo::Core::TImage *> swapchain_images = swapchain->GetImages();

    std::vector<Turbo::Core::TImageView *> swapchain_image_views;
    for (Turbo::Core::TImage *swapchain_image_item : swapchain_images)
    {
        Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        swapchain_image_views.push_back(swapchain_view);
    }

    int current_width = window_width;
    int current_height = window_height;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        Turbo::FrameGraph::TFrameGraph fg;

        struct ClearPassData
        {
            Turbo::FrameGraph::TResource colorTexture;
        };

        fg.AddPass<ClearPassData>(
            "ClearPass",
            [&](Turbo::FrameGraph::TFrameGraph::TBuilder &builder, ClearPassData &data) {
                data.colorTexture = builder.Create<Turbo::Render::TTexture2D>("ColorTexture", {(uint32_t)current_width, (uint32_t)current_height, 1, Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT | Turbo::Render::TImageUsageBits::TRANSFER_SRC | Turbo::Render::TImageUsageBits::TRANSFER_DST, Turbo::Render::TDomainBits::GPU});

                auto subpass0 = builder.CreateSubpass();
                data.colorTexture = subpass0.Write(data.colorTexture);

                fg.GetBlackboard()["ColorTexture"] = data.colorTexture;
            },
            [&](const ClearPassData &data, const Turbo::FrameGraph::TResources &resources, void *_context) {
                // TODO:Clear Color Texture

                Turbo::Render::TTexture2D color_texture = resources.Get<Turbo::Render::TTexture2D>(data.colorTexture);

                if (_context != nullptr)
                {
                    Turbo::Render::TContext *temp_context = (Turbo::Render::TContext *)_context;

                    auto now_time = glfwGetTime();

                    float r = (std::sin(now_time) + 1) * 0.5;
                    float g = (std::cos(now_time) + 1) * 0.5;
                    float b = (std::cos(now_time + 3.1415926 / 3) + 1) * 0.5;

                    temp_context->Flush();
                    temp_context->Wait(UINT64_MAX);

                    Turbo::Core::TFence *fence = new Turbo::Core::TFence(temp_context->GetDevice());
                    Turbo::Core::TCommandBuffer *cb = temp_context->AllocateCommandBuffer();
                    cb->Begin();
                    cb->CmdClearColorImage(temp_context->GetTextureImage(color_texture), Turbo::Core::TImageLayout::GENERAL, r, g, b, 1);
                    cb->End();
                    temp_context->GetDeviceQueue()->Submit(nullptr, nullptr, cb, fence);

                    fence->WaitUntil();

                    delete fence;
                    temp_context->FreeCommandBuffer(cb);

                    auto format = color_texture.GetFormat();
                }
            });

        struct PresentPassData
        {
            Turbo::FrameGraph::TResource colorTexture;
        };

        fg.AddPass<PresentPassData>(
            "PresentPass",
            [&](Turbo::FrameGraph::TFrameGraph::TBuilder &builder, PresentPassData &data) {
                data.colorTexture = fg.GetBlackboard()["ColorTexture"];

                auto subpass0 = builder.CreateSubpass();
                data.colorTexture = subpass0.Read(data.colorTexture);

                builder.SideEffect();
            },
            [&](const PresentPassData &data, const Turbo::FrameGraph::TResources &resources, void *_context) {
                // TODO:Present Color Texture

                Turbo::Render::TTexture2D color_texture = resources.Get<Turbo::Render::TTexture2D>(data.colorTexture);

                if (_context != nullptr)
                {
                    Turbo::Render::TContext *temp_context = (Turbo::Render::TContext *)_context;

                    // TODO: present
                    uint32_t index = UINT32_MAX;
                    Turbo::Core::TFence *acquire_next_image_fence = new Turbo::Core::TFence(temp_context->GetDevice());
                    Turbo::Core::TResult result = swapchain->AcquireNextImageUntil(nullptr, acquire_next_image_fence, &index);
                    acquire_next_image_fence->WaitUntil();
                    delete acquire_next_image_fence;
                    if (result == Turbo::Core::TResult::SUCCESS)
                    {
                        // TODO:Blit Image

                        auto show_target = swapchain_image_views[index];

                        Turbo::Core::TFence *fence = new Turbo::Core::TFence(temp_context->GetDevice());
                        Turbo::Core::TCommandBuffer *cb = temp_context->AllocateCommandBuffer();
                        cb->Begin();
                        cb->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::GENERAL, show_target);
                        cb->CmdBlitImage(temp_context->GetTextureImage(color_texture), Turbo::Core::TImageLayout::GENERAL, show_target->GetImage(), Turbo::Core::TImageLayout::GENERAL, 0, 0, 0, current_width, current_height, 1, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1, 0, 0, 0, current_width, current_height, 1, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1);
                        cb->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::PRESENT_SRC_KHR, show_target);
                        cb->End();
                        temp_context->GetDeviceQueue()->Submit(nullptr, nullptr, cb, fence);

                        fence->WaitUntil();

                        delete fence;
                        temp_context->FreeCommandBuffer(cb);
                        Turbo::Core::TResult result = temp_context->GetDeviceQueue()->Present(swapchain, index);
                        switch (result)
                        {
                        case Turbo::Core::TResult::MISMATCH: {
                            Turbo::Core::TDevice *device = temp_context->GetDevice();
                            device->WaitIdle();

                            swapchain_images.clear();

                            for (Turbo::Core::TImageView *swapchain_image_view_item : swapchain_image_views)
                            {
                                delete swapchain_image_view_item;
                            }

                            swapchain_image_views.clear();

                            Turbo::Extension::TSwapchain *old_swapchain = swapchain;
                            Turbo::Extension::TSwapchain *new_swapchain = new Turbo::Extension::TSwapchain(old_swapchain);
                            delete old_swapchain;

                            swapchain = new_swapchain;

                            swapchain_images = swapchain->GetImages();
                            for (Turbo::Core::TImage *swapchain_image_item : swapchain_images)
                            {
                                Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                                swapchain_image_views.push_back(swapchain_view);
                            }

                            glfwGetWindowSize(window, &current_width, &current_height);
                        }
                        break;
                        default: {
                        }
                        break;
                        }
                        // std::cout << "Present" << std::endl;
                    }
                }
            });

        fg.Compile();
        fg.Execute(&context, &resource_allocator);
    }

    context.GetDevice()->WaitIdle();

    for (Turbo::Core::TImageView *swapchain_image_view_item : swapchain_image_views)
    {
        delete swapchain_image_view_item;
    }

    delete swapchain;
    delete surface;
    PFN_vkDestroySurfaceKHR pfn_vk_destroy_surface_khr = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkDestroySurfaceKHR>(context.GetInstance()->GetVkInstance(), "vkDestroySurfaceKHR");
    pfn_vk_destroy_surface_khr(context.GetInstance()->GetVkInstance(), vk_surface_khr, nullptr);
    glfwTerminate();

    return 0;
}