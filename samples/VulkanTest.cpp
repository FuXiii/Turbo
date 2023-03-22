#include "TInstance.h"
#include "core/include/TDevice.h"
#include "core/include/TDeviceQueue.h"
#include "core/include/TEngine.h"
#include "core/include/TFormatInfo.h"
#include "core/include/TPhysicalDevice.h"
#include "core/include/TVulkanAllocator.h"

#include "core/include/TBuffer.h"
#include "core/include/TCommandBuffer.h"
#include "core/include/TCommandBufferPool.h"
#include "core/include/TImage.h"
#include "core/include/TImageView.h"

#include "core/include/TShader.h"

#include "core/include/TAttachment.h"
#include "core/include/TGraphicsPipeline.h"
#include "core/include/TRenderPass.h"
#include "core/include/TSubpass.h"

#include "core/include/TDescriptorPool.h"
#include "core/include/TDescriptorSet.h"
#include "core/include/TDescriptorSetLayout.h"
#include "core/include/TFramebuffer.h"

#include "TFence.h"
#include "TSemaphore.h"

#include <cstddef>
#include <exception>
#include <fstream>

#include <GLFW/glfw3.h>

#include "core/include/TSurface.h"
#include "core/include/TSwapchain.h"

#include <math.h>

#include "core/include/TPipelineDescriptorSet.h"
#include "core/include/TSampler.h"

#include "core/include/TVulkanLoader.h"
#include "vulkan/vulkan_core.h"

#include <memory>
#include <stdio.h>
#include <string.h>

#include <chrono>

#include <core/include/TPipelineCache.h>

class TTimer
{
  private:
    std::chrono::system_clock::time_point start_point;
    std::chrono::system_clock::time_point end_point;

  public:
    TTimer()
    {
        this->start_point = std::chrono::system_clock::now();
        this->end_point = this->start_point;
    }
    ~TTimer() = default;

    void Begin()
    {
        this->start_point = std::chrono::system_clock::now();
    }

    void End()
    {
        this->end_point = std::chrono::system_clock::now();
    }

    double GetDeltaSecond()
    {
        return std::chrono::duration<double>(this->end_point - this->start_point).count();
    }
};

std::string ReadTextFile(const std::string &filename)
{
    std::vector<std::string> data;

    std::ifstream file;

    file.open(filename, std::ios::in);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    return std::string{(std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>())};
}

std::vector<uint8_t> ReadBinaryFile(const std::string &filename, const uint32_t count)
{
    std::vector<uint8_t> data;

    std::ifstream file;

    file.open(filename, std::ios::in | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    uint64_t read_count = count;
    if (count == 0)
    {
        file.seekg(0, std::ios::end);
        read_count = static_cast<uint64_t>(file.tellg());
        file.seekg(0, std::ios::beg);
    }

    data.resize(static_cast<size_t>(read_count));
    file.read(reinterpret_cast<char *>(data.data()), read_count);
    file.close();

    return data;
}

void WriteBinaryFile(const std::string &filename, const uint32_t count, const std::vector<uint8_t> &data)
{
    std::ofstream file;

    file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    uint64_t write_count = count;
    if (count == 0)
    {
        write_count = data.size();
    }

    file.write(reinterpret_cast<const char *>(data.data()), write_count);
    file.close();
}

void PAUSE(const std::string &message)
{
    std::cout << message << std::endl;
    // system("pause");
    std::cin.get();
}

void Test0(Turbo::Core::TDeviceQueue *deviceQueue)
{
    std::cout << "<Test0>" << std::endl;
    Turbo::Core::TDevice *device = deviceQueue->GetDevice();
    Turbo::Core::TPhysicalDevice *physical_device = device->GetPhysicalDevice();

    VkImageCreateInfo vk_image_create_info = {};
    vk_image_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    vk_image_create_info.pNext = nullptr;
    vk_image_create_info.flags = 0;
    vk_image_create_info.imageType = VkImageType::VK_IMAGE_TYPE_2D;
    vk_image_create_info.format = VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
    vk_image_create_info.extent.width = 512;
    vk_image_create_info.extent.height = 512;
    vk_image_create_info.extent.depth = 1;
    vk_image_create_info.mipLevels = 1;
    vk_image_create_info.arrayLayers = 1;
    vk_image_create_info.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
    vk_image_create_info.tiling = VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
    vk_image_create_info.usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    vk_image_create_info.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
    vk_image_create_info.queueFamilyIndexCount = 0;
    vk_image_create_info.pQueueFamilyIndices = nullptr;
    vk_image_create_info.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;

    // GPU only
    VkImage vk_image = VK_NULL_HANDLE;
    VkResult result = Turbo::Core::vkCreateImage(device->GetVkDevice(), &vk_image_create_info, nullptr, &vk_image);
    Turbo::Core::vkDestroyImage(device->GetVkDevice(), vk_image, nullptr);
    std::cout << "======================================== GPU only" << std::endl;
    // staging upload
    vk_image_create_info.usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT;
    vk_image_create_info.tiling = VkImageTiling::VK_IMAGE_TILING_LINEAR;
    result = Turbo::Core::vkCreateImage(device->GetVkDevice(), &vk_image_create_info, nullptr, &vk_image);
    Turbo::Core::vkDestroyImage(device->GetVkDevice(), vk_image, nullptr);
    std::cout << "======================================== staging upload" << std::endl;

    // read back
    vk_image_create_info.usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    vk_image_create_info.tiling = VkImageTiling::VK_IMAGE_TILING_LINEAR;
    result = Turbo::Core::vkCreateImage(device->GetVkDevice(), &vk_image_create_info, nullptr, &vk_image);
    Turbo::Core::vkDestroyImage(device->GetVkDevice(), vk_image, nullptr);
    std::cout << "======================================== read back" << std::endl;

    // advanced upload
    vk_image_create_info.usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    vk_image_create_info.tiling = VkImageTiling::VK_IMAGE_TILING_LINEAR;
    result = Turbo::Core::vkCreateImage(device->GetVkDevice(), &vk_image_create_info, nullptr, &vk_image);
    Turbo::Core::vkDestroyImage(device->GetVkDevice(), vk_image, nullptr);
    std::cout << "======================================== advanced upload" << std::endl;

    Turbo::Core::TImage *temp_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::B8G8R8A8_UNORM, 512, 512, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::LINEAR, Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE);
    delete temp_image;

    std::vector<Turbo::Core::TFormatInfo> support_format = physical_device->GetSupportFormats();

    for (Turbo::Core::TFormatInfo &format_info_item : support_format)
    {
        VkFormat vk_format = format_info_item.GetVkFormat();
        std::cout << "vk_format::" << vk_format << std::endl;
    }

    const std::string vert_shader_str = ReadTextFile("../../asset/shaders/shader_base.vert");
    const std::string fragment_shader_str = ReadTextFile("../../asset/shaders/shader_base.frag");

    Turbo::Core::TVertexShader *vs = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, vert_shader_str);
    Turbo::Core::TFragmentShader *fs = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, fragment_shader_str);

    std::cout << vs->ToString() << std::endl;
    std::cout << fs->ToString() << std::endl;

    delete vs;
    delete fs;
    std::cout << "</Test0>" << std::endl;
}

void Test1(Turbo::Core::TDeviceQueue *deviceQueue)
{
    std::cout << "<Test1>" << std::endl;
    Turbo::Core::TDevice *device = deviceQueue->GetDevice();
    Turbo::Core::TPhysicalDevice *physical_device = device->GetPhysicalDevice();
    Turbo::Core::TInstance *instance = physical_device->GetInstance();

    VkInstance vk_instance = instance->GetVkInstance();
    VkPhysicalDevice vk_physical_device = physical_device->GetVkPhysicalDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkQueue vk_queue = deviceQueue->GetVkQueue();

    Turbo::Core::TPipelineCache *pipeline_cache = new Turbo::Core::TPipelineCache(device);
    VkPipelineCache vk_pipeline_cache = pipeline_cache->GetVkPipelineCache();

    VkResult result = VkResult::VK_ERROR_UNKNOWN;
    {
        size_t pipeline_cache_size = 0;
        result = Turbo::Core::vkGetPipelineCacheData(vk_device, vk_pipeline_cache, &pipeline_cache_size, nullptr);
        if (result != VkResult::VK_SUCCESS)
        {
            std::cout << "!!!Error::Turbo::Core::vkGetPipelineCacheData(...)::GetPipelineCacheData::Size" << std::endl;
        }
        std::cout << "pipeline cache size::" << pipeline_cache_size << std::endl;

        {
            void *pipeline_cache_data = malloc(pipeline_cache_size);
            result = Turbo::Core::vkGetPipelineCacheData(vk_device, vk_pipeline_cache, &pipeline_cache_size, pipeline_cache_data);
            if (result != VkResult::VK_SUCCESS)
            {
                std::cout << "!!!Error::Turbo::Core::vkGetPipelineCacheData(...)::GetPipelineCacheData::Data" << std::endl;
            }

            {
                // FIXME:The Vulkan spec states: dstCache must not appear in the list of source caches
                // result = Turbo::Core::vkMergePipelineCaches(vk_device, vk_pipeline_cache, 1, &vk_pipeline_cache);
                // if (result != VkResult::VK_SUCCESS)
                // {
                //     std::cout << "!!!Error::Turbo::Core::vkMergePipelineCaches(...)" << std::endl;
                // }
            }

            VkPipelineCacheHeaderVersionOne vk_pipeline_cache_header_version_one = {};
            memcpy(&vk_pipeline_cache_header_version_one, pipeline_cache_data, sizeof(VkPipelineCacheHeaderVersionOne));

            uint32_t header_size = vk_pipeline_cache_header_version_one.headerSize;
            std::cout << "Cache header size::" << header_size << std::endl;

            VkPipelineCacheHeaderVersion header_version = vk_pipeline_cache_header_version_one.headerVersion;
            std::cout << "Cache header version::" << header_version << std::endl;

            uint32_t vendor_id = vk_pipeline_cache_header_version_one.vendorID;
            std::cout << "Cache vendor id::" << vendor_id << std::endl;

            uint32_t device_id = vk_pipeline_cache_header_version_one.deviceID;
            std::cout << "Cache device id::" << device_id << std::endl;

            for (uint32_t cache_uuid_index = 0; cache_uuid_index < VK_UUID_SIZE; cache_uuid_index++)
            {
                uint8_t uuid = vk_pipeline_cache_header_version_one.pipelineCacheUUID[cache_uuid_index];
                std::cout << cache_uuid_index << "::UUID::" << uuid << std::endl;
            }

            free(pipeline_cache_data);
        }

        {
            Turbo::Core::TPipelineCacheUUID physcial_device_uuid = physical_device->GetDevicePiplineCacheUUID();
            for (uint32_t cache_uuid_index = 0; cache_uuid_index < VK_UUID_SIZE; cache_uuid_index++)
            {
                uint8_t uuid = physcial_device_uuid.uuid[cache_uuid_index];
                std::cout << cache_uuid_index << "::Physica Device UUID::" << uuid << std::endl;
            }
        }

        {
            uint32_t physcial_device_id = physical_device->GetPhysicalDeviceID();
            std::cout << "Physica Device id::" << physcial_device_id << std::endl;

            uint32_t physcial_device_vendor_id = physical_device->GetVendor().GetVendorID();
            std::cout << "Physica Device vendor id::" << physcial_device_vendor_id << std::endl;
        }
    }

    {
        const std::string vert_shader_str = ReadTextFile("../../asset/shaders/shader_base.vert");
        const std::string fragment_shader_str = ReadTextFile("../../asset/shaders/shader_base.frag");

        Turbo::Core::TVertexShader *vs = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, vert_shader_str);
        Turbo::Core::TFragmentShader *fs = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, fragment_shader_str);

        Turbo::Core::TFormatInfo format_info = physical_device->GetFormatInfo(Turbo::Core::TFormatType::B8G8R8A8_SRGB);
        if (format_info.IsOptimalTilingSupportColorAttachment())
        {
            std::cout << "B8G8R8A8_SRGB support color attachment" << std::endl;
        }
        else
        {
            std::cout << "B8G8R8A8_SRGB Not support color attachment" << std::endl;
        }

        Turbo::Core::TImage *color_attachment_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, 512, 512, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
        Turbo::Core::TImageView *color_attachment_image_view = new Turbo::Core::TImageView(color_attachment_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, color_attachment_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);

        std::vector<Turbo::Core::TAttachment> attachments;
        attachments.push_back({color_attachment_image_view->GetFormat(), Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL});

        Turbo::Core::TSubpass subpass(Turbo::Core::TPipelineType::Graphics);
        subpass.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);

        std::vector<Turbo::Core::TSubpass> subpasses;
        subpasses.push_back(subpass);

        Turbo::Core::TRenderPass *render_pass = new Turbo::Core::TRenderPass(device, attachments, subpasses);
        std::vector<Turbo::Core::TVertexBinding> vb;

        TTimer timer;
        timer.Begin();
        Turbo::Core::TGraphicsPipeline *gp = new Turbo::Core::TGraphicsPipeline(render_pass, 0, vb, vs, fs);
        timer.End();
        std::cout << "Create TGraphicsPipeline without cache use:" << timer.GetDeltaSecond() << "s" << std::endl;

        {
            Turbo::Core::TGraphicsPipeline *_gp = new Turbo::Core::TGraphicsPipeline(pipeline_cache, render_pass, 0, vb, vs, fs);

            size_t _pipeline_cache_size = 0;
            VkResult _result = Turbo::Core::vkGetPipelineCacheData(vk_device, vk_pipeline_cache, &_pipeline_cache_size, nullptr);
            if (result != VkResult::VK_SUCCESS)
            {
                std::cout << "!!!Error::Turbo::Core::vkGetPipelineCacheData(...)::GetPipelineCacheData::Size" << std::endl;
            }
            std::cout << "After first create a pipeline with cache the Pipeline Cache size:" << _pipeline_cache_size << std::endl;
            delete _gp;
        }

        uint32_t create_pipeline_loop_count = 10;
        for (uint32_t create_pipeline_loop_index = 0; create_pipeline_loop_index < create_pipeline_loop_count; create_pipeline_loop_index++)
        {
            {
                uint32_t create_pipeline_count = 32768;
                std::vector<Turbo::Core::TGraphicsPipeline *> gps(create_pipeline_count);
                TTimer timer1;
                timer1.Begin();
                for (uint32_t create_pipeline_index = 0; create_pipeline_index < create_pipeline_count; create_pipeline_index++)
                {
                    Turbo::Core::TGraphicsPipeline *_gp = new Turbo::Core::TGraphicsPipeline(render_pass, 0, vb, vs, fs);
                    gps[create_pipeline_index] = _gp;
                }
                timer1.End();
                std::cout << "Create " << create_pipeline_count << " TGraphicsPipeline without cache use:" << timer1.GetDeltaSecond() << "s" << std::endl;

                for (uint32_t create_pipeline_index = 0; create_pipeline_index < create_pipeline_count; create_pipeline_index++)
                {
                    delete gps[create_pipeline_index];
                }
            }

            {
                uint32_t create_pipeline_count = 32768;
                std::vector<Turbo::Core::TGraphicsPipeline *> gps(create_pipeline_count);
                TTimer timer2;
                timer2.Begin();
                for (uint32_t create_pipeline_index = 0; create_pipeline_index < create_pipeline_count; create_pipeline_index++)
                {
                    Turbo::Core::TGraphicsPipeline *_gp = new Turbo::Core::TGraphicsPipeline(pipeline_cache, render_pass, 0, vb, vs, fs);
                    gps[create_pipeline_index] = _gp;
                }
                timer2.End();
                std::cout << "Create " << create_pipeline_count << " TGraphicsPipeline with cache use:" << timer2.GetDeltaSecond() << "s" << std::endl;

                for (uint32_t create_pipeline_index = 0; create_pipeline_index < create_pipeline_count; create_pipeline_index++)
                {
                    delete gps[create_pipeline_index];
                }
            }

            {
                size_t _pipeline_cache_size = 0;
                VkResult _result = Turbo::Core::vkGetPipelineCacheData(vk_device, vk_pipeline_cache, &_pipeline_cache_size, nullptr);
                if (result != VkResult::VK_SUCCESS)
                {
                    std::cout << "!!!Error::Turbo::Core::vkGetPipelineCacheData(...)::GetPipelineCacheData::Size" << std::endl;
                }
                std::cout << "Current Pipeline Cache size:" << _pipeline_cache_size << std::endl;
            }
        }

        {
            size_t _pipeline_cache_size = 0;
            VkResult _result = Turbo::Core::vkGetPipelineCacheData(vk_device, vk_pipeline_cache, &_pipeline_cache_size, nullptr);
            if (result != VkResult::VK_SUCCESS)
            {
                std::cout << "!!!Error::Turbo::Core::vkGetPipelineCacheData(...)::GetPipelineCacheData::Size" << std::endl;
            }
            std::cout << "After Create a lot of pipeline the Pipeline Cache size:" << _pipeline_cache_size << std::endl;

            void *pipeline_cache_data = malloc(_pipeline_cache_size);
            result = Turbo::Core::vkGetPipelineCacheData(vk_device, vk_pipeline_cache, &_pipeline_cache_size, pipeline_cache_data);
            if (result != VkResult::VK_SUCCESS)
            {
                std::cout << "!!!Error::Turbo::Core::vkGetPipelineCacheData(...)::GetPipelineCacheData::Data" << std::endl;
            }

            VkPipelineCacheHeaderVersionOne vk_pipeline_cache_header_version_one = {};
            memcpy(&vk_pipeline_cache_header_version_one, pipeline_cache_data, sizeof(VkPipelineCacheHeaderVersionOne));

            uint32_t header_size = vk_pipeline_cache_header_version_one.headerSize;
            std::cout << "Cache header size::" << header_size << std::endl;

            VkPipelineCacheHeaderVersion header_version = vk_pipeline_cache_header_version_one.headerVersion;
            std::cout << "Cache header version::" << header_version << std::endl;

            uint32_t vendor_id = vk_pipeline_cache_header_version_one.vendorID;
            std::cout << "Cache vendor id::" << vendor_id << std::endl;

            uint32_t device_id = vk_pipeline_cache_header_version_one.deviceID;
            std::cout << "Cache device id::" << device_id << std::endl;

            for (uint32_t cache_uuid_index = 0; cache_uuid_index < VK_UUID_SIZE; cache_uuid_index++)
            {
                uint8_t uuid = vk_pipeline_cache_header_version_one.pipelineCacheUUID[cache_uuid_index];
                std::cout << cache_uuid_index << "::UUID::" << uuid << std::endl;
            }

            free(pipeline_cache_data);
        }

        Turbo::Core::TPipelineCache *new_pipeline_cache = new Turbo::Core::TPipelineCache(device);
        VkPipelineCache new_vk_pipeline_cache = new_pipeline_cache->GetVkPipelineCache();
        {
            uint32_t create_pipeline_count = 32768;
            std::vector<Turbo::Core::TGraphicsPipeline *> gps(create_pipeline_count);
            TTimer timer2;
            timer2.Begin();
            for (uint32_t create_pipeline_index = 0; create_pipeline_index < create_pipeline_count; create_pipeline_index++)
            {
                Turbo::Core::TGraphicsPipeline *_gp = new Turbo::Core::TGraphicsPipeline(new_pipeline_cache, render_pass, 0, vb, vs, fs, Turbo::Core::TTopologyType::TRIANGLE_FAN, false, false, false, Turbo::Core::TPolygonMode::FILL);
                gps[create_pipeline_index] = _gp;
            }
            timer2.End();
            std::cout << "Create " << create_pipeline_count << " new TGraphicsPipeline with cache use:" << timer2.GetDeltaSecond() << "s" << std::endl;

            for (uint32_t create_pipeline_index = 0; create_pipeline_index < create_pipeline_count; create_pipeline_index++)
            {
                delete gps[create_pipeline_index];
            }
        }

        {
            size_t _pipeline_cache_size = 0;
            VkResult _result = Turbo::Core::vkGetPipelineCacheData(vk_device, new_vk_pipeline_cache, &_pipeline_cache_size, nullptr);
            if (result != VkResult::VK_SUCCESS)
            {
                std::cout << "!!!Error::Turbo::Core::vkGetPipelineCacheData(...)::GetPipelineCacheData::Size" << std::endl;
            }
            std::cout << "after new pipeline the Pipeline Cache size:" << _pipeline_cache_size << std::endl;
        }

        VkPipelineCacheCreateInfo merge_vk_pipeline_cache_create_info = {};
        merge_vk_pipeline_cache_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        merge_vk_pipeline_cache_create_info.pNext = nullptr;
        merge_vk_pipeline_cache_create_info.flags = 0;
        merge_vk_pipeline_cache_create_info.initialDataSize = 0;
        merge_vk_pipeline_cache_create_info.pInitialData = nullptr;
        VkPipelineCache merge_pipeline_cache = VK_NULL_HANDLE;
        Turbo::Core::vkCreatePipelineCache(vk_device, &merge_vk_pipeline_cache_create_info, nullptr, &merge_pipeline_cache);

        {
            std::vector<VkPipelineCache> merge_caches;
            merge_caches.push_back(vk_pipeline_cache);
            merge_caches.push_back(new_vk_pipeline_cache);
            VkResult __result = Turbo::Core::vkMergePipelineCaches(vk_device, merge_pipeline_cache, merge_caches.size(), merge_caches.data());
            if (__result != VkResult::VK_SUCCESS)
            {
                std::cout << "!!!Error::Pipeline Cache merge::Failed" << std::endl;
            }

            {
                size_t _pipeline_cache_size = 0;
                VkResult _result = Turbo::Core::vkGetPipelineCacheData(vk_device, merge_pipeline_cache, &_pipeline_cache_size, nullptr);
                if (result != VkResult::VK_SUCCESS)
                {
                    std::cout << "!!!Error::Turbo::Core::vkGetPipelineCacheData(...)::GetPipelineCacheData::Size" << std::endl;
                }
                std::cout << "after merge pipeline the Pipeline Cache size:" << _pipeline_cache_size << std::endl;
            }
        }

        Turbo::Core::vkDestroyPipelineCache(vk_device, merge_pipeline_cache, nullptr);
        delete new_pipeline_cache;

        delete gp;
        delete render_pass;
        delete color_attachment_image_view;
        delete color_attachment_image;
        delete fs;
        delete vs;
    }

    delete pipeline_cache;
    std::cout << "</Test1>" << std::endl;
}

void Test2(Turbo::Core::TDeviceQueue *deviceQueue)
{
    std::cout << "<Test2>" << std::endl;
    Turbo::Core::TDevice *device = deviceQueue->GetDevice();
    Turbo::Core::TPhysicalDevice *physical_device = device->GetPhysicalDevice();
    Turbo::Core::TInstance *instance = physical_device->GetInstance();

    VkInstance vk_instance = instance->GetVkInstance();
    VkPhysicalDevice vk_physical_device = physical_device->GetVkPhysicalDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkQueue vk_queue = deviceQueue->GetVkQueue();

    Turbo::Core::TPipelineCache *pc = new Turbo::Core::TPipelineCache(device);

    uint32_t header_size = pc->GetHeaderSize();
    Turbo::Core::TPipelineCacheHeaderVersion cache_version = pc->GetHeaderVersion();
    Turbo::Core::TVendorInfo vendor = pc->GetVendor();
    uint32_t device_id = pc->GetDeviceID();
    std::vector<uint8_t> cache_uuid = pc->GetUUID();

    std::string cache_uuid_str;
    for (auto &uuid_item : cache_uuid)
    {
        cache_uuid_str += std::to_string(uuid_item) + " ";
    }

    std::cout << "Cache Size::" << pc->GetSize() << std::endl;
    std::cout << "Cache Header:"
              << "\n"
              << "\tsize:" << header_size << "\n"
              << "\tversion:" << (uint32_t)cache_version << "\n"
              << "\tvendor:" << vendor.ToString() << "\n"
              << "\tdevice id:" << device_id << "\n"
              << "\tuuid:" << cache_uuid_str << std::endl;

    size_t cache_size = pc->GetSize();
    void *cache_data = malloc(cache_size);
    pc->GetData(cache_size, cache_data);

    free(cache_data);
    delete pc;
    std::cout << "</Test2>" << std::endl;
}

void Test3(Turbo::Core::TDeviceQueue *deviceQueue)
{
    std::cout << "<Test3>" << std::endl;
    Turbo::Core::TDevice *device = deviceQueue->GetDevice();
    Turbo::Core::TPhysicalDevice *physical_device = device->GetPhysicalDevice();
    Turbo::Core::TInstance *instance = physical_device->GetInstance();

    VkInstance vk_instance = instance->GetVkInstance();
    VkPhysicalDevice vk_physical_device = physical_device->GetVkPhysicalDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkQueue vk_queue = deviceQueue->GetVkQueue();

    std::cout << "device name:" << physical_device->GetDeviceName() << std::endl;
    std::cout << "driver version:" << physical_device->GetDriverVersion() << std::endl;

    VkPipelineCacheCreateInfo vk_pipeline_cache_create_info = {};
    vk_pipeline_cache_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    vk_pipeline_cache_create_info.pNext = nullptr;
    vk_pipeline_cache_create_info.flags = 0;
    vk_pipeline_cache_create_info.initialDataSize = 0;
    vk_pipeline_cache_create_info.pInitialData = nullptr;

    VkPipelineCache vk_pipeline_cache = VK_NULL_HANDLE;
    VkResult result = device->GetDeviceDriver()->vkCreatePipelineCache(vk_device, &vk_pipeline_cache_create_info, nullptr, &vk_pipeline_cache);
    if (result != VkResult::VK_SUCCESS)
    {
        exit(-1);
    }

    {
        size_t cache_size = 0;
        device->GetDeviceDriver()->vkGetPipelineCacheData(vk_device, vk_pipeline_cache, &cache_size, nullptr);
        std::cout << "Cache size:" << cache_size << std::endl;
    }

    {
        VkPipelineCacheHeaderVersionOne vk_pipeline_cache_header_version_one = {};
        size_t cache_size = sizeof(VkPipelineCacheHeaderVersionOne);
        device->GetDeviceDriver()->vkGetPipelineCacheData(vk_device, vk_pipeline_cache, &cache_size, &vk_pipeline_cache_header_version_one);
        std::cout << "Cache size:" << cache_size << std::endl;
        std::cout << "\t Header size:" << vk_pipeline_cache_header_version_one.headerSize << std::endl;
    }

    {
        size_t cache_size = 0;
        device->GetDeviceDriver()->vkGetPipelineCacheData(vk_device, vk_pipeline_cache, &cache_size, nullptr);
        std::cout << "Cache size:" << cache_size << std::endl;
        void *cache_data = malloc(cache_size);
        device->GetDeviceDriver()->vkGetPipelineCacheData(vk_device, vk_pipeline_cache, &cache_size, cache_data);
        VkPipelineCacheHeaderVersionOne vk_pipeline_cache_header_version_one = {};
        memcpy(&vk_pipeline_cache_header_version_one, cache_data, sizeof(VkPipelineCacheHeaderVersionOne));
        std::cout << "\t Header size:" << vk_pipeline_cache_header_version_one.headerSize << std::endl;
        free(cache_data);
    }

    {
        size_t cache_size = sizeof(VkPipelineCacheHeaderVersionOne);
        void *cache_data = malloc(cache_size);
        device->GetDeviceDriver()->vkGetPipelineCacheData(vk_device, vk_pipeline_cache, &cache_size, cache_data);
        VkPipelineCacheHeaderVersionOne vk_pipeline_cache_header_version_one = {};
        memcpy(&vk_pipeline_cache_header_version_one, cache_data, sizeof(VkPipelineCacheHeaderVersionOne));
        std::cout << "\t Header size:" << vk_pipeline_cache_header_version_one.headerSize << std::endl;
        free(cache_data);
    }

    device->GetDeviceDriver()->vkDestroyPipelineCache(vk_device, vk_pipeline_cache, nullptr);

    std::cout << "</Test3>" << std::endl;
}

void Test4(Turbo::Core::TDeviceQueue *deviceQueue)
{
    std::cout << "<Test4>" << std::endl;
    Turbo::Core::TDevice *device = deviceQueue->GetDevice();
    Turbo::Core::TPhysicalDevice *physical_device = device->GetPhysicalDevice();
    Turbo::Core::TInstance *instance = physical_device->GetInstance();

    VkInstance vk_instance = instance->GetVkInstance();
    VkPhysicalDevice vk_physical_device = physical_device->GetVkPhysicalDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkQueue vk_queue = deviceQueue->GetVkQueue();

    {
        const std::string vert_shader_str = ReadTextFile("../../asset/shaders/shader_base.vert");
        const std::string fragment_shader_str = ReadTextFile("../../asset/shaders/shader_base.frag");

        Turbo::Core::TVertexShader *vs = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, vert_shader_str);
        Turbo::Core::TFragmentShader *fs = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, fragment_shader_str);

        Turbo::Core::TFormatInfo format_info = physical_device->GetFormatInfo(Turbo::Core::TFormatType::B8G8R8A8_SRGB);
        if (format_info.IsOptimalTilingSupportColorAttachment())
        {
            std::cout << "B8G8R8A8_SRGB support color attachment" << std::endl;
        }
        else
        {
            std::cout << "B8G8R8A8_SRGB Not support color attachment" << std::endl;
        }

        Turbo::Core::TImage *color_attachment_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, 512, 512, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
        Turbo::Core::TImageView *color_attachment_image_view = new Turbo::Core::TImageView(color_attachment_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, color_attachment_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);

        std::vector<Turbo::Core::TAttachment> attachments;
        attachments.push_back({color_attachment_image_view->GetFormat(), Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL});

        Turbo::Core::TSubpass subpass(Turbo::Core::TPipelineType::Graphics);
        subpass.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);

        std::vector<Turbo::Core::TSubpass> subpasses;
        subpasses.push_back(subpass);

        Turbo::Core::TRenderPass *render_pass = new Turbo::Core::TRenderPass(device, attachments, subpasses);
        std::vector<Turbo::Core::TVertexBinding> vb;
        std::vector<uint8_t> meta_cache_data;
        try
        {
            meta_cache_data = ReadBinaryFile("./PipelineCache.bin", 0);
        }
        catch (const std::exception &e)
        {
            std::cout << "!!!Exception::" << e.what() << std::endl;
        }

        Turbo::Core::TPipelineCache *pipeline_cache = new Turbo::Core::TPipelineCache(device, meta_cache_data.size(), meta_cache_data.data());

        TTimer timer;
        timer.Begin();
        Turbo::Core::TGraphicsPipeline *gp = new Turbo::Core::TGraphicsPipeline(pipeline_cache, render_pass, 0, vb, vs, fs);
        timer.End();
        std::cout << "Create TGraphicsPipeline cache use:" << timer.GetDeltaSecond() << "s" << std::endl;

        size_t cache_size = pipeline_cache->GetSize();
        void *cache_data = malloc(cache_size);
        pipeline_cache->GetData(cache_size, cache_data);

        meta_cache_data.clear();

        std::vector<uint8_t> data(cache_size);
        memcpy(data.data(), cache_data, cache_size);
        WriteBinaryFile("./PipelineCache.bin", cache_size, data);
        free(cache_data);

        delete pipeline_cache;
        delete gp;
        delete render_pass;
        delete color_attachment_image_view;
        delete color_attachment_image;
        delete fs;
        delete vs;
    }

    std::cout << "</Test4>" << std::endl;
}

void Test5(Turbo::Core::TDeviceQueue *deviceQueue)
{
    std::cout << "<Test5>" << std::endl;
    Turbo::Core::TDevice *device = deviceQueue->GetDevice();
    Turbo::Core::TPhysicalDevice *physical_device = device->GetPhysicalDevice();
    Turbo::Core::TInstance *instance = physical_device->GetInstance();

    VkInstance vk_instance = instance->GetVkInstance();
    VkPhysicalDevice vk_physical_device = physical_device->GetVkPhysicalDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkQueue vk_queue = deviceQueue->GetVkQueue();
    std::cout << "</Test5>" << std::endl;
}

void Test6(Turbo::Core::TDeviceQueue *deviceQueue)
{
    Turbo::Core::TDevice *device = deviceQueue->GetDevice();
    Turbo::Core::TPhysicalDevice *physical_device = device->GetPhysicalDevice();
    Turbo::Core::TInstance *instance = physical_device->GetInstance();

    VkInstance vk_instance = instance->GetVkInstance();
    VkPhysicalDevice vk_physical_device = physical_device->GetVkPhysicalDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkQueue vk_queue = deviceQueue->GetVkQueue();

    PAUSE("Init env finished");

    const std::string vert_shader_str = ReadTextFile("../../asset/shaders/shader_base.vert");
    const std::string fragment_shader_str = ReadTextFile("../../asset/shaders/shader_base.frag");

    Turbo::Core::TVertexShader *vs = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, vert_shader_str);
    Turbo::Core::TFragmentShader *fs = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, fragment_shader_str);

    Turbo::Core::TImage *color_attachment_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, 512, 512, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
    Turbo::Core::TImageView *color_attachment_image_view = new Turbo::Core::TImageView(color_attachment_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, color_attachment_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);

    std::vector<Turbo::Core::TAttachment> attachments;
    attachments.push_back({color_attachment_image_view->GetFormat(), Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL});

    Turbo::Core::TSubpass subpass(Turbo::Core::TPipelineType::Graphics);
    subpass.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);

    std::vector<Turbo::Core::TSubpass> subpasses;
    subpasses.push_back(subpass);

    Turbo::Core::TRenderPass *render_pass = new Turbo::Core::TRenderPass(device, attachments, subpasses);
    std::vector<Turbo::Core::TVertexBinding> vertex_binding;
    PAUSE("Init create finished");

    {
        PAUSE("Begin Create TGraphicsPipeline");
        uint32_t create_count = UINT32_MAX;
        for (uint32_t index = 0; index < create_count; index++)
        {
            // 将会调用 vkCreateDescriptorSetLayout(...), vkCreatePipelineLayout(...) 和 vkCreateGraphicsPipelines(...)
            Turbo::Core::TGraphicsPipeline *temp_graphics_pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 0, vertex_binding, vs, fs);

            // 将会调用 vkDestroyPipeline(...), vkDestroyPipelineLayout(...) 和 vkDestroyDescriptorSetLayout(...)
            delete temp_graphics_pipeline;

            std::cout << index << std::endl;
        }
        PAUSE("End Create TGraphicsPipeline");
    }

    delete render_pass;
    delete color_attachment_image_view;
    delete color_attachment_image;
    delete fs;
    delete vs;

    PAUSE("Init destroy finished");
}

void Test7(Turbo::Core::TDeviceQueue *deviceQueue)
{
    Turbo::Core::TDevice *device = deviceQueue->GetDevice();
    Turbo::Core::TPhysicalDevice *physical_device = device->GetPhysicalDevice();
    Turbo::Core::TInstance *instance = physical_device->GetInstance();

    VkInstance vk_instance = instance->GetVkInstance();
    VkPhysicalDevice vk_physical_device = physical_device->GetVkPhysicalDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkQueue vk_queue = deviceQueue->GetVkQueue();

    uint32_t create_count = UINT32_MAX;
    for (uint32_t index = 0; index < create_count; index++)
    {
        VkPipelineLayoutCreateInfo vk_pipeline_layout_create_info = {};
        vk_pipeline_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        vk_pipeline_layout_create_info.pNext = nullptr;
        vk_pipeline_layout_create_info.flags = 0;
        vk_pipeline_layout_create_info.setLayoutCount = 0;
        vk_pipeline_layout_create_info.pSetLayouts = nullptr;
        vk_pipeline_layout_create_info.pushConstantRangeCount = 0;
        vk_pipeline_layout_create_info.pPushConstantRanges = nullptr;

        VkPipelineLayout vk_pipeline_layout = VK_NULL_HANDLE;
        VkResult result = device->GetDeviceDriver()->vkCreatePipelineLayout(vk_device, &vk_pipeline_layout_create_info, nullptr, &vk_pipeline_layout);
        if (result == VkResult::VK_SUCCESS)
        {
            device->GetDeviceDriver()->vkDestroyPipelineLayout(vk_device, vk_pipeline_layout, nullptr);
            std::cout << index << std::endl;
        }
        else
        {
            PAUSE("vkCreatePipelineLayout failed!!!");
            return;
        }
    }
}

void Test8(Turbo::Core::TDeviceQueue *deviceQueue)
{
    Turbo::Core::TDevice *device = deviceQueue->GetDevice();
    Turbo::Core::TPhysicalDevice *physical_device = device->GetPhysicalDevice();
    Turbo::Core::TInstance *instance = physical_device->GetInstance();

    VkInstance vk_instance = instance->GetVkInstance();
    VkPhysicalDevice vk_physical_device = physical_device->GetVkPhysicalDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkQueue vk_queue = deviceQueue->GetVkQueue();
}

int main()
{
    std::cout << "Vulkan Version:" << Turbo::Core::TVulkanLoader::Instance()->GetVulkanVersion().ToString() << std::endl;

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
    if (khronos_validation.GetLayerType() != Turbo::Core::TLayerType::UNDEFINED)
    {
        enable_layer.push_back(khronos_validation);
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_instance_extensions;
    std::vector<Turbo::Core::TExtensionInfo> instance_support_extensions = engine.GetInstance().GetSupportExtensions();
    for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_WIN32_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_WAYLAND_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XCB_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XLIB_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
    }

    Turbo::Core::TVersion instance_version(1, 3, 0, 0);
    Turbo::Core::TInstance *instance = new Turbo::Core::TInstance(&enable_layer, &enable_instance_extensions, &instance_version);
    Turbo::Core::TPhysicalDevice *physical_device = instance->GetBestPhysicalDevice();
    std::cout << "Physical Device:" << physical_device->GetDeviceName() << std::endl;

    auto physical_device_support_extensions = physical_device->GetSupportExtensions();
    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
        {
            enable_device_extensions.push_back(extension);
        }
    }

    Turbo::Core::TPhysicalDeviceFeatures physical_device_features = {};
    Turbo::Core::TDevice *device = new Turbo::Core::TDevice(physical_device, nullptr, &enable_device_extensions, &physical_device_features);
    Turbo::Core::TDeviceQueue *queue = device->GetBestGraphicsQueue();

    // Test0(queue);
    //  Test1(queue);
    // Test2(queue);
    // Test3(queue);
    // Test4(queue);
    // Test5(queue);
    // Test6(queue);

    // Test7(queue);
    Test8(queue);

    delete device;
    delete instance;

    return 0;
}