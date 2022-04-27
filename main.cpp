#include "TDevice.h"
#include "TDeviceQueue.h"
#include "TEngine.h"
#include "TPhysicalDevice.h"
#include "TVulkanAllocator.h"

#include "TBuffer.h"
#include "TCommandBuffer.h"
#include "TCommandBufferPool.h"
#include "TImage.h"
#include "TImageView.h"

#include "TShader.h"

#include "TAttachment.h"
#include "TPipeline.h"
#include "TRenderPass.h"
#include "TSubpass.h"

#include "TDescriptorPool.h"
#include "TDescriptorSet.h"
#include "TDescriptorSetLayout.h"
#include "TFramebuffer.h"

#include "TFence.h"
#include "TSemaphore.h"

#include <fstream>

bool memory_type_from_properties(VkPhysicalDeviceMemoryProperties &physcialDeviceMemoryProperties, uint32_t typeBits, VkFlags requirements_mask, uint32_t *typeIndex)
{
    // Search memtypes to find first index with those properties
    for (uint32_t i = 0; i < physcialDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((typeBits & 1) == 1)
        {
            // Type is available, does it match user properties?
            if ((physcialDeviceMemoryProperties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask)
            {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    // No memory types matched, return failure
    return false;
}

void set_image_layout(VkCommandBuffer &commandBuffer, VkImage image, VkImageAspectFlags aspectMask, VkImageLayout old_image_layout, VkImageLayout new_image_layout, VkPipelineStageFlags src_stages, VkPipelineStageFlags dest_stages)
{
    /* DEPENDS on info.cmd and info.queue initialized */

    VkImageMemoryBarrier image_memory_barrier = {};
    image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    image_memory_barrier.pNext = NULL;
    image_memory_barrier.srcAccessMask = 0;
    image_memory_barrier.dstAccessMask = 0;
    image_memory_barrier.oldLayout = old_image_layout;
    image_memory_barrier.newLayout = new_image_layout;
    image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    image_memory_barrier.image = image;
    image_memory_barrier.subresourceRange.aspectMask = aspectMask;
    image_memory_barrier.subresourceRange.baseMipLevel = 0;
    image_memory_barrier.subresourceRange.levelCount = 1;
    image_memory_barrier.subresourceRange.baseArrayLayer = 0;
    image_memory_barrier.subresourceRange.layerCount = 1;

    switch (old_image_layout)
    {
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        image_memory_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_PREINITIALIZED:
        image_memory_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
        break;

    default:
        break;
    }

    switch (new_image_layout)
    {
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        image_memory_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        image_memory_barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        break;

    default:
        break;
    }

    vkCmdPipelineBarrier(commandBuffer, src_stages, dest_stages, 0, 0, NULL, 0, NULL, 1, &image_memory_barrier);
}

struct Vertex
{
    float posX, posY, posZ, posW; // Position data
    float r, g, b, a;             // Color
};

#define XYZ1(_x_, _y_, _z_) (_x_), (_y_), (_z_), 1.f
#define RGB(_x_, _y_, _z_) (_x_), (_y_), (_z_)

static const Vertex VERTEX_DATA[] = {
    {XYZ1(0.0f, -0.5f, 0), RGB(1.f, 0.f, 0.f)},
    {XYZ1(0.5f, 0.5f, 0), RGB(0.f, 1.f, 0.f)},
    {XYZ1(-0.5f, 0.5f, 0), RGB(0.f, 0.f, 1.f)},
};

const std::string VERT_SHADER_STR = "#version 450 core\n"
                                    "layout (set = 0, binding = 0) uniform bufferVals {\n"
                                    "    float scale;\n"
                                    "} myBufferVals;\n"
                                    "layout (location = 0) in vec4 pos;\n"
                                    "layout (location = 1) in vec4 inColor;\n"
                                    "layout (location = 0) out vec4 outColor;\n"
                                    "void main() {\n"
                                    "   outColor = inColor;\n"
                                    "   gl_Position = myBufferVals.scale * pos;\n"
                                    "}\n";

const std::string FRAG_SHADER_STR = "#version 450 core\n"
                                    "layout (location = 0) in vec4 color;\n"
                                    "layout (location = 0) out vec4 outColor;\n"
                                    "void main() {\n"
                                    "	outColor = color;\n"
                                    "}\n";

int main()
{
    float scale = 1.0f;

    Turbo::Core::TEngine engine;

    Turbo::Core::TLayerInfo khronos_validation;
    std::vector<Turbo::Core::TLayerInfo> support_layers = engine.GetInstance().GetSupportLayers();
    for (Turbo::Core::TLayerInfo &layer : support_layers)
    {
        if (layer.GetLayerType() == Turbo::Core::TLayerType::VK_LAYER_KHRONOS_VALIDATION)
        {
            khronos_validation = layer;
            break;
        }
    }

    std::vector<Turbo::Core::TLayerInfo> enable_layer;
    enable_layer.push_back(khronos_validation);

    Turbo::Core::TInstance *instance = new Turbo::Core::TInstance(&enable_layer);
    Turbo::Core::TPhysicalDevice *physical_device = instance->GetBestPhysicalDevice();
    Turbo::Core::TDevice *device = new Turbo::Core::TDevice(physical_device);
    Turbo::Core::TDeviceQueue *queue = device->GetBestGraphicsQueue();

    Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
    Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();

    Turbo::Core::TBuffer *scale_buffer = new Turbo::Core::TBuffer(device, 0, VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(float));
    void *scale_ptr = scale_buffer->Map();
    memcpy(scale_ptr, &scale, sizeof(scale));
    scale_buffer->Unmap();

    Turbo::Core::TBuffer *vertex_buffer = new Turbo::Core::TBuffer(device, 0, VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(VERTEX_DATA));
    void *vertex_buffer_ptr = vertex_buffer->Map();
    memcpy(vertex_buffer_ptr, VERTEX_DATA, sizeof(VERTEX_DATA));
    vertex_buffer->Unmap();

    Turbo::Core::TImage *color_image = new Turbo::Core::TImage(device, 0, VkImageType::VK_IMAGE_TYPE_2D, Turbo::Core::TFormatInfo(Turbo::Core::TFormatType::B8G8R8A8_SRGB), 500, 500, 1, 1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED);
    Turbo::Core::TImageView *color_image_view = new Turbo::Core::TImageView(color_image, VkImageViewType::VK_IMAGE_VIEW_TYPE_2D, color_image->GetFormat(), VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);

    Turbo::Core::TImage *depth_image = new Turbo::Core::TImage(device, 0, VkImageType::VK_IMAGE_TYPE_2D, Turbo::Core::TFormatInfo(Turbo::Core::TFormatType::D32_SFLOAT), 500, 500, 1, 1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED);
    Turbo::Core::TImageView *depth_image_view = new Turbo::Core::TImageView(depth_image, VkImageViewType::VK_IMAGE_VIEW_TYPE_2D, depth_image->GetFormat(), VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1);

    Turbo::Core::TShader *vertex_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::VERTEX, Turbo::Core::TShaderLanguage::GLSL, VERT_SHADER_STR);
    Turbo::Core::TShader *fragment_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::FRAGMENT, Turbo::Core::TShaderLanguage::GLSL, FRAG_SHADER_STR);

    std::cout << vertex_shader->ToString() << std::endl;
    std::cout << fragment_shader->ToString() << std::endl;

    Turbo::Core::TDescriptorSize uniform_buffer_descriptor_size(Turbo::Core::TDescriptorType::UNIFORM_BUFFER, 100);
    std::vector<Turbo::Core::TDescriptorSize> descriptor_sizes;
    descriptor_sizes.push_back(uniform_buffer_descriptor_size);

    Turbo::Core::TDescriptorPool *descriptor_pool = new Turbo::Core::TDescriptorPool(device, 100, descriptor_sizes);

    Turbo::Core::TDescriptorSetLayout *descriptor_set_layout = vertex_shader->GetDescriptorSetLayouts()[0];

    Turbo::Core::TDescriptorSet *descriptor_set = descriptor_pool->Allocate(descriptor_set_layout);
    std::vector<Turbo::Core::TBuffer *> buffers;
    buffers.push_back(scale_buffer);
    descriptor_set->BindData(0, 0, buffers);

    Turbo::Core::TSubpass subpass;
    subpass.AddColorAttachmentReference(0, VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    subpass.SetDepthStencilAttachmentReference(1, VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    Turbo::Core::TAttachment color_attachment(color_image->GetFormat().GetVkFormat(), VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR, VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE, VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE, VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED, VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    Turbo::Core::TAttachment depth_attachment(VkFormat::VK_FORMAT_D32_SFLOAT, VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR, VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE, VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE, VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED, VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    std::vector<Turbo::Core::TAttachment> attachemts;
    attachemts.push_back(color_attachment);
    attachemts.push_back(depth_attachment);

    std::vector<Turbo::Core::TSubpass> subpasses;
    subpasses.push_back(subpass);
    Turbo::Core::TRenderPass *render_pass = new Turbo::Core::TRenderPass(device, Turbo::Core::TPipelineType::Graphics, attachemts, subpasses);

    std::vector<Turbo::Core::TImageView *> image_views;
    image_views.push_back(color_image_view);
    image_views.push_back(depth_image_view);

    Turbo::Core::TFramebuffer *frame_buffer = new Turbo::Core::TFramebuffer(render_pass, image_views);

    VkVertexInputBindingDescription vertex_input_binding_description;
    vertex_input_binding_description.binding = 0;
    vertex_input_binding_description.stride = sizeof(VERTEX_DATA[0]);
    vertex_input_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription vertex_position_attribute;
    vertex_position_attribute.location = 0;
    vertex_position_attribute.binding = 0;
    vertex_position_attribute.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    vertex_position_attribute.offset = 0;

    VkVertexInputAttributeDescription vertex_color_attribute;
    vertex_color_attribute.location = 1;
    vertex_color_attribute.binding = 0;
    vertex_color_attribute.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    vertex_color_attribute.offset = 16;

    std::vector<VkVertexInputBindingDescription> vk_vertex_input_binding_descriptions{vertex_input_binding_description};

    std::vector<VkVertexInputAttributeDescription> vertex_input_attribute_descriptions{vertex_position_attribute, vertex_color_attribute};

    Turbo::Core::TViewport viewport(0, 0, 500, 500, 0, 1);
    Turbo::Core::TScissor scissor(0, 0, 500, 500);

    std::vector<Turbo::Core::TViewport> viewports;
    viewports.push_back(viewport);

    std::vector<Turbo::Core::TScissor> scissors;
    scissors.push_back(scissor);

    std::vector<Turbo::Core::TShader *> shaders{vertex_shader, fragment_shader};
    Turbo::Core::TPipeline *pipeline = new Turbo::Core::TPipeline(render_pass, 0, Turbo::Core::TPipelineType::Graphics, VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false, vk_vertex_input_binding_descriptions, vertex_input_attribute_descriptions, viewports, scissors, false, false, VkPolygonMode::VK_POLYGON_MODE_FILL, VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT, VkFrontFace::VK_FRONT_FACE_CLOCKWISE, false, 0, 0, 0, 1, shaders);

    std::vector<Turbo::Core::TDescriptorSet *> descriptor_sets;
    descriptor_sets.push_back(descriptor_set);

    std::vector<Turbo::Core::TBuffer *> vertex_buffers;
    vertex_buffers.push_back(vertex_buffer);

    command_buffer->Begin();
    command_buffer->BeginRenderPass(render_pass, frame_buffer);
    command_buffer->BindPipeline(pipeline);
    command_buffer->BindDescriptorSets(pipeline, 0, descriptor_sets);
    command_buffer->BindVertexBuffers(vertex_buffers);
    command_buffer->SetViewport(viewports);
    command_buffer->SetScissor(scissors);
    command_buffer->Draw(3, 1, 0, 0);
    command_buffer->EndRenderPass();
    command_buffer->End();

    Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
    std::vector<Turbo::Core::TSemaphore *> wait_semaphores;
    std::vector<Turbo::Core::TSemaphore *> signal_semaphores;

    queue->Submit(wait_semaphores, signal_semaphores, command_buffer, fence);

    fence->WaitUntil();

    {
        VkResult res = VkResult::VK_ERROR_UNKNOWN;
        VkImageCreateInfo image_create_info_ = {};
        image_create_info_.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info_.pNext = NULL;
        image_create_info_.imageType = VK_IMAGE_TYPE_2D;
        image_create_info_.format = color_image->GetFormat().GetVkFormat();
        image_create_info_.extent.width = color_image->GetWidth();
        image_create_info_.extent.height = color_image->GetHeight();
        image_create_info_.extent.depth = 1;
        image_create_info_.mipLevels = 1;
        image_create_info_.arrayLayers = 1;
        image_create_info_.samples = VK_SAMPLE_COUNT_1_BIT;
        image_create_info_.tiling = VK_IMAGE_TILING_LINEAR;
        image_create_info_.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_create_info_.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        image_create_info_.queueFamilyIndexCount = 0;
        image_create_info_.pQueueFamilyIndices = NULL;
        image_create_info_.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        image_create_info_.flags = 0;

        VkImage mappableImage = VK_NULL_HANDLE;
        res = vkCreateImage(device->GetVkDevice(), &image_create_info_, nullptr, &mappableImage);

        VkMemoryRequirements mem_reqs;

        vkGetImageMemoryRequirements(device->GetVkDevice(), mappableImage, &mem_reqs);

        VkPhysicalDeviceMemoryProperties physical_device_memory_properties = {};
        vkGetPhysicalDeviceMemoryProperties(physical_device->GetVkPhysicalDevice(), &physical_device_memory_properties);

        uint32_t _index = UINT32_MAX;
        bool is_success = memory_type_from_properties(physical_device_memory_properties, mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &_index);

        VkMemoryAllocateInfo mem_alloc = {};
        mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        mem_alloc.pNext = NULL;
        mem_alloc.allocationSize = mem_reqs.size;
        mem_alloc.memoryTypeIndex = _index;

        VkDeviceMemory mappableMemory;
        res = vkAllocateMemory(device->GetVkDevice(), &mem_alloc, NULL, &(mappableMemory));

        res = vkBindImageMemory(device->GetVkDevice(), mappableImage, mappableMemory, 0);

        VkCommandBufferAllocateInfo commandBuffer_Allocate_Info;
        commandBuffer_Allocate_Info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBuffer_Allocate_Info.pNext = nullptr;
        commandBuffer_Allocate_Info.commandBufferCount = 1;
        commandBuffer_Allocate_Info.commandPool = command_pool->GetVkCommandPool();
        commandBuffer_Allocate_Info.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        VkCommandBuffer _commandBuffer;
        res = vkAllocateCommandBuffers(device->GetVkDevice(), &commandBuffer_Allocate_Info, &_commandBuffer);

        VkCommandBufferBeginInfo cmd_buf_info = {};
        cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cmd_buf_info.pNext = NULL;
        cmd_buf_info.flags = 0;
        cmd_buf_info.pInheritanceInfo = NULL;

        res = vkBeginCommandBuffer(_commandBuffer, &cmd_buf_info);

        set_image_layout(_commandBuffer, mappableImage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

        set_image_layout(_commandBuffer, color_image->GetVkImage(), VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

        VkImageCopy copy_region;
        copy_region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copy_region.srcSubresource.mipLevel = 0;
        copy_region.srcSubresource.baseArrayLayer = 0;
        copy_region.srcSubresource.layerCount = 1;
        copy_region.srcOffset.x = 0;
        copy_region.srcOffset.y = 0;
        copy_region.srcOffset.z = 0;
        copy_region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copy_region.dstSubresource.mipLevel = 0;
        copy_region.dstSubresource.baseArrayLayer = 0;
        copy_region.dstSubresource.layerCount = 1;
        copy_region.dstOffset.x = 0;
        copy_region.dstOffset.y = 0;
        copy_region.dstOffset.z = 0;
        copy_region.extent.width = color_image->GetWidth();
        copy_region.extent.height = color_image->GetHeight();
        copy_region.extent.depth = 1;

        vkCmdCopyImage(_commandBuffer, color_image->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, mappableImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);

        set_image_layout(_commandBuffer, mappableImage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT);

        vkEndCommandBuffer(_commandBuffer);

        VkSubmitInfo submit_info = {};
        submit_info.pNext = NULL;
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = 0;
        submit_info.pWaitSemaphores = NULL;
        submit_info.pWaitDstStageMask = NULL;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &_commandBuffer;
        submit_info.signalSemaphoreCount = 0;
        submit_info.pSignalSemaphores = NULL;

        VkFenceCreateInfo fenceInfo;
        VkFence cmdFence;
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.pNext = NULL;
        fenceInfo.flags = 0;
        vkCreateFence(device->GetVkDevice(), &fenceInfo, NULL, &cmdFence);

        vkQueueSubmit(queue->GetVkQueue(), 1, &submit_info, cmdFence);

        do
        {
            res = vkWaitForFences(device->GetVkDevice(), 1, &cmdFence, VK_TRUE, UINT64_MAX);
        } while (res == VK_TIMEOUT);

        vkDestroyFence(device->GetVkDevice(), cmdFence, NULL);

        std::string filename;

        filename.append("./VulkanImage");
        filename.append(".ppm");

        VkImageSubresource subres = {};
        subres.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subres.mipLevel = 0;
        subres.arrayLayer = 0;
        VkSubresourceLayout sr_layout;
        vkGetImageSubresourceLayout(device->GetVkDevice(), mappableImage, &subres, &sr_layout);

        char *ptr;
        res = vkMapMemory(device->GetVkDevice(), mappableMemory, 0, mem_reqs.size, 0, (void **)&ptr);

        ptr += sr_layout.offset;
        std::ofstream file(filename.c_str(), std::ios::binary);

        file << "P6\n";
        file << color_image->GetWidth() << " ";
        file << color_image->GetHeight() << "\n";
        file << 255 << "\n";

        int x = 0;
        int y = 0;

        for (y = 0; y < color_image->GetHeight(); y++)
        {
            const int *row = (const int *)ptr;
            int swapped;

            if (color_image->GetFormat().GetVkFormat() == VK_FORMAT_B8G8R8A8_UNORM || color_image->GetFormat().GetVkFormat() == VK_FORMAT_B8G8R8A8_SRGB)
            {
                for (x = 0; x < color_image->GetWidth(); x++)
                {
                    swapped = (*row & 0xff00ff00) | (*row & 0x000000ff) << 16 | (*row & 0x00ff0000) >> 16;
                    file.write((char *)&swapped, 3);
                    row++;
                }
            }
            else if (color_image->GetFormat().GetVkFormat() == VK_FORMAT_R8G8B8A8_UNORM)
            {
                for (x = 0; x < color_image->GetWidth(); x++)
                {
                    file.write((char *)row, 3);
                    row++;
                }
            }
            else
            {
                printf("Unrecognized image format - will not write image files");
                break;
            }

            ptr += sr_layout.rowPitch;
        }

        file.close();
        vkUnmapMemory(device->GetVkDevice(), mappableMemory);
        vkDestroyImage(device->GetVkDevice(), mappableImage, NULL);
        vkFreeMemory(device->GetVkDevice(), mappableMemory, NULL);
    }

    delete fence;

    delete pipeline;

    delete frame_buffer;

    delete render_pass;

    descriptor_pool->Free(descriptor_set);

    delete descriptor_pool;
    delete vertex_shader;
    delete fragment_shader;
    delete depth_image_view;
    delete depth_image;
    delete color_image_view;
    delete color_image;
    delete vertex_buffer;
    delete scale_buffer;

    command_pool->Free(command_buffer);
    delete command_pool;
    delete device;
    delete instance;

    return 0;
}
