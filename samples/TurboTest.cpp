#include <TDevice.h>
#include <TDeviceQueue.h>
#include <TEngine.h>
#include <TMemoryTypeInfo.h>
#include <TPhysicalDevice.h>

#include <TBuffer.h>
#include <TCommandBuffer.h>
#include <TCommandBufferPool.h>
#include <TImage.h>
#include <TImageView.h>

#include <TShader.h>

#include <TAttachment.h>
#include <TGraphicsPipeline.h>
#include <TRenderPass.h>
#include <TSubpass.h>

#include <TDescriptorPool.h>
#include <TDescriptorSet.h>
#include <TDescriptorSetLayout.h>
#include <TFramebuffer.h>

#include <TFence.h>
#include <TSemaphore.h>

#include <GLFW/glfw3.h>

#include <TSurface.h>
#include <TSwapchain.h>

#include <iostream>

#include <TPipelineDescriptorSet.h>
#include <TSampler.h>

#include <TVulkanLoader.h>

#include <ktx.h>

#include <stdio.h>
#include <string.h>
#include <string>
#include <utility>

std::string asset_root(TURBO_ASSET_ROOT);

void OutputMemoryInfo(Turbo::Core::TMemoryTypeInfo &memoryTypeInfo)
{
    Turbo::Core::TMemoryTypeInfo current_memory_type_info = memoryTypeInfo;
    Turbo::Core::TMemoryHeapInfo current_memory_heap_info = current_memory_type_info.GetMemoryHeap();
    if (current_memory_heap_info.IsDeviceLocal())
    {
        std::cout << "\tHeap:"
                  << "Device_Local" << std::endl;
    }
    if (current_memory_heap_info.IsMultiInstance())
    {
        std::cout << "\tHeap:"
                  << "Multi_Instance" << std::endl;
    }

    if (current_memory_type_info.IsDeviceLocal())
    {
        std::cout << "\t\tType:"
                  << "Device_Local" << std::endl;
    }
    if (current_memory_type_info.IsHostCached())
    {
        std::cout << "\t\tType:"
                  << "Host_Cached" << std::endl;
    }
    if (current_memory_type_info.IsHostCoherent())
    {
        std::cout << "\t\tType:"
                  << "Host_Coherent" << std::endl;
    }
    if (current_memory_type_info.IsHostVisible())
    {
        std::cout << "\t\tType:"
                  << "Host_Visible" << std::endl;
    }
    if (current_memory_type_info.IsLazilyAllocated())
    {
        std::cout << "\t\tType:"
                  << "Lazily_Allocated" << std::endl;
    }
    if (current_memory_type_info.IsProtected())
    {
        std::cout << "\t\tType:"
                  << "Protected" << std::endl;
    }
}

void OutputResourceMemoryInfo(const std::string &name, Turbo::Core::TBuffer *buffer)
{
    std::cout << "Buffer:[" << name << "]:" << std::endl;
    Turbo::Core::TMemoryTypeInfo current_memory_type_info = buffer->GetMemoryTypeInfo();
    OutputMemoryInfo(current_memory_type_info);
}

void OutputResourceMemoryInfo(const std::string &name, Turbo::Core::TImage *image)
{
    std::cout << "Image:[" << name << "]:" << std::endl;
    Turbo::Core::TMemoryTypeInfo current_memory_type_info = image->GetMemoryTypeInfo();
    OutputMemoryInfo(current_memory_type_info);
}

const std::string VERT_SHADER_STR = "#version 450 core\n"
                                    "layout (set = 0, binding = 0) uniform bufferVals {\n"
                                    "    float value;\n"
                                    "} myBufferVals;\n"
                                    "layout (location = 0) in vec3 pos;\n"
                                    "layout (location = 1) in vec3 color;"
                                    "layout (location = 2) in vec2 uv;"
                                    "layout (location = 3) out vec2 outUV;"
                                    "layout (location = 4) out vec3 outColor;"
                                    "layout (location = 5) out float outValue;\n"
                                    "void main() {\n"
                                    "   gl_Position = vec4(pos,1);\n"
                                    "   outColor = color;\n"
                                    "   outValue = myBufferVals.value;\n"
                                    "   outUV = uv;\n"
                                    "}\n";

const std::string FRAG_SHADER_STR = "#version 450 core\n"
                                    "layout (set = 0, binding = 1) uniform texture2D myTexture;\n"
                                    "layout (set = 0, binding = 2) uniform sampler mySampler;\n"
                                    "layout (location = 3) in vec2 inUV;\n"
                                    "layout (location = 4) in vec3 inColor;\n"
                                    "layout (location = 5) in float inValue;\n"
                                    "layout (location = 0) out vec4 outColor;\n"
                                    "layout (push_constant) uniform my_push_constants_t\n"
                                    "{"
                                    "   float value;\n"
                                    "} my_push_constants;\n"
                                    "void main() {\n"
                                    "   vec3 line_color=inColor;\n"
                                    "   line_color.r<=0.04045?line_color.r=inColor.r/12.92 : line_color.r=pow((inColor.r+0.055)/1.055,2.4);\n"
                                    "   line_color.g<=0.04045?line_color.g=inColor.g/12.92 : line_color.g=pow((inColor.g+0.055)/1.055,2.4);\n"
                                    "   line_color.b<=0.04045?line_color.b=inColor.b/12.92 : line_color.b=pow((inColor.b+0.055)/1.055,2.4);\n"
                                    "   outColor = vec4(line_color,1)/**texture(sampler2D(myTexture, mySampler), inUV, 0)*/;\n"
                                    "}\n";

typedef struct POSITION
{
    float x;
    float y;
    float z;
} POSITION;

typedef struct COLOR
{
    float r;
    float g;
    float b;
} COLOR;

typedef struct POSITION_AND_COLOR
{
    POSITION position;
    COLOR color;
} POSITION_AND_COLOR;

typedef struct NORMAL
{
    float x;
    float y;
    float z;
} NORMAL;

typedef struct TEXCOORD
{
    float u;
    float v;
} TEXCOORD;

typedef struct POSITION_COLOR_UV
{
    POSITION position;
    COLOR color;
    TEXCOORD uv;
} POSITION_COLOR_UV;

using INDICE = uint32_t;

struct PUSH_CONSTANT_DATA
{
    float value;
};

int main()
{
    std::vector<POSITION_COLOR_UV> POSITION_COLOR_UV_DATA;
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{0.5f, 0.5f, 0.0f}, {1.f, 0.f, 0.f}, {1.f, 1.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{-0.5f, 0.5f, 0.0f}, {0.f, 1.f, 0.f}, {0.0f, 1.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{-0.5f, -0.5f, 0.0f}, {0.f, 0.f, 1.f}, {0.0f, 0.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{0.5f, -0.5f, 0.0f}, {1.f, 1.f, 0.f}, {1.0f, 0.0f}});

    std::vector<INDICE> INDICES_DATA = {0, 1, 2, 2, 3, 0};

    float value = -10.0f;

    PUSH_CONSTANT_DATA push_constant_data = {};

    std::vector<Turbo::Core::TLayerInfo> support_layers;
    std::vector<Turbo::Core::TExtensionInfo> instance_support_extensions;
    {
        Turbo::Core::TRefPtr<Turbo::Core::TInstance> temp_instance = new Turbo::Core::TInstance();
        support_layers = temp_instance->GetSupportLayers();
        instance_support_extensions = temp_instance->GetSupportExtensions();
    }

    Turbo::Core::TLayerInfo khronos_validation;
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
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_EXT_SWAPCHAIN_COLORSPACE)
        {
            enable_instance_extensions.push_back(extension);
        }
    }

    Turbo::Core::TVersion instance_version(1, 0, 0, 0);
    Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance = new Turbo::Core::TInstance(&enable_layer, &enable_instance_extensions, &instance_version);
    Turbo::Core::TRefPtr<Turbo::Core::TPhysicalDevice> physical_device = instance->GetBestPhysicalDevice();

    if (!glfwInit())
        return -1;
    GLFWwindow *window;
    int window_width = 500;
    int window_height = 500;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(window_width, window_height, "Turbo", NULL, NULL);
    VkSurfaceKHR vk_surface_khr = VK_NULL_HANDLE;
    VkInstance vk_instance = instance->GetVkInstance();
    glfwCreateWindowSurface(vk_instance, window, NULL, &vk_surface_khr);

    Turbo::Core::TPhysicalDeviceFeatures physical_device_features = {};
    physical_device_features.sampleRateShading = true;
    physical_device_features.logicOp = true;

    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    physical_device->GetSupportExtensions();
    std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = physical_device->GetSupportExtensions();
    for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
        {
            enable_device_extensions.push_back(extension);
        }
    }

    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device = new Turbo::Core::TDevice(physical_device, nullptr, &enable_device_extensions, &physical_device_features);
    Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> queue = device->GetBestGraphicsQueue();

    Turbo::Core::TRefPtr<Turbo::Extension::TSurface> surface = new Turbo::Extension::TSurface(device, nullptr, vk_surface_khr);
    uint32_t max_image_count = surface->GetMaxImageCount();
    uint32_t min_image_count = surface->GetMinImageCount();

    uint32_t swapchain_image_count = max_image_count <= min_image_count ? min_image_count : max_image_count - 1;

    Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> swapchain = new Turbo::Extension::TSwapchain(surface, swapchain_image_count, Turbo::Core::TFormatType::B8G8R8A8_SRGB, 1, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, true);

    std::vector<Turbo::Core::TImage *> swapchain_images = swapchain->GetImages();

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
    for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
    {
        Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        swapchain_image_views.push_back(swapchain_view);
    }

    Turbo::Core::TRefPtr<Turbo::Core::TCommandBufferPool> command_pool = new Turbo::Core::TCommandBufferPool(queue);
    Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> command_buffer = command_pool->Allocate();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> value_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(float));
    void *value_ptr = value_buffer->Map();
    memcpy(value_ptr, &value, sizeof(value));
    value_buffer->Unmap();

    OutputResourceMemoryInfo("value_buffer", value_buffer);

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION_COLOR_UV) * POSITION_COLOR_UV_DATA.size());
    void *vertx_buffer_ptr = vertex_buffer->Map();
    memcpy(vertx_buffer_ptr, POSITION_COLOR_UV_DATA.data(), sizeof(POSITION_COLOR_UV) * POSITION_COLOR_UV_DATA.size());
    vertex_buffer->Unmap();
    POSITION_COLOR_UV_DATA.clear();

    OutputResourceMemoryInfo("vertex_buffer", vertex_buffer);

    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{0.5f + 0.1f, 0.5f, 0.0f}, {0.f, 0.f, 0.f}, {1.f, 1.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{-0.5f + 0.1f, 0.5f, 0.0f}, {0.f, 0.f, 0.f}, {0.0f, 1.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{-0.5f + 0.1f, -0.5f, 0.0f}, {0.f, 0.f, 0.f}, {0.0f, 0.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{0.5f + 0.1f, -0.5f, 0.0f}, {0.f, 0.f, 0.f}, {1.0f, 0.0f}});
    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> color_0_vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION_COLOR_UV) * POSITION_COLOR_UV_DATA.size());
    void *color_0_vertx_buffer_ptr = color_0_vertex_buffer->Map();
    memcpy(color_0_vertx_buffer_ptr, POSITION_COLOR_UV_DATA.data(), sizeof(POSITION_COLOR_UV) * POSITION_COLOR_UV_DATA.size());
    color_0_vertex_buffer->Unmap();
    POSITION_COLOR_UV_DATA.clear();

    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{0.5f + 0.2f, 0.5f, 0.0f}, {0.05f, 0.05f, 0.05f}, {1.f, 1.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{-0.5f + 0.2f, 0.5f, 0.0f}, {0.05f, 0.05f, 0.05f}, {0.0f, 1.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{-0.5f + 0.2f, -0.5f, 0.0f}, {0.05f, 0.05f, 0.05f}, {0.0f, 0.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{0.5f + 0.2f, -0.5f, 0.0f}, {0.05f, 0.05f, 0.05f}, {1.0f, 0.0f}});
    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> color_1_vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION_COLOR_UV) * POSITION_COLOR_UV_DATA.size());
    void *color_1_vertx_buffer_ptr = color_1_vertex_buffer->Map();
    memcpy(color_1_vertx_buffer_ptr, POSITION_COLOR_UV_DATA.data(), sizeof(POSITION_COLOR_UV) * POSITION_COLOR_UV_DATA.size());
    color_1_vertex_buffer->Unmap();
    POSITION_COLOR_UV_DATA.clear();

    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{0.5f + 0.3f, 0.5f, 0.0f}, {0.1f, 0.1f, 0.1f}, {1.f, 1.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{-0.5f + 0.3f, 0.5f, 0.0f}, {0.1f, 0.1f, 0.1f}, {0.0f, 1.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{-0.5f + 0.3f, -0.5f, 0.0f}, {0.1f, 0.1f, 0.1f}, {0.0f, 0.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{0.5f + 0.3f, -0.5f, 0.0f}, {0.1f, 0.1f, 0.1f}, {1.0f, 0.0f}});
    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> color_2_vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION_COLOR_UV) * POSITION_COLOR_UV_DATA.size());
    void *color_2_vertx_buffer_ptr = color_2_vertex_buffer->Map();
    memcpy(color_2_vertx_buffer_ptr, POSITION_COLOR_UV_DATA.data(), sizeof(POSITION_COLOR_UV) * POSITION_COLOR_UV_DATA.size());
    color_2_vertex_buffer->Unmap();
    POSITION_COLOR_UV_DATA.clear();

    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{0.5f + 0.4f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.5f}, {1.f, 1.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{-0.5f + 0.4f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{-0.5f + 0.4f, -0.5f, 0.0f}, {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{0.5f + 0.4f, -0.5f, 0.0f}, {0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}});
    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> color_3_vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION_COLOR_UV) * POSITION_COLOR_UV_DATA.size());
    void *color_3_vertx_buffer_ptr = color_3_vertex_buffer->Map();
    memcpy(color_3_vertx_buffer_ptr, POSITION_COLOR_UV_DATA.data(), sizeof(POSITION_COLOR_UV) * POSITION_COLOR_UV_DATA.size());
    color_3_vertex_buffer->Unmap();
    POSITION_COLOR_UV_DATA.clear();

    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{0.5f + 0.5f, 0.5f, 0.0f}, {1.f, 1.f, 1.f}, {1.f, 1.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{-0.5f + 0.5f, 0.5f, 0.0f}, {1.f, 1.f, 1.f}, {0.0f, 1.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{-0.5f + 0.5f, -0.5f, 0.0f}, {1.f, 1.f, 1.f}, {0.0f, 0.0f}});
    POSITION_COLOR_UV_DATA.push_back(POSITION_COLOR_UV{{0.5f + 0.5f, -0.5f, 0.0f}, {1.f, 1.f, 1.f}, {1.0f, 0.0f}});
    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> color_4_vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION_COLOR_UV) * POSITION_COLOR_UV_DATA.size());
    void *color_4_vertx_buffer_ptr = color_4_vertex_buffer->Map();
    memcpy(color_4_vertx_buffer_ptr, POSITION_COLOR_UV_DATA.data(), sizeof(POSITION_COLOR_UV) * POSITION_COLOR_UV_DATA.size());
    color_4_vertex_buffer->Unmap();
    POSITION_COLOR_UV_DATA.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> index_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_INDEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(INDICE) * INDICES_DATA.size());
    void *index_buffer_ptr = index_buffer->Map();
    memcpy(index_buffer_ptr, INDICES_DATA.data(), sizeof(INDICE) * INDICES_DATA.size());
    index_buffer->Unmap();

    Turbo::Core::TRefPtr<Turbo::Core::TImage> ktx_image = nullptr;
    //<KTX Texture>
    {
        std::string ktx_filename = asset_root + "/images/metalplate01_rgba.ktx";

        ktxTexture *ktx_texture;
        KTX_error_code ktx_result;

        ktx_result = ktxTexture_CreateFromNamedFile(ktx_filename.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktx_texture);

        if (ktx_texture == nullptr)
        {
            throw std::runtime_error("Couldn't load texture");
        }

        uint32_t ktx_texture_width = ktx_texture->baseWidth;
        uint32_t ktx_texture_height = ktx_texture->baseHeight;
        uint32_t ktx_texture_mip_levels = ktx_texture->numLevels;

        ktx_uint8_t *ktx_texture_data = ktx_texture->pData;
        ktx_size_t ktx_texture_size = ktx_texture->dataSize;

        Turbo::Core::TRefPtr<Turbo::Core::TBuffer> ktx_staging_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, ktx_texture_size);
        void *ktx_ptr = ktx_staging_buffer->Map();
        memcpy(ktx_ptr, ktx_texture_data, ktx_texture_size);
        ktx_staging_buffer->Unmap();

        ktx_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R8G8B8A8_UNORM, ktx_texture_width, ktx_texture_height, 1, ktx_texture_mip_levels, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST | Turbo::Core::TImageUsageBits::IMAGE_SAMPLED, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);

        Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> ktx_command_buffer = command_pool->Allocate();
        ktx_command_buffer->Begin();
        ktx_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::HOST_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::HOST_WRITE_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, ktx_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, ktx_texture_mip_levels, 0, 1);
        for (uint32_t mip_index = 0; mip_index < ktx_texture_mip_levels; mip_index++)
        {
            uint32_t copy_width = ktx_texture_width >> mip_index;
            uint32_t copy_height = ktx_texture_height >> mip_index;
            uint32_t copy_mip_level = mip_index;
            ktx_size_t copy_buffer_offset = 0;
            ktx_result = ktxTexture_GetImageOffset(ktx_texture, mip_index, 0, 0, &copy_buffer_offset);
            ktx_command_buffer->CmdCopyBufferToImage(ktx_staging_buffer, ktx_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, copy_buffer_offset, copy_width, copy_height, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, copy_mip_level, 0, 1, 0, 0, 0, copy_width, copy_height, 1);
        }
        ktx_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::FRAGMENT_SHADER_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TAccessBits::SHADER_READ_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::SHADER_READ_ONLY_OPTIMAL, ktx_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, ktx_texture_mip_levels, 0, 1);
        ktx_command_buffer->End();

        Turbo::Core::TRefPtr<Turbo::Core::TFence> ktx_fence = new Turbo::Core::TFence(device);

        queue->Submit(ktx_command_buffer, ktx_fence);

        ktx_fence->WaitUntil();

        command_pool->Free(ktx_command_buffer);
        ktxTexture_Destroy(ktx_texture);
    }
    //</KTX Texture>
    OutputResourceMemoryInfo("ktx_image", ktx_image);

    Turbo::Core::TRefPtr<Turbo::Core::TImageView> ktx_texture_view = new Turbo::Core::TImageView(ktx_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, ktx_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, ktx_image->GetMipLevels(), 0, 1);
    Turbo::Core::TRefPtr<Turbo::Core::TSampler> sampler = new Turbo::Core::TSampler(device, Turbo::Core::TFilter::LINEAR, Turbo::Core::TFilter::LINEAR, Turbo::Core::TMipmapMode::LINEAR, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TBorderColor::FLOAT_OPAQUE_WHITE, 0.0f, 0.0f, ktx_image->GetMipLevels());

    Turbo::Core::TRefPtr<Turbo::Core::TImage> depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

    Turbo::Core::TRefPtr<Turbo::Core::TShader> vertex_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::VERTEX, Turbo::Core::TShaderLanguage::GLSL, VERT_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TShader> fragment_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::FRAGMENT, Turbo::Core::TShaderLanguage::GLSL, FRAG_SHADER_STR);

    std::cout << vertex_shader->ToString() << std::endl;
    std::cout << fragment_shader->ToString() << std::endl;

    std::vector<Turbo::Core::TDescriptorSize> descriptor_sizes;
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::UNIFORM_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::COMBINED_IMAGE_SAMPLER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::SAMPLER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::SAMPLED_IMAGE, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_IMAGE, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::UNIFORM_TEXEL_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_TEXEL_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::UNIFORM_BUFFER_DYNAMIC, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_BUFFER_DYNAMIC, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::INPUT_ATTACHMENT, 1000));

    Turbo::Core::TRefPtr<Turbo::Core::TDescriptorPool> descriptor_pool = new Turbo::Core::TDescriptorPool(device, descriptor_sizes.size() * 1000, descriptor_sizes);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> buffers;
    buffers.push_back(value_buffer);

    std::vector<std::pair<Turbo::Core::TRefPtr<Turbo::Core::TImageView>, Turbo::Core::TRefPtr<Turbo::Core::TSampler>>> combined_image_samplers;
    combined_image_samplers.push_back(std::make_pair(ktx_texture_view, sampler));

    Turbo::Core::TSubpass subpass(Turbo::Core::TPipelineType::Graphics);
    subpass.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);                // swapchain color image
    subpass.SetDepthStencilAttachmentReference(1, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL); // depth image

    std::vector<Turbo::Core::TSubpass> subpasses;
    subpasses.push_back(subpass); // subpass 0

    Turbo::Core::TAttachment swapchain_color_attachment(swapchain_images[0]->GetFormat(), swapchain_images[0]->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::PRESENT_SRC_KHR);
    Turbo::Core::TAttachment depth_attachment(depth_image->GetFormat(), depth_image->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    std::vector<Turbo::Core::TAttachment> attachemts;
    attachemts.push_back(swapchain_color_attachment);
    attachemts.push_back(depth_attachment);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> textures;
    textures.push_back(ktx_texture_view);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TSampler>> samplers;
    samplers.push_back(sampler);

    Turbo::Core::TRefPtr<Turbo::Core::TRenderPass> render_pass = new Turbo::Core::TRenderPass(device, attachemts, subpasses);

    Turbo::Core::TVertexBinding vertex_binding(0, sizeof(POSITION_COLOR_UV), Turbo::Core::TVertexRate::VERTEX);
    vertex_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32B32_SFLOAT, offsetof(POSITION_COLOR_UV, position)); // position
    vertex_binding.AddAttribute(1, Turbo::Core::TFormatType::R32G32B32_SFLOAT, offsetof(POSITION_COLOR_UV, color));    // color
    vertex_binding.AddAttribute(2, Turbo::Core::TFormatType::R32G32_SFLOAT, offsetof(POSITION_COLOR_UV, uv));          // texcoord/uv

    std::vector<Turbo::Core::TVertexBinding> vertex_bindings;
    vertex_bindings.push_back(vertex_binding);

    Turbo::Core::TViewport viewport(0, 0, 500, 500, 0, 1);
    Turbo::Core::TScissor scissor(0, 0, 500, 500);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TShader>> shaders{vertex_shader, fragment_shader};
    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 0, vertex_bindings, shaders, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, true, true, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false);

    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> pipeline_descriptor_set = descriptor_pool->Allocate(pipeline->GetPipelineLayout());
    pipeline_descriptor_set->BindData(0, 0, 0, buffers);
    pipeline_descriptor_set->BindData(0, 1, 0, textures);
    pipeline_descriptor_set->BindData(0, 2, 0, samplers);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> vertex_buffers;
    vertex_buffers.push_back(vertex_buffer);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer>> swpachain_framebuffers;
    for (Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_image_view_item : swapchain_image_views)
    {
        std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> image_views;
        image_views.push_back(swapchain_image_view_item);
        image_views.push_back(depth_image_view);

        Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, image_views);
        swpachain_framebuffers.push_back(swapchain_framebuffer);
    }

    bool show_demo_window = true;

    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vk_get_physical_device_surface_formats_khr = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");

    uint32_t support_color_count = 0;
    vk_get_physical_device_surface_formats_khr(physical_device->GetVkPhysicalDevice(), vk_surface_khr, &support_color_count, nullptr);

    std::vector<VkSurfaceFormatKHR> vk_surface_format_khr;
    vk_surface_format_khr.resize(support_color_count);
    vk_get_physical_device_surface_formats_khr(physical_device->GetVkPhysicalDevice(), vk_surface_khr, &support_color_count, vk_surface_format_khr.data());

    for (VkSurfaceFormatKHR &vk_surface_format_khr_item : vk_surface_format_khr)
    {
        switch (vk_surface_format_khr_item.colorSpace)
        {
        case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_SRGB_NONLINEAR_KHR" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_BT709_LINEAR_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_BT709_LINEAR_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_BT709_NONLINEAR_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_BT709_NONLINEAR_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_BT2020_LINEAR_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_BT2020_LINEAR_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_HDR10_ST2084_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_HDR10_ST2084_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_DOLBYVISION_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_DOLBYVISION_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_HDR10_HLG_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_HDR10_HLG_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_PASS_THROUGH_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_PASS_THROUGH_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT" << std::endl;
            break;
        }
        case VK_COLOR_SPACE_DISPLAY_NATIVE_AMD: {
            std::cout << "VkFormat:" << vk_surface_format_khr_item.format << " support space:"
                      << "VK_COLOR_SPACE_DISPLAY_NATIVE_AMD" << std::endl;
            break;
        }
        }
    }

    float _time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        void *_ptr = value_buffer->Map();
        memcpy(_ptr, &value, sizeof(value));
        value_buffer->Unmap();

        push_constant_data.value = (std::sin(_time) + 1) / 2.0f;

        //<Begin Rendering>
        uint32_t current_image_index = UINT32_MAX;
        Turbo::Core::TRefPtr<Turbo::Core::TSemaphore> wait_image_ready = new Turbo::Core::TSemaphore(device, Turbo::Core::TPipelineStageBits::COLOR_ATTACHMENT_OUTPUT_BIT);
        Turbo::Core::TResult result = swapchain->AcquireNextImageUntil(wait_image_ready, nullptr, &current_image_index);
        switch (result)
        {
        case Turbo::Core::TResult::SUCCESS: {
            // successed get image and go on rendering

            // because we just have one command buffer, so we should reset the command buffer for each frame
            // If we create command buffer for each swapchain image, we don't need to reset it each frame

            Turbo::Core::TViewport frame_viewport(0, 0, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight(), 0, 1);
            Turbo::Core::TScissor frame_scissor(0, 0, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight());

            std::vector<Turbo::Core::TViewport> frame_viewports;
            frame_viewports.push_back(frame_viewport);

            std::vector<Turbo::Core::TScissor> frame_scissors;
            frame_scissors.push_back(frame_scissor);

            command_buffer->Begin();
            command_buffer->CmdBeginRenderPass(render_pass, swpachain_framebuffers[current_image_index]);
            command_buffer->CmdBindPipeline(pipeline);

            // Square
            // command_buffer->CmdBindPipeline(pipeline);
            command_buffer->CmdPushConstants(0, sizeof(push_constant_data), &push_constant_data);
            command_buffer->CmdBindPipelineDescriptorSet(pipeline_descriptor_set);
            command_buffer->CmdBindVertexBuffers(vertex_buffers);
            command_buffer->CmdSetViewport(frame_viewports);
            command_buffer->CmdSetScissor(frame_scissors);
            command_buffer->CmdBindIndexBuffer(index_buffer);
            command_buffer->CmdDrawIndexed(INDICES_DATA.size(), 1, 0, 0, 0);

            // Color 0
            std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> color_0_vertex_buffers;
            color_0_vertex_buffers.push_back(color_0_vertex_buffer);
            command_buffer->CmdBindVertexBuffers(color_0_vertex_buffers);
            command_buffer->CmdDrawIndexed(INDICES_DATA.size(), 1, 0, 0, 0);
            // Color 1
            std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> color_1_vertex_buffers;
            color_1_vertex_buffers.push_back(color_1_vertex_buffer);
            command_buffer->CmdBindVertexBuffers(color_1_vertex_buffers);
            command_buffer->CmdDrawIndexed(INDICES_DATA.size(), 1, 0, 0, 0);
            // Color 2
            std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> color_2_vertex_buffers;
            color_2_vertex_buffers.push_back(color_2_vertex_buffer);
            command_buffer->CmdBindVertexBuffers(color_2_vertex_buffers);
            command_buffer->CmdDrawIndexed(INDICES_DATA.size(), 1, 0, 0, 0);
            // Color 3
            std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> color_3_vertex_buffers;
            color_3_vertex_buffers.push_back(color_3_vertex_buffer);
            command_buffer->CmdBindVertexBuffers(color_3_vertex_buffers);
            command_buffer->CmdDrawIndexed(INDICES_DATA.size(), 1, 0, 0, 0);
            // Color 4
            std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> color_4_vertex_buffers;
            color_4_vertex_buffers.push_back(color_4_vertex_buffer);
            command_buffer->CmdBindVertexBuffers(color_4_vertex_buffers);
            command_buffer->CmdDrawIndexed(INDICES_DATA.size(), 1, 0, 0, 0);

            command_buffer->CmdEndRenderPass();
            command_buffer->End();

            Turbo::Core::TRefPtr<Turbo::Core::TFence> fence = new Turbo::Core::TFence(device);
            std::vector<Turbo::Core::TRefPtr<Turbo::Core::TSemaphore>> wait_semaphores;
            wait_semaphores.push_back(wait_image_ready);

            queue->Submit(wait_semaphores, {}, command_buffer, fence);

            fence->WaitUntil(); // or you can use semaphore to wait for get higher performance

            // delete fence;

            command_buffer->Reset(); // you can create an command buffer each for one swapchain image,for now just one command buffer

            Turbo::Core::TResult present_result = queue->Present(swapchain, current_image_index);
            switch (present_result)
            {
            case Turbo::Core::TResult::MISMATCH: {
                // the size of the window had changed you need to recreate swapchain

                // waite for idle
                device->WaitIdle();

                // destroy related resource
                // clear old swapchain image
                swapchain_images.clear();

                // destroy swapchain image views
                swapchain_image_views.clear();

                // destroy depth image and view

                // destroy framebuffer
                swpachain_framebuffers.clear();

                // recreate swapchain
                Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> new_swapchain = new Turbo::Extension::TSwapchain(swapchain);
                swapchain = new_swapchain;

                // recreate swapchain image views
                swapchain_images = swapchain->GetImages();
                for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
                {
                    Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                    swapchain_image_views.push_back(swapchain_view);
                }

                // recreate depth image and view
                depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
                depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

                // recreate framebuffer
                for (Turbo::Core::TRefPtr<Turbo::Core::TImageView> image_view_item : swapchain_image_views)
                {
                    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
                    swapchain_image_views.push_back(image_view_item);
                    swapchain_image_views.push_back(depth_image_view);

                    Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, swapchain_image_views);
                    swpachain_framebuffers.push_back(swapchain_framebuffer);
                }
            }
            break;
            default: {
                //
            }
            break;
            }
        }
        break;
        case Turbo::Core::TResult::TIMEOUT: {
            // you need to wait, or do something else
        }
        break;
        case Turbo::Core::TResult::NOT_READY: {
            // you need to wait, or do something else
        }
        break;
        case Turbo::Core::TResult::MISMATCH: {
            // the size of the window had changed you need to recreate swapchain

            // waite for idle
            device->WaitIdle();

            // destroy related resource
            // clear old swapchain image
            swapchain_images.clear();

            // destroy swapchain image views
            swapchain_image_views.clear();

            // destroy depth image and view

            // destroy framebuffer
            swpachain_framebuffers.clear();

            // recreate swapchain
            Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> new_swapchain = new Turbo::Extension::TSwapchain(swapchain);
            swapchain = new_swapchain;

            // recreate swapchain image views
            swapchain_images = swapchain->GetImages();
            for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
            {
                Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                swapchain_image_views.push_back(swapchain_view);
            }

            // recreate depth image and view
            depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
            depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

            // recreate framebuffer
            for (Turbo::Core::TRefPtr<Turbo::Core::TImageView> image_view_item : swapchain_image_views)
            {
                std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
                swapchain_image_views.push_back(image_view_item);
                swapchain_image_views.push_back(depth_image_view);

                Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, swapchain_image_views);
                swpachain_framebuffers.push_back(swapchain_framebuffer);
            }
        }
        break;
        default: {
            //
        }
        break;
        }

        // delete wait_image_ready;
        //</End Rendering>
    }

    descriptor_pool->Free(pipeline_descriptor_set);
    command_pool->Free(command_buffer);
    glfwTerminate();

    return 0;
}