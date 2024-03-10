#include "core/include/TDevice.h"
#include "core/include/TDeviceQueue.h"
#include "core/include/TEngine.h"
#include "core/include/TPhysicalDevice.h"

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

#include "core/include/TPipelineDescriptorSet.h"
#include "core/include/TSampler.h"

#include <glm/ext.hpp>

#include "core/include/TVulkanLoader.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tiny_gltf.h>

#include <ktxvulkan.h>

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

const std::string VERT_SHADER_STR = ReadTextFile("../../asset/shaders/TessellationAndGeometry.vert");
const std::string TESC_SHADER_STR = ReadTextFile("../../asset/shaders/TessellationAndGeometry.tesc");
const std::string TESE_SHADER_STR = ReadTextFile("../../asset/shaders/TessellationAndGeometry.tese");
const std::string GEOM_SHADER_STR = ReadTextFile("../../asset/shaders/TessellationAndGeometry.geom");
const std::string FRAG_SHADER_STR = ReadTextFile("../../asset/shaders/TessellationAndGeometry.frag");

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
    int inner0;
    int outer0;
    int outer1;
    int outer2;
    float scale;
    float time;
    float value;
};

struct MATRIXS_BUFFER_DATA
{
    glm::mat4 m;
    glm::mat4 v;
    glm::mat4 p;
};

int main()
{
    std::cout << "Vulkan Version:" << Turbo::Core::TVulkanLoader::Instance()->GetVulkanVersion().ToString() << std::endl;

    MY_BUFFER_DATA my_buffer_data = {};
    my_buffer_data.inner0 = 0;
    my_buffer_data.outer0 = 1;
    my_buffer_data.outer1 = 1;
    my_buffer_data.outer2 = 1;
    my_buffer_data.scale = 0.01;
    my_buffer_data.value = 0.1;

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

        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, "../../asset/models/material_sphere.gltf");
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
    }

    Turbo::Core::TVersion instance_version(1, 2, 0, 0);
    Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance = new Turbo::Core::TInstance(&enable_layer, &enable_instance_extensions, &instance_version);
    Turbo::Core::TRefPtr<Turbo::Core::TPhysicalDevice> physical_device = instance->GetBestPhysicalDevice();

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

    Turbo::Core::TPhysicalDeviceFeatures physcial_device_support_features = physical_device->GetDeviceFeatures();
    if (physcial_device_support_features.tessellationShader)
    {
        physical_device_features.tessellationShader = true;
    }
    else
    {
        glfwTerminate();
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Not support tessellation feature");
    }

    if (physcial_device_support_features.geometryShader)
    {
        physical_device_features.geometryShader = true;
    }
    else
    {
        glfwTerminate();
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Not support geometry feature");
    }

    if (physcial_device_support_features.fillModeNonSolid)
    {
        physical_device_features.fillModeNonSolid = true;
    }

    uint32_t max_tessellation_generation_level = physical_device->GetDeviceLimits().maxTessellationGenerationLevel;

    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
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

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImage>> swapchain_images = swapchain->GetImages();

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
    for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
    {
        Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        swapchain_image_views.push_back(swapchain_view);
    }

    Turbo::Core::TRefPtr<Turbo::Core::TCommandBufferPool> command_pool = new Turbo::Core::TCommandBufferPool(queue);
    Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> command_buffer = command_pool->Allocate();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)swapchain->GetWidth() / (float)swapchain->GetHeight(), 0.1f, 100.0f);

    matrixs_buffer_data.m = model;
    matrixs_buffer_data.v = view;
    matrixs_buffer_data.p = projection;

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> matrixs_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(matrixs_buffer_data));
    void *mvp_ptr = matrixs_buffer->Map();
    memcpy(mvp_ptr, &matrixs_buffer_data, sizeof(matrixs_buffer_data));
    matrixs_buffer->Unmap();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> my_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(my_buffer_data));
    void *my_buffer_ptr = my_buffer->Map();
    memcpy(my_buffer_ptr, &my_buffer_data, sizeof(my_buffer_data));
    my_buffer->Unmap();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> position_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION) * POSITION_data.size());
    void *position_buffer_ptr = position_buffer->Map();
    memcpy(position_buffer_ptr, POSITION_data.data(), sizeof(POSITION) * POSITION_data.size());
    position_buffer->Unmap();
    POSITION_data.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> normal_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(NORMAL) * NORMAL_data.size());
    void *normal_buffer_ptr = normal_buffer->Map();
    memcpy(normal_buffer_ptr, NORMAL_data.data(), sizeof(NORMAL) * NORMAL_data.size());
    normal_buffer->Unmap();
    NORMAL_data.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> texcoord_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(TEXCOORD) * TEXCOORD_data.size());
    void *texcoord_buffer_ptr = texcoord_buffer->Map();
    memcpy(texcoord_buffer_ptr, TEXCOORD_data.data(), sizeof(TEXCOORD) * TEXCOORD_data.size());
    texcoord_buffer->Unmap();
    TEXCOORD_data.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> index_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_INDEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(uint32_t) * INDICES_data.size());
    void *index_buffer_ptr = index_buffer->Map();
    memcpy(index_buffer_ptr, INDICES_data.data(), sizeof(uint32_t) * INDICES_data.size());
    index_buffer->Unmap();
    INDICES_data.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> tangent_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(TANGENT) * TANGENT_data.size());
    void *tangent_buffer_ptr = tangent_buffer->Map();
    memcpy(tangent_buffer_ptr, TANGENT_data.data(), sizeof(TANGENT) * TANGENT_data.size());
    tangent_buffer->Unmap();
    TANGENT_data.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TImage> depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

    Turbo::Core::TRefPtr<Turbo::Core::TVertexShader> vertex_shader = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, VERT_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TTessellationControlShader> tessellation_control_shader = new Turbo::Core::TTessellationControlShader(device, Turbo::Core::TShaderLanguage::GLSL, TESC_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TTessellationEvaluationShader> tessellation_evaluation_shader = new Turbo::Core::TTessellationEvaluationShader(device, Turbo::Core::TShaderLanguage::GLSL, TESE_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TGeometryShader> geometry_shader = new Turbo::Core::TGeometryShader(device, Turbo::Core::TShaderLanguage::GLSL, GEOM_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TFragmentShader> fragment_shader = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, FRAG_SHADER_STR);

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
    buffers.push_back(my_buffer);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> matrixs_buffers;
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

    Turbo::Core::TAttachment swapchain_color_attachment(swapchain_images[0]->GetFormat(), swapchain_images[0]->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::PRESENT_SRC_KHR);
    Turbo::Core::TAttachment depth_attachment(depth_image->GetFormat(), depth_image->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    std::vector<Turbo::Core::TAttachment> attachemts;
    attachemts.push_back(swapchain_color_attachment);
    attachemts.push_back(depth_attachment);

    Turbo::Core::TRefPtr<Turbo::Core::TRenderPass> render_pass = new Turbo::Core::TRenderPass(device, attachemts, subpasses);

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
    if (physical_device_features.fillModeNonSolid)
    {
        polygon_mode = Turbo::Core::TPolygonMode::LINE;
    }
    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 0, vertex_bindings, vertex_shader, tessellation_control_shader, tessellation_evaluation_shader, geometry_shader, fragment_shader, patch_control_points, false, false, false, polygon_mode, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, true, true, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, 0, 0, false, Turbo::Core::TLogicOp::NO_OP, true, Turbo::Core::TBlendFactor::SRC_ALPHA, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendOp::ADD, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendFactor::ZERO, Turbo::Core::TBlendOp::ADD);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> input_attachment_depths;
    input_attachment_depths.push_back(depth_image_view);

    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> pipeline_descriptor_set = descriptor_pool->Allocate(pipeline->GetPipelineLayout());
    pipeline_descriptor_set->BindData(0, 0, 0, matrixs_buffers);
    pipeline_descriptor_set->BindData(0, 1, 0, buffers);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> vertex_buffers;
    vertex_buffers.push_back(position_buffer);
    vertex_buffers.push_back(normal_buffer);
    vertex_buffers.push_back(texcoord_buffer);
    vertex_buffers.push_back(tangent_buffer);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer>> swpachain_framebuffers;
    for (Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_image_view_item : swapchain_image_views)
    {
        std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> image_views;
        image_views.push_back(swapchain_image_view_item);
        image_views.push_back(depth_image_view);

        Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, image_views);
        swpachain_framebuffers.push_back(swapchain_framebuffer);
    }

    //<IMGUI>
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    Turbo::Core::TRefPtr<Turbo::Core::TSampler> imgui_sampler = new Turbo::Core::TSampler(device);
    Turbo::Core::TRefPtr<Turbo::Core::TShader> imgui_vertex_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::VERTEX, Turbo::Core::TShaderLanguage::GLSL, IMGUI_VERT_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TShader> imgui_fragment_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::FRAGMENT, Turbo::Core::TShaderLanguage::GLSL, IMGUI_FRAG_SHADER_STR);

    Turbo::Core::TVertexBinding imgui_vertex_binding(0, sizeof(ImDrawVert), Turbo::Core::TVertexRate::VERTEX);
    imgui_vertex_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32_SFLOAT, IM_OFFSETOF(ImDrawVert, pos));  // position
    imgui_vertex_binding.AddAttribute(1, Turbo::Core::TFormatType::R32G32_SFLOAT, IM_OFFSETOF(ImDrawVert, uv));   // uv
    imgui_vertex_binding.AddAttribute(2, Turbo::Core::TFormatType::R8G8B8A8_UNORM, IM_OFFSETOF(ImDrawVert, col)); // color

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TShader>> imgui_shaders;
    imgui_shaders.push_back(imgui_vertex_shader);
    imgui_shaders.push_back(imgui_fragment_shader);

    std::vector<Turbo::Core::TVertexBinding> imgui_vertex_bindings;
    imgui_vertex_bindings.push_back(imgui_vertex_binding);

    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> imgui_pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 1, imgui_vertex_bindings, imgui_shaders, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, false, false, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, 0, 0, false, Turbo::Core::TLogicOp::NO_OP, true, Turbo::Core::TBlendFactor::SRC_ALPHA, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendOp::ADD, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendFactor::ZERO, Turbo::Core::TBlendOp::ADD);

    unsigned char *imgui_font_pixels;
    int imgui_font_width, imgui_font_height;
    io.Fonts->GetTexDataAsRGBA32(&imgui_font_pixels, &imgui_font_width, &imgui_font_height);
    size_t imgui_upload_size = imgui_font_width * imgui_font_height * 4 * sizeof(char);

    Turbo::Core::TRefPtr<Turbo::Core::TImage> imgui_font_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R8G8B8A8_UNORM, imgui_font_width, imgui_font_height, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_SAMPLED | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> imgui_font_image_view = new Turbo::Core::TImageView(imgui_font_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, imgui_font_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
    {
        Turbo::Core::TRefPtr<Turbo::Core::TBuffer> imgui_font_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, imgui_upload_size);
        void *imgui_font_ptr = imgui_font_buffer->Map();
        memcpy(imgui_font_ptr, imgui_font_pixels, imgui_upload_size);
        imgui_font_buffer->Unmap();

        Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> imgui_copy_command_buffer = command_pool->Allocate();
        imgui_copy_command_buffer->Begin();
        imgui_copy_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::HOST_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::HOST_WRITE_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, imgui_font_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        imgui_copy_command_buffer->CmdCopyBufferToImage(imgui_font_buffer, imgui_font_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, 0, imgui_font_width, imgui_font_height, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1, 0, 0, 0, imgui_font_width, imgui_font_height, 1);
        imgui_copy_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::FRAGMENT_SHADER_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TAccessBits::SHADER_READ_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::SHADER_READ_ONLY_OPTIMAL, imgui_font_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        imgui_copy_command_buffer->End();

        Turbo::Core::TRefPtr<Turbo::Core::TFence> imgui_font_copy_fence = new Turbo::Core::TFence(device);
        queue->Submit(imgui_copy_command_buffer, imgui_font_copy_fence);

        imgui_font_copy_fence->WaitUntil();
    }

    std::vector<std::pair<Turbo::Core::TRefPtr<Turbo::Core::TImageView>, Turbo::Core::TRefPtr<Turbo::Core::TSampler>>> imgui_combined_image_samplers;
    imgui_combined_image_samplers.push_back(std::make_pair(imgui_font_image_view, imgui_sampler));

    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> imgui_pipeline_descriptor_set = descriptor_pool->Allocate(imgui_pipeline->GetPipelineLayout());
    imgui_pipeline_descriptor_set->BindData(0, 0, 0, imgui_combined_image_samplers);

    io.Fonts->TexID = (ImTextureID)(intptr_t)(imgui_font_image->GetVkImage());

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> imgui_vertex_buffer = nullptr;
    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> imgui_index_buffer = nullptr;
    //</IMGUI>

    glm::vec3 camera_position = glm::vec3(1.00025, -1.50862, -2.52088);

    float horizontal_angle = 383.2;
    float vertical_angle = 28;

    glm::vec2 previous_mouse_pos = glm::vec2(0, 0);
    glm::vec2 current_mouse_pos = glm::vec2(0, 0);

    float angle = 0;
    float _time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        my_buffer_data.time = glfwGetTime();

        void *_ptr = my_buffer->Map();
        memcpy(_ptr, &my_buffer_data, sizeof(my_buffer_data));
        my_buffer->Unmap();

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
                view = glm::lookAt(camera_position, look_point, up_dir);
                projection = glm::perspective(glm::radians(45.0f), (float)(swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth()) / (float)(swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight()), 0.1f, 300.0f);

                matrixs_buffer_data.m = model;
                matrixs_buffer_data.v = view;
                matrixs_buffer_data.p = projection;

                _ptr = matrixs_buffer->Map();
                memcpy(_ptr, &matrixs_buffer_data, sizeof(matrixs_buffer_data));
                matrixs_buffer->Unmap();
            }

            ImGui::NewFrame();

            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("TessellationAndGeometry");
                ImGui::Text("W,A,S,D to move.");
                ImGui::Text("Push down and drag mouse right button to rotate view.");
                ImGui::SliderFloat("angle", &angle, 0.0f, 360);
                ImGui::SliderInt("inner 0", &my_buffer_data.inner0, 0, max_tessellation_generation_level);
                ImGui::SliderInt("outer 0", &my_buffer_data.outer0, 1, max_tessellation_generation_level);
                ImGui::SliderInt("outer 1", &my_buffer_data.outer1, 1, max_tessellation_generation_level);
                ImGui::SliderInt("outer 2", &my_buffer_data.outer2, 1, max_tessellation_generation_level);
                ImGui::SliderFloat("scale", &my_buffer_data.scale, 0.01, 1);
                ImGui::SliderFloat("value", &my_buffer_data.value, 0, 1);
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
            command_buffer->CmdBeginRenderPass(render_pass, swpachain_framebuffers[current_image_index]);

            // material_sphere
            command_buffer->CmdBindPipeline(pipeline);
            command_buffer->CmdBindPipelineDescriptorSet(pipeline_descriptor_set);
            command_buffer->CmdBindVertexBuffers(vertex_buffers);
            command_buffer->CmdSetViewport(frame_viewports);
            command_buffer->CmdSetScissor(frame_scissors);
            command_buffer->CmdSetLineWidth(1);
            command_buffer->CmdBindIndexBuffer(index_buffer);
            command_buffer->CmdDrawIndexed(indices_count, 1, 0, 0, 0);

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
                        imgui_vertex_buffer = nullptr;
                    }

                    if (imgui_index_buffer != nullptr)
                    {
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

                    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> imgui_vertex_buffers;
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

                                    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> __imgui_vertex_buffers;
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

            Turbo::Core::TRefPtr<Turbo::Core::TFence> fence = new Turbo::Core::TFence(device);
            std::vector<Turbo::Core::TRefPtr<Turbo::Core::TSemaphore>> wait_semaphores;
            wait_semaphores.push_back(wait_image_ready);

            queue->Submit(wait_semaphores, {}, command_buffer, fence);

            fence->WaitUntil(); // or you can use semaphore to wait for get higher performance

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
        //</End Rendering>
    }

    descriptor_pool->Free(imgui_pipeline_descriptor_set);
    descriptor_pool->Free(pipeline_descriptor_set);
    
    command_pool->Free(command_buffer);
    
    glfwTerminate();

    return 0;
}
