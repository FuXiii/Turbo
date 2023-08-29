#include "core/include/TDevice.h"
#include "core/include/TDeviceQueue.h"
#include "core/include/TEngine.h"
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

#include "core/include/TFence.h"
#include "core/include/TSemaphore.h"

#include <fstream>

#include <GLFW/glfw3.h>

#include "core/include/TSurface.h"
#include "core/include/TSwapchain.h"

#include <math.h>

#include "core/include/TPipelineDescriptorSet.h"
#include "core/include/TSampler.h"

#include <glm/ext.hpp>

#include "core/include/TVulkanLoader.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tiny_gltf.h>

// #include <ktxvulkan.h>
#include <ktx.h>

#include <imgui.h>

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

static bool g_MouseJustPressed[ImGuiMouseButton_COUNT] = {};
static GLFWcursor *g_MouseCursors[ImGuiMouseCursor_COUNT] = {};

const std::string IMGUI_VERT_SHADER_STR = ReadTextFile("../../asset/shaders/imgui.vert");
const std::string IMGUI_FRAG_SHADER_STR = ReadTextFile("../../asset/shaders/imgui.frag");

const std::string VERT_SHADER_STR = ReadTextFile("../../asset/shaders/GeometryTest.vert");
const std::string GEOM_SHADER_STR = ReadTextFile("../../asset/shaders/GeometryTest.geom");
const std::string FRAG_SHADER_STR = ReadTextFile("../../asset/shaders/GeometryTest.frag");

const std::string RAY_GENERATION_SHADER_STR = ReadTextFile("../../asset/shaders/RayTracingAnyHitShader.rgen");
const std::string MISS_SHADER_STR = ReadTextFile("../../asset/shaders/RayTracingAnyHitShader.rmiss");
const std::string SHADOW_MISS_SHADER_STR = ReadTextFile("../../asset/shaders/RayTracingAnyHitShaderShadow.rmiss");
const std::string CLOSEST_HIT_SHADER_STR = ReadTextFile("../../asset/shaders/RayTracingAnyHitShader.rchit");

const std::string SHADER_INCLUDE_PATH = "../../asset/shaders";

typedef struct POSITION
{
    float x;
    float y;
    float z;
} POSITION;

typedef struct NORMAL
{
    float x;
    float y;
    float z;
} NORMAL;

typedef struct TANGENT
{
    float x;
    float y;
    float z;
    float w;
} TANGENT;

typedef struct TEXCOORD
{
    float u;
    float v;
} TEXCOORD;

struct MY_BUFFER_DATA
{
    float scale;
};

struct MATRIXS_BUFFER_DATA
{
    glm::mat4 m;
    glm::mat4 v;
    glm::mat4 p;
};

struct VERTEX
{
    POSITION position;
    NORMAL normal;
    TEXCOORD texcoord;
};

struct BOTTOM_LEVEL_ACCELERATION_STRUCTURE_DEVICE_ADDRESS
{
    VkDeviceAddress vertexDeviceAddress;
    VkDeviceAddress indexDeviceAddress;
};

struct RAY_TRACING_MATRIXS_BUFFER_DATA
{
    glm::mat4 v;
    glm::mat4 p;
};

struct RAY_TRACING_PUSH_CONSTANT
{
    float accumulateWeight;
    int32_t frame;
};

int main()
{
    std::cout << "Vulkan Version:" << Turbo::Core::TVulkanLoader::Instance()->GetVulkanVersion().ToString() << std::endl;

    RAY_TRACING_PUSH_CONSTANT ray_tracing_push_constant = {};
    ray_tracing_push_constant.frame = 0;
    ray_tracing_push_constant.accumulateWeight = 1;

    MY_BUFFER_DATA my_buffer_data = {};
    my_buffer_data.scale = 0.03;

    MATRIXS_BUFFER_DATA matrixs_buffer_data = {};

    //<gltf for material_sphere>
    std::vector<POSITION> POSITION_data;
    std::vector<NORMAL> NORMAL_data;
    std::vector<TEXCOORD> TEXCOORD_data;
    std::vector<TANGENT> TANGENT_data;
    std::vector<uint32_t> INDICES_data;
    {
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;

        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, "../../asset/models/Suzanne_without_Yup.gltf");
        const tinygltf::Scene &scene = model.scenes[model.defaultScene];
        tinygltf::Node &node = model.nodes[scene.nodes[0]];
        tinygltf::Mesh &mesh = model.meshes[node.mesh];
        tinygltf::Primitive &primitive = mesh.primitives[0];
        int mode = primitive.mode;
        int position_accesser_index = primitive.attributes["POSITION"];
        int normal_accesser_index = primitive.attributes["NORMAL"];
        int texcoord_0_accesser_index = primitive.attributes["TEXCOORD_0"];
        int tangent_accesser_index = primitive.attributes["TANGENT"];
        int indices_accesser_index = primitive.indices;
        tinygltf::Accessor &position_accessor = model.accessors[position_accesser_index];
        tinygltf::Accessor &normal_accessor = model.accessors[normal_accesser_index];
        tinygltf::Accessor &texcoord_0_accessor = model.accessors[texcoord_0_accesser_index];
        tinygltf::Accessor &indices_accessor = model.accessors[indices_accesser_index];
        tinygltf::Accessor &tangent_accessor = model.accessors[tangent_accesser_index];

        tinygltf::BufferView &position_buffer_view = model.bufferViews[position_accessor.bufferView];
        tinygltf::BufferView &normal_buffer_view = model.bufferViews[normal_accessor.bufferView];
        tinygltf::BufferView &texcoord_0_buffer_view = model.bufferViews[texcoord_0_accessor.bufferView];
        tinygltf::BufferView &indices_buffer_view = model.bufferViews[indices_accessor.bufferView];
        tinygltf::BufferView &tangent_buffer_view = model.bufferViews[tangent_accessor.bufferView];

        int position_buffer_index = position_buffer_view.buffer;
        size_t position_buffer_byteLength = position_buffer_view.byteLength;
        int position_buffer_byteOffset = position_buffer_view.byteOffset;
        int position_type = position_accessor.type;

        int normal_buffer_index = normal_buffer_view.buffer;
        size_t normal_buffer_byteLength = normal_buffer_view.byteLength;
        int normal_buffer_byteOffset = normal_buffer_view.byteOffset;
        int normal_type = normal_accessor.type;

        int texcoord_0_buffer_index = texcoord_0_buffer_view.buffer;
        size_t texcoord_0_buffer_byteLength = texcoord_0_buffer_view.byteLength;
        int texcoord_0_buffer_byteOffset = texcoord_0_buffer_view.byteOffset;
        int texcoord_0_type = texcoord_0_accessor.type;

        int indices_buffer_index = indices_buffer_view.buffer;
        size_t indices_buffer_byteLength = indices_buffer_view.byteLength;
        int indices_buffer_byteOffset = indices_buffer_view.byteOffset;
        int indices_type = indices_accessor.type;

        int tangent_buffer_index = tangent_buffer_view.buffer;
        size_t tangent_buffer_byteLength = tangent_buffer_view.byteLength;
        int tangent_buffer_byteOffset = tangent_buffer_view.byteOffset;
        int tangent_type = tangent_accessor.type;

        tinygltf::Buffer &position_buffer = model.buffers[position_buffer_index];
        tinygltf::Buffer &normal_buffer = model.buffers[normal_buffer_index];
        tinygltf::Buffer &texcoord_0_buffer = model.buffers[texcoord_0_buffer_index];
        tinygltf::Buffer &indices_buffer = model.buffers[indices_buffer_index];
        tinygltf::Buffer &tangent_buffer = model.buffers[tangent_buffer_index];

        std::vector<unsigned char> &position_data = position_buffer.data;
        std::vector<unsigned char> &normal_data = normal_buffer.data;
        std::vector<unsigned char> &texcoord_0_data = texcoord_0_buffer.data;
        std::vector<unsigned char> &indices_data = indices_buffer.data;
        std::vector<unsigned char> &tangent_data = tangent_buffer.data;

        std::vector<unsigned short> temp_indices_data;

        POSITION_data.resize(position_buffer_byteLength / sizeof(POSITION));
        NORMAL_data.resize(normal_buffer_byteLength / sizeof(NORMAL));
        TEXCOORD_data.resize(texcoord_0_buffer_byteLength / sizeof(TEXCOORD));
        temp_indices_data.resize(indices_buffer_byteLength / sizeof(unsigned short));
        TANGENT_data.resize(tangent_buffer_byteLength / sizeof(TANGENT));

        memcpy(POSITION_data.data(), position_data.data() + position_buffer_byteOffset, position_buffer_byteLength);
        memcpy(NORMAL_data.data(), normal_data.data() + normal_buffer_byteOffset, normal_buffer_byteLength);
        memcpy(TEXCOORD_data.data(), texcoord_0_data.data() + texcoord_0_buffer_byteOffset, texcoord_0_buffer_byteLength);
        memcpy(temp_indices_data.data(), indices_data.data() + indices_buffer_byteOffset, indices_buffer_byteLength);
        memcpy(TANGENT_data.data(), tangent_data.data() + tangent_buffer_byteOffset, tangent_buffer_byteLength);

        for (unsigned short &temp_indices_item : temp_indices_data)
        {
            INDICES_data.push_back(temp_indices_item);
        }
    }
    //</gltf for material_sphere>

    uint32_t indices_count = INDICES_data.size();
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
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2)
        {
            enable_instance_extensions.push_back(extension);
        }
    }

    Turbo::Core::TVersion instance_version(1, 2, 0, 0);
    Turbo::Core::TInstance *instance = new Turbo::Core::TInstance(&enable_layer, &enable_instance_extensions, &instance_version);
    Turbo::Core::TPhysicalDevice *physical_device = instance->GetBestPhysicalDevice();

    Turbo::Core::TPhysicalDeviceFeatures physical_device_support_features = physical_device->GetDeviceFeatures();
    {
        if (physical_device_support_features.accelerationStructure)
        {
            std::cout << "Support acceleration structure feature" << std::endl;
        }
        else
        {
            std::cout << "Not support acceleration structure feature" << std::endl;
        }

        if (physical_device_support_features.accelerationStructureCaptureReplay)
        {
            std::cout << "Support acceleration structure capture replay feature" << std::endl;
        }
        else
        {
            std::cout << "Not support acceleration structure capture replay feature" << std::endl;
        }

        if (physical_device_support_features.accelerationStructureHostCommands)
        {
            std::cout << "Support acceleration structure host commands feature" << std::endl;
        }
        else
        {
            std::cout << "Not support acceleration structure host commands feature" << std::endl;
        }

        if (physical_device_support_features.accelerationStructureIndirectBuild)
        {
            std::cout << "Support acceleration structure indirect build feature" << std::endl;
        }
        else
        {
            std::cout << "Not support acceleration structure indirect build feature" << std::endl;
        }

        if (physical_device_support_features.bufferDeviceAddress)
        {
            std::cout << "Support bufferDeviceAddress feature" << std::endl;
        }
        else
        {
            std::cout << "Not support bufferDeviceAddress feature" << std::endl;
        }

        if (physical_device_support_features.bufferDeviceAddressCaptureReplay)
        {
            std::cout << "Support bufferDeviceAddressCaptureReplay feature" << std::endl;
        }
        else
        {
            std::cout << "Not support bufferDeviceAddressCaptureReplay feature" << std::endl;
        }

        if (physical_device_support_features.bufferDeviceAddressMultiDevice)
        {
            std::cout << "Support bufferDeviceAddressMultiDevice feature" << std::endl;
        }
        else
        {
            std::cout << "Not support bufferDeviceAddressMultiDevice feature" << std::endl;
        }

        if (physical_device_support_features.rayTracingPipeline)
        {
            std::cout << "Support ray tracing pipeline feature" << std::endl;
        }
        else
        {
            std::cout << "Not support ray tracing pipeline feature" << std::endl;
        }
    }

    if (!physical_device_support_features.accelerationStructure || !physical_device_support_features.rayTracingPipeline)
    {
        delete instance;
        std::cout << "Please use a GPU which support hardware real-time ray tracing" << std::endl;
        return 0;
    }

    if (!glfwInit())
        return -1;
    GLFWwindow *window;
    int window_width = 1920 / 2.0;
    int window_height = 1080 / 2.0;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(window_width, window_height, "Turbo", NULL, NULL);
    VkSurfaceKHR vk_surface_khr = VK_NULL_HANDLE;
    VkInstance vk_instance = instance->GetVkInstance();
    glfwCreateWindowSurface(vk_instance, window, NULL, &vk_surface_khr);

    Turbo::Core::TPhysicalDeviceFeatures physical_device_features = {};
    physical_device_features.sampleRateShading = true;
    physical_device_features.accelerationStructure = physical_device_support_features.accelerationStructure;
    physical_device_features.accelerationStructureCaptureReplay = physical_device_support_features.accelerationStructureCaptureReplay;
    physical_device_features.accelerationStructureHostCommands = physical_device_support_features.accelerationStructureHostCommands;
    physical_device_features.accelerationStructureIndirectBuild = physical_device_support_features.accelerationStructureIndirectBuild;
    physical_device_features.bufferDeviceAddress = physical_device_support_features.bufferDeviceAddress;
    physical_device_features.rayTracingPipeline = physical_device_support_features.rayTracingPipeline;

    if (physical_device_support_features.geometryShader)
    {
        physical_device_features.geometryShader = true;
    }
    else
    {
        glfwTerminate();
        delete instance;
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Not support geometry shader feature");
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = physical_device->GetSupportExtensions();
    for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
        {
            enable_device_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_ACCELERATION_STRUCTURE)
        {
            enable_device_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SPIRV_1_4)
        {
            enable_device_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_RAY_TRACING_PIPELINE)
        {
            enable_device_extensions.push_back(extension);
        }
    }

    Turbo::Core::TDevice *device = new Turbo::Core::TDevice(physical_device, nullptr, &enable_device_extensions, &physical_device_features);
    Turbo::Core::TDeviceQueue *queue = device->GetBestGraphicsQueue();

    Turbo::Extension::TSurface *surface = new Turbo::Extension::TSurface(device, vk_surface_khr);
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

    Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
    Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();

    Turbo::Core::TImage *ktx_image = nullptr;
    //<KTX Texture>
    {
        std::string ktx_filename = "../../asset/images/checkerboard_rgba.ktx";

        ktxTexture *ktx_texture;
        KTX_error_code ktx_result;

        ktx_result = ktxTexture_CreateFromNamedFile(ktx_filename.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktx_texture);

        if (ktx_texture == nullptr || ktx_result != KTX_SUCCESS)
        {
            throw std::runtime_error("Couldn't load texture");
        }

        uint32_t ktx_texture_width = ktx_texture->baseWidth;
        uint32_t ktx_texture_height = ktx_texture->baseHeight;
        uint32_t ktx_texture_mip_levels = ktx_texture->numLevels;

        ktx_uint8_t *ktx_texture_data = ktx_texture->pData;
        ktx_size_t ktx_texture_size = ktx_texture->dataSize;

        Turbo::Core::TBuffer *ktx_staging_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, ktx_texture_size);
        void *ktx_ptr = ktx_staging_buffer->Map();
        memcpy(ktx_ptr, ktx_texture_data, ktx_texture_size);
        ktx_staging_buffer->Unmap();

        ktx_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R8G8B8A8_UNORM, ktx_texture_width, ktx_texture_height, 1, ktx_texture_mip_levels, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST | Turbo::Core::TImageUsageBits::IMAGE_SAMPLED, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);

        Turbo::Core::TCommandBuffer *ktx_command_buffer = command_pool->Allocate();
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

        Turbo::Core::TFence *ktx_fence = new Turbo::Core::TFence(device);

        queue->Submit(nullptr, nullptr, ktx_command_buffer, ktx_fence);

        ktx_fence->WaitUntil();

        delete ktx_fence;
        delete ktx_staging_buffer;
        command_pool->Free(ktx_command_buffer);
        ktxTexture_Destroy(ktx_texture);
    }
    //</KTX Texture>
    Turbo::Core::TImageView *ktx_texture_view = new Turbo::Core::TImageView(ktx_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, ktx_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, ktx_image->GetMipLevels(), 0, 1);
    Turbo::Core::TSampler *sampler = new Turbo::Core::TSampler(device, Turbo::Core::TFilter::LINEAR, Turbo::Core::TFilter::LINEAR, Turbo::Core::TMipmapMode::LINEAR, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TBorderColor::FLOAT_OPAQUE_WHITE, 0.0f, 0.0f, ktx_image->GetMipLevels());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 previous_view = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
    previous_view = view;

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)swapchain->GetWidth() / (float)swapchain->GetHeight(), 0.1f, 100.0f);

    matrixs_buffer_data.m = model;
    matrixs_buffer_data.v = view;
    matrixs_buffer_data.p = projection;

    Turbo::Core::TBuffer *matrixs_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(matrixs_buffer_data));
    void *mvp_ptr = matrixs_buffer->Map();
    memcpy(mvp_ptr, &matrixs_buffer_data, sizeof(matrixs_buffer_data));
    matrixs_buffer->Unmap();

    Turbo::Core::TBuffer *my_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(my_buffer_data));
    void *my_buffer_ptr = my_buffer->Map();
    memcpy(my_buffer_ptr, &my_buffer_data, sizeof(my_buffer_data));
    my_buffer->Unmap();

    Turbo::Core::TBuffer *position_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION) * POSITION_data.size());
    void *position_buffer_ptr = position_buffer->Map();
    memcpy(position_buffer_ptr, POSITION_data.data(), sizeof(POSITION) * POSITION_data.size());
    position_buffer->Unmap();
    // POSITION_data.clear();

    Turbo::Core::TBuffer *normal_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(NORMAL) * NORMAL_data.size());
    void *normal_buffer_ptr = normal_buffer->Map();
    memcpy(normal_buffer_ptr, NORMAL_data.data(), sizeof(NORMAL) * NORMAL_data.size());
    normal_buffer->Unmap();
    // NORMAL_data.clear();

    Turbo::Core::TBuffer *texcoord_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(TEXCOORD) * TEXCOORD_data.size());
    void *texcoord_buffer_ptr = texcoord_buffer->Map();
    memcpy(texcoord_buffer_ptr, TEXCOORD_data.data(), sizeof(TEXCOORD) * TEXCOORD_data.size());
    texcoord_buffer->Unmap();
    // TEXCOORD_data.clear();

    Turbo::Core::TBuffer *index_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_INDEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(uint32_t) * INDICES_data.size());
    void *index_buffer_ptr = index_buffer->Map();
    memcpy(index_buffer_ptr, INDICES_data.data(), sizeof(uint32_t) * INDICES_data.size());
    index_buffer->Unmap();
    // INDICES_data.clear();

    Turbo::Core::TBuffer *tangent_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(TANGENT) * TANGENT_data.size());
    void *tangent_buffer_ptr = tangent_buffer->Map();
    memcpy(tangent_buffer_ptr, TANGENT_data.data(), sizeof(TANGENT) * TANGENT_data.size());
    tangent_buffer->Unmap();
    // TANGENT_data.clear();

    Turbo::Core::TImage *depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
    Turbo::Core::TImageView *depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

    Turbo::Core::TVertexShader *vertex_shader = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, VERT_SHADER_STR);
    Turbo::Core::TGeometryShader *geometry_shader = new Turbo::Core::TGeometryShader(device, Turbo::Core::TShaderLanguage::GLSL, GEOM_SHADER_STR);
    Turbo::Core::TFragmentShader *fragment_shader = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, FRAG_SHADER_STR);

    // for ray tracing image
    Turbo::Core::TImage *ray_tracing_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R32G32B32A32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_STORAGE, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
    Turbo::Core::TImageView *ray_tracing_image_view = new Turbo::Core::TImageView(ray_tracing_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, ray_tracing_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
    {

        Turbo::Core::TCommandBuffer *change_ray_tracing_image_layout_command_buffer = command_pool->Allocate();

        change_ray_tracing_image_layout_command_buffer->Begin();
        change_ray_tracing_image_layout_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::BOTTOM_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::GENERAL, ray_tracing_image_view);
        change_ray_tracing_image_layout_command_buffer->End();

        Turbo::Core::TFence *change_image_layout_fence = new Turbo::Core::TFence(device);

        queue->Submit(nullptr, nullptr, change_ray_tracing_image_layout_command_buffer, change_image_layout_fence);

        change_image_layout_fence->WaitUntil();
        delete change_image_layout_fence;

        command_pool->Free(change_ray_tracing_image_layout_command_buffer);
    }

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
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::ACCELERATION_STRUCTURE, 1000));

    Turbo::Core::TDescriptorPool *descriptor_pool = new Turbo::Core::TDescriptorPool(device, descriptor_sizes.size() * 1000, descriptor_sizes);

    const Turbo::Core::TDeviceDriver *device_driver = device->GetDeviceDriver();
    const Turbo::Core::TPhysicalDeviceDriver *physical_device_driver = physical_device->GetPhysicalDeviceDriver();

    // Acceleration Structure
    Turbo::Core::TBuffer *device_local_vertex_buffer = nullptr;
    Turbo::Core::TBuffer *device_local_index_buffer = nullptr;
    Turbo::Core::TBuffer *bottom_level_acceleration_structure_buffer = nullptr;
    VkAccelerationStructureKHR bottom_level_acceleration_structure_khr = VK_NULL_HANDLE;
    Turbo::Core::TBuffer *top_level_acceleration_structure_buffer = nullptr;
    VkAccelerationStructureKHR top_level_acceleration_structure_khr = VK_NULL_HANDLE;
    Turbo::Core::TBuffer *instance_buffer = nullptr;

    Turbo::Core::TBuffer *bottom_level_acceleration_structure_device_address_buffer = nullptr;

    {
        VkPhysicalDeviceAccelerationStructurePropertiesKHR vk_physical_device_acceleration_structure_properties_khr = {};
        vk_physical_device_acceleration_structure_properties_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR;
        vk_physical_device_acceleration_structure_properties_khr.pNext = nullptr;
        vk_physical_device_acceleration_structure_properties_khr.maxGeometryCount = 0;
        vk_physical_device_acceleration_structure_properties_khr.maxInstanceCount = 0;
        vk_physical_device_acceleration_structure_properties_khr.maxPrimitiveCount = 0;
        vk_physical_device_acceleration_structure_properties_khr.maxPerStageDescriptorAccelerationStructures = 0;
        vk_physical_device_acceleration_structure_properties_khr.maxPerStageDescriptorUpdateAfterBindAccelerationStructures = 0;
        vk_physical_device_acceleration_structure_properties_khr.maxDescriptorSetAccelerationStructures = 0;
        vk_physical_device_acceleration_structure_properties_khr.maxDescriptorSetUpdateAfterBindAccelerationStructures = 0;
        vk_physical_device_acceleration_structure_properties_khr.minAccelerationStructureScratchOffsetAlignment = 0;

        VkPhysicalDeviceProperties2 vk_physical_device_properties_2 = {};
        vk_physical_device_properties_2.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        vk_physical_device_properties_2.pNext = &vk_physical_device_acceleration_structure_properties_khr;
        vk_physical_device_properties_2.properties = {};

        if (physical_device_driver->vkGetPhysicalDeviceProperties2 != nullptr)
        {
            physical_device_driver->vkGetPhysicalDeviceProperties2(physical_device->GetVkPhysicalDevice(), &vk_physical_device_properties_2);
        }
        else if (physical_device_driver->vkGetPhysicalDeviceProperties2KHR != nullptr)
        {
            physical_device_driver->vkGetPhysicalDeviceProperties2KHR(physical_device->GetVkPhysicalDevice(), &vk_physical_device_properties_2);
        }

        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxGeometryCount = " << vk_physical_device_acceleration_structure_properties_khr.maxGeometryCount << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxInstanceCount = " << vk_physical_device_acceleration_structure_properties_khr.maxInstanceCount << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxPrimitiveCount = " << vk_physical_device_acceleration_structure_properties_khr.maxPrimitiveCount << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxPerStageDescriptorAccelerationStructures = " << vk_physical_device_acceleration_structure_properties_khr.maxPerStageDescriptorAccelerationStructures << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxPerStageDescriptorUpdateAfterBindAccelerationStructures = " << vk_physical_device_acceleration_structure_properties_khr.maxPerStageDescriptorUpdateAfterBindAccelerationStructures << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxDescriptorSetAccelerationStructures = " << vk_physical_device_acceleration_structure_properties_khr.maxDescriptorSetAccelerationStructures << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxDescriptorSetUpdateAfterBindAccelerationStructures = " << vk_physical_device_acceleration_structure_properties_khr.maxDescriptorSetUpdateAfterBindAccelerationStructures << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.minAccelerationStructureScratchOffsetAlignment = " << vk_physical_device_acceleration_structure_properties_khr.minAccelerationStructureScratchOffsetAlignment << std::endl;

        std::vector<VERTEX> ray_tracing_vertexs;
        for (uint32_t vertex_index = 0; vertex_index < POSITION_data.size(); vertex_index++)
        {
            VERTEX ray_tracing_vertex;
            ray_tracing_vertex.position = POSITION_data[vertex_index];
            ray_tracing_vertex.normal = NORMAL_data[vertex_index];
            ray_tracing_vertex.texcoord = TEXCOORD_data[vertex_index];

            ray_tracing_vertexs.push_back(ray_tracing_vertex);
        }

        device_local_vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS | Turbo::Core::TBufferUsageBits::BUFFER_STORAGE_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, sizeof(VERTEX) * ray_tracing_vertexs.size());
        {
            Turbo::Core::TBuffer *staging_vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(VERTEX) * ray_tracing_vertexs.size());
            memcpy(staging_vertex_buffer->Map(), ray_tracing_vertexs.data(), sizeof(VERTEX) * ray_tracing_vertexs.size());
            staging_vertex_buffer->Unmap();

            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            command_buffer->CmdCopyBuffer(staging_vertex_buffer, device_local_vertex_buffer, 0, 0, sizeof(VERTEX) * ray_tracing_vertexs.size());
            command_buffer->End();
            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            delete fence;
            command_pool->Free(command_buffer);
            delete command_pool;
            delete staging_vertex_buffer;
        }

        VkDeviceAddress device_local_vertex_buffer_device_address = 0;

        VkBufferDeviceAddressInfo device_local_vertex_buffer_device_address_info = {};
        device_local_vertex_buffer_device_address_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        device_local_vertex_buffer_device_address_info.pNext = nullptr;
        device_local_vertex_buffer_device_address_info.buffer = device_local_vertex_buffer->GetVkBuffer();

        if (device_driver->vkGetBufferDeviceAddress != nullptr)
        {
            device_local_vertex_buffer_device_address = device_driver->vkGetBufferDeviceAddress(device->GetVkDevice(), &device_local_vertex_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressKHR != nullptr)
        {
            device_local_vertex_buffer_device_address = device_driver->vkGetBufferDeviceAddressKHR(device->GetVkDevice(), &device_local_vertex_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressEXT != nullptr)
        {
            device_local_vertex_buffer_device_address = device_driver->vkGetBufferDeviceAddressEXT(device->GetVkDevice(), &device_local_vertex_buffer_device_address_info);
        }

        if (device_local_vertex_buffer_device_address == 0)
        {
            throw std::runtime_error("Get ray tracing device local vertex buffer address failed");
        }

        VkDeviceOrHostAddressConstKHR vertex_data = {};
        vertex_data.deviceAddress = device_local_vertex_buffer_device_address;

        device_local_index_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_INDEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS | Turbo::Core::TBufferUsageBits::BUFFER_STORAGE_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, sizeof(uint32_t) * INDICES_data.size());
        {
            Turbo::Core::TBuffer *staging_index_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(uint32_t) * INDICES_data.size());
            memcpy(staging_index_buffer->Map(), INDICES_data.data(), sizeof(uint32_t) * INDICES_data.size());
            staging_index_buffer->Unmap();

            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            command_buffer->CmdCopyBuffer(staging_index_buffer, device_local_index_buffer, 0, 0, sizeof(uint32_t) * INDICES_data.size());
            command_buffer->End();
            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            delete fence;
            command_pool->Free(command_buffer);
            delete command_pool;
            delete staging_index_buffer;
        }

        VkDeviceAddress device_local_index_buffer_device_address = 0;

        VkBufferDeviceAddressInfo device_local_index_buffer_device_address_info = {};
        device_local_index_buffer_device_address_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        device_local_index_buffer_device_address_info.pNext = nullptr;
        device_local_index_buffer_device_address_info.buffer = device_local_index_buffer->GetVkBuffer();

        if (device_driver->vkGetBufferDeviceAddress != nullptr)
        {
            device_local_index_buffer_device_address = device_driver->vkGetBufferDeviceAddress(device->GetVkDevice(), &device_local_index_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressKHR != nullptr)
        {
            device_local_index_buffer_device_address = device_driver->vkGetBufferDeviceAddressKHR(device->GetVkDevice(), &device_local_index_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressEXT != nullptr)
        {
            device_local_index_buffer_device_address = device_driver->vkGetBufferDeviceAddressEXT(device->GetVkDevice(), &device_local_index_buffer_device_address_info);
        }

        if (device_local_index_buffer_device_address == 0)
        {
            throw std::runtime_error("Get ray tracing device local index buffer address failed");
        }

        BOTTOM_LEVEL_ACCELERATION_STRUCTURE_DEVICE_ADDRESS ray_tracing_bottom_level_acceleration_structure;
        ray_tracing_bottom_level_acceleration_structure.vertexDeviceAddress = device_local_vertex_buffer_device_address;
        ray_tracing_bottom_level_acceleration_structure.indexDeviceAddress = device_local_index_buffer_device_address;

        std::vector<BOTTOM_LEVEL_ACCELERATION_STRUCTURE_DEVICE_ADDRESS> bottom_level_acceleration_structure_device_addresses;
        bottom_level_acceleration_structure_device_addresses.push_back(ray_tracing_bottom_level_acceleration_structure);

        bottom_level_acceleration_structure_device_address_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_STORAGE_BUFFER, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, sizeof(BOTTOM_LEVEL_ACCELERATION_STRUCTURE_DEVICE_ADDRESS) * bottom_level_acceleration_structure_device_addresses.size());
        {
            Turbo::Core::TBuffer *staging_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(BOTTOM_LEVEL_ACCELERATION_STRUCTURE_DEVICE_ADDRESS) * bottom_level_acceleration_structure_device_addresses.size());
            memcpy(staging_buffer->Map(), bottom_level_acceleration_structure_device_addresses.data(), sizeof(BOTTOM_LEVEL_ACCELERATION_STRUCTURE_DEVICE_ADDRESS) * bottom_level_acceleration_structure_device_addresses.size());
            staging_buffer->Unmap();

            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            command_buffer->CmdCopyBuffer(staging_buffer, bottom_level_acceleration_structure_device_address_buffer, 0, 0, sizeof(BOTTOM_LEVEL_ACCELERATION_STRUCTURE_DEVICE_ADDRESS) * bottom_level_acceleration_structure_device_addresses.size());
            command_buffer->End();
            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            delete fence;
            command_pool->Free(command_buffer);
            delete command_pool;
            delete staging_buffer;
        }

        VkDeviceOrHostAddressConstKHR index_data = {};
        index_data.deviceAddress = device_local_index_buffer_device_address;

        VkAccelerationStructureGeometryTrianglesDataKHR vk_acceleration_structure_geometry_triangles_data_khr = {};
        vk_acceleration_structure_geometry_triangles_data_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
        vk_acceleration_structure_geometry_triangles_data_khr.pNext = nullptr;
        vk_acceleration_structure_geometry_triangles_data_khr.vertexFormat = VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
        vk_acceleration_structure_geometry_triangles_data_khr.vertexData = vertex_data;
        vk_acceleration_structure_geometry_triangles_data_khr.vertexStride = sizeof(VERTEX);
        vk_acceleration_structure_geometry_triangles_data_khr.maxVertex = ray_tracing_vertexs.size();
        vk_acceleration_structure_geometry_triangles_data_khr.indexType = VkIndexType::VK_INDEX_TYPE_UINT32;
        vk_acceleration_structure_geometry_triangles_data_khr.indexData = index_data;
        vk_acceleration_structure_geometry_triangles_data_khr.transformData.deviceAddress = 0;

        VkAccelerationStructureGeometryDataKHR vk_acceleration_structure_geometry_data_khr = {};
        vk_acceleration_structure_geometry_data_khr.triangles = vk_acceleration_structure_geometry_triangles_data_khr;

        VkAccelerationStructureGeometryKHR vk_acceleration_structure_geometry_khr = {};
        vk_acceleration_structure_geometry_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
        vk_acceleration_structure_geometry_khr.pNext = nullptr;
        vk_acceleration_structure_geometry_khr.geometryType = VkGeometryTypeKHR::VK_GEOMETRY_TYPE_TRIANGLES_KHR;
        vk_acceleration_structure_geometry_khr.geometry = vk_acceleration_structure_geometry_data_khr;
        vk_acceleration_structure_geometry_khr.flags = VkGeometryFlagBitsKHR::VK_GEOMETRY_OPAQUE_BIT_KHR;

        VkAccelerationStructureBuildGeometryInfoKHR vk_acceleration_structure_build_geometry_info_khr = {};
        vk_acceleration_structure_build_geometry_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
        vk_acceleration_structure_build_geometry_info_khr.pNext = nullptr;
        vk_acceleration_structure_build_geometry_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
        vk_acceleration_structure_build_geometry_info_khr.flags = VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR | VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
        vk_acceleration_structure_build_geometry_info_khr.mode = VkBuildAccelerationStructureModeKHR::VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
        vk_acceleration_structure_build_geometry_info_khr.srcAccelerationStructure = VK_NULL_HANDLE;
        vk_acceleration_structure_build_geometry_info_khr.dstAccelerationStructure = VK_NULL_HANDLE;
        vk_acceleration_structure_build_geometry_info_khr.geometryCount = 1;
        vk_acceleration_structure_build_geometry_info_khr.pGeometries = &vk_acceleration_structure_geometry_khr;
        vk_acceleration_structure_build_geometry_info_khr.ppGeometries = nullptr;
        vk_acceleration_structure_build_geometry_info_khr.scratchData.deviceAddress = 0;

        std::vector<uint32_t> max_primitive_counts(vk_acceleration_structure_build_geometry_info_khr.geometryCount);
        for (uint32_t index = 0; index < vk_acceleration_structure_build_geometry_info_khr.geometryCount; index++)
        {
            // max_primitive_counts[index] = POSITION_data.size() / 3;
            max_primitive_counts[index] = INDICES_data.size() / 3;
        }

        std::cout << "POSITION_data.size():" << POSITION_data.size() << std::endl;
        std::cout << "INDICES_data.size():" << INDICES_data.size() << std::endl;

        VkAccelerationStructureBuildSizesInfoKHR vk_acceleration_structure_build_sizes_info_khr = {};
        vk_acceleration_structure_build_sizes_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
        vk_acceleration_structure_build_sizes_info_khr.pNext = nullptr;
        vk_acceleration_structure_build_sizes_info_khr.accelerationStructureSize = 0;
        vk_acceleration_structure_build_sizes_info_khr.updateScratchSize = 0;
        vk_acceleration_structure_build_sizes_info_khr.buildScratchSize = 0;
        if (device_driver->vkGetAccelerationStructureBuildSizesKHR != nullptr)
        {
            device_driver->vkGetAccelerationStructureBuildSizesKHR(device->GetVkDevice(), VkAccelerationStructureBuildTypeKHR::VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &vk_acceleration_structure_build_geometry_info_khr, max_primitive_counts.data(), &vk_acceleration_structure_build_sizes_info_khr);
        }

        std::cout << "Bottom Level VkAccelerationStructureBuildSizesInfoKHR.accelerationStructureSize = " << vk_acceleration_structure_build_sizes_info_khr.accelerationStructureSize << std::endl;
        std::cout << "Bottom Level VkAccelerationStructureBuildSizesInfoKHR.updateScratchSize = " << vk_acceleration_structure_build_sizes_info_khr.updateScratchSize << std::endl;
        std::cout << "Bottom Level VkAccelerationStructureBuildSizesInfoKHR.buildScratchSize = " << vk_acceleration_structure_build_sizes_info_khr.buildScratchSize << std::endl;

        bottom_level_acceleration_structure_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_STORAGE | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, vk_acceleration_structure_build_sizes_info_khr.accelerationStructureSize);

        // create acceleration structure
        VkDevice vk_device = device->GetVkDevice();
        VkAccelerationStructureCreateInfoKHR vk_acceleration_structure_create_info_khr = {};
        vk_acceleration_structure_create_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
        vk_acceleration_structure_create_info_khr.pNext = nullptr;
        vk_acceleration_structure_create_info_khr.createFlags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
        vk_acceleration_structure_create_info_khr.buffer = bottom_level_acceleration_structure_buffer->GetVkBuffer();                     // 将用于存储加速结构的缓存。大小一般可以为VkAccelerationStructureCreateInfoKHR::size，usage为VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR|VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT<由于之后创建顶层加速结构需要底层加速结构的地址，所以需要VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT>
        vk_acceleration_structure_create_info_khr.offset = 0;                                                                             // 单位比特，相对于buffer的偏移之后存储加速结构，需要是256的倍数。
        vk_acceleration_structure_create_info_khr.size = vk_acceleration_structure_build_sizes_info_khr.accelerationStructureSize;        // 该加速结构需要的大小。大小来源于vkGetAccelerationStructureBuildSizesKHR()函数中VkAccelerationStructureBuildSizesInfoKHR::accelerationStructureSize。
        vk_acceleration_structure_create_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR; // 加速结构的类型：TOP，BOTTOM，GENERIC
        vk_acceleration_structure_create_info_khr.deviceAddress = 0;                                                                      // 如果激活使用了accelerationStructureCaptureReplay特性，该地址为加速结构要求的那个设备地址。目前为VK_NULL_HANDLE

        VkAllocationCallbacks *vk_allocation_callbacks = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();

        VkResult result = device->GetDeviceDriver()->vkCreateAccelerationStructureKHR(vk_device, &vk_acceleration_structure_create_info_khr, vk_allocation_callbacks, &bottom_level_acceleration_structure_khr);
        if (result != VK_SUCCESS)
        {
            std::cout << "Create VkAccelerationStructureKHR Failed" << std::endl;
        }

        std::cout << "Create VkAccelerationStructureKHR Success" << std::endl;

        Turbo::Core::TBuffer *scratch_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_STORAGE_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, vk_acceleration_structure_build_sizes_info_khr.buildScratchSize);

        VkBufferDeviceAddressInfo scratch_buffer_device_address_info = {};
        scratch_buffer_device_address_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        scratch_buffer_device_address_info.pNext = nullptr;
        scratch_buffer_device_address_info.buffer = scratch_buffer->GetVkBuffer();

        VkDeviceAddress scratch_buffer_device_address = 0;
        if (device_driver->vkGetBufferDeviceAddress != nullptr)
        {
            scratch_buffer_device_address = device_driver->vkGetBufferDeviceAddress(vk_device, &scratch_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressKHR != nullptr)
        {
            scratch_buffer_device_address = device_driver->vkGetBufferDeviceAddressKHR(vk_device, &scratch_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressEXT != nullptr)
        {
            scratch_buffer_device_address = device_driver->vkGetBufferDeviceAddressEXT(vk_device, &scratch_buffer_device_address_info);
        }

        if (scratch_buffer_device_address != 0)
        {
            std::cout << "Successfully get scratch_buffer VkBuffer device local address " << std::endl;
        }

        vk_acceleration_structure_build_geometry_info_khr.dstAccelerationStructure = bottom_level_acceleration_structure_khr;
        vk_acceleration_structure_build_geometry_info_khr.scratchData.deviceAddress = scratch_buffer_device_address;

        VkAccelerationStructureBuildRangeInfoKHR vk_acceleration_structure_build_range_info_khr = {};
        vk_acceleration_structure_build_range_info_khr.primitiveCount = INDICES_data.size() / 3;
        vk_acceleration_structure_build_range_info_khr.primitiveOffset = 0;
        vk_acceleration_structure_build_range_info_khr.firstVertex = 0;
        vk_acceleration_structure_build_range_info_khr.transformOffset = 0;

        std::vector<VkAccelerationStructureBuildRangeInfoKHR> vk_acceleration_structure_build_range_info_khrs;
        vk_acceleration_structure_build_range_info_khrs.push_back(vk_acceleration_structure_build_range_info_khr);

        std::vector<VkAccelerationStructureBuildRangeInfoKHR * /*指向个数为 VkAccelerationStructureBuildGeometryInfoKHR::geometryCount 的 VkAccelerationStructureBuildRangeInfoKHR 数组*/> build_range_infos; // 该数组内元素个数应与 vkCmdBuildAccelerationStructuresKHR::infoCount 对应
        build_range_infos.push_back(vk_acceleration_structure_build_range_info_khrs.data());
        // TODO: compaction query pool for compact acceleration structure
        {
            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            device->GetDeviceDriver()->vkCmdBuildAccelerationStructuresKHR(command_buffer->GetVkCommandBuffer(), 1, &vk_acceleration_structure_build_geometry_info_khr, build_range_infos.data());
            command_buffer->End();
            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            delete fence;
            command_pool->Free(command_buffer);
            delete command_pool;
        }

        // TODO: compact acceleration structure
        {
            VkQueryPool query_pool = VK_NULL_HANDLE;

            VkQueryPoolCreateInfo vk_query_pool_create_info = {};
            vk_query_pool_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
            vk_query_pool_create_info.pNext = nullptr;
            vk_query_pool_create_info.flags = 0;
            vk_query_pool_create_info.queryType = VkQueryType::VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR;
            vk_query_pool_create_info.queryCount = 1;
            vk_query_pool_create_info.pipelineStatistics = 0;

            result = device->GetDeviceDriver()->vkCreateQueryPool(vk_device, &vk_query_pool_create_info, vk_allocation_callbacks, &query_pool);
            if (result != VK_SUCCESS)
            {
                std::cout << "Create VkQueryPool Failed" << std::endl;
            }

            std::cout << "Create VkQueryPool Success" << std::endl;

            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            device->GetDeviceDriver()->vkCmdResetQueryPool(command_buffer->GetVkCommandBuffer(), query_pool, 0, 1);
            device->GetDeviceDriver()->vkCmdWriteAccelerationStructuresPropertiesKHR(command_buffer->GetVkCommandBuffer(), 1, &bottom_level_acceleration_structure_khr, VkQueryType::VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR, query_pool, 0);
            command_buffer->End();
            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            VkDeviceSize compact_size = 0;
            device->GetDeviceDriver()->vkGetQueryPoolResults(vk_device, query_pool, 0, 1, sizeof(VkDeviceSize), &compact_size, sizeof(VkDeviceSize), VK_QUERY_RESULT_WAIT_BIT | VK_QUERY_RESULT_64_BIT);
            std::cout << "compact size:" << compact_size << std::endl;

            delete fence;
            command_pool->Free(command_buffer);

            VkAccelerationStructureKHR compact_bottom_level_acceleration_structure_khr = VK_NULL_HANDLE;

            Turbo::Core::TBuffer *compact_bottom_level_acceleration_structure_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_STORAGE | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, compact_size);

            VkAccelerationStructureCreateInfoKHR compact_vk_acceleration_structure_create_info_khr = {};
            compact_vk_acceleration_structure_create_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
            compact_vk_acceleration_structure_create_info_khr.pNext = nullptr;
            compact_vk_acceleration_structure_create_info_khr.createFlags = 0;
            compact_vk_acceleration_structure_create_info_khr.buffer = compact_bottom_level_acceleration_structure_buffer->GetVkBuffer();
            compact_vk_acceleration_structure_create_info_khr.offset = 0;
            compact_vk_acceleration_structure_create_info_khr.size = compact_size;
            compact_vk_acceleration_structure_create_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
            compact_vk_acceleration_structure_create_info_khr.deviceAddress = 0;

            result = device->GetDeviceDriver()->vkCreateAccelerationStructureKHR(vk_device, &compact_vk_acceleration_structure_create_info_khr, vk_allocation_callbacks, &compact_bottom_level_acceleration_structure_khr);
            if (result != VkResult::VK_SUCCESS)
            {
                std::cout << "vkCreateAccelerationStructureKHR create compact VkAccelerationStructureKHR Failed" << std::endl;
            }

            std::cout << "vkCreateAccelerationStructureKHR create compact VkAccelerationStructureKHR Success" << std::endl;

            VkCopyAccelerationStructureInfoKHR vk_copy_acceleration_structure_info_khr = {};
            vk_copy_acceleration_structure_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR;
            vk_copy_acceleration_structure_info_khr.pNext = nullptr;
            vk_copy_acceleration_structure_info_khr.src = bottom_level_acceleration_structure_khr;
            vk_copy_acceleration_structure_info_khr.dst = compact_bottom_level_acceleration_structure_khr;
            vk_copy_acceleration_structure_info_khr.mode = VkCopyAccelerationStructureModeKHR::VK_COPY_ACCELERATION_STRUCTURE_MODE_COMPACT_KHR;

            command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            device->GetDeviceDriver()->vkCmdCopyAccelerationStructureKHR(command_buffer->GetVkCommandBuffer(), &vk_copy_acceleration_structure_info_khr);
            command_buffer->End();
            fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();
            delete fence;
            command_pool->Free(command_buffer);
            delete command_pool;
            device->GetDeviceDriver()->vkDestroyQueryPool(vk_device, query_pool, vk_allocation_callbacks);

            // destroy no compact acceleration structure
            device->GetDeviceDriver()->vkDestroyAccelerationStructureKHR(vk_device, bottom_level_acceleration_structure_khr, vk_allocation_callbacks);
            delete bottom_level_acceleration_structure_buffer;

            // set acceleration structure value to compact version
            bottom_level_acceleration_structure_khr = compact_bottom_level_acceleration_structure_khr;
            bottom_level_acceleration_structure_buffer = compact_bottom_level_acceleration_structure_buffer;
        }

        // Top Level Acceleration Structure
        glm::mat4 acceleration_structure_model = glm::mat4(1.0f);
        acceleration_structure_model = glm::rotate(acceleration_structure_model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        VkTransformMatrixKHR vk_transform_matrix_khr = {};
        memcpy(&vk_transform_matrix_khr, &acceleration_structure_model, sizeof(VkTransformMatrixKHR));

        VkAccelerationStructureDeviceAddressInfoKHR bottom_level_acceleration_structure_device_address_info_khr = {};
        bottom_level_acceleration_structure_device_address_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
        bottom_level_acceleration_structure_device_address_info_khr.pNext = nullptr;
        bottom_level_acceleration_structure_device_address_info_khr.accelerationStructure = bottom_level_acceleration_structure_khr;

        VkDeviceAddress bottom_level_acceleration_structure_device_address = device->GetDeviceDriver()->vkGetAccelerationStructureDeviceAddressKHR(vk_device, &bottom_level_acceleration_structure_device_address_info_khr);

        VkAccelerationStructureInstanceKHR vk_acceleration_structure_instance_khr = {};
        vk_acceleration_structure_instance_khr.transform = vk_transform_matrix_khr;
        vk_acceleration_structure_instance_khr.instanceCustomIndex = 0;
        vk_acceleration_structure_instance_khr.mask = 0xFF;
        vk_acceleration_structure_instance_khr.instanceShaderBindingTableRecordOffset = 0;
        vk_acceleration_structure_instance_khr.flags = VkGeometryInstanceFlagBitsKHR::VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
        vk_acceleration_structure_instance_khr.accelerationStructureReference = bottom_level_acceleration_structure_device_address;

        {
            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();

            Turbo::Core::TBuffer *staging_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(VkAccelerationStructureInstanceKHR));
            void *staging_ptr = staging_buffer->Map();
            if (staging_ptr)
            {
                memcpy(staging_ptr, &vk_acceleration_structure_instance_khr, sizeof(VkAccelerationStructureInstanceKHR));
            }
            staging_buffer->Unmap();

            instance_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, sizeof(VkAccelerationStructureInstanceKHR));

            command_buffer->Begin();
            command_buffer->CmdCopyBuffer(staging_buffer, instance_buffer, 0, 0, sizeof(VkAccelerationStructureInstanceKHR));
            command_buffer->End();

            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            delete fence;
            delete staging_buffer;
            command_pool->Free(command_buffer);
            delete command_pool;
        }

        VkBufferDeviceAddressInfo instance_buffer_device_address_info = {};
        instance_buffer_device_address_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        instance_buffer_device_address_info.pNext = nullptr;
        instance_buffer_device_address_info.buffer = instance_buffer->GetVkBuffer();

        VkDeviceAddress instance_buffer_device_address = 0;
        if (device_driver->vkGetBufferDeviceAddress != nullptr)
        {
            instance_buffer_device_address = device_driver->vkGetBufferDeviceAddress(vk_device, &instance_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressKHR != nullptr)
        {
            instance_buffer_device_address = device_driver->vkGetBufferDeviceAddressKHR(vk_device, &instance_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressEXT != nullptr)
        {
            instance_buffer_device_address = device_driver->vkGetBufferDeviceAddressEXT(vk_device, &instance_buffer_device_address_info);
        }

        if (instance_buffer_device_address != 0)
        {
            std::cout << "Successfully get instance_buffer VkBuffer device local address " << std::endl;
        }

        // Create Top Level Acceleration Structure
        VkAccelerationStructureGeometryInstancesDataKHR vk_acceleration_structure_geometry_instances_data_khr = {};
        vk_acceleration_structure_geometry_instances_data_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
        vk_acceleration_structure_geometry_instances_data_khr.pNext = nullptr;
        vk_acceleration_structure_geometry_instances_data_khr.arrayOfPointers = VK_FALSE;
        vk_acceleration_structure_geometry_instances_data_khr.data.deviceAddress = instance_buffer_device_address;

        VkAccelerationStructureGeometryKHR top_level_acceleration_structure_geometry_khr = {};
        top_level_acceleration_structure_geometry_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
        top_level_acceleration_structure_geometry_khr.pNext = nullptr;
        top_level_acceleration_structure_geometry_khr.geometryType = VkGeometryTypeKHR::VK_GEOMETRY_TYPE_INSTANCES_KHR;
        top_level_acceleration_structure_geometry_khr.geometry.instances = vk_acceleration_structure_geometry_instances_data_khr;
        top_level_acceleration_structure_geometry_khr.flags = 0;

        VkAccelerationStructureBuildGeometryInfoKHR top_level_acceleration_structure_build_geometry_info_khr = {};
        top_level_acceleration_structure_build_geometry_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
        top_level_acceleration_structure_build_geometry_info_khr.pNext = nullptr;
        top_level_acceleration_structure_build_geometry_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
        top_level_acceleration_structure_build_geometry_info_khr.flags = VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR;
        top_level_acceleration_structure_build_geometry_info_khr.mode = VkBuildAccelerationStructureModeKHR::VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
        top_level_acceleration_structure_build_geometry_info_khr.srcAccelerationStructure = VK_NULL_HANDLE;
        top_level_acceleration_structure_build_geometry_info_khr.dstAccelerationStructure = VK_NULL_HANDLE;
        top_level_acceleration_structure_build_geometry_info_khr.geometryCount = 1;
        top_level_acceleration_structure_build_geometry_info_khr.pGeometries = &top_level_acceleration_structure_geometry_khr;
        top_level_acceleration_structure_build_geometry_info_khr.ppGeometries = nullptr;
        top_level_acceleration_structure_build_geometry_info_khr.scratchData.deviceAddress = 0;
        top_level_acceleration_structure_build_geometry_info_khr.scratchData.hostAddress = 0;

        uint32_t instance_count = 1;
        VkAccelerationStructureBuildSizesInfoKHR top_level_acceleration_structure_build_sizes_info_khr = {};
        top_level_acceleration_structure_build_sizes_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
        top_level_acceleration_structure_build_sizes_info_khr.pNext = nullptr;
        top_level_acceleration_structure_build_sizes_info_khr.accelerationStructureSize = 0;
        top_level_acceleration_structure_build_sizes_info_khr.updateScratchSize = 0;
        top_level_acceleration_structure_build_sizes_info_khr.buildScratchSize = 0;

        device->GetDeviceDriver()->vkGetAccelerationStructureBuildSizesKHR(vk_device, VkAccelerationStructureBuildTypeKHR::VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &top_level_acceleration_structure_build_geometry_info_khr, &instance_count, &top_level_acceleration_structure_build_sizes_info_khr);

        std::cout << "Top Level VkAccelerationStructureBuildSizesInfoKHR.accelerationStructureSize = " << top_level_acceleration_structure_build_sizes_info_khr.accelerationStructureSize << std::endl;
        std::cout << "Top Level VkAccelerationStructureBuildSizesInfoKHR.updateScratchSize = " << top_level_acceleration_structure_build_sizes_info_khr.updateScratchSize << std::endl;
        std::cout << "Top Level VkAccelerationStructureBuildSizesInfoKHR.buildScratchSize = " << top_level_acceleration_structure_build_sizes_info_khr.buildScratchSize << std::endl;

        top_level_acceleration_structure_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_STORAGE | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, top_level_acceleration_structure_build_sizes_info_khr.accelerationStructureSize);

        VkAccelerationStructureCreateInfoKHR top_level_acceleration_structure_create_info_khr = {};
        top_level_acceleration_structure_create_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
        top_level_acceleration_structure_create_info_khr.pNext = nullptr;
        top_level_acceleration_structure_create_info_khr.createFlags = 0;
        top_level_acceleration_structure_create_info_khr.buffer = top_level_acceleration_structure_buffer->GetVkBuffer();
        top_level_acceleration_structure_create_info_khr.offset = 0;
        top_level_acceleration_structure_create_info_khr.size = top_level_acceleration_structure_build_sizes_info_khr.accelerationStructureSize;
        top_level_acceleration_structure_create_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
        top_level_acceleration_structure_create_info_khr.deviceAddress = 0;

        result = device->GetDeviceDriver()->vkCreateAccelerationStructureKHR(vk_device, &top_level_acceleration_structure_create_info_khr, vk_allocation_callbacks, &top_level_acceleration_structure_khr);
        if (result != VkResult::VK_SUCCESS)
        {
            std::cout << "vkCreateAccelerationStructureKHR create Top Level VkAccelerationStructureKHR Failed" << std::endl;
        }

        std::cout << "vkCreateAccelerationStructureKHR create Top Level VkAccelerationStructureKHR Success" << std::endl;

        Turbo::Core::TBuffer *top_level_scratch_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_STORAGE_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, top_level_acceleration_structure_build_sizes_info_khr.buildScratchSize);

        VkBufferDeviceAddressInfo top_level_scratch_buffer_device_address_info = {};
        top_level_scratch_buffer_device_address_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        top_level_scratch_buffer_device_address_info.pNext = nullptr;
        top_level_scratch_buffer_device_address_info.buffer = top_level_scratch_buffer->GetVkBuffer();

        VkDeviceAddress top_level_scratch_buffer_device_address = 0;
        if (device_driver->vkGetBufferDeviceAddress != nullptr)
        {
            top_level_scratch_buffer_device_address = device_driver->vkGetBufferDeviceAddress(vk_device, &top_level_scratch_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressKHR != nullptr)
        {
            top_level_scratch_buffer_device_address = device_driver->vkGetBufferDeviceAddressKHR(vk_device, &top_level_scratch_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressEXT != nullptr)
        {
            top_level_scratch_buffer_device_address = device_driver->vkGetBufferDeviceAddressEXT(vk_device, &top_level_scratch_buffer_device_address_info);
        }

        if (top_level_scratch_buffer_device_address != 0)
        {
            std::cout << "Successfully get top_level_scratch_buffer VkBuffer device local address " << std::endl;
        }

        // Build Top Level Acceleration Structure
        top_level_acceleration_structure_build_geometry_info_khr.dstAccelerationStructure = top_level_acceleration_structure_khr;
        top_level_acceleration_structure_build_geometry_info_khr.scratchData.deviceAddress = top_level_scratch_buffer_device_address;

        VkAccelerationStructureBuildRangeInfoKHR top_level_acceleration_structure_build_range_info_khr = {};
        top_level_acceleration_structure_build_range_info_khr.primitiveCount = 1;
        top_level_acceleration_structure_build_range_info_khr.primitiveOffset = 0;
        top_level_acceleration_structure_build_range_info_khr.firstVertex = 0;
        top_level_acceleration_structure_build_range_info_khr.transformOffset = 0;

        std::vector<VkAccelerationStructureBuildRangeInfoKHR> top_level_acceleration_structure_build_range_info_khrs;
        top_level_acceleration_structure_build_range_info_khrs.push_back(top_level_acceleration_structure_build_range_info_khr);

        std::vector<VkAccelerationStructureBuildRangeInfoKHR *> top_level_acceleration_structure_ppBuildRangeInfos;
        top_level_acceleration_structure_ppBuildRangeInfos.push_back(top_level_acceleration_structure_build_range_info_khrs.data());

        {
            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();

            command_buffer->Begin();
            device->GetDeviceDriver()->vkCmdBuildAccelerationStructuresKHR(command_buffer->GetVkCommandBuffer(), 1, &top_level_acceleration_structure_build_geometry_info_khr, top_level_acceleration_structure_ppBuildRangeInfos.data());
            command_buffer->End();

            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            delete fence;
            command_pool->Free(command_buffer);
            delete command_pool;
        }

        // Top Level Acceleration Structure Compact
        {
            VkQueryPool query_pool = VK_NULL_HANDLE;

            VkQueryPoolCreateInfo vk_query_pool_create_info = {};
            vk_query_pool_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
            vk_query_pool_create_info.pNext = nullptr;
            vk_query_pool_create_info.flags = 0;
            vk_query_pool_create_info.queryType = VkQueryType::VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR;
            vk_query_pool_create_info.queryCount = 1;
            vk_query_pool_create_info.pipelineStatistics = 0;

            result = device->GetDeviceDriver()->vkCreateQueryPool(vk_device, &vk_query_pool_create_info, vk_allocation_callbacks, &query_pool);
            if (result != VK_SUCCESS)
            {
                std::cout << "Create VkQueryPool Failed" << std::endl;
            }

            std::cout << "Create VkQueryPool Success" << std::endl;

            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            device->GetDeviceDriver()->vkCmdResetQueryPool(command_buffer->GetVkCommandBuffer(), query_pool, 0, 1);
            device->GetDeviceDriver()->vkCmdWriteAccelerationStructuresPropertiesKHR(command_buffer->GetVkCommandBuffer(), 1, &top_level_acceleration_structure_khr, VkQueryType::VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR, query_pool, 0);
            command_buffer->End();
            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            VkDeviceSize compact_size = 0;
            device->GetDeviceDriver()->vkGetQueryPoolResults(vk_device, query_pool, 0, 1, sizeof(VkDeviceSize), &compact_size, sizeof(VkDeviceSize), VK_QUERY_RESULT_WAIT_BIT | VK_QUERY_RESULT_64_BIT);
            std::cout << "top level acceleration structure compact size:" << compact_size << std::endl;

            delete fence;
            command_pool->Free(command_buffer);

            VkAccelerationStructureKHR compact_acceleration_structure_khr = VK_NULL_HANDLE;

            Turbo::Core::TBuffer *compact_acceleration_structure_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_STORAGE | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, compact_size);

            VkAccelerationStructureCreateInfoKHR compact_vk_acceleration_structure_create_info_khr = {};
            compact_vk_acceleration_structure_create_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
            compact_vk_acceleration_structure_create_info_khr.pNext = nullptr;
            compact_vk_acceleration_structure_create_info_khr.createFlags = 0;
            compact_vk_acceleration_structure_create_info_khr.buffer = compact_acceleration_structure_buffer->GetVkBuffer();
            compact_vk_acceleration_structure_create_info_khr.offset = 0;
            compact_vk_acceleration_structure_create_info_khr.size = compact_size;
            compact_vk_acceleration_structure_create_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
            compact_vk_acceleration_structure_create_info_khr.deviceAddress = 0;

            result = device->GetDeviceDriver()->vkCreateAccelerationStructureKHR(vk_device, &compact_vk_acceleration_structure_create_info_khr, vk_allocation_callbacks, &compact_acceleration_structure_khr);
            if (result != VkResult::VK_SUCCESS)
            {
                std::cout << "vkCreateAccelerationStructureKHR create compact VkAccelerationStructureKHR Failed" << std::endl;
            }

            std::cout << "vkCreateAccelerationStructureKHR create compact VkAccelerationStructureKHR Success" << std::endl;

            VkCopyAccelerationStructureInfoKHR vk_copy_acceleration_structure_info_khr = {};
            vk_copy_acceleration_structure_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR;
            vk_copy_acceleration_structure_info_khr.pNext = nullptr;
            vk_copy_acceleration_structure_info_khr.src = top_level_acceleration_structure_khr;
            vk_copy_acceleration_structure_info_khr.dst = compact_acceleration_structure_khr;
            vk_copy_acceleration_structure_info_khr.mode = VkCopyAccelerationStructureModeKHR::VK_COPY_ACCELERATION_STRUCTURE_MODE_COMPACT_KHR;

            command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            device->GetDeviceDriver()->vkCmdCopyAccelerationStructureKHR(command_buffer->GetVkCommandBuffer(), &vk_copy_acceleration_structure_info_khr);
            command_buffer->End();
            fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();
            delete fence;
            command_pool->Free(command_buffer);
            delete command_pool;
            device->GetDeviceDriver()->vkDestroyQueryPool(vk_device, query_pool, vk_allocation_callbacks);

            // destroy no compact acceleration structure
            device->GetDeviceDriver()->vkDestroyAccelerationStructureKHR(vk_device, top_level_acceleration_structure_khr, vk_allocation_callbacks);
            delete top_level_acceleration_structure_buffer;

            // set acceleration structure value to compact version
            top_level_acceleration_structure_khr = compact_acceleration_structure_khr;
            top_level_acceleration_structure_buffer = compact_acceleration_structure_buffer;
        }

        delete top_level_scratch_buffer;
        delete scratch_buffer;
    }

    // Ray Tracing Pipeline
    Turbo::Core::TRayGenerationShader *ray_generation_shader_test = nullptr;
    Turbo::Core::TMissShader *miss_shader_test = nullptr;
    Turbo::Core::TClosestHitShader *closest_hit_shader_test = nullptr;

    Turbo::Core::TMissShader *shadow_miss_shader_test = nullptr;

    VkDescriptorSetLayout ray_tracing_descriptor_set_layout = VK_NULL_HANDLE;
    VkDescriptorSet ray_tracing_descriptor_set = VK_NULL_HANDLE;
    VkPipelineLayout ray_tracing_pipeline_layout = VK_NULL_HANDLE;
    VkPipeline ray_tracing_pipeline = VK_NULL_HANDLE;

    VkStridedDeviceAddressRegionKHR ray_generation_binding_table = {};
    VkStridedDeviceAddressRegionKHR miss_binding_table = {};
    VkStridedDeviceAddressRegionKHR closest_hit_binding_table = {};
    VkStridedDeviceAddressRegionKHR callable_binding_table = {}; /*Not used but need statement because vkCmdTraceRaysKHR(..., const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, ...) must be a valid pointer to a valid VkStridedDeviceAddressRegionKHR structure <https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap40.html#VUID-vkCmdTraceRaysKHR-pCallableShaderBindingTable-parameter>*/

    Turbo::Core::TBuffer *sbt_buffer = nullptr;
    {
        VkDescriptorSetLayoutBinding vk_ray_tracing_binding_acceleration_structure = {};
        vk_ray_tracing_binding_acceleration_structure.binding = 0;
        vk_ray_tracing_binding_acceleration_structure.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
        vk_ray_tracing_binding_acceleration_structure.descriptorCount = 1;
        vk_ray_tracing_binding_acceleration_structure.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_RAYGEN_BIT_KHR | VkShaderStageFlagBits::VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
        vk_ray_tracing_binding_acceleration_structure.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutBinding vk_ray_tracing_binding_storage_image = {};
        vk_ray_tracing_binding_storage_image.binding = 1;
        vk_ray_tracing_binding_storage_image.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        vk_ray_tracing_binding_storage_image.descriptorCount = 1;
        vk_ray_tracing_binding_storage_image.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_RAYGEN_BIT_KHR;
        vk_ray_tracing_binding_storage_image.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutBinding vk_ray_tracing_binding_matrixs = {};
        vk_ray_tracing_binding_matrixs.binding = 2;
        vk_ray_tracing_binding_matrixs.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        vk_ray_tracing_binding_matrixs.descriptorCount = 1;
        vk_ray_tracing_binding_matrixs.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_RAYGEN_BIT_KHR;
        vk_ray_tracing_binding_matrixs.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutBinding vk_ray_tracing_binding_bottom_level_acceleration_structure_device_address = {};
        vk_ray_tracing_binding_bottom_level_acceleration_structure_device_address.binding = 3;
        vk_ray_tracing_binding_bottom_level_acceleration_structure_device_address.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        vk_ray_tracing_binding_bottom_level_acceleration_structure_device_address.descriptorCount = 1;
        vk_ray_tracing_binding_bottom_level_acceleration_structure_device_address.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
        vk_ray_tracing_binding_bottom_level_acceleration_structure_device_address.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutBinding vk_ray_tracing_binding_texture = {};
        vk_ray_tracing_binding_texture.binding = 4;
        vk_ray_tracing_binding_texture.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        vk_ray_tracing_binding_texture.descriptorCount = 1;
        vk_ray_tracing_binding_texture.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
        vk_ray_tracing_binding_texture.pImmutableSamplers = nullptr;

        std::vector<VkDescriptorSetLayoutBinding> ray_tracing_descriptor_set_layout_bindings = {};
        ray_tracing_descriptor_set_layout_bindings.push_back(vk_ray_tracing_binding_acceleration_structure);
        ray_tracing_descriptor_set_layout_bindings.push_back(vk_ray_tracing_binding_storage_image);
        ray_tracing_descriptor_set_layout_bindings.push_back(vk_ray_tracing_binding_matrixs);
        ray_tracing_descriptor_set_layout_bindings.push_back(vk_ray_tracing_binding_bottom_level_acceleration_structure_device_address);
        ray_tracing_descriptor_set_layout_bindings.push_back(vk_ray_tracing_binding_texture);

        VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info = {};
        descriptor_set_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptor_set_layout_create_info.pNext = nullptr;
        descriptor_set_layout_create_info.flags = 0;
        descriptor_set_layout_create_info.bindingCount = ray_tracing_descriptor_set_layout_bindings.size();
        descriptor_set_layout_create_info.pBindings = ray_tracing_descriptor_set_layout_bindings.data();

        VkAllocationCallbacks *vk_allocation_callbacks = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
        VkResult ray_tracing_descriptor_set_layout_create_result = device_driver->vkCreateDescriptorSetLayout(device->GetVkDevice(), &descriptor_set_layout_create_info, vk_allocation_callbacks, &ray_tracing_descriptor_set_layout);
        if (ray_tracing_descriptor_set_layout_create_result != VkResult::VK_SUCCESS)
        {
            throw std::runtime_error("Create ray tracing descriptor set layout failed");
        }

        VkDescriptorSetAllocateInfo ray_tracing_descriptor_set_allocate_info = {};
        ray_tracing_descriptor_set_allocate_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        ray_tracing_descriptor_set_allocate_info.pNext = nullptr;
        ray_tracing_descriptor_set_allocate_info.descriptorPool = descriptor_pool->GetVkDescriptorPool();
        ray_tracing_descriptor_set_allocate_info.descriptorSetCount = 1;
        ray_tracing_descriptor_set_allocate_info.pSetLayouts = &ray_tracing_descriptor_set_layout;

        VkResult ray_tracing_descriptor_sets_allocate_result = device_driver->vkAllocateDescriptorSets(device->GetVkDevice(), &ray_tracing_descriptor_set_allocate_info, &ray_tracing_descriptor_set);
        if (ray_tracing_descriptor_sets_allocate_result != VkResult::VK_SUCCESS)
        {
            throw std::runtime_error("Allocate ray tracing descriptor set failed");
        }

        VkWriteDescriptorSetAccelerationStructureKHR vk_write_descriptor_set_acceleration_structure_khr = {};
        vk_write_descriptor_set_acceleration_structure_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
        vk_write_descriptor_set_acceleration_structure_khr.pNext = nullptr;
        vk_write_descriptor_set_acceleration_structure_khr.accelerationStructureCount = 1;
        vk_write_descriptor_set_acceleration_structure_khr.pAccelerationStructures = &top_level_acceleration_structure_khr;

        VkWriteDescriptorSet ray_tracing_write_acceleration_structure = {};
        ray_tracing_write_acceleration_structure.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        ray_tracing_write_acceleration_structure.pNext = &vk_write_descriptor_set_acceleration_structure_khr;
        ray_tracing_write_acceleration_structure.dstSet = ray_tracing_descriptor_set;
        ray_tracing_write_acceleration_structure.dstBinding = 0;
        ray_tracing_write_acceleration_structure.dstArrayElement = 0;
        ray_tracing_write_acceleration_structure.descriptorCount = 1;
        ray_tracing_write_acceleration_structure.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
        ray_tracing_write_acceleration_structure.pImageInfo = nullptr;
        ray_tracing_write_acceleration_structure.pBufferInfo = nullptr;
        ray_tracing_write_acceleration_structure.pTexelBufferView = nullptr;

        VkDescriptorImageInfo vk_descriptor_image_info = {};
        vk_descriptor_image_info.sampler = VK_NULL_HANDLE;
        vk_descriptor_image_info.imageView = ray_tracing_image_view->GetVkImageView();
        vk_descriptor_image_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_GENERAL;

        VkWriteDescriptorSet ray_tracing_write_image = {};
        ray_tracing_write_image.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        ray_tracing_write_image.pNext = nullptr;
        ray_tracing_write_image.dstSet = ray_tracing_descriptor_set;
        ray_tracing_write_image.dstBinding = 1;
        ray_tracing_write_image.dstArrayElement = 0;
        ray_tracing_write_image.descriptorCount = 1;
        ray_tracing_write_image.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        ray_tracing_write_image.pImageInfo = &vk_descriptor_image_info;
        ray_tracing_write_image.pBufferInfo = nullptr;
        ray_tracing_write_image.pTexelBufferView = nullptr;

        VkDescriptorBufferInfo vk_descriptor_matrixs_buffer_info = {};
        vk_descriptor_matrixs_buffer_info.buffer = matrixs_buffer->GetVkBuffer();
        vk_descriptor_matrixs_buffer_info.offset = 0;
        vk_descriptor_matrixs_buffer_info.range = VK_WHOLE_SIZE;

        VkWriteDescriptorSet ray_tracing_write_matrixs = {};
        ray_tracing_write_matrixs.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        ray_tracing_write_matrixs.pNext = nullptr;
        ray_tracing_write_matrixs.dstSet = ray_tracing_descriptor_set;
        ray_tracing_write_matrixs.dstBinding = 2;
        ray_tracing_write_matrixs.dstArrayElement = 0;
        ray_tracing_write_matrixs.descriptorCount = 1;
        ray_tracing_write_matrixs.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        ray_tracing_write_matrixs.pImageInfo = nullptr;
        ray_tracing_write_matrixs.pBufferInfo = &vk_descriptor_matrixs_buffer_info;
        ray_tracing_write_matrixs.pTexelBufferView = nullptr;

        VkDescriptorBufferInfo vk_descriptor_bottom_level_acceleration_structure_device_address_buffer_info = {};
        vk_descriptor_bottom_level_acceleration_structure_device_address_buffer_info.buffer = bottom_level_acceleration_structure_device_address_buffer->GetVkBuffer();
        vk_descriptor_bottom_level_acceleration_structure_device_address_buffer_info.offset = 0;
        vk_descriptor_bottom_level_acceleration_structure_device_address_buffer_info.range = VK_WHOLE_SIZE;

        VkWriteDescriptorSet ray_tracing_write_bottom_level_acceleration_structure_device_address_buffer = {};
        ray_tracing_write_bottom_level_acceleration_structure_device_address_buffer.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        ray_tracing_write_bottom_level_acceleration_structure_device_address_buffer.pNext = nullptr;
        ray_tracing_write_bottom_level_acceleration_structure_device_address_buffer.dstSet = ray_tracing_descriptor_set;
        ray_tracing_write_bottom_level_acceleration_structure_device_address_buffer.dstBinding = 3;
        ray_tracing_write_bottom_level_acceleration_structure_device_address_buffer.dstArrayElement = 0;
        ray_tracing_write_bottom_level_acceleration_structure_device_address_buffer.descriptorCount = 1;
        ray_tracing_write_bottom_level_acceleration_structure_device_address_buffer.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        ray_tracing_write_bottom_level_acceleration_structure_device_address_buffer.pImageInfo = nullptr;
        ray_tracing_write_bottom_level_acceleration_structure_device_address_buffer.pBufferInfo = &vk_descriptor_bottom_level_acceleration_structure_device_address_buffer_info;
        ray_tracing_write_bottom_level_acceleration_structure_device_address_buffer.pTexelBufferView = nullptr;

        VkDescriptorImageInfo vk_descriptor_texture_info = {};
        vk_descriptor_texture_info.sampler = sampler->GetVkSampler();
        vk_descriptor_texture_info.imageView = ktx_texture_view->GetVkImageView();
        vk_descriptor_texture_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkWriteDescriptorSet ray_tracing_write_texture = {};
        ray_tracing_write_texture.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        ray_tracing_write_texture.pNext = nullptr;
        ray_tracing_write_texture.dstSet = ray_tracing_descriptor_set;
        ray_tracing_write_texture.dstBinding = 4;
        ray_tracing_write_texture.dstArrayElement = 0;
        ray_tracing_write_texture.descriptorCount = 1;
        ray_tracing_write_texture.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        ray_tracing_write_texture.pImageInfo = &vk_descriptor_texture_info;
        ray_tracing_write_texture.pBufferInfo = nullptr;
        ray_tracing_write_texture.pTexelBufferView = nullptr;

        std::vector<VkWriteDescriptorSet> vk_write_descriptor_sets;
        vk_write_descriptor_sets.push_back(ray_tracing_write_acceleration_structure);
        vk_write_descriptor_sets.push_back(ray_tracing_write_image);
        vk_write_descriptor_sets.push_back(ray_tracing_write_matrixs);
        vk_write_descriptor_sets.push_back(ray_tracing_write_bottom_level_acceleration_structure_device_address_buffer);
        vk_write_descriptor_sets.push_back(ray_tracing_write_texture);

        device_driver->vkUpdateDescriptorSets(device->GetVkDevice(), vk_write_descriptor_sets.size(), vk_write_descriptor_sets.data(), 0, nullptr);

        VkPushConstantRange ray_tracing_pipeline_push_constant_range = {};
        ray_tracing_pipeline_push_constant_range.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_RAYGEN_BIT_KHR;
        ray_tracing_pipeline_push_constant_range.offset = 0;
        ray_tracing_pipeline_push_constant_range.size = sizeof(RAY_TRACING_PUSH_CONSTANT);

        VkPipelineLayoutCreateInfo ray_tracing_pipeline_layout_create_info = {};
        ray_tracing_pipeline_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        ray_tracing_pipeline_layout_create_info.pNext = nullptr;
        ray_tracing_pipeline_layout_create_info.flags = 0;
        ray_tracing_pipeline_layout_create_info.setLayoutCount = 1;
        ray_tracing_pipeline_layout_create_info.pSetLayouts = &ray_tracing_descriptor_set_layout;
        ray_tracing_pipeline_layout_create_info.pushConstantRangeCount = 1;
        ray_tracing_pipeline_layout_create_info.pPushConstantRanges = &ray_tracing_pipeline_push_constant_range;

        VkResult ray_tracing_pipeline_layout_create_result = device_driver->vkCreatePipelineLayout(device->GetVkDevice(), &ray_tracing_pipeline_layout_create_info, vk_allocation_callbacks, &ray_tracing_pipeline_layout);
        if (ray_tracing_pipeline_layout_create_result != VkResult::VK_SUCCESS)
        {
            throw std::runtime_error("Create ray tracing pipeline layout failed");
        }

        ray_generation_shader_test = new Turbo::Core::TRayGenerationShader(device, Turbo::Core::TShaderLanguage::GLSL, RAY_GENERATION_SHADER_STR, {SHADER_INCLUDE_PATH});
        miss_shader_test = new Turbo::Core::TMissShader(device, Turbo::Core::TShaderLanguage::GLSL, MISS_SHADER_STR);
        shadow_miss_shader_test = new Turbo::Core::TMissShader(device, Turbo::Core::TShaderLanguage::GLSL, SHADOW_MISS_SHADER_STR);
        closest_hit_shader_test = new Turbo::Core::TClosestHitShader(device, Turbo::Core::TShaderLanguage::GLSL, CLOSEST_HIT_SHADER_STR);

        VkPipelineShaderStageCreateInfo ray_generation_shader_stage_create_info = {};
        ray_generation_shader_stage_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        ray_generation_shader_stage_create_info.pNext = nullptr;
        ray_generation_shader_stage_create_info.flags = 0;
        ray_generation_shader_stage_create_info.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_RAYGEN_BIT_KHR;
        ray_generation_shader_stage_create_info.module = ray_generation_shader_test->GetVkShaderModule();
        ray_generation_shader_stage_create_info.pName = "main";
        ray_generation_shader_stage_create_info.pSpecializationInfo = nullptr;

        VkPipelineShaderStageCreateInfo miss_shader_stage_create_info = {};
        miss_shader_stage_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        miss_shader_stage_create_info.pNext = nullptr;
        miss_shader_stage_create_info.flags = 0;
        miss_shader_stage_create_info.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_MISS_BIT_KHR;
        miss_shader_stage_create_info.module = miss_shader_test->GetVkShaderModule();
        miss_shader_stage_create_info.pName = "main";
        miss_shader_stage_create_info.pSpecializationInfo = nullptr;

        VkPipelineShaderStageCreateInfo shadow_miss_shader_stage_create_info = {};
        shadow_miss_shader_stage_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shadow_miss_shader_stage_create_info.pNext = nullptr;
        shadow_miss_shader_stage_create_info.flags = 0;
        shadow_miss_shader_stage_create_info.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_MISS_BIT_KHR;
        shadow_miss_shader_stage_create_info.module = shadow_miss_shader_test->GetVkShaderModule();
        shadow_miss_shader_stage_create_info.pName = "main";
        shadow_miss_shader_stage_create_info.pSpecializationInfo = nullptr;

        VkPipelineShaderStageCreateInfo closest_hit_shader_stage_create_info = {};
        closest_hit_shader_stage_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        closest_hit_shader_stage_create_info.pNext = nullptr;
        closest_hit_shader_stage_create_info.flags = 0;
        closest_hit_shader_stage_create_info.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
        closest_hit_shader_stage_create_info.module = closest_hit_shader_test->GetVkShaderModule();
        closest_hit_shader_stage_create_info.pName = "main";
        closest_hit_shader_stage_create_info.pSpecializationInfo = nullptr;

        std::vector<VkPipelineShaderStageCreateInfo> ray_tracing_pipeline_shader_stages = {};
        ray_tracing_pipeline_shader_stages.push_back(ray_generation_shader_stage_create_info);
        ray_tracing_pipeline_shader_stages.push_back(miss_shader_stage_create_info);
        ray_tracing_pipeline_shader_stages.push_back(shadow_miss_shader_stage_create_info);
        ray_tracing_pipeline_shader_stages.push_back(closest_hit_shader_stage_create_info);

        VkRayTracingShaderGroupCreateInfoKHR ray_generation_shader_group = {};
        ray_generation_shader_group.sType = VkStructureType::VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        ray_generation_shader_group.pNext = nullptr;
        ray_generation_shader_group.type = VkRayTracingShaderGroupTypeKHR::VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        ray_generation_shader_group.generalShader = 0;
        ray_generation_shader_group.closestHitShader = VK_SHADER_UNUSED_KHR;
        ray_generation_shader_group.anyHitShader = VK_SHADER_UNUSED_KHR;
        ray_generation_shader_group.intersectionShader = VK_SHADER_UNUSED_KHR;
        ray_generation_shader_group.pShaderGroupCaptureReplayHandle = nullptr;

        VkRayTracingShaderGroupCreateInfoKHR miss_shader_group = {};
        miss_shader_group.sType = VkStructureType::VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        miss_shader_group.pNext = nullptr;
        miss_shader_group.type = VkRayTracingShaderGroupTypeKHR::VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        miss_shader_group.generalShader = 1;
        miss_shader_group.closestHitShader = VK_SHADER_UNUSED_KHR;
        miss_shader_group.anyHitShader = VK_SHADER_UNUSED_KHR;
        miss_shader_group.intersectionShader = VK_SHADER_UNUSED_KHR;
        miss_shader_group.pShaderGroupCaptureReplayHandle = nullptr;

        VkRayTracingShaderGroupCreateInfoKHR shadow_miss_shader_group = {};
        shadow_miss_shader_group.sType = VkStructureType::VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        shadow_miss_shader_group.pNext = nullptr;
        shadow_miss_shader_group.type = VkRayTracingShaderGroupTypeKHR::VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        shadow_miss_shader_group.generalShader = 2;
        shadow_miss_shader_group.closestHitShader = VK_SHADER_UNUSED_KHR;
        shadow_miss_shader_group.anyHitShader = VK_SHADER_UNUSED_KHR;
        shadow_miss_shader_group.intersectionShader = VK_SHADER_UNUSED_KHR;
        shadow_miss_shader_group.pShaderGroupCaptureReplayHandle = nullptr;

        VkRayTracingShaderGroupCreateInfoKHR close_hit_shader_group = {};
        close_hit_shader_group.sType = VkStructureType::VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        close_hit_shader_group.pNext = nullptr;
        close_hit_shader_group.type = VkRayTracingShaderGroupTypeKHR::VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
        close_hit_shader_group.generalShader = VK_SHADER_UNUSED_KHR;
        close_hit_shader_group.closestHitShader = 3;
        close_hit_shader_group.anyHitShader = VK_SHADER_UNUSED_KHR;
        close_hit_shader_group.intersectionShader = VK_SHADER_UNUSED_KHR;
        close_hit_shader_group.pShaderGroupCaptureReplayHandle = nullptr;

        std::vector<VkRayTracingShaderGroupCreateInfoKHR> shader_groups = {};
        shader_groups.push_back(ray_generation_shader_group);
        shader_groups.push_back(miss_shader_group);
        shader_groups.push_back(shadow_miss_shader_group);
        shader_groups.push_back(close_hit_shader_group);

        VkRayTracingPipelineCreateInfoKHR vk_ray_tracing_pipeline_create_info_khr = {};
        vk_ray_tracing_pipeline_create_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
        vk_ray_tracing_pipeline_create_info_khr.pNext = nullptr;
        vk_ray_tracing_pipeline_create_info_khr.flags = 0;
        vk_ray_tracing_pipeline_create_info_khr.stageCount = ray_tracing_pipeline_shader_stages.size();
        vk_ray_tracing_pipeline_create_info_khr.pStages = ray_tracing_pipeline_shader_stages.data();
        vk_ray_tracing_pipeline_create_info_khr.groupCount = shader_groups.size();
        vk_ray_tracing_pipeline_create_info_khr.pGroups = shader_groups.data();
        vk_ray_tracing_pipeline_create_info_khr.maxPipelineRayRecursionDepth = 2;
        vk_ray_tracing_pipeline_create_info_khr.pLibraryInfo = nullptr;
        vk_ray_tracing_pipeline_create_info_khr.pLibraryInterface = nullptr;
        vk_ray_tracing_pipeline_create_info_khr.pDynamicState = nullptr;
        vk_ray_tracing_pipeline_create_info_khr.layout = ray_tracing_pipeline_layout;
        vk_ray_tracing_pipeline_create_info_khr.basePipelineHandle = VK_NULL_HANDLE;
        vk_ray_tracing_pipeline_create_info_khr.basePipelineIndex = 0;

        VkResult create_ray_tracing_pipeline_result = device_driver->vkCreateRayTracingPipelinesKHR(device->GetVkDevice(), VK_NULL_HANDLE, VK_NULL_HANDLE, 1, &vk_ray_tracing_pipeline_create_info_khr, vk_allocation_callbacks, &ray_tracing_pipeline);
        if (create_ray_tracing_pipeline_result != VkResult::VK_SUCCESS)
        {
            throw std::runtime_error("Create ray tracing pipeline failed");
        }

        // Shader Binding Table
        VkPhysicalDeviceRayTracingPipelinePropertiesKHR vk_physical_device_ray_tracing_pipeline_properties_khr = {};
        vk_physical_device_ray_tracing_pipeline_properties_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
        vk_physical_device_ray_tracing_pipeline_properties_khr.pNext = nullptr;
        vk_physical_device_ray_tracing_pipeline_properties_khr.shaderGroupHandleSize = 0;
        vk_physical_device_ray_tracing_pipeline_properties_khr.maxRayRecursionDepth = 0;
        vk_physical_device_ray_tracing_pipeline_properties_khr.maxShaderGroupStride = 0;
        vk_physical_device_ray_tracing_pipeline_properties_khr.shaderGroupBaseAlignment = 0;
        vk_physical_device_ray_tracing_pipeline_properties_khr.shaderGroupHandleCaptureReplaySize = 0;
        vk_physical_device_ray_tracing_pipeline_properties_khr.maxRayDispatchInvocationCount = 0;
        vk_physical_device_ray_tracing_pipeline_properties_khr.shaderGroupHandleAlignment = 0;
        vk_physical_device_ray_tracing_pipeline_properties_khr.maxRayHitAttributeSize = 0;

        VkPhysicalDeviceProperties2 vk_physical_device_properties_2 = {};
        vk_physical_device_properties_2.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        vk_physical_device_properties_2.pNext = &vk_physical_device_ray_tracing_pipeline_properties_khr;
        vk_physical_device_properties_2.properties = {};

        if (physical_device_driver->vkGetPhysicalDeviceProperties2 != nullptr)
        {
            physical_device_driver->vkGetPhysicalDeviceProperties2(physical_device->GetVkPhysicalDevice(), &vk_physical_device_properties_2);
        }
        else if (physical_device_driver->vkGetPhysicalDeviceProperties2KHR != nullptr)
        {
            physical_device_driver->vkGetPhysicalDeviceProperties2KHR(physical_device->GetVkPhysicalDevice(), &vk_physical_device_properties_2);
        }

        std::cout << "VkPhysicalDeviceRayTracingPipelinePropertiesKHR::shaderGroupHandleSize:" << vk_physical_device_ray_tracing_pipeline_properties_khr.shaderGroupHandleSize << std::endl;
        std::cout << "VkPhysicalDeviceRayTracingPipelinePropertiesKHR::maxRayRecursionDepth:" << vk_physical_device_ray_tracing_pipeline_properties_khr.maxRayRecursionDepth << std::endl;
        std::cout << "VkPhysicalDeviceRayTracingPipelinePropertiesKHR::maxShaderGroupStride:" << vk_physical_device_ray_tracing_pipeline_properties_khr.maxShaderGroupStride << std::endl;
        std::cout << "VkPhysicalDeviceRayTracingPipelinePropertiesKHR::shaderGroupBaseAlignment:" << vk_physical_device_ray_tracing_pipeline_properties_khr.shaderGroupBaseAlignment << std::endl;
        std::cout << "VkPhysicalDeviceRayTracingPipelinePropertiesKHR::shaderGroupHandleCaptureReplaySize:" << vk_physical_device_ray_tracing_pipeline_properties_khr.shaderGroupHandleCaptureReplaySize << std::endl;
        std::cout << "VkPhysicalDeviceRayTracingPipelinePropertiesKHR::maxRayDispatchInvocationCount:" << vk_physical_device_ray_tracing_pipeline_properties_khr.maxRayDispatchInvocationCount << std::endl;
        std::cout << "VkPhysicalDeviceRayTracingPipelinePropertiesKHR::shaderGroupHandleAlignment:" << vk_physical_device_ray_tracing_pipeline_properties_khr.shaderGroupHandleAlignment << std::endl;
        std::cout << "VkPhysicalDeviceRayTracingPipelinePropertiesKHR::maxRayHitAttributeSize:" << vk_physical_device_ray_tracing_pipeline_properties_khr.maxRayHitAttributeSize << std::endl;

        uint32_t ray_generation_count = 1;
        uint32_t miss_count = 2;
        uint32_t closest_hit_count = 1;

        uint32_t handle_count = ray_generation_count + miss_count + closest_hit_count;
        uint32_t handle_size = vk_physical_device_ray_tracing_pipeline_properties_khr.shaderGroupHandleSize;
        uint32_t shader_group_handle_alignment = vk_physical_device_ray_tracing_pipeline_properties_khr.shaderGroupHandleAlignment;
        uint32_t shader_group_base_alignment = vk_physical_device_ray_tracing_pipeline_properties_khr.shaderGroupBaseAlignment;

        uint32_t handle_size_aligned = Turbo::Core::TVulkanAllocator::AlignUp(handle_size, shader_group_handle_alignment);

        ray_generation_binding_table.deviceAddress = 0;
        ray_generation_binding_table.stride = Turbo::Core::TVulkanAllocator::AlignUp(handle_size_aligned, shader_group_base_alignment);
        ray_generation_binding_table.size = ray_generation_binding_table.stride; // The size member of pRayGenShaderBindingTable must be equal to its stride member (https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap40.html#VUID-vkCmdTraceRaysKHR-size-04023)

        miss_binding_table.deviceAddress = 0;
        miss_binding_table.stride = handle_size_aligned;
        miss_binding_table.size = Turbo::Core::TVulkanAllocator::AlignUp(miss_count * handle_size_aligned, shader_group_base_alignment);

        closest_hit_binding_table.deviceAddress = 0;
        closest_hit_binding_table.stride = handle_size_aligned;
        closest_hit_binding_table.size = Turbo::Core::TVulkanAllocator::AlignUp(closest_hit_count * handle_size_aligned, shader_group_base_alignment);

        uint32_t data_size = handle_count * handle_size_aligned;
        std::vector<uint8_t> handles(data_size);

        VkResult vk_get_ray_tracing_shader_group_handles_khr_result = device_driver->vkGetRayTracingShaderGroupHandlesKHR(device->GetVkDevice(), ray_tracing_pipeline, 0, handle_count, data_size, handles.data());
        if (vk_get_ray_tracing_shader_group_handles_khr_result != VkResult::VK_SUCCESS)
        {
            throw std::runtime_error("Get shader group handle failed");
        }

        VkDeviceSize sbt_buffer_size = ray_generation_binding_table.size + miss_binding_table.size + closest_hit_binding_table.size;
        sbt_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_BINDING_TABLE, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sbt_buffer_size);

        VkBufferDeviceAddressInfo sbt_buffer_device_address_info = {};
        sbt_buffer_device_address_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        sbt_buffer_device_address_info.pNext = nullptr;
        sbt_buffer_device_address_info.buffer = sbt_buffer->GetVkBuffer();

        VkDeviceAddress sbt_buffer_device_address = 0;
        if (device_driver->vkGetBufferDeviceAddress != nullptr)
        {
            sbt_buffer_device_address = device_driver->vkGetBufferDeviceAddress(device->GetVkDevice(), &sbt_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressKHR != nullptr)
        {
            sbt_buffer_device_address = device_driver->vkGetBufferDeviceAddressKHR(device->GetVkDevice(), &sbt_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressEXT != nullptr)
        {
            sbt_buffer_device_address = device_driver->vkGetBufferDeviceAddressEXT(device->GetVkDevice(), &sbt_buffer_device_address_info);
        }

        if (sbt_buffer_device_address == 0)
        {
            throw std::runtime_error("Get shader binding table buffer address failed");
        }

        ray_generation_binding_table.deviceAddress = sbt_buffer_device_address;
        miss_binding_table.deviceAddress = sbt_buffer_device_address + ray_generation_binding_table.size;
        closest_hit_binding_table.deviceAddress = sbt_buffer_device_address + ray_generation_binding_table.size + miss_binding_table.size;

        void *sbt_buffer_point = sbt_buffer->Map();
        // ray generation
        memcpy((uint8_t *)sbt_buffer_point, handles.data() + 0 * handle_size, handle_size);
        // miss
        for (uint32_t miss_index = 0; miss_index < miss_count; miss_index++)
        {
            memcpy((uint8_t *)sbt_buffer_point + ray_generation_binding_table.size + miss_index * miss_binding_table.stride, handles.data() + ray_generation_count * handle_size + miss_index * handle_size, handle_size);
        }
        // closest_hit
        for (uint32_t closest_hit_index = 0; closest_hit_index < closest_hit_count; closest_hit_index++)
        {
            memcpy((uint8_t *)sbt_buffer_point + ray_generation_binding_table.size + miss_binding_table.size + closest_hit_index * closest_hit_binding_table.stride, handles.data() + ray_generation_count * handle_size + miss_count * handle_size + closest_hit_index * handle_size, handle_size);
        }

        sbt_buffer->Unmap();
    }

    std::vector<Turbo::Core::TBuffer *> my_buffers;
    my_buffers.push_back(my_buffer);

    std::vector<Turbo::Core::TBuffer *> matrixs_buffers;
    matrixs_buffers.push_back(matrixs_buffer);

    Turbo::Core::TSubpass subpass(Turbo::Core::TPipelineType::Graphics);
    subpass.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);                // swapchain color image
    subpass.SetDepthStencilAttachmentReference(1, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL); // depth image

    Turbo::Core::TSubpass subpass1(Turbo::Core::TPipelineType::Graphics);
    subpass1.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);               // swapchain color image
    subpass.SetDepthStencilAttachmentReference(1, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL); // depth image

    std::vector<Turbo::Core::TSubpass> subpasses;
    subpasses.push_back(subpass);  // subpass 0
    subpasses.push_back(subpass1); // subpass 1

    Turbo::Core::TAttachment swapchain_color_attachment(swapchain_images[0]->GetFormat(), swapchain_images[0]->GetSampleCountBits(), Turbo::Core::TLoadOp::LOAD, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::PRESENT_SRC_KHR);
    Turbo::Core::TAttachment depth_attachment(depth_image->GetFormat(), depth_image->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    std::vector<Turbo::Core::TAttachment> attachemts;
    attachemts.push_back(swapchain_color_attachment);
    attachemts.push_back(depth_attachment);

    Turbo::Core::TRenderPass *render_pass = new Turbo::Core::TRenderPass(device, attachemts, subpasses);

    Turbo::Core::TVertexBinding position_binding(0, sizeof(POSITION), Turbo::Core::TVertexRate::VERTEX);
    position_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32B32_SFLOAT, 0); // position
    Turbo::Core::TVertexBinding normal_binding(1, sizeof(NORMAL), Turbo::Core::TVertexRate::VERTEX);
    normal_binding.AddAttribute(1, Turbo::Core::TFormatType::R32G32B32_SFLOAT, 0); // normal
    Turbo::Core::TVertexBinding texcoord_binding(2, sizeof(TEXCOORD), Turbo::Core::TVertexRate::VERTEX);
    texcoord_binding.AddAttribute(2, Turbo::Core::TFormatType::R32G32_SFLOAT, 0); // texcoord/uv
    Turbo::Core::TVertexBinding tangent_binding(3, sizeof(TANGENT), Turbo::Core::TVertexRate::VERTEX);
    tangent_binding.AddAttribute(3, Turbo::Core::TFormatType::R32G32B32A32_SFLOAT, 0); // tangent

    std::vector<Turbo::Core::TVertexBinding> vertex_bindings;
    vertex_bindings.push_back(position_binding);
    vertex_bindings.push_back(normal_binding);
    vertex_bindings.push_back(texcoord_binding);
    vertex_bindings.push_back(tangent_binding);

    Turbo::Core::TViewport viewport(0, 0, surface->GetCurrentWidth(), surface->GetCurrentHeight(), 0, 1);
    Turbo::Core::TScissor scissor(0, 0, surface->GetCurrentWidth(), surface->GetCurrentHeight());

    uint32_t patch_control_points = 3;
    Turbo::Core::TPolygonMode polygon_mode = Turbo::Core::TPolygonMode::FILL;
    Turbo::Core::TGraphicsPipeline *pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 0, vertex_bindings, vertex_shader, geometry_shader, fragment_shader, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, polygon_mode, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, true, true, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, 0, 0, false, Turbo::Core::TLogicOp::NO_OP, true, Turbo::Core::TBlendFactor::SRC_ALPHA, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendOp::ADD, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendFactor::ZERO, Turbo::Core::TBlendOp::ADD);

    std::vector<Turbo::Core::TImageView *> input_attachment_depths;
    input_attachment_depths.push_back(depth_image_view);

    Turbo::Core::TPipelineDescriptorSet *pipeline_descriptor_set = descriptor_pool->Allocate(pipeline->GetPipelineLayout());
    pipeline_descriptor_set->BindData(0, 0, 0, matrixs_buffers);
    pipeline_descriptor_set->BindData(0, 1, 0, my_buffers);

    std::vector<Turbo::Core::TBuffer *> vertex_buffers;
    vertex_buffers.push_back(position_buffer);
    vertex_buffers.push_back(normal_buffer);
    vertex_buffers.push_back(texcoord_buffer);
    vertex_buffers.push_back(tangent_buffer);

    std::vector<Turbo::Core::TFramebuffer *> swpachain_framebuffers;
    for (Turbo::Core::TImageView *swapchain_image_view_item : swapchain_image_views)
    {
        std::vector<Turbo::Core::TImageView *> image_views;
        image_views.push_back(swapchain_image_view_item);
        image_views.push_back(depth_image_view);

        Turbo::Core::TFramebuffer *swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, image_views);
        swpachain_framebuffers.push_back(swapchain_framebuffer);
    }

    //<IMGUI>
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    Turbo::Core::TSampler *imgui_sampler = new Turbo::Core::TSampler(device);
    Turbo::Core::TShader *imgui_vertex_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::VERTEX, Turbo::Core::TShaderLanguage::GLSL, IMGUI_VERT_SHADER_STR);
    Turbo::Core::TShader *imgui_fragment_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::FRAGMENT, Turbo::Core::TShaderLanguage::GLSL, IMGUI_FRAG_SHADER_STR);

    Turbo::Core::TVertexBinding imgui_vertex_binding(0, sizeof(ImDrawVert), Turbo::Core::TVertexRate::VERTEX);
    imgui_vertex_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32_SFLOAT, IM_OFFSETOF(ImDrawVert, pos));  // position
    imgui_vertex_binding.AddAttribute(1, Turbo::Core::TFormatType::R32G32_SFLOAT, IM_OFFSETOF(ImDrawVert, uv));   // uv
    imgui_vertex_binding.AddAttribute(2, Turbo::Core::TFormatType::R8G8B8A8_UNORM, IM_OFFSETOF(ImDrawVert, col)); // color

    std::vector<Turbo::Core::TShader *> imgui_shaders;
    imgui_shaders.push_back(imgui_vertex_shader);
    imgui_shaders.push_back(imgui_fragment_shader);

    std::vector<Turbo::Core::TVertexBinding> imgui_vertex_bindings;
    imgui_vertex_bindings.push_back(imgui_vertex_binding);

    Turbo::Core::TGraphicsPipeline *imgui_pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 1, imgui_vertex_bindings, imgui_shaders, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, false, false, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, 0, 0, false, Turbo::Core::TLogicOp::NO_OP, true, Turbo::Core::TBlendFactor::SRC_ALPHA, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendOp::ADD, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendFactor::ZERO, Turbo::Core::TBlendOp::ADD);

    unsigned char *imgui_font_pixels;
    int imgui_font_width, imgui_font_height;
    io.Fonts->GetTexDataAsRGBA32(&imgui_font_pixels, &imgui_font_width, &imgui_font_height);
    size_t imgui_upload_size = imgui_font_width * imgui_font_height * 4 * sizeof(char);

    Turbo::Core::TImage *imgui_font_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R8G8B8A8_UNORM, imgui_font_width, imgui_font_height, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_SAMPLED | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
    Turbo::Core::TImageView *imgui_font_image_view = new Turbo::Core::TImageView(imgui_font_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, imgui_font_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
    {
        Turbo::Core::TBuffer *imgui_font_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, imgui_upload_size);
        void *imgui_font_ptr = imgui_font_buffer->Map();
        memcpy(imgui_font_ptr, imgui_font_pixels, imgui_upload_size);
        imgui_font_buffer->Unmap();

        Turbo::Core::TCommandBuffer *imgui_copy_command_buffer = command_pool->Allocate();
        imgui_copy_command_buffer->Begin();
        imgui_copy_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::HOST_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::HOST_WRITE_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, imgui_font_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        imgui_copy_command_buffer->CmdCopyBufferToImage(imgui_font_buffer, imgui_font_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, 0, imgui_font_width, imgui_font_height, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1, 0, 0, 0, imgui_font_width, imgui_font_height, 1);
        imgui_copy_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::FRAGMENT_SHADER_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TAccessBits::SHADER_READ_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::SHADER_READ_ONLY_OPTIMAL, imgui_font_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        imgui_copy_command_buffer->End();

        Turbo::Core::TFence *imgui_font_copy_fence = new Turbo::Core::TFence(device);
        queue->Submit(nullptr, nullptr, imgui_copy_command_buffer, imgui_font_copy_fence);

        imgui_font_copy_fence->WaitUntil();

        delete imgui_font_buffer;
        delete imgui_font_copy_fence;
    }

    std::vector<std::pair<Turbo::Core::TImageView *, Turbo::Core::TSampler *>> imgui_combined_image_samplers;
    imgui_combined_image_samplers.push_back(std::make_pair(imgui_font_image_view, imgui_sampler));

    Turbo::Core::TPipelineDescriptorSet *imgui_pipeline_descriptor_set = descriptor_pool->Allocate(imgui_pipeline->GetPipelineLayout());
    imgui_pipeline_descriptor_set->BindData(0, 0, 0, imgui_combined_image_samplers);

    io.Fonts->TexID = (ImTextureID)(intptr_t)(imgui_font_image->GetVkImage());

    Turbo::Core::TBuffer *imgui_vertex_buffer = nullptr;
    Turbo::Core::TBuffer *imgui_index_buffer = nullptr;
    //</IMGUI>

    glm::vec3 camera_position = glm::vec3(1.00025, -1.50862, -2.52088);

    float horizontal_angle = 383.2;
    float vertical_angle = 28;

    glm::vec2 previous_mouse_pos = glm::vec2(0, 0);
    glm::vec2 current_mouse_pos = glm::vec2(0, 0);

    float angle = 0;
    float _time = glfwGetTime();

    bool is_need_refresh_frame = false;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        void *_ptr = my_buffer->Map();
        memcpy(_ptr, &my_buffer_data, sizeof(my_buffer_data));
        my_buffer->Unmap();

        //<Begin Rendering>
        uint32_t current_image_index = UINT32_MAX;
        Turbo::Core::TSemaphore *wait_image_ready = new Turbo::Core::TSemaphore(device, Turbo::Core::TPipelineStageBits::COLOR_ATTACHMENT_OUTPUT_BIT);
        Turbo::Core::TResult result = swapchain->AcquireNextImageUntil(wait_image_ready, nullptr, &current_image_index);
        switch (result)
        {
        case Turbo::Core::TResult::SUCCESS: {
            // successed get image and go on rendering

            // because we just have one command buffer, so we should reset the command buffer for each frame
            // If we create command buffer for each swapchain image, we don't need to reset it each frame

            //<IMGUI Update>
            int window_w, window_h;
            int display_w, display_h;
            glfwGetWindowSize(window, &window_w, &window_h);
            glfwGetFramebufferSize(window, &display_w, &display_h);
            io.DisplaySize = ImVec2((float)window_w, (float)window_h);
            if (window_w > 0 && window_h > 0)
            {
                io.DisplayFramebufferScale = ImVec2((float)display_w / window_w, (float)display_h / window_h);
            }
            double current_time = glfwGetTime();
            io.DeltaTime = _time > 0.0 ? (float)(current_time - _time) : (float)(1.0f / 60.0f);
            _time = current_time;

            // UpdateMousePosAndButtons
            {
                // Update buttons
                ImGuiIO &io = ImGui::GetIO();
                for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
                {
                    // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
                    io.MouseDown[i] = g_MouseJustPressed[i] || glfwGetMouseButton(window, i) != 0;
                    g_MouseJustPressed[i] = false;
                }

                // Update mouse position
                const ImVec2 mouse_pos_backup = io.MousePos;
                io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

                const bool focused = glfwGetWindowAttrib(window, GLFW_FOCUSED) != 0;
                if (focused)
                {
                    if (io.WantSetMousePos)
                    {
                        glfwSetCursorPos(window, (double)mouse_pos_backup.x, (double)mouse_pos_backup.y);
                    }
                    else
                    {
                        double mouse_x, mouse_y;
                        glfwGetCursorPos(window, &mouse_x, &mouse_y);
                        io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
                    }
                }
            }

            // IUpdateMouseCursor
            {
                ImGuiIO &io = ImGui::GetIO();
                if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
                    break;

                ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
                if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
                {
                    // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                }
                else
                {
                    // Show OS mouse cursor
                    // FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with GLFW 3.2, but 3.3 works here.
                    glfwSetCursor(window, g_MouseCursors[imgui_cursor] ? g_MouseCursors[imgui_cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
            }

            // UpdateKeyboard
            {
                ImVec2 mouse_pos = io.MousePos;
                current_mouse_pos = glm::vec2(mouse_pos.x, mouse_pos.y);
                glm::vec2 mouse_pos_delte = current_mouse_pos - previous_mouse_pos;
                previous_mouse_pos = current_mouse_pos;
                mouse_pos_delte.y = mouse_pos_delte.y;

                int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
                if (state == GLFW_PRESS)
                {
                    horizontal_angle += mouse_pos_delte.x * 0.2;
                    vertical_angle += mouse_pos_delte.y * 0.2;

                    if (vertical_angle > 90)
                    {
                        vertical_angle = 90;
                    }

                    if (vertical_angle < -90)
                    {
                        vertical_angle = -90;
                    }
                }

                float delte_time = io.DeltaTime;
                float speed = 1;

                glm::vec3 forward_axis = glm::vec3(0, 0, 1);
                glm::mat4 forward_rotate_mat = glm::rotate(glm::mat4(1), glm::radians(-horizontal_angle), glm::vec3(0, 1, 0));
                forward_rotate_mat = glm::rotate(forward_rotate_mat, glm::radians(-vertical_angle), glm::vec3(1, 0, 0));

                glm::vec3 look_forward = forward_rotate_mat * glm::vec4(forward_axis, 1.0);
                look_forward = glm::normalize(look_forward);

                glm::vec3 forward_dir = look_forward;                  // 向前向量
                glm::vec3 up_dir = glm::vec3(0, 1, 0);                 // 向上向量
                glm::vec3 right_dir = glm::cross(forward_dir, up_dir); // 向右向量
                up_dir = glm::cross(right_dir, forward_dir);

                right_dir = glm::normalize(right_dir);
                up_dir = glm::normalize(up_dir);

                int key_W_state = glfwGetKey(window, GLFW_KEY_W);
                if (key_W_state == GLFW_PRESS)
                {
                    // TODO: 向前
                    camera_position += forward_dir * speed * delte_time;
                }

                int key_A_state = glfwGetKey(window, GLFW_KEY_A);
                if (key_A_state == GLFW_PRESS)
                {
                    // TODO: 向左
                    camera_position += -right_dir * speed * delte_time;
                }

                int key_S_state = glfwGetKey(window, GLFW_KEY_S);
                if (key_S_state == GLFW_PRESS)
                {
                    // TODO: 向后
                    camera_position += -forward_dir * speed * delte_time;
                }

                int key_D_state = glfwGetKey(window, GLFW_KEY_D);
                if (key_D_state == GLFW_PRESS)
                {
                    // TODO: 向右
                    camera_position += right_dir * speed * delte_time;
                }

                model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = model * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

                glm::vec3 look_point = camera_position + forward_dir;

                previous_view = view;
                view = glm::lookAt(camera_position, look_point, up_dir);

                if (previous_view != view)
                {
                    is_need_refresh_frame = true;
                }

                projection = glm::perspective(glm::radians(45.0f), (float)(swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth()) / (float)(swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight()), 0.1f, 300.0f);

                matrixs_buffer_data.m = model;
                matrixs_buffer_data.v = view;
                matrixs_buffer_data.p = projection;

                _ptr = matrixs_buffer->Map();
                memcpy(_ptr, &matrixs_buffer_data, sizeof(matrixs_buffer_data));
                matrixs_buffer->Unmap();
            }

            ImGui::NewFrame();

            static bool is_force_accumulate_frame = false;
            static bool is_force_refresh_frame = false;

            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("VulkanKHRRayTracingJitterCamera");
                ImGui::Text("W,A,S,D to move.");
                ImGui::Text("Push down and drag mouse right button to rotate view.");
                ImGui::SliderFloat("angle", &angle, 0.0f, 360);
                ImGui::SliderFloat("scale", &my_buffer_data.scale, 0.03, 0.1);
                if (ImGui::Checkbox("Is force accumulate frame", &is_force_accumulate_frame))
                {
                    if (!is_force_accumulate_frame)
                    {
                        is_need_refresh_frame = true;
                    }
                }

                if (ImGui::Checkbox("Is force refresh frame", &is_force_refresh_frame))
                {
                    if (!is_force_refresh_frame)
                    {
                        is_need_refresh_frame = true;
                    }
                }

                if (is_force_accumulate_frame)
                {
                    ImGui::SliderFloat("Accumulate weight", &ray_tracing_push_constant.accumulateWeight, 0.0f, 300);
                }

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }
            //</IMGUI Update>

            Turbo::Core::TViewport frame_viewport(0, 0, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight(), 0, 1);
            Turbo::Core::TScissor frame_scissor(0, 0, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight());

            std::vector<Turbo::Core::TViewport> frame_viewports;
            frame_viewports.push_back(frame_viewport);

            std::vector<Turbo::Core::TScissor> frame_scissors;
            frame_scissors.push_back(frame_scissor);

            command_buffer->Begin();

            // ray tracing commands
            {
                VkCommandBuffer vk_command_buffer = command_buffer->GetVkCommandBuffer();
                device_driver->vkCmdBindPipeline(vk_command_buffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, ray_tracing_pipeline);
                device_driver->vkCmdBindDescriptorSets(vk_command_buffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, ray_tracing_pipeline_layout, 0, 1, &ray_tracing_descriptor_set, 0, nullptr);

                if (is_force_accumulate_frame)
                {
                    is_need_refresh_frame = false;
                }

                if (is_need_refresh_frame || is_force_refresh_frame)
                {
                    ray_tracing_push_constant.frame = 0;
                    is_need_refresh_frame = false;
                }
                else
                {
                    ray_tracing_push_constant.frame++;
                }

                device_driver->vkCmdPushConstants(vk_command_buffer, ray_tracing_pipeline_layout, VkShaderStageFlagBits::VK_SHADER_STAGE_RAYGEN_BIT_KHR, 0, sizeof(RAY_TRACING_PUSH_CONSTANT), &ray_tracing_push_constant);
                device_driver->vkCmdTraceRaysKHR(vk_command_buffer, &ray_generation_binding_table, &miss_binding_table, &closest_hit_binding_table, &callable_binding_table, swapchain->GetWidth(), swapchain->GetHeight(), 1);

                // copy ray tracing image into color target image
                VkImageBlit vk_image_blit = {};
                vk_image_blit.srcSubresource.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
                vk_image_blit.srcSubresource.mipLevel = 0;
                vk_image_blit.srcSubresource.baseArrayLayer = 0;
                vk_image_blit.srcSubresource.layerCount = 1;
                vk_image_blit.srcOffsets[0].x = 0;
                vk_image_blit.srcOffsets[0].y = 0;
                vk_image_blit.srcOffsets[0].z = 0;
                vk_image_blit.srcOffsets[1].x = swapchain->GetWidth();
                vk_image_blit.srcOffsets[1].y = swapchain->GetHeight();
                vk_image_blit.srcOffsets[1].z = 1;
                vk_image_blit.dstSubresource.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
                vk_image_blit.dstSubresource.mipLevel = 0;
                vk_image_blit.dstSubresource.baseArrayLayer = 0;
                vk_image_blit.dstSubresource.layerCount = 1;
                vk_image_blit.dstOffsets[0].x = 0;
                vk_image_blit.dstOffsets[0].y = 0;
                vk_image_blit.dstOffsets[0].z = 0;
                vk_image_blit.dstOffsets[1].x = swapchain->GetWidth();
                vk_image_blit.dstOffsets[1].y = swapchain->GetHeight();
                vk_image_blit.dstOffsets[1].z = 1;

                command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, swpachain_framebuffers[current_image_index]->GetAttachments()[0]);
                device_driver->vkCmdBlitImage(vk_command_buffer, ray_tracing_image->GetVkImage(), VkImageLayout::VK_IMAGE_LAYOUT_GENERAL, swpachain_framebuffers[current_image_index]->GetAttachments()[0]->GetImage()->GetVkImage(), VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &vk_image_blit, VkFilter::VK_FILTER_LINEAR);
                command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::COLOR_ATTACHMENT_OUTPUT_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::COLOR_ATTACHMENT_WRITE_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL, swpachain_framebuffers[current_image_index]->GetAttachments()[0]);
            }

            command_buffer->CmdBeginRenderPass(render_pass, swpachain_framebuffers[current_image_index]);

            command_buffer->CmdSetViewport(frame_viewports);
            command_buffer->CmdSetScissor(frame_scissors);

            // material_sphere
            // command_buffer->CmdBindPipeline(pipeline);
            // command_buffer->CmdBindPipelineDescriptorSet(pipeline_descriptor_set);
            // command_buffer->CmdBindVertexBuffers(vertex_buffers);
            // command_buffer->CmdSetLineWidth(1);
            // command_buffer->CmdBindIndexBuffer(index_buffer);
            // command_buffer->CmdDrawIndexed(indices_count, 1, 0, 0, 0);

            command_buffer->CmdNextSubpass();

            //<IMGUI Rendering>
            ImGui::Render();
            ImDrawData *draw_data = ImGui::GetDrawData();
            const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

            if (!is_minimized)
            {
                // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
                int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
                int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
                if (fb_width <= 0 || fb_height <= 0)
                {
                    break;
                }

                if (draw_data->TotalVtxCount > 0)
                {
                    size_t vertex_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
                    size_t index_size = draw_data->TotalIdxCount * sizeof(ImDrawIdx);

                    if (imgui_vertex_buffer != nullptr)
                    {
                        delete imgui_vertex_buffer;
                        imgui_vertex_buffer = nullptr;
                    }

                    if (imgui_index_buffer != nullptr)
                    {
                        delete imgui_index_buffer;
                        imgui_index_buffer = nullptr;
                    }

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

                    command_buffer->CmdBindPipeline(imgui_pipeline);
                    command_buffer->CmdBindPipelineDescriptorSet(imgui_pipeline_descriptor_set);

                    std::vector<Turbo::Core::TBuffer *> imgui_vertex_buffers;
                    imgui_vertex_buffers.push_back(imgui_vertex_buffer);
                    command_buffer->CmdBindVertexBuffers(imgui_vertex_buffers);
                    command_buffer->CmdBindIndexBuffer(imgui_index_buffer, 0, sizeof(ImDrawIdx) == 2 ? Turbo::Core::TIndexType::UINT16 : Turbo::Core::TIndexType::UINT32);

                    float scale[2];
                    scale[0] = 2.0f / draw_data->DisplaySize.x;
                    scale[1] = 2.0f / draw_data->DisplaySize.y;
                    float translate[2];
                    translate[0] = -1.0f - draw_data->DisplayPos.x * scale[0];
                    translate[1] = -1.0f - draw_data->DisplayPos.y * scale[1];

                    command_buffer->CmdPushConstants(sizeof(float) * 0, sizeof(float) * 2, scale);
                    command_buffer->CmdPushConstants(sizeof(float) * 2, sizeof(float) * 2, translate);

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
                                    command_buffer->CmdBindPipeline(imgui_pipeline);
                                    command_buffer->CmdBindPipelineDescriptorSet(imgui_pipeline_descriptor_set);

                                    std::vector<Turbo::Core::TBuffer *> __imgui_vertex_buffers;
                                    __imgui_vertex_buffers.push_back(imgui_vertex_buffer);
                                    command_buffer->CmdBindVertexBuffers(imgui_vertex_buffers);
                                    command_buffer->CmdBindIndexBuffer(imgui_index_buffer, 0, sizeof(ImDrawIdx) == 2 ? Turbo::Core::TIndexType::UINT16 : Turbo::Core::TIndexType::UINT32);

                                    float __scale[2];
                                    __scale[0] = 2.0f / draw_data->DisplaySize.x;
                                    __scale[1] = 2.0f / draw_data->DisplaySize.y;
                                    float __translate[2];
                                    __translate[0] = -1.0f - draw_data->DisplayPos.x * scale[0];
                                    __translate[1] = -1.0f - draw_data->DisplayPos.y * scale[1];

                                    command_buffer->CmdPushConstants(sizeof(float) * 0, sizeof(float) * 2, __scale);
                                    command_buffer->CmdPushConstants(sizeof(float) * 2, sizeof(float) * 2, __translate);
                                }
                                else
                                    pcmd->UserCallback(cmd_list, pcmd);
                            }
                            else
                            {
                                // Project scissor/clipping rectangles into framebuffer space
                                ImVec4 clip_rect;
                                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                                {
                                    // Negative offsets are illegal for vkCmdSetScissor
                                    if (clip_rect.x < 0.0f)
                                        clip_rect.x = 0.0f;
                                    if (clip_rect.y < 0.0f)
                                        clip_rect.y = 0.0f;

                                    // Apply scissor/clipping rectangle
                                    VkRect2D scissor;
                                    scissor.offset.x = (int32_t)(clip_rect.x);
                                    scissor.offset.y = (int32_t)(clip_rect.y);
                                    scissor.extent.width = (uint32_t)(clip_rect.z - clip_rect.x);
                                    scissor.extent.height = (uint32_t)(clip_rect.w - clip_rect.y);

                                    Turbo::Core::TScissor imgui_scissor(scissor.offset.x, scissor.offset.y, scissor.extent.width, scissor.extent.height);
                                    std::vector<Turbo::Core::TScissor> imgui_scissors;
                                    imgui_scissors.push_back(imgui_scissor);
                                    command_buffer->CmdSetScissor(imgui_scissors);

                                    // Draw
                                    command_buffer->CmdDrawIndexed(pcmd->ElemCount, 1, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset, 0);
                                }
                            }
                        }
                        global_idx_offset += cmd_list->IdxBuffer.Size;
                        global_vtx_offset += cmd_list->VtxBuffer.Size;
                    }
                }
            }
            //</IMGUI Rendering>

            command_buffer->CmdEndRenderPass();
            command_buffer->End();

            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            std::vector<Turbo::Core::TSemaphore *> wait_semaphores;
            wait_semaphores.push_back(wait_image_ready);

            queue->Submit(&wait_semaphores, nullptr, command_buffer, fence);

            fence->WaitUntil(); // or you can use semaphore to wait for get higher performance

            delete fence;

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
                for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
                {
                    delete image_view_item;
                }

                swapchain_image_views.clear();

                // destroy depth image and view
                delete depth_image_view;
                delete depth_image;

                // destroy framebuffer
                for (Turbo::Core::TFramebuffer *frame_buffer_item : swpachain_framebuffers)
                {
                    delete frame_buffer_item;
                }
                swpachain_framebuffers.clear();

                // destroy ray tracing image
                delete ray_tracing_image_view;
                delete ray_tracing_image;

                // recreate swapchain
                Turbo::Extension::TSwapchain *old_swapchain = swapchain;
                Turbo::Extension::TSwapchain *new_swapchain = new Turbo::Extension::TSwapchain(old_swapchain);
                delete old_swapchain;

                swapchain = new_swapchain;

                // recreate swapchain image views
                swapchain_images = swapchain->GetImages();
                for (Turbo::Core::TImage *swapchain_image_item : swapchain_images)
                {
                    Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                    swapchain_image_views.push_back(swapchain_view);
                }

                // recreate depth image and view
                depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
                depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

                // recreate framebuffer
                for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
                {
                    std::vector<Turbo::Core::TImageView *> swapchain_image_views;
                    swapchain_image_views.push_back(image_view_item);
                    swapchain_image_views.push_back(depth_image_view);

                    Turbo::Core::TFramebuffer *swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, swapchain_image_views);
                    swpachain_framebuffers.push_back(swapchain_framebuffer);
                }

                // recreate ray tracing image
                ray_tracing_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R32G32B32A32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_STORAGE, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
                ray_tracing_image_view = new Turbo::Core::TImageView(ray_tracing_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, ray_tracing_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                {
                    Turbo::Core::TCommandBuffer *change_ray_tracing_image_layout_command_buffer = command_pool->Allocate();

                    change_ray_tracing_image_layout_command_buffer->Begin();
                    change_ray_tracing_image_layout_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::BOTTOM_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::GENERAL, ray_tracing_image_view);
                    change_ray_tracing_image_layout_command_buffer->End();

                    Turbo::Core::TFence *change_image_layout_fence = new Turbo::Core::TFence(device);

                    queue->Submit(nullptr, nullptr, change_ray_tracing_image_layout_command_buffer, change_image_layout_fence);

                    change_image_layout_fence->WaitUntil();
                    delete change_image_layout_fence;

                    command_pool->Free(change_ray_tracing_image_layout_command_buffer);
                }

                VkDescriptorImageInfo vk_descriptor_image_info = {};
                vk_descriptor_image_info.sampler = VK_NULL_HANDLE;
                vk_descriptor_image_info.imageView = ray_tracing_image_view->GetVkImageView();
                vk_descriptor_image_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_GENERAL;

                VkWriteDescriptorSet ray_tracing_write_image = {};
                ray_tracing_write_image.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ray_tracing_write_image.pNext = nullptr;
                ray_tracing_write_image.dstSet = ray_tracing_descriptor_set;
                ray_tracing_write_image.dstBinding = 1;
                ray_tracing_write_image.dstArrayElement = 0;
                ray_tracing_write_image.descriptorCount = 1;
                ray_tracing_write_image.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                ray_tracing_write_image.pImageInfo = &vk_descriptor_image_info;
                ray_tracing_write_image.pBufferInfo = nullptr;
                ray_tracing_write_image.pTexelBufferView = nullptr;

                std::vector<VkWriteDescriptorSet> vk_write_descriptor_sets;
                vk_write_descriptor_sets.push_back(ray_tracing_write_image);

                device_driver->vkUpdateDescriptorSets(device->GetVkDevice(), vk_write_descriptor_sets.size(), vk_write_descriptor_sets.data(), 0, nullptr);

                is_need_refresh_frame = true;
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
            for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
            {
                delete image_view_item;
            }

            swapchain_image_views.clear();

            // destroy depth image and view
            delete depth_image_view;
            delete depth_image;

            // destroy framebuffer
            for (Turbo::Core::TFramebuffer *frame_buffer_item : swpachain_framebuffers)
            {
                delete frame_buffer_item;
            }
            swpachain_framebuffers.clear();

            // destroy ray tracing image
            delete ray_tracing_image_view;
            delete ray_tracing_image;

            // recreate swapchain
            Turbo::Extension::TSwapchain *old_swapchain = swapchain;
            Turbo::Extension::TSwapchain *new_swapchain = new Turbo::Extension::TSwapchain(old_swapchain);
            delete old_swapchain;

            swapchain = new_swapchain;

            // recreate swapchain image views
            swapchain_images = swapchain->GetImages();
            for (Turbo::Core::TImage *swapchain_image_item : swapchain_images)
            {
                Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                swapchain_image_views.push_back(swapchain_view);
            }

            // recreate depth image and view
            depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
            depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

            // recreate framebuffer
            for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
            {
                std::vector<Turbo::Core::TImageView *> swapchain_image_views;
                swapchain_image_views.push_back(image_view_item);
                swapchain_image_views.push_back(depth_image_view);

                Turbo::Core::TFramebuffer *swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, swapchain_image_views);
                swpachain_framebuffers.push_back(swapchain_framebuffer);
            }

            // recreate ray tracing image
            ray_tracing_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R32G32B32A32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_STORAGE, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
            ray_tracing_image_view = new Turbo::Core::TImageView(ray_tracing_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, ray_tracing_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
            {
                Turbo::Core::TCommandBuffer *change_ray_tracing_image_layout_command_buffer = command_pool->Allocate();

                change_ray_tracing_image_layout_command_buffer->Begin();
                change_ray_tracing_image_layout_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::BOTTOM_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::GENERAL, ray_tracing_image_view);
                change_ray_tracing_image_layout_command_buffer->End();

                Turbo::Core::TFence *change_image_layout_fence = new Turbo::Core::TFence(device);

                queue->Submit(nullptr, nullptr, change_ray_tracing_image_layout_command_buffer, change_image_layout_fence);

                change_image_layout_fence->WaitUntil();
                delete change_image_layout_fence;

                command_pool->Free(change_ray_tracing_image_layout_command_buffer);
            }

            VkDescriptorImageInfo vk_descriptor_image_info = {};
            vk_descriptor_image_info.sampler = VK_NULL_HANDLE;
            vk_descriptor_image_info.imageView = ray_tracing_image_view->GetVkImageView();
            vk_descriptor_image_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_GENERAL;

            VkWriteDescriptorSet ray_tracing_write_image = {};
            ray_tracing_write_image.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            ray_tracing_write_image.pNext = nullptr;
            ray_tracing_write_image.dstSet = ray_tracing_descriptor_set;
            ray_tracing_write_image.dstBinding = 1;
            ray_tracing_write_image.dstArrayElement = 0;
            ray_tracing_write_image.descriptorCount = 1;
            ray_tracing_write_image.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            ray_tracing_write_image.pImageInfo = &vk_descriptor_image_info;
            ray_tracing_write_image.pBufferInfo = nullptr;
            ray_tracing_write_image.pTexelBufferView = nullptr;

            std::vector<VkWriteDescriptorSet> vk_write_descriptor_sets;
            vk_write_descriptor_sets.push_back(ray_tracing_write_image);

            device_driver->vkUpdateDescriptorSets(device->GetVkDevice(), vk_write_descriptor_sets.size(), vk_write_descriptor_sets.data(), 0, nullptr);

            is_need_refresh_frame = true;
        }
        break;
        default: {
            //
        }
        break;
        }

        delete wait_image_ready;
        //</End Rendering>
    }

    if (imgui_vertex_buffer != nullptr)
    {
        delete imgui_vertex_buffer;
    }
    if (imgui_index_buffer != nullptr)
    {
        delete imgui_index_buffer;
    }
    descriptor_pool->Free(imgui_pipeline_descriptor_set);
    delete imgui_font_image_view;
    delete imgui_font_image;
    delete imgui_pipeline;
    delete imgui_vertex_shader;
    delete imgui_fragment_shader;
    delete imgui_sampler;

    descriptor_pool->Free(pipeline_descriptor_set);
    delete pipeline;
    for (Turbo::Core::TFramebuffer *framebuffer_item : swpachain_framebuffers)
    {
        delete framebuffer_item;
    }

    delete render_pass;

    delete sbt_buffer;

    device_driver->vkDestroyPipeline(device->GetVkDevice(), ray_tracing_pipeline, Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks());
    device_driver->vkDestroyPipelineLayout(device->GetVkDevice(), ray_tracing_pipeline_layout, Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks());
    device_driver->vkFreeDescriptorSets(device->GetVkDevice(), descriptor_pool->GetVkDescriptorPool(), 1, &ray_tracing_descriptor_set);
    device_driver->vkDestroyDescriptorSetLayout(device->GetVkDevice(), ray_tracing_descriptor_set_layout, Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks());

    delete ray_generation_shader_test;
    delete miss_shader_test;
    delete closest_hit_shader_test;

    delete shadow_miss_shader_test;

    delete descriptor_pool;
    delete vertex_shader;
    delete geometry_shader;
    delete fragment_shader;
    delete depth_image_view;
    delete depth_image;
    for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
    {
        delete image_view_item;
    }
    delete index_buffer;
    delete position_buffer;
    delete tangent_buffer;
    delete normal_buffer;
    delete texcoord_buffer;
    delete my_buffer;
    delete matrixs_buffer;

    delete ktx_texture_view;
    delete sampler;
    delete ktx_image;

    device->GetDeviceDriver()->vkDestroyAccelerationStructureKHR(device->GetVkDevice(), top_level_acceleration_structure_khr, Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks());
    delete top_level_acceleration_structure_buffer;
    delete instance_buffer;
    device->GetDeviceDriver()->vkDestroyAccelerationStructureKHR(device->GetVkDevice(), bottom_level_acceleration_structure_khr, Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks());
    delete bottom_level_acceleration_structure_buffer;
    delete device_local_index_buffer;
    delete device_local_vertex_buffer;
    delete bottom_level_acceleration_structure_device_address_buffer;

    delete ray_tracing_image_view;
    delete ray_tracing_image;
    command_pool->Free(command_buffer);
    delete command_pool;
    delete swapchain;
    delete surface;
    PFN_vkDestroySurfaceKHR pfn_vk_destroy_surface_khr = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkDestroySurfaceKHR>(instance, "vkDestroySurfaceKHR");
    pfn_vk_destroy_surface_khr(instance->GetVkInstance(), vk_surface_khr, nullptr);
    glfwTerminate();
    delete device;
    delete instance;

    return 0;
}
