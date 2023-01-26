#include "render/include/TContext.h"
#include "TAttachment.h"
#include "TRenderPass.h"
#include "render/include/TImage.h"
#include "render/include/TRenderPass.h"
#include "vulkan/vulkan_core.h"
#include <algorithm>
#include <core/include/TBuffer.h>
#include <core/include/TCommandBuffer.h>
#include <core/include/TCommandBufferPool.h>
#include <core/include/TCore.h>
#include <core/include/TDeviceQueue.h>
#include <core/include/TException.h>
#include <core/include/TFence.h>
#include <core/include/TImage.h>
#include <core/include/TInstance.h>
#include <core/include/TPhysicalDevice.h>
#include <core/include/TRenderPass.h>
#include <core/include/TVersion.h>
#include <core/include/TVulkanLoader.h>
#include <cstdint>
#include <framegraph/include/TFrameGraph.hpp>
#include <stdint.h>
#include <vector>

void Turbo::Render::TRenderPassPool::TRenderPassProxy::Create(Turbo::Render::TRenderPass &renderPass)
{
    // TODO:Create Turbo::Core::TRenderPass from Turbo::Render::TRenderPass
    // TODO:this->renderPass = new Turbo::Core::TRenderPass(...);
}

void Turbo::Render::TRenderPassPool::TRenderPassProxy::Destroy()
{
    // TODO:Destroy Turbo::Core::TRenderPass
    // TODO:delete this->renderPass;
}

Turbo::Render::TRenderPassPool::TRenderPassProxy::~TRenderPassProxy()
{
}

bool Turbo::Render::TRenderPassPool::TRenderPassProxy::IsValid()
{
    if (this->renderPass != nullptr)
    {
        return true;
    }

    return false;
}

Turbo::Render::TRenderPassPool::TRenderPassPool()
{
}

Turbo::Render::TRenderPassPool::~TRenderPassPool()
{
    // TODO:release this->renderPassProxies;
}

Turbo::Render::TRenderPassPool::TRenderPassProxy Turbo::Render::TRenderPassPool::Find(Turbo::Render::TRenderPass &renderPass)
{
    // first we need to judge if renderPass which input from external was empty we need return empty TRenderPassProxy
    // if(renderPass.IsEmpty())...

    auto find_result = std::find_if(this->renderPassProxies.begin(), this->renderPassProxies.end(), [&](TRenderPassProxy &renderPassProxyItem) {
        std::vector<Turbo::Render::TSubpass> render_subpasses = renderPass.GetSubpasses();
        Turbo::Core::TRenderPass *core_render_pass = renderPassProxyItem.renderPass;
        if (core_render_pass != nullptr)
        {
            std::vector<Turbo::Core::TAttachment> core_attachments = core_render_pass->GetAttachments();
            std::vector<Turbo::Core::TSubpass> core_subpasses = core_render_pass->GetSubpasses();

            size_t render_subpass_count = render_subpasses.size();
            size_t core_subpass_count = core_subpasses.size();
            if (render_subpass_count == /*<=*/core_subpass_count) // TODO:try to test <=, if available
            {
                for (uint32_t subpass_index = 0; subpass_index < render_subpass_count; subpass_index++)
                {
                    Turbo::Core::TSubpass &core_subpass = core_subpasses[subpass_index];
                    Turbo::Render::TSubpass &render_subpass = render_subpasses[subpass_index];

                    std::vector<VkAttachmentReference> *core_color_attachment_references = core_subpass.GetColorAttachmentReferences();
                    std::vector<VkAttachmentReference> *core_input_attachment_references = core_subpass.GetInputAttachmentReferences();
                    VkAttachmentReference *core_depth_stencil_attachment_reference = core_subpass.GetDepthStencilAttachmentReference();

                    std::vector<Turbo::Core::TAttachment> core_color_attachments;
                    for (VkAttachmentReference &core_color_attachment_item : *core_color_attachment_references)
                    {
                        core_color_attachments.push_back(core_attachments[core_color_attachment_item.attachment]);
                    }

                    std::vector<Turbo::Core::TAttachment> core_input_attachments;
                    for (VkAttachmentReference &core_input_attachment_item : *core_input_attachment_references)
                    {
                        core_input_attachments.push_back(core_attachments[core_input_attachment_item.attachment]);
                    }

                    // Use vector to storage depth stencil the purpose is to determine whether DepthStencilAttachment exists
                    // usually we will only have one DepthStencilAttachment or without DepthStencilAttachment
                    std::vector<Turbo::Core::TAttachment> core_depth_stencil_attachments;
                    if (core_depth_stencil_attachment_reference->attachment != UINT32_MAX)
                    {
                        core_depth_stencil_attachments.push_back(core_attachments[core_depth_stencil_attachment_reference->attachment]);
                    }

                    std::vector<Turbo::Render::TColorImage> render_color_attachments = render_subpass.GetColorAttachments();
                    std::vector<Turbo::Render::TImage> render_input_attachments = render_subpass.GetInputAttachments();
                    Turbo::Render::TDepthStencilImage render_depth_stencil_attachment = render_subpass.GetDepthStencilAttachment();

                    // To Compare Core:: and Render::
                    uint32_t core_color_attachments_count = core_color_attachments.size();
                    uint32_t core_input_attachments_count = core_input_attachments.size();
                    uint32_t core_depth_stencil_attachments_count = core_depth_stencil_attachments.size();

                    uint32_t render_color_attachments_count = render_color_attachments.size();
                    uint32_t render_input_attachments_count = render_input_attachments.size();
                    uint32_t render_depth_stencil_attachment_count = render_depth_stencil_attachment.IsValid() ? 1 : 0;

                    if (core_color_attachments_count < render_color_attachments_count)
                    {
                        return false;
                    }

                    if (core_input_attachments_count < render_input_attachments_count)
                    {
                        return false;
                    }

                    if (core_depth_stencil_attachments_count < render_depth_stencil_attachment_count)
                    {
                        return false;
                    }

                    // TODO: try to compare attachments between Core and Render
                    /*
                    Vulkan Spec: Two attachment references are compatible if they have matching format and sample count
                    {
                        Attachment A;
                        Attachment B;

                        A == B
                        {
                            A.format == B.format;
                            A.sample == B.sample;
                        }
                    }

                    Vulkan Spec: Two arrays of attachment references are compatible if all corresponding pairs of attachments are
                    compatible. If the arrays are of different lengths, attachment references not present in the smaller
                    array are treated as VK_ATTACHMENT_UNUSED
                    {
                        std::vector<Attachment> A;
                        std::vector<Attachment> B;

                        A.[0][1][2][3][4][5][6][7][8][9]
                        B.[0][1][2][3][4][5][/][/][/][/]

                        A == B
                        {
                            A.[0] == B.[0];
                            A.[1] == B.[1];
                            A.[2] == B.[2];
                            A.[3] == B.[3];
                            A.[4] == B.[4];
                            A.[5] == B.[5];
                            A.[6] == B.[/];
                            A.[7] == B.[/];
                            A.[8] == B.[/];
                            A.[9] == B.[/];
                        }
                    }

                    Vulkan Spec: Two render passes are compatible if their corresponding color, input, resolve, and depth/stencil
                    attachment references are compatible and if they are otherwise identical except for:
                    {
                        RenderPass A;
                        RenderPass B;

                        A == B
                        {
                            A.colors == B.colors
                        }
                    }
                    */

                    auto test = core_color_attachments[0];
                }
            }
        }

        return false;
    });

    if (find_result != this->renderPassProxies.end())
    {
        return *find_result;
    }

    return TRenderPassProxy();
}

Turbo::Render::TRenderPassPool::TRenderPassProxy Turbo::Render::TRenderPassPool::Allocate(Turbo::Render::TRenderPass &renderPass)
{
    // TODO:return a valid TRenderPassProxy
    // TODO:find a valid RenderPassProxy
    // TODO:if not found create a new RenderPassProxy/RenderPass
    // TODO:if found return what we want
    TRenderPassProxy find_render_pass_proxy = this->Find(renderPass);
    if (find_render_pass_proxy.IsValid())
    {
        return find_render_pass_proxy;
    }

    // create a new RenderPass/TRenderPassProxy
    this->renderPassProxies.push_back(TRenderPassProxy());
    size_t render_pass_index = this->renderPassProxies.size() - 1;
    this->renderPassProxies[render_pass_index].Create(renderPass);
    return this->renderPassProxies[render_pass_index];
}

void Turbo::Render::TRenderPassPool::Free(Turbo::Render::TRenderPass &renderPass)
{
    // TODO:let renderPass into pool
    // this->renderPassProxies[x].Destroy();
}

Turbo::Render::TContext::TContext()
{
    // check Vulkan support
    if (!Turbo::Core::TInstance::IsSupportVulkan())
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Render::TContext::TContext()", "Unsupport Vulkan, please try to update your GPU drive or install Vulkan Runtime");
    }

    Turbo::Core::TVersion support_vulkan_version = Turbo::Core::TVulkanLoader::Instance()->GetVulkanVersion();

    // temp instance for collect information
    Turbo::Core::TInstance *temp_instance = new Turbo::Core::TInstance();
    Turbo::Core::TVersion temp_vulkan_version = temp_instance->GetVulkanVersion();
    Turbo::Core::TPhysicalDevice *temp_best_physical_device = temp_instance->GetBestPhysicalDevice();

    if (temp_best_physical_device->GetVkPhysicalDevice() != VK_NULL_HANDLE)
    {
        if (temp_best_physical_device->GetDeviceApiVersion().GetValidVulkanVersion() >= support_vulkan_version.GetValidVulkanVersion())
        {
            Turbo::Core::TVersion instance_support_vulkan_version = support_vulkan_version.GetValidVulkanVersion();
            delete temp_instance;
            temp_instance = new Turbo::Core::TInstance(nullptr, nullptr, &instance_support_vulkan_version);
        }
        else
        {
            Turbo::Core::TVersion device_support_vulkan_version = temp_best_physical_device->GetDeviceApiVersion().GetValidVulkanVersion();
            delete temp_instance;
            temp_instance = new Turbo::Core::TInstance(nullptr, nullptr, &device_support_vulkan_version);
        }
    }

    Turbo::Core::TVersion instance_support_version = temp_instance->GetVulkanVersion();
    std::vector<Turbo::Core::TLayerInfo> instance_support_layers = temp_instance->GetSupportLayers();
    std::vector<Turbo::Core::TExtensionInfo> instance_support_extensions = temp_instance->GetSupportExtensions();

    delete temp_instance;

    Turbo::Core::TLayerInfo khronos_validation_layer;
    for (Turbo::Core::TLayerInfo &layer : instance_support_layers)
    {
        if (layer.GetLayerType() == Turbo::Core::TLayerType::VK_LAYER_KHRONOS_VALIDATION)
        {
            khronos_validation_layer = layer;
            break;
        }
    }

    std::vector<Turbo::Core::TLayerInfo> enable_instance_layer;
    if (khronos_validation_layer.GetLayerType() != Turbo::Core::TLayerType::UNDEFINED)
    {
        enable_instance_layer.push_back(khronos_validation_layer);
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_instance_extensions;

    Turbo::Core::TExtensionInfo vk_khr_surface_extension;
    for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SURFACE)
        {
            vk_khr_surface_extension = extension;
            break;
        }
    }

    if (vk_khr_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_surface_extension);
    }

#if defined(TURBO_PLATFORM_WINDOWS)
    Turbo::Core::TExtensionInfo vk_khr_win32_surface_extension;
    for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SURFACE)
        {
            vk_khr_win32_surface_extension = extension;
            break;
        }
    }

    if (vk_khr_win32_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_win32_surface_extension);
    }
#elif defined(TURBO_PLATFORM_LINUX)
    Turbo::Core::TExtensionInfo vk_khr_wayland_surface_extension;
    Turbo::Core::TExtensionInfo vk_khr_xcb_surface_extension;
    Turbo::Core::TExtensionInfo vk_khr_xlib_surface_extension;

    for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
    {
        if (vk_khr_wayland_surface_extension.GetExtensionType() == Turbo::Core::TExtensionType::UNDEFINED && extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_WAYLAND_SURFACE)
        {
            vk_khr_wayland_surface_extension = extension;
        }
        else if (vk_khr_xcb_surface_extension.GetExtensionType() == Turbo::Core::TExtensionType::UNDEFINED && extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XCB_SURFACE)
        {
            vk_khr_xcb_surface_extension = extension;
        }
        else if (vk_khr_xlib_surface_extension.GetExtensionType() == Turbo::Core::TExtensionType::UNDEFINED && extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XLIB_SURFACE)
        {
            vk_khr_xlib_surface_extension = extension;
        }
    }

    if (vk_khr_wayland_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_wayland_surface_extension);
    }

    if (vk_khr_xcb_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_xcb_surface_extension);
    }

    if (vk_khr_xlib_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_xlib_surface_extension);
    }
#else
    throw Turbo::Core::TException(Turbo::Core::TResult::UNIMPLEMENTED, "Turbo::Render::TContext::TContext()", "Turbo had not implemented this platform");
#endif

    // create instance
    this->instance = new Turbo::Core::TInstance(&enable_instance_layer, &enable_instance_extensions, &instance_support_version);
    std::cout << "Support Vulkan Version:" << support_vulkan_version.ToString() << std::endl;
    std::cout << "Turbo Vulkan Version:" << this->instance->GetVulkanVersion().ToString() << std::endl;

    this->physicalDevice = this->instance->GetBestPhysicalDevice();

    std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = this->physicalDevice->GetSupportExtensions();

    Turbo::Core::TExtensionInfo vk_khr_swapchain_extension;
    for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
        {
            vk_khr_swapchain_extension = extension;
            break;
        }
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    if (vk_khr_swapchain_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_device_extensions.push_back(vk_khr_swapchain_extension);
    }

    VkPhysicalDeviceFeatures vk_physical_device_features = {VK_FALSE};
    vk_physical_device_features.sampleRateShading = VK_TRUE;
    this->device = new Turbo::Core::TDevice(this->physicalDevice, nullptr, &enable_device_extensions, &vk_physical_device_features);
    this->graphicsQueue = this->device->GetBestGraphicsQueue();

    this->commandBufferPool = new Turbo::Core::TCommandBufferPool(this->graphicsQueue);
    this->currentCommandBuffer.commandBuffer = this->commandBufferPool->Allocate();
    this->currentCommandBuffer.fence = new Turbo::Core::TFence(this->device);
    this->currentCommandBuffer.commandBuffer->Begin();
}

Turbo::Render::TContext::~TContext()
{
    if (this->currentCommandBuffer.commandBuffer != nullptr)
    {
        this->commandBufferPool->Free(this->currentCommandBuffer.commandBuffer);
        this->currentCommandBuffer.commandBuffer = nullptr;
    }

    if (this->currentCommandBuffer.fence != nullptr)
    {
        delete this->currentCommandBuffer.fence;
        this->currentCommandBuffer.fence = nullptr;
    }

    delete this->commandBufferPool;
    this->commandBufferPool = nullptr;
    this->graphicsQueue = nullptr;
    delete this->device;
    this->physicalDevice = nullptr;
    delete this->instance;
}

Turbo::Core::TImage *Turbo::Render::TContext::CreateImage(const TImage::Descriptor &descriptor)
{
    TImageCreateFlags image_create_flags = descriptor.flags;
    TFormat format = descriptor.format;
    uint32_t width = descriptor.width;
    uint32_t height = descriptor.height;
    uint32_t depth = descriptor.depth;
    uint32_t layers = descriptor.layers;
    uint32_t mip_levels = descriptor.mipLevels;
    TImageUsages usages = descriptor.usages;
    TDomain domain = descriptor.domain;

    if (width == 0 && height == 0 && depth == 0)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Core::TImage *Turbo::Render::TContext::CreateImage(const TImage::Descriptor &descriptor)", "Try to create image, but all three dimensions are zero");
    }

    Turbo::Core::TDevice *device = this->device;

    VkImageCreateFlags vk_image_create_flags = 0;
    if ((image_create_flags & Turbo::Render::TImageCreateFlagBits::CUBE) == Turbo::Render::TImageCreateFlagBits::CUBE)
    {
        vk_image_create_flags |= VkImageCreateFlagBits::VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    }

    Turbo::Core::TImageType type = Turbo::Core::TImageType::DIMENSION_3D;
    if (depth == 1)
    {
        type = Turbo::Core::TImageType::DIMENSION_2D;
    }
    if (height == 1)
    {
        type = Turbo::Core::TImageType::DIMENSION_1D;
    }

    Turbo::Core::TFormatType format_type = static_cast<Turbo::Core::TFormatType>(format);

    Turbo::Core::TSampleCountBits sample_count_bits = Turbo::Core::TSampleCountBits::SAMPLE_1_BIT;

    Turbo::Core::TImageTiling image_tiling = Turbo::Core::TImageTiling::OPTIMAL;

    Turbo::Core::TImageUsages image_usages = usages;

    Turbo::Core::TMemoryFlags memory_flags = Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY;

    if ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
    {
        image_tiling = Turbo::Core::TImageTiling::LINEAR;
    }
    else
    {
        image_tiling = Turbo::Core::TImageTiling::OPTIMAL;
    }

    // GPU only
    if (((domain & Turbo::Render::TDomainBits::CPU) != Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) == Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY;
    }
    // Staging copy for upload
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_SRC) == Turbo::Render::TImageUsageBits::TRANSFER_SRC) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) != Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE;
    }
    // Readback
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_DST) == Turbo::Render::TImageUsageBits::TRANSFER_DST) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) != Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_RANDOM;
    }
    // Advanced data uploading(Both CPU and GPU domain can access)
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_DST) == Turbo::Render::TImageUsageBits::TRANSFER_DST) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) == Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE | Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_ALLOW_TRANSFER_INSTEAD;
    }

    Turbo::Core::TImageLayout layout = Turbo::Core::TImageLayout::UNDEFINED;

    return new Turbo::Core::TImage(this->device, vk_image_create_flags, type, format_type, width, height, depth, mip_levels, layers, sample_count_bits, image_tiling, image_usages, memory_flags, layout);
}

void Turbo::Render::TContext::DestroyImage(Turbo::Core::TImage *image)
{
    if (image != nullptr && image->GetVkImage() != VK_NULL_HANDLE)
    {
        delete image;
    }
}

Turbo::Core::TBuffer *Turbo::Render::TContext::CreateBuffer(const TBuffer::Descriptor &descriptor)
{
    TImageUsages usages = descriptor.usages;
    uint32_t size = descriptor.size;
    TDomain domain = descriptor.domain;

    Turbo::Core::TBufferUsages buffer_usages = usages;
    Turbo::Core::TMemoryFlags memory_flags = Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY;

    // GPU only
    if (((domain & Turbo::Render::TDomainBits::CPU) != Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) == Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY;
    }
    // Staging copy for upload
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_SRC) == Turbo::Render::TImageUsageBits::TRANSFER_SRC) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) != Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE;
    }
    // Readback
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_DST) == Turbo::Render::TImageUsageBits::TRANSFER_DST) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) != Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_RANDOM;
    }
    // Advanced data uploading(Both CPU and GPU domain can access)
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_DST) == Turbo::Render::TImageUsageBits::TRANSFER_DST) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) == Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE | Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_ALLOW_TRANSFER_INSTEAD;
    }

    Turbo::Core::TImageUsages image_usages = usages;

    return new Turbo::Core::TBuffer(this->device, 0, buffer_usages, memory_flags, size);
}

void Turbo::Render::TContext::DestroyBuffer(Turbo::Core::TBuffer *buffer)
{
    if (buffer != nullptr && buffer->GetVkBuffer() != VK_NULL_HANDLE)
    {
        delete buffer;
    }
}

Turbo::Core::TCommandBuffer *Turbo::Render::TContext::AllocateCommandBuffer()
{
    return this->commandBufferPool->Allocate();
}

void Turbo::Render::TContext::FreeCommandBuffer(Turbo::Core::TCommandBuffer *commandBuffer)
{
    this->commandBufferPool->Free(commandBuffer);
}

void Turbo::Render::TContext::BeginRenderPass(const Turbo::FrameGraph::TRenderPass &renderPass)
{
    // TODO: convert Turbo::FrameGraph::TRenderPass to Turbo::Render::TRenderPass
}

void Turbo::Render::TContext::BeginRenderPass(const Turbo::Render::TRenderPass &renderPass)
{
    // TODO: create Turbo::Core::TRenderPass and Turbo::Core::TFramebuffer
}

void Turbo::Render::TContext::BindPipeline(const Turbo::Render::TComputePipeline &computePipeline)
{
    // TODO: create Turbo::Core::TComputePipeline if didn't create before
}

void Turbo::Render::TContext::BindPipeline(const Turbo::Render::TGraphicsPipeline &graphicsPipeline)
{
    // TODO: create Turbo::Core::TGraphicsPipeline if didn't create before
}

void Turbo::Render::TContext::Flush()
{
    if (this->currentCommandBuffer.commandBuffer != nullptr && this->currentCommandBuffer.commandBuffer->GetVkCommandBuffer() != VK_NULL_HANDLE)
    {
        this->currentCommandBuffer.commandBuffer->End();

        this->graphicsQueue->Submit(nullptr, nullptr, this->currentCommandBuffer.commandBuffer, this->currentCommandBuffer.fence);
        this->commandBuffers.push_back(this->currentCommandBuffer);

        this->currentCommandBuffer.commandBuffer = this->commandBufferPool->Allocate();
        this->currentCommandBuffer.fence = new Turbo::Core::TFence(this->device);
        this->currentCommandBuffer.commandBuffer->Begin();
    }
}

bool Turbo::Render::TContext::Wait(uint64_t timeout)
{
    Turbo::Core::TFences fences;
    for (Turbo::Render::TCommandBuffer &command_buffer_item : this->commandBuffers)
    {
        fences.Add(command_buffer_item.fence);
    }

    Turbo::Core::TResult result = fences.Wait(timeout);
    if (result == Turbo::Core::TResult::TIMEOUT)
    {
        for (std::vector<Turbo::Render::TCommandBuffer>::iterator it = this->commandBuffers.begin(); it != this->commandBuffers.end();)
        {
            Turbo::Render::TCommandBuffer command_buffer = *it;
            result = command_buffer.fence->Wait(0);

            if (result == Turbo::Core::TResult::SUCCESS)
            {
                delete command_buffer.fence;
                this->commandBufferPool->Free(command_buffer.commandBuffer);
                it = this->commandBuffers.erase(it);
            }
            else
            {
                ++it;
            }
        }
        return false;
    }

    for (Turbo::Render::TCommandBuffer &command_buffer_item : this->commandBuffers)
    {
        delete command_buffer_item.fence;
        this->commandBufferPool->Free(command_buffer_item.commandBuffer);
    }

    this->commandBuffers.clear();
    return true;
}

Turbo::Core::TInstance *Turbo::Render::TContext::GetInstance()
{
    return this->instance;
}

Turbo::Core::TPhysicalDevice *Turbo::Render::TContext::GetPhysicalDevice()
{
    return this->physicalDevice;
}

Turbo::Core::TDevice *Turbo::Render::TContext::GetDevice()
{
    return this->device;
}

Turbo::Core::TDeviceQueue *Turbo::Render::TContext::GetDeviceQueue()
{
    return this->graphicsQueue;
}

Turbo::Render::TCommandBuffer Turbo::Render::TContext::GetCommandBuffer()
{
    return this->currentCommandBuffer;
}
