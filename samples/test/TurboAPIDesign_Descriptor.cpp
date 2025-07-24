#include <iostream>
#include <TReferenced.h>
#include <TRefPtr.h>
#include <vulkan/vulkan_core.h>

#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <array>

#include <TDevice.h>
#include <TDeviceQueue.h>
#include <TVulkanHandle.h>
#include <TVulkanAllocator.h>
#include <TVulkanLoader.h>
#include <TDescriptor.h>
#include <TDescriptorSetLayout.h>
#include <TShader.h>

#include <TPipelineLayout.h>

#include <ReadFile.h>

std::string asset_root(TURBO_ASSET_ROOT);

class PushConstant // OK
{
  private:
    uint32_t offset = 0;
    uint32_t size = 0;

  public:
    PushConstant() = default;
    PushConstant(uint32_t offset, uint32_t size)
    {
        this->offset = offset;
        this->size = size;
    }

    uint32_t GetOffset() const
    {
        return this->offset;
    }

    uint32_t GetSize() const
    {
        return this->size;
    }
};

class Descriptor // OK
{
  public:
    enum class Type
    {
        SAMPLER = 0,
        COMBINED_IMAGE_SAMPLER = 1,
        SAMPLED_IMAGE = 2,
        STORAGE_IMAGE = 3,
        UNIFORM_TEXEL_BUFFER = 4,
        STORAGE_TEXEL_BUFFER = 5,
        UNIFORM_BUFFER = 6,
        STORAGE_BUFFER = 7,
        UNIFORM_BUFFER_DYNAMIC = 8,
        STORAGE_BUFFER_DYNAMIC = 9,
        INPUT_ATTACHMENT = 10,
        // Provided by VK_VERSION_1_3
        INLINE_UNIFORM_BLOCK = 1000138000,
        // Provided by VK_KHR_acceleration_structure
        ACCELERATION_STRUCTURE_KHR = 1000150000,
        // Provided by VK_NV_ray_tracing
        ACCELERATION_STRUCTURE_NV = 1000165000,
        // Provided by VK_QCOM_image_processing
        SAMPLE_WEIGHT_IMAGE_QCOM = 1000440000,
        // Provided by VK_QCOM_image_processing
        BLOCK_MATCH_IMAGE_QCOM = 1000440001,
        // Provided by VK_EXT_mutable_descriptor_type
        MUTABLE_EXT = 1000351000,
        // Provided by VK_NV_partitioned_acceleration_structure
        PARTITIONED_ACCELERATION_STRUCTURE_NV = 1000570000
    };

  private:
    Type type = Type::SAMPLER; // 描述符类型
    size_t count = 0;          // 数量

  public:
    Descriptor() = default;
    Descriptor(Type type, size_t count = 1)
    {
        this->type = type;
        this->count = count;
    }

    const Type &GetType() const
    {
        return this->type;
    }

    const size_t &GetCount() const
    {
        return this->count;
    }

    bool operator==(const Descriptor &other) const
    {
        return (this->type == other.type) && (this->count == other.count);
    }

    bool operator!=(const Descriptor &other) const
    {
        return !((*this) == other);
    }

    std::string ToString() const
    {
        std::string result;
        result += "{";
        switch (this->type)
        {
        case Descriptor::Type::SAMPLER: {
            result += "sampler";
        }
        break;
        case Descriptor::Type::COMBINED_IMAGE_SAMPLER: {
            result += "combined_image_sampler";
        }
        break;
        case Descriptor::Type::SAMPLED_IMAGE: {
            result += "sampled_image";
        }
        break;
        case Descriptor::Type::STORAGE_IMAGE: {
            result += "storage_image";
        }
        break;
        case Descriptor::Type::UNIFORM_TEXEL_BUFFER: {
            result += "uniform_texel_buffer";
        }
        break;
        case Descriptor::Type::STORAGE_TEXEL_BUFFER: {
            result += "storage_texel_buffer";
        }
        break;
        case Descriptor::Type::UNIFORM_BUFFER: {
            result += "uniform_buffer";
        }
        break;
        case Descriptor::Type::STORAGE_BUFFER: {
            result += "storage_buffer";
        }
        break;
        case Descriptor::Type::UNIFORM_BUFFER_DYNAMIC: {
            result += "uniform_buffer_dynamic";
        }
        break;
        case Descriptor::Type::STORAGE_BUFFER_DYNAMIC: {
            result += "storage_buffer_dynamic";
        }
        break;
        case Descriptor::Type::INPUT_ATTACHMENT: {
            result += "input_attachment";
        }
        break;
        case Descriptor::Type::INLINE_UNIFORM_BLOCK: {
            result += "inline_uniform_block";
        }
        break;
        case Descriptor::Type::ACCELERATION_STRUCTURE_KHR: {
            result += "acceleration_structure_khr";
        }
        break;
        case Descriptor::Type::ACCELERATION_STRUCTURE_NV: {
            result += "acceleration_structure_nv";
        }
        break;
        case Descriptor::Type::SAMPLE_WEIGHT_IMAGE_QCOM: {
            result += "sample_weight_image_qcom";
        }
        break;
        case Descriptor::Type::BLOCK_MATCH_IMAGE_QCOM: {
            result += "block_match_image_qcom";
        }
        break;
        case Descriptor::Type::MUTABLE_EXT: {
            result += "mutable_ext";
        }
        break;
        case Descriptor::Type::PARTITIONED_ACCELERATION_STRUCTURE_NV: {
            result += "partitioned_acceleration_structure_nv";
        }
        break;
        }
        result += " : ";
        result += std::to_string(this->count);
        result += "}";

        return result;
    }
};

class StorageImage : public Descriptor
{
  public:
    StorageImage(size_t count = 1) : Descriptor(Descriptor::Type::STORAGE_IMAGE, count)
    {
    }
};

using Binding = size_t;
using Bindings = std::unordered_map<Binding, Descriptor>;

void OutputBindings(const Bindings &bindings)
{
    for (auto &item : bindings)
    {
        Binding binding = item.first;
        const Descriptor &descriptor = item.second;
        std::cout << binding << " : " << descriptor.ToString() << std::endl;
    }
}

class DescriptorSetLayout : public Turbo::Core::TVulkanHandle // OK
{
  public:
    class Layout
    {
      private:
        Bindings bindings;

      public:
        Layout() = default;
        Layout(const Bindings &bindings)
        {
            this->bindings = bindings;
        }

        size_t GetCount() const
        {
            return this->bindings.size();
        }

        Bindings::const_iterator begin() const
        {
            return this->bindings.begin();
        }

        Bindings::const_iterator end() const
        {
            return this->bindings.end();
        }

        // TODO: hash for key

        bool operator==(const Layout &other) const
        {
            return this->bindings == other.bindings;
        }
    };

  private:
    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device = nullptr;
    VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;

    Layout layout;

  protected:
    void InternalCreate() override
    {
        if (this->device.Valid())
        {
            std::vector<VkDescriptorSetLayoutBinding> bindings(this->layout.GetCount());
            {
                size_t index = 0;
                for (auto &item : this->layout)
                {
                    VkDescriptorSetLayoutBinding vk_descriptor_set_layout_binding = {};
                    vk_descriptor_set_layout_binding.binding = static_cast<uint32_t>(item.first);
                    vk_descriptor_set_layout_binding.descriptorType = static_cast<VkDescriptorType>(item.second.GetType());
                    vk_descriptor_set_layout_binding.descriptorCount = static_cast<uint32_t>(item.second.GetCount());
                    vk_descriptor_set_layout_binding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_ALL;
                    if (vk_descriptor_set_layout_binding.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
                    {
                        vk_descriptor_set_layout_binding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
                    }
                    vk_descriptor_set_layout_binding.pImmutableSamplers = nullptr;

                    bindings[index] = vk_descriptor_set_layout_binding;
                    index += 1;
                }
            }

            VkDescriptorSetLayoutCreateInfo vk_descriptor_set_layout_create_info = {};
            vk_descriptor_set_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            vk_descriptor_set_layout_create_info.pNext = nullptr;
            vk_descriptor_set_layout_create_info.flags = 0;
            vk_descriptor_set_layout_create_info.bindingCount = bindings.size();
            vk_descriptor_set_layout_create_info.pBindings = bindings.data();

            auto driver = this->device->GetDeviceDriver();
            VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
            VkResult result = driver->vkCreateDescriptorSetLayout(this->device->GetVkDevice(), &vk_descriptor_set_layout_create_info, allocator, &(this->vkDescriptorSetLayout));
            if (result != VkResult::VK_SUCCESS)
            {
                throw Turbo::Core::TException(Turbo::Core::TResult::FAIL, "vkCreateDescriptorSetLayout failed!");
            }
        }
    }

    void InternalDestroy() override
    {
        if (this->device.Valid() && this->vkDescriptorSetLayout != VK_NULL_HANDLE)
        {
            auto driver = this->device->GetDeviceDriver();
            VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
            driver->vkDestroyDescriptorSetLayout(this->device->GetVkDevice(), this->vkDescriptorSetLayout, allocator);
            this->vkDescriptorSetLayout = VK_NULL_HANDLE;
        }
    }

  protected:
    ~DescriptorSetLayout()
    {
        this->InternalDestroy();

        // this->layout = Layout();
        // this->device = nullptr;
    }

  public:
    // DescriptorSetLayout(Turbo::Core::TDevice *device, const Bindings &bindings)
    DescriptorSetLayout(Turbo::Core::TDevice *device, const Layout &layout)
    {
        this->device = device;
        this->layout = layout;

        this->InternalCreate();
    }

    const Layout &GetLayout() const
    {
        return this->layout;
    }

    std::string ToString() const
    {
        return "DescriptorSetLayout";
    }
};

namespace std
{
template <>
class hash<DescriptorSetLayout::Layout>
{
  public:
    std::size_t operator()(const DescriptorSetLayout::Layout &layout) const
    {
        class LayoutHasher
        {
          private:
            std::string *str;

          public:
            LayoutHasher(const DescriptorSetLayout::Layout &layout)
            {
                this->str = new std::string();
                for (auto &item : layout)
                {
                    auto &binding = item.first;
                    auto &descriptor_type = item.second.GetType();
                    auto &descriptor_count = item.second.GetCount();

                    this->str->append(reinterpret_cast<const std::string::value_type *>(&binding), sizeof(binding));
                    this->str->append(reinterpret_cast<const std::string::value_type *>(&descriptor_type), sizeof(descriptor_type));
                    this->str->append(reinterpret_cast<const std::string::value_type *>(&descriptor_count), sizeof(descriptor_count));
                }
            }

            ~LayoutHasher()
            {
                delete this->str;
            }

            std::size_t GetHash() const
            {
                return std::hash<std::string>{}(*(this->str));
            }
        };

        return LayoutHasher(layout).GetHash();
    }
};
} // namespace std

using Set = size_t;
using Sets = std::unordered_map<Set, Turbo::Core::TRefPtr<DescriptorSetLayout>>;

using Offset = uint32_t;
using Size = uint32_t;
using PushConstants = std::unordered_map<Offset, std::unordered_map<Size, VkShaderStageFlags>>;

class PipelineLayout : public Turbo::Core::TVulkanHandle // OK
{
  public:
    class Layout
    {
      private:
        Sets sets;
        PushConstants pushConstants;

      public:
        Layout() = default;
        Layout(const Sets &sets, const PushConstants &pushConstants)
        {
            this->sets = sets;
            this->pushConstants = pushConstants;
        }

        // TODO: iter begin end
        // TODO: hash for key
        //  ...
    };

  private:
    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device;
    VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

    Layout layout;

  protected:
    void InternalCreate() override
    {
        // TODO: Create VkPipelineLayout
    }
    void InternalDestroy() override
    {
        // TODO: Destroy VkPipelineLayout
    }

  protected:
    ~PipelineLayout()
    {
        this->InternalDestroy();
    }

  public:
    PipelineLayout(Turbo::Core::TDevice *device, const Sets &sets, const PushConstants &pushConstants)
    {
        this->device = device;
        this->layout = Layout(sets, pushConstants);

        this->InternalCreate();
    }

    std::string ToString() const
    {
        return "PipelineLayout";
    }
};

class VulkanContext
{
  private:
    Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance;
    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device;
    Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> queue;

  public:
    VulkanContext()
    {
        auto vulkan_verssion = Turbo::Core::TInstance::GetVulkanInstanceVersion();
        std::cout << "vulkan verssion: " << vulkan_verssion.ToString() << std::endl;
        std::vector<Turbo::Core::TLayerInfo> enable_layers;
        {
            auto instance_support_layers = Turbo::Core::TLayerInfo::GetInstanceLayers();
            for (auto &layer : instance_support_layers)
            {
                switch (layer.GetLayerType())
                {
                case Turbo::Core::TLayerType::VK_LAYER_KHRONOS_VALIDATION: {
                    enable_layers.push_back(layer);
                }
                break;
                default: {
                }
                break;
                }
            }
        }

        std::vector<Turbo::Core::TExtensionInfo> enable_extensions;
        {
            auto instance_support_extensions = Turbo::Core::TExtensionInfo::GetInstanceExtensions();
            for (auto &extension : instance_support_extensions)
            {
                switch (extension.GetExtensionType())
                {
                case Turbo::Core::TExtensionType::VK_KHR_SURFACE: {
                    enable_extensions.push_back(extension);
                }
                break;
                case Turbo::Core::TExtensionType::VK_KHR_WIN32_SURFACE: {
                    enable_extensions.push_back(extension);
                }
                break;
                case Turbo::Core::TExtensionType::VK_KHR_WAYLAND_SURFACE: {
                    enable_extensions.push_back(extension);
                }
                break;
                case Turbo::Core::TExtensionType::VK_KHR_XCB_SURFACE: {
                    enable_extensions.push_back(extension);
                }
                break;
                case Turbo::Core::TExtensionType::VK_KHR_XLIB_SURFACE: {
                    enable_extensions.push_back(extension);
                }
                break;
                default: {
                }
                break;
                }
            }
        }

        this->instance = new Turbo::Core::TInstance(enable_layers, enable_extensions, vulkan_verssion);

        auto physical_device = this->instance->GetBestPhysicalDevice();

        std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
        {
            std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = physical_device->GetSupportExtensions();
            for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
            {
                switch (extension.GetExtensionType())
                {
                case Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN: {
                    enable_device_extensions.push_back(extension);
                }
                break;
                default: {
                }
                break;
                }
            }
        }

        this->device = new Turbo::Core::TDevice(physical_device, {}, enable_device_extensions, {});
        this->queue = device->GetBestGraphicsQueue();
    }

    Turbo::Core::TInstance *Instance()
    {
        return this->instance;
    }

    Turbo::Core::TDevice *Device()
    {
        return this->device;
    }

    Turbo::Core::TDeviceQueue *Queue()
    {
        return this->queue;
    }
};

enum class TestFlagBits
{
    _1 = 0b1,
    _10 = 0b10,
    _100 = 0b100,
    _1000 = 0b1000,
    _10000 = 0b10000,
    _100000 = 0b100000,
    _1000000 = 0b1000000,
};

#include <TFlags.h>

// OK
//                                                                                                                                                                      #define TURBO_DECLARE_INLINE_FLAGS_BITS_OPERATOR(T)\
//inline Turbo::Core::TFlags<T> operator|(const T &left, const Turbo::Core::TFlags<T> &right)\
//{\
//    Turbo::Core::TFlags<T> flags;\
//    flags |= left;\
//    flags |= right;\
//    return flags;\
//}
//
// TURBO_DECLARE_INLINE_FLAGS_BITS_OPERATOR(TestFlagBits)

// template <typename T>
// Turbo::Core::TFlags<T> operator|(const T &left, const T &right)
//{
//     Turbo::Core::TFlags<T> flags;
//     flags |= left;
//     flags |= right;
//     return flags;
// }

// template <typename T>
// Turbo::Core::TFlags<T> operator|(const T &left, const Turbo::Core::TFlags<T> &right)
//{
//     Turbo::Core::TFlags<T> flags;
//     flags |= left;
//     flags |= right;
//     return flags;
// }

// Turbo::Core::TFlags<TestFlagBits> operator|(const TestFlagBits &left, const Turbo::Core::TFlags<TestFlagBits> &right)
//{
//     Turbo::Core::TFlags<TestFlagBits> flags;
//     flags |= left;
//     flags |= right;
//     return flags;
// }

// Turbo::Core::TFlags<TestFlagBits> operator|(const TestFlagBits &left, const Turbo::Core::TFlags<TestFlagBits> &right);

//#define Test_Flags_(TTT) \
//Turbo::Core::TFlags<TestFlagBits> operator|(TestFlagBits f1, const Turbo::Core::TFlags<TestFlagBits> &f2) noexcept \
//{return Turbo::Core::TFlags<TestFlagBits>(f1) | f2;}

//#define Test_Flags_(TTT) \
//Turbo::Core::TFlags<TestFlagBits> operator|(TestFlagBits f1, const Turbo::Core::TFlags<TestFlagBits> &f2) noexcept \
//{return Turbo::Core::TFlags<TestFlagBits>(f1) | f2;}

void FlagsTest()
{
    // TFlags flags = TestFlagBits::_1 | TestFlagBits::_10;
    // std::cout << "flags: " << flags << std::endl;

    Turbo::Core::TFlags<TestFlagBits> flags = TestFlagBits::_10000;
    flags |= TestFlagBits::_10000;
    flags |= TestFlagBits::_1000;
    flags |= TestFlagBits::_100;
    flags |= Turbo::Core::TShaderType::VERTEX;

    // auto tt = TestFlagBits::_10000 | TestFlagBits::_1000;

    Turbo::Core::TFlags<TestFlagBits> a = TestFlagBits::_100;
    Turbo::Core::TFlags<Turbo::Core::TShaderType> b = Turbo::Core::TShaderType::VERTEX;
    b = a;

    std::cout << "flags: " << flags << std::endl;

    if (flags.Has(TestFlagBits::_1))
    {
        std::cout << "has 1" << std::endl;
    }
    if (flags.Has(TestFlagBits::_10))
    {
        std::cout << "has 10" << std::endl;
    }
    if (flags.Has(TestFlagBits::_100))
    {
        std::cout << "has 100" << std::endl;
    }
    if (flags.Has(TestFlagBits::_1000))
    {
        std::cout << "has 1000" << std::endl;
    }
    if (flags.Has(TestFlagBits::_10000))
    {
        std::cout << "has 10000" << std::endl;
    }
    if (flags.Has(TestFlagBits::_100000))
    {
        std::cout << "has 100000" << std::endl;
    }
    if (flags.Has(TestFlagBits::_1000000))
    {
        std::cout << "has 1000000" << std::endl;
    }
}

void VulkanFlagsTest()
{
    VkShaderStageFlags;
    Turbo::Core::TFlags<VkShaderStageFlags> flags = VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT | VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
    flags |= VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
    flags |= VkShaderStageFlagBits::VK_SHADER_STAGE_MESH_BIT_EXT;
    // flags |= VkCullModeFlagBits::VK_CULL_MODE_FRONT_BIT;
    std::cout << "flags: " << flags << std::endl;
    std::cout << "flags.ToVkFlags(): " << flags.ToVkFlags() << std::endl;

    Turbo::Core::TFlags<VkShaderStageFlags> flags0 = VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT | VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
    flags0 |= VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
    flags0 |= VkShaderStageFlagBits::VK_SHADER_STAGE_MESH_BIT_EXT;

    if (flags == flags0)
    {
        std::cout << "before flags == flags0" << std::endl;
    }
    else
    {
        std::cout << "before flags != flags0" << std::endl;
    }

    flags0 |= VkShaderStageFlagBits::VK_SHADER_STAGE_RAYGEN_BIT_KHR;

    if (flags == flags0)
    {
        std::cout << "after flags == flags0" << std::endl;
    }
    else
    {
        std::cout << "after flags != flags0" << std::endl;
    }

    VkShaderStageFlags vk_shader_stage_flags = VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT | VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
    vk_shader_stage_flags |= VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
    vk_shader_stage_flags |= VkShaderStageFlagBits::VK_SHADER_STAGE_MESH_BIT_EXT;
    std::cout << "vk_shader_stage_flags: " << vk_shader_stage_flags << std::endl;

    assert(vk_shader_stage_flags == flags.ToVkFlags());

    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT))
    {
        std::cout << "Has VERTEX_BIT" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT))
    {
        std::cout << "Has TESSELLATION_CONTROL_BIT" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT))
    {
        std::cout << "Has TESSELLATION_EVALUATION_BIT" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT))
    {
        std::cout << "Has GEOMETRY_BIT" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT))
    {
        std::cout << "Has FRAGMENT_BIT" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT))
    {
        std::cout << "Has COMPUTE_BIT" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_ALL_GRAPHICS))
    {
        std::cout << "Has ALL_GRAPHICS" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_ALL))
    {
        std::cout << "Has ALL" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_RAYGEN_BIT_KHR))
    {
        std::cout << "Has RAYGEN_BIT_KHR" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_ANY_HIT_BIT_KHR))
    {
        std::cout << "Has ANY_HIT_BIT_KHR" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR))
    {
        std::cout << "Has CLOSEST_HIT_BIT_KHR" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_MISS_BIT_KHR))
    {
        std::cout << "Has MISS_BIT_KHR" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_INTERSECTION_BIT_KHR))
    {
        std::cout << "Has INTERSECTION_BIT_KHR" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_CALLABLE_BIT_KHR))
    {
        std::cout << "Has CALLABLE_BIT_KHR" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_TASK_BIT_EXT))
    {
        std::cout << "Has TASK_BIT_EXT" << std::endl;
    }
    if (flags.Has(VkShaderStageFlagBits::VK_SHADER_STAGE_MESH_BIT_EXT))
    {
        std::cout << "Has MESH_BIT_EXT" << std::endl;
    }
}

void Test_PipelineLayout(Turbo::Core::TInstance *instance, Turbo::Core::TDevice *device, Turbo::Core::TDeviceQueue *queue)
{
    VkInstance vk_instance = instance->GetVkInstance();
    VkDevice vk_device = device->GetVkDevice();
    VkQueue vk_queue = queue->GetVkQueue();

    // Provided by VK_VERSION_1_0
    auto create_VkDescriptorSetLayoutBinding = [](uint32_t binding, VkDescriptorType type, uint32_t count) -> VkDescriptorSetLayoutBinding {
        VkDescriptorSetLayoutBinding result;
        result.binding = binding;
        result.descriptorType = type;
        result.descriptorCount = count;
        result.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_ALL_GRAPHICS;
        result.pImmutableSamplers = VK_NULL_HANDLE;
        return result;
    };

    VkDescriptorSetLayout placeholder_set_layout = VK_NULL_HANDLE;
    {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        {
            bindings.push_back(create_VkDescriptorSetLayoutBinding(0, VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER, 1));
        }

        VkDescriptorSetLayoutCreateInfo set = {};
        {
            set.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            set.pNext = 0;
            set.flags = 0;
            set.bindingCount = bindings.size();
            set.pBindings = bindings.data();
        }

        VkResult result0 = device->GetDeviceDriver()->vkCreateDescriptorSetLayout(vk_device, &set, nullptr, &placeholder_set_layout);
        if (result0 == VK_SUCCESS)
        {
            std::cout << "[Message] vkCreateDescriptorSetLayout placeholder_set_layout Success" << std::endl;
        }
        else
        {
            std::cout << "[Message] vkCreateDescriptorSetLayout placeholder_set_layout Failed" << std::endl;
        }
    }

    VkDescriptorSetLayout set_layout_0 = VK_NULL_HANDLE;
    {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        {
            bindings.push_back(create_VkDescriptorSetLayoutBinding(0, VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER, 1));
            bindings.push_back(create_VkDescriptorSetLayoutBinding(1, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1));
            bindings.push_back(create_VkDescriptorSetLayoutBinding(2, VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1));
            bindings.push_back(create_VkDescriptorSetLayoutBinding(3, VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1));
        }

        VkDescriptorSetLayoutCreateInfo set = {};
        {
            set.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            set.pNext = 0;
            set.flags = 0;
            set.bindingCount = bindings.size();
            set.pBindings = bindings.data();
        }

        VkResult result0 = device->GetDeviceDriver()->vkCreateDescriptorSetLayout(vk_device, &set, nullptr, &set_layout_0);
        if (result0 == VK_SUCCESS)
        {
            std::cout << "[Message] vkCreateDescriptorSetLayout Success" << std::endl;
        }
        else
        {
            std::cout << "[Message] vkCreateDescriptorSetLayout Failed" << std::endl;
        }
    }

    VkDescriptorSetLayout set_layout_1 = VK_NULL_HANDLE;
    {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        {
            bindings.push_back(create_VkDescriptorSetLayoutBinding(0, VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER, 1));
            bindings.push_back(create_VkDescriptorSetLayoutBinding(2, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1));
            bindings.push_back(create_VkDescriptorSetLayoutBinding(4, VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1));
            bindings.push_back(create_VkDescriptorSetLayoutBinding(7, VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1));
        }

        VkDescriptorSetLayoutCreateInfo set = {};
        {
            set.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            set.pNext = 0;
            set.flags = 0;
            set.bindingCount = bindings.size();
            set.pBindings = bindings.data();
        }

        VkResult result1 = device->GetDeviceDriver()->vkCreateDescriptorSetLayout(vk_device, &set, nullptr, &set_layout_1);
        if (result1 == VK_SUCCESS)
        {
            std::cout << "[Message] vkCreateDescriptorSetLayout Success" << std::endl;
            device->GetDeviceDriver()->vkDestroyDescriptorSetLayout(vk_device, set_layout_1, nullptr);
        }
        else
        {
            std::cout << "[Message] vkCreateDescriptorSetLayout Failed" << std::endl;
        }
    }

    if (set_layout_0 != VK_NULL_HANDLE && set_layout_1 != VK_NULL_HANDLE && placeholder_set_layout != VK_NULL_HANDLE)
    {

        std::vector<VkDescriptorSetLayout> set_layouts;
        set_layouts.push_back(set_layout_0);
        set_layouts.push_back(placeholder_set_layout);
        set_layouts.push_back(set_layout_1);

        VkPipelineLayoutCreateInfo pipeline_layout_create_info = {};
        pipeline_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_create_info.pNext = 0;
        pipeline_layout_create_info.flags = 0;
        pipeline_layout_create_info.setLayoutCount = set_layouts.size();
        pipeline_layout_create_info.pSetLayouts = set_layouts.data();
        pipeline_layout_create_info.pushConstantRangeCount = 0;
        pipeline_layout_create_info.pPushConstantRanges = nullptr;

        VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
        VkResult result1 = device->GetDeviceDriver()->vkCreatePipelineLayout(vk_device, &pipeline_layout_create_info, nullptr, &pipeline_layout);
        if (result1 == VK_SUCCESS)
        {
            std::cout << "[Message] vkCreatePipelineLayout Success" << std::endl;
            device->GetDeviceDriver()->vkDestroyPipelineLayout(vk_device, pipeline_layout, nullptr);
        }
        else
        {
            std::cout << "[Message] vkCreatePipelineLayout Failed" << std::endl;
        }
    }

    if (set_layout_0 != VK_NULL_HANDLE)
    {
        device->GetDeviceDriver()->vkDestroyDescriptorSetLayout(vk_device, set_layout_0, nullptr);
    }

    if (set_layout_1 != VK_NULL_HANDLE)
    {
        device->GetDeviceDriver()->vkDestroyDescriptorSetLayout(vk_device, set_layout_1, nullptr);
    }

    if (placeholder_set_layout != VK_NULL_HANDLE)
    {
        device->GetDeviceDriver()->vkDestroyDescriptorSetLayout(vk_device, placeholder_set_layout, nullptr);
    }
}

void Test_PushConstants(Turbo::Core::TInstance *instance, Turbo::Core::TDevice *device, Turbo::Core::TDeviceQueue *queue)
{
    Turbo::Core::TPipelineLayout::TLayout::TPushConstants pcs;
    pcs.Merge(Turbo::Core::TShaderType::VERTEX, 0, 8);
    pcs.Merge(Turbo::Core::TShaderType::GEOMETRY, 0, 4);
    pcs.Merge(Turbo::Core::TShaderType::TESSELLATION_EVALUATION, 0, 32);
    pcs.Merge(Turbo::Core::TShaderType::FRAGMENT, 0, 64);
    pcs.Merge(Turbo::Core::TShaderType::TESSELLATION_CONTROL, 0, 16);

    std::vector<Turbo::Core::TShaderType> shaders;

    auto &constants = pcs.GetConstants();
    for (auto &item : constants)
    {
        std::cout << "shader type: " << item.first << std::endl;
        std::cout << "offset: " << item.second.first << std::endl;
        std::cout << "second: " << item.second.second << std::endl;
        std::cout << "------------------------" << std::endl;

        shaders.push_back(item.first);
    }

    std::sort(shaders.begin(), shaders.end());
    std::cout << "After sort------------------------" << std::endl;

    {
        pcs.Merge(Turbo::Core::TShaderType::VERTEX, 16);
        pcs.Merge(Turbo::Core::TShaderType::GEOMETRY, 8);
        pcs.Merge(Turbo::Core::TShaderType::TESSELLATION_EVALUATION, 64);
        pcs.Merge(Turbo::Core::TShaderType::FRAGMENT, 32);
        pcs.Merge(Turbo::Core::TShaderType::TESSELLATION_CONTROL, 128);
    }

    std::size_t temp_offset = 0;
    for (auto &item : shaders)
    {
        std::cout << "shader type: " << item << std::endl;
        auto offset = constants.at(item).first;
        auto size = constants.at(item).second;

        std::cout << "offset: " << offset << std::endl;
        std::cout << "size: " << size << std::endl;
        std::cout << "------------------------" << std::endl;

        temp_offset += size;
    }
}

void Test_TurboPipelineLayout(Turbo::Core::TInstance *instance, Turbo::Core::TDevice *device, Turbo::Core::TDeviceQueue *queue)
{
    Turbo::Core::TRefPtr<Turbo::Core::TVertexShader> vertex_shader = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(asset_root + "/shaders/Test/Layout.vert"));
    Turbo::Core::TRefPtr<Turbo::Core::TFragmentShader> fragment_shader = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(asset_root + "/shaders/Test/Layout.frag"));

    auto vert_layout = vertex_shader->GetLayout();
    auto frag_layout = fragment_shader->GetLayout();

    std::cout << vert_layout.ToString() << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << frag_layout.ToString() << std::endl;

    Turbo::Core::TPipelineLayout::TLayout layout;
    layout.Merge(vert_layout);
    layout.Merge(frag_layout);

    std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

    std::cout << layout.ToString() << std::endl;

    std::cout << "**************************************************************" << std::endl;

    layout.Merge(vertex_shader->GetType(), 16);
    layout.Merge(fragment_shader->GetType(), 32);

    std::cout << layout.ToString() << std::endl;
}

void Test_ShadersToPipelineLayout(Turbo::Core::TInstance *instance, Turbo::Core::TDevice *device, Turbo::Core::TDeviceQueue *queue)
{
    Turbo::Core::TRefPtr<Turbo::Core::TVertexShader> vertex_shader = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(asset_root + "/shaders/Test/Layout.vert"));
    Turbo::Core::TRefPtr<Turbo::Core::TFragmentShader> fragment_shader = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(asset_root + "/shaders/Test/Layout.frag"));

    Turbo::Core::TPipelineLayout::TLayout layout;

    layout << (*vertex_shader) << (*fragment_shader);

    layout.Merge(vertex_shader->GetType(), 16);
    layout.Merge(fragment_shader->GetType(), 32);

    std::cout << layout.ToString() << std::endl;
}

void Test_ShadersToPipelineLayoutWithTOffsets(Turbo::Core::TInstance *instance, Turbo::Core::TDevice *device, Turbo::Core::TDeviceQueue *queue)
{
    Turbo::Core::TRefPtr<Turbo::Core::TVertexShader> vertex_shader = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(asset_root + "/shaders/Test/Layout.vert"));
    Turbo::Core::TRefPtr<Turbo::Core::TFragmentShader> fragment_shader = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(asset_root + "/shaders/Test/Layout.frag"));

    std::hash<Turbo::Core::TPipelineLayout::TLayout> hash;

    Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TOffsets offsets;
    offsets.Merge(vertex_shader->GetType(), 16);
    offsets.Merge(fragment_shader->GetType(), 32);

    {
        Turbo::Core::TPipelineLayout::TLayout layout;
        layout << (*vertex_shader) << (*fragment_shader) << offsets;
        std::cout << layout.ToString() << std::endl;
        std::cout << hash(layout) << std::endl;
    }
    std::cout << "----------------------------------------------" << std::endl;
    {
        Turbo::Core::TPipelineLayout::TLayout layout;
        layout << offsets << (*vertex_shader) << (*fragment_shader);
        std::cout << layout.ToString() << std::endl;
        std::cout << hash(layout) << std::endl;
    }
    std::cout << "----------------------------------------------" << std::endl;
    {
        Turbo::Core::TPipelineLayout::TLayout layout;
        layout << (*vertex_shader) << offsets << (*fragment_shader);
        std::cout << layout.ToString() << std::endl;
        std::cout << hash(layout) << std::endl;
    }
    std::cout << "----------------------------------------------" << std::endl;
    {
        Turbo::Core::TPipelineLayout::TLayout layout;
        layout << offsets;
        std::cout << layout.ToString() << std::endl;
        std::cout << hash(layout) << std::endl;
    }
}

// void Test_(Turbo::Core::TInstance *instance, Turbo::Core::TDevice *device, Turbo::Core::TDeviceQueue *queue)

int main()
{
    VulkanContext vulkan_context;

    Turbo::Core::TInstance *instance = vulkan_context.Instance();
    Turbo::Core::TDevice *device = vulkan_context.Device();
    Turbo::Core::TDeviceQueue *queue = vulkan_context.Queue();

    // Test_PipelineLayout(instance, device, queue);
    // Test_TurboPipelineLayout(instance, device, queue);
    // Test_ShadersToPipelineLayout(instance, device, queue);
    Test_ShadersToPipelineLayoutWithTOffsets(instance, device, queue);

    if (false)
    {
        // FlagsTest();
        VulkanFlagsTest();
    }

    if (false)
    {
        Bindings bindings;
        bindings.insert(std::make_pair(0, Descriptor(Descriptor::Type::SAMPLER)));
        bindings.insert({1, Descriptor(Descriptor::Type::COMBINED_IMAGE_SAMPLER, 2)});
        bindings.insert({2, Descriptor(Descriptor::Type::UNIFORM_BUFFER)});
        bindings.insert({3, StorageImage()});
        bindings.insert({4, StorageImage(2)});

        for (auto &item : bindings)
        {
            Binding binding = item.first;
            Descriptor &descriptor = item.second;
            std::cout << binding << " : " << descriptor.ToString() << std::endl;
        }

        Turbo::Core::TRefPtr<DescriptorSetLayout> descriptor_set_layout = new DescriptorSetLayout(device, bindings);
        if (descriptor_set_layout.Valid())
        {
            std::cout << "[OK] DescriptorSetLayout create successed!" << std::endl;
        }
    }

    if (false)
    {
        Bindings bindings;
        bindings.insert({0, Descriptor(Descriptor::Type::SAMPLER)});
        bindings.insert({1, Descriptor(Descriptor::Type::COMBINED_IMAGE_SAMPLER)});
        bindings.insert({2, Descriptor(Descriptor::Type::SAMPLED_IMAGE)});
        bindings.insert({3, Descriptor(Descriptor::Type::STORAGE_IMAGE)});
        bindings.insert({4, Descriptor(Descriptor::Type::UNIFORM_TEXEL_BUFFER)});
        bindings.insert({5, Descriptor(Descriptor::Type::STORAGE_TEXEL_BUFFER)});
        bindings.insert({6, Descriptor(Descriptor::Type::UNIFORM_BUFFER)});
        bindings.insert({7, Descriptor(Descriptor::Type::STORAGE_BUFFER)});
        bindings.insert({8, Descriptor(Descriptor::Type::UNIFORM_BUFFER_DYNAMIC)});
        bindings.insert({9, Descriptor(Descriptor::Type::STORAGE_BUFFER_DYNAMIC)});
        bindings.insert({10, Descriptor(Descriptor::Type::INPUT_ATTACHMENT)});
        // bindings.insert({11, Descriptor(Descriptor::Type::INLINE_UNIFORM_BLOCK)});
        // bindings.insert({12, Descriptor(Descriptor::Type::ACCELERATION_STRUCTURE_KHR)});
        // bindings.insert({13, Descriptor(Descriptor::Type::ACCELERATION_STRUCTURE_NV)});
        // bindings.insert({14, Descriptor(Descriptor::Type::SAMPLE_WEIGHT_IMAGE_QCOM)});
        // bindings.insert({15, Descriptor(Descriptor::Type::BLOCK_MATCH_IMAGE_QCOM)});
        // bindings.insert({16, Descriptor(Descriptor::Type::MUTABLE_EXT)});
        // bindings.insert({17, Descriptor(Descriptor::Type::PARTITIONED_ACCELERATION_STRUCTURE_NV)});

        OutputBindings(bindings);

        Turbo::Core::TRefPtr<DescriptorSetLayout> descriptor_set_layout = new DescriptorSetLayout(device, bindings);
        if (descriptor_set_layout.Valid())
        {
            std::cout << "[OK] DescriptorSetLayout create successed!" << std::endl;
        }
    }

    if (false)
    {
        Bindings bindings;
        bindings.insert({0, Descriptor(Descriptor::Type::SAMPLER)});
        bindings.insert({2, Descriptor(Descriptor::Type::COMBINED_IMAGE_SAMPLER, 2)});
        bindings.insert({12, Descriptor(Descriptor::Type::SAMPLED_IMAGE, 30)});
        bindings.insert({23, Descriptor(Descriptor::Type::STORAGE_IMAGE, 40)});
        bindings.insert({43, Descriptor(Descriptor::Type::UNIFORM_TEXEL_BUFFER, 50)});
        bindings.insert({55, Descriptor(Descriptor::Type::STORAGE_TEXEL_BUFFER, 60)});
        bindings.insert({16, Descriptor(Descriptor::Type::UNIFORM_BUFFER, 70)});
        bindings.insert({67, Descriptor(Descriptor::Type::STORAGE_BUFFER, 80)});
        bindings.insert({88, Descriptor(Descriptor::Type::UNIFORM_BUFFER_DYNAMIC, 90)});
        bindings.insert({99, Descriptor(Descriptor::Type::STORAGE_BUFFER_DYNAMIC, 100)});
        bindings.insert({100, Descriptor(Descriptor::Type::INPUT_ATTACHMENT, 2000)});

        OutputBindings(bindings);

        Turbo::Core::TRefPtr<DescriptorSetLayout> descriptor_set_layout = new DescriptorSetLayout(device, bindings);
        if (descriptor_set_layout.Valid())
        {
            std::cout << "[OK] DescriptorSetLayout create successed!" << std::endl;
        }
    }

    if (false)
    {
        Bindings bindings;
        bindings.insert({0, Descriptor(Descriptor::Type::SAMPLER)});
        bindings.insert({1, Descriptor(Descriptor::Type::COMBINED_IMAGE_SAMPLER)});
        bindings.insert({2, Descriptor(Descriptor::Type::SAMPLED_IMAGE)});
        bindings.insert({3, Descriptor(Descriptor::Type::STORAGE_IMAGE)});
        bindings.insert({4, Descriptor(Descriptor::Type::UNIFORM_TEXEL_BUFFER)});
        bindings.insert({5, Descriptor(Descriptor::Type::STORAGE_TEXEL_BUFFER)});
        bindings.insert({6, Descriptor(Descriptor::Type::UNIFORM_BUFFER)});
        bindings.insert({7, Descriptor(Descriptor::Type::STORAGE_BUFFER)});
        bindings.insert({8, Descriptor(Descriptor::Type::UNIFORM_BUFFER_DYNAMIC)});
        bindings.insert({9, Descriptor(Descriptor::Type::STORAGE_BUFFER_DYNAMIC)});
        bindings.insert({10, Descriptor(Descriptor::Type::INPUT_ATTACHMENT)});

        OutputBindings(bindings);

        Turbo::Core::TRefPtr<DescriptorSetLayout> descriptor_set_layout_0 = new DescriptorSetLayout(device, bindings);

        Turbo::Core::TRefPtr<DescriptorSetLayout> descriptor_set_layout_1 = new DescriptorSetLayout(device, bindings);

        bindings.insert({11, Descriptor(Descriptor::Type::SAMPLER)});
        Turbo::Core::TRefPtr<DescriptorSetLayout> descriptor_set_layout_2 = new DescriptorSetLayout(device, bindings);

        bindings[5] = Descriptor(Descriptor::Type::UNIFORM_BUFFER);
        Turbo::Core::TRefPtr<DescriptorSetLayout> descriptor_set_layout_3 = new DescriptorSetLayout(device, bindings);

        descriptor_set_layout_0->GetLayout() == descriptor_set_layout_1->GetLayout() ? std::cout << "[OK] Equal!" << std::endl : std::cout << "[NO] Not Equal!" << std::endl;
        descriptor_set_layout_1->GetLayout() == descriptor_set_layout_2->GetLayout() ? std::cout << "[OK] Equal!" << std::endl : std::cout << "[NO] Not Equal!" << std::endl;
        descriptor_set_layout_2->GetLayout() == descriptor_set_layout_3->GetLayout() ? std::cout << "[OK] Equal!" << std::endl : std::cout << "[NO] Not Equal!" << std::endl;
    }

    if (false)
    {
        Bindings bindings;
        bindings.insert({0, Descriptor(Descriptor::Type::SAMPLER)});
        DescriptorSetLayout::Layout layout_0(bindings);
        bindings.insert({1, Descriptor(Descriptor::Type::COMBINED_IMAGE_SAMPLER)});
        DescriptorSetLayout::Layout layout_1(bindings);
        bindings.insert({2, Descriptor(Descriptor::Type::SAMPLED_IMAGE)});
        DescriptorSetLayout::Layout layout_2(bindings);
        bindings.insert({3, Descriptor(Descriptor::Type::STORAGE_IMAGE)});
        DescriptorSetLayout::Layout layout_3(bindings);
        bindings.insert({4, Descriptor(Descriptor::Type::UNIFORM_TEXEL_BUFFER)});
        DescriptorSetLayout::Layout layout_4(bindings);
        bindings.insert({5, Descriptor(Descriptor::Type::STORAGE_TEXEL_BUFFER)});
        DescriptorSetLayout::Layout layout_5(bindings);
        bindings.insert({6, Descriptor(Descriptor::Type::UNIFORM_BUFFER)});
        DescriptorSetLayout::Layout layout_6(bindings);
        bindings.insert({7, Descriptor(Descriptor::Type::STORAGE_BUFFER)});
        DescriptorSetLayout::Layout layout_7(bindings);
        bindings.insert({8, Descriptor(Descriptor::Type::UNIFORM_BUFFER_DYNAMIC)});
        DescriptorSetLayout::Layout layout_8(bindings);
        bindings.insert({9, Descriptor(Descriptor::Type::STORAGE_BUFFER_DYNAMIC)});
        DescriptorSetLayout::Layout layout_9(bindings);
        DescriptorSetLayout::Layout layout_12(bindings);
        bindings.insert({10, Descriptor(Descriptor::Type::INPUT_ATTACHMENT)});
        DescriptorSetLayout::Layout layout_10(bindings);
        DescriptorSetLayout::Layout layout_11(bindings);

        // std::cout << "0 " << std::hash<DescriptorSetLayout::Layout>{}(layout_0) << std::endl;
        // std::cout << "1 " << std::hash<DescriptorSetLayout::Layout>{}(layout_1) << std::endl;
        // std::cout << "2 " << std::hash<DescriptorSetLayout::Layout>{}(layout_2) << std::endl;
        // std::cout << "3 " << std::hash<DescriptorSetLayout::Layout>{}(layout_3) << std::endl;
        // std::cout << "4 " << std::hash<DescriptorSetLayout::Layout>{}(layout_4) << std::endl;
        // std::cout << "5 " << std::hash<DescriptorSetLayout::Layout>{}(layout_5) << std::endl;
        // std::cout << "6 " << std::hash<DescriptorSetLayout::Layout>{}(layout_6) << std::endl;
        // std::cout << "7 " << std::hash<DescriptorSetLayout::Layout>{}(layout_7) << std::endl;
        // std::cout << "8 " << std::hash<DescriptorSetLayout::Layout>{}(layout_8) << std::endl;
        // std::cout << "9 " << std::hash<DescriptorSetLayout::Layout>{}(layout_9) << std::endl;
        // std::cout << "10 "<< std::hash<DescriptorSetLayout::Layout>{}(layout_10) << std::endl;

        std::cout << "0:  " << std::hash<DescriptorSetLayout::Layout>{}(layout_0) << std::endl;
        std::cout << "1:  " << std::hash<DescriptorSetLayout::Layout>{}(layout_1) << std::endl;
        std::cout << "2:  " << std::hash<DescriptorSetLayout::Layout>{}(layout_2) << std::endl;
        std::cout << "3:  " << std::hash<DescriptorSetLayout::Layout>{}(layout_3) << std::endl;
        std::cout << "4:  " << std::hash<DescriptorSetLayout::Layout>{}(layout_4) << std::endl;
        std::cout << "5:  " << std::hash<DescriptorSetLayout::Layout>{}(layout_5) << std::endl;
        std::cout << "6:  " << std::hash<DescriptorSetLayout::Layout>{}(layout_6) << std::endl;
        std::cout << "7:  " << std::hash<DescriptorSetLayout::Layout>{}(layout_7) << std::endl;
        std::cout << "8:  " << std::hash<DescriptorSetLayout::Layout>{}(layout_8) << std::endl;
        std::cout << "9:  " << std::hash<DescriptorSetLayout::Layout>{}(layout_9) << std::endl;
        std::cout << "12: " << std::hash<DescriptorSetLayout::Layout>{}(layout_12) << std::endl;
        std::cout << "10: " << std::hash<DescriptorSetLayout::Layout>{}(layout_10) << std::endl;
        std::cout << "11: " << std::hash<DescriptorSetLayout::Layout>{}(layout_11) << std::endl;
    }

    if (false)
    {
        // Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TConstants constants_0;
        // constants_0[0][512] = 0b00001;
        // constants_0[10][123] = 0b00011;
        // constants_0[21][456] = 0b00101;

        // Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TConstants constants_1;
        // constants_1[0][512] = 0b00110;
        // constants_1[10][123] = 0b00100;
        // constants_1[21][456] = 0b10001;
        // constants_1[21][455] = 0b01111;
        // constants_1[33][789] = 0b00001;

        // Turbo::Core::TPipelineLayout::TLayout::TPushConstants push_constants_0(constants_0);
        // std::cout << push_constants_0.ToString() << std::endl;

        // Turbo::Core::TPipelineLayout::TLayout::TPushConstants push_constants_1(constants_1);
        // std::cout << push_constants_1.ToString() << std::endl;

        // push_constants_0.Merge(push_constants_1);
        // std::cout << push_constants_0.ToString() << std::endl;
    }

    if (false)
    {
        Turbo::Core::TDescriptorSetLayout::TLayout::TBindings bindings_0;
        bindings_0.insert({0, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::SAMPLER, 1)});
        bindings_0.insert({2, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::COMBINED_IMAGE_SAMPLER, 2)});
        bindings_0.insert({12, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::SAMPLED_IMAGE, 30)});
        bindings_0.insert({23, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::STORAGE_IMAGE, 40)});
        bindings_0.insert({43, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::UNIFORM_TEXEL_BUFFER, 50)});

        Turbo::Core::TDescriptorSetLayout::TLayout::TBindings bindings_1;
        bindings_1.insert({55, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::STORAGE_TEXEL_BUFFER, 60)});
        bindings_1.insert({16, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::UNIFORM_BUFFER, 70)});
        bindings_1.insert({67, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::STORAGE_BUFFER, 80)});
        bindings_1.insert({88, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::UNIFORM_BUFFER_DYNAMIC, 90)});
        bindings_1.insert({99, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::STORAGE_BUFFER_DYNAMIC, 100)});
        bindings_1.insert({100, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::INPUT_ATTACHMENT, 2000)});

        Turbo::Core::TDescriptorSetLayout::TLayout layout_0(bindings_0);
        Turbo::Core::TDescriptorSetLayout::TLayout layout_1(bindings_1);
        std::cout << layout_0.ToString() << std::endl;
        std::cout << "--------------------------------" << std::endl;
        std::cout << layout_1.ToString() << std::endl;
        std::cout << "--------------------------------" << std::endl;
        layout_0.Merge(layout_1);
        std::cout << layout_0.ToString() << std::endl;
    }

    if (false)
    {
        Turbo::Core::TShader::TLayout::TPushConstant push_constant_0(Turbo::Core::TShaderType::VERTEX, 512);
        Turbo::Core::TShader::TLayout::TPushConstant push_constant_1(Turbo::Core::TShaderType::FRAGMENT, 512);

        Turbo::Core::TDescriptorSetLayout::TLayout::TBindings bindings_0;
        bindings_0.insert({0, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::SAMPLER, 1)});
        bindings_0.insert({2, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::COMBINED_IMAGE_SAMPLER, 2)});
        bindings_0.insert({12, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::SAMPLED_IMAGE, 30)});

        Turbo::Core::TDescriptorSetLayout::TLayout::TBindings bindings_1;
        bindings_1.insert({23, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::STORAGE_IMAGE, 40)});
        bindings_1.insert({43, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::UNIFORM_TEXEL_BUFFER, 50)});

        Turbo::Core::TDescriptorSetLayout::TLayout::TBindings bindings_2;
        bindings_2.insert({55, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::STORAGE_TEXEL_BUFFER, 60)});
        bindings_2.insert({16, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::UNIFORM_BUFFER, 70)});
        bindings_2.insert({67, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::STORAGE_BUFFER, 80)});

        Turbo::Core::TDescriptorSetLayout::TLayout::TBindings bindings_3;
        bindings_3.insert({88, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::UNIFORM_BUFFER_DYNAMIC, 90)});
        bindings_3.insert({99, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::STORAGE_BUFFER_DYNAMIC, 100)});
        bindings_3.insert({100, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::INPUT_ATTACHMENT, 2000)});

        Turbo::Core::TShader::TLayout::TSets sets_0;
        sets_0[0] = bindings_0;
        sets_0[1] = bindings_1;

        Turbo::Core::TShader::TLayout::TSets sets_1;
        sets_1[0] = bindings_2;
        sets_1[2] = bindings_3;

        Turbo::Core::TShader::TLayout layout_0(sets_0, push_constant_0);
        Turbo::Core::TShader::TLayout layout_1(sets_1, push_constant_1);

        std::cout << "layout_0:" << std::endl;
        std::cout << layout_0.ToString() << std::endl;
        std::cout << "-----------------------------------------------" << std::endl;
        std::cout << "layout_1:" << std::endl;
        std::cout << layout_1.ToString() << std::endl;
        std::cout << "-----------------------------------------------" << std::endl;
        std::cout << "layout_1 merge into layout_0:" << std::endl;
        layout_0.Merge(layout_1);
        std::cout << layout_0.ToString() << std::endl;
    }

    if (false)
    {
        DescriptorSetLayout::Layout layout; // empty layout
        Turbo::Core::TRefPtr<DescriptorSetLayout> descriptor_set_layout = new DescriptorSetLayout(device, layout);
        if (descriptor_set_layout.Valid())
        {
            std::cout << "[OK] DescriptorSetLayout create successed!" << std::endl;
        }
        else
        {
            std::cout << "[Error] DescriptorSetLayout create failed!" << std::endl;
        }
    }

    if (false)
    {
        Turbo::Core::TDescriptorSetLayout::TLayout::TBindings bindings_0;
        bindings_0.insert({0, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::SAMPLER, 1)});
        bindings_0.insert({2, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::COMBINED_IMAGE_SAMPLER, 2)});
        bindings_0.insert({12, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::SAMPLED_IMAGE, 30)});
        bindings_0.insert({23, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::STORAGE_IMAGE, 40)});
        bindings_0.insert({43, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::UNIFORM_TEXEL_BUFFER, 50)});

        Turbo::Core::TDescriptorSetLayout::TLayout::TBindings bindings_1;
        bindings_1.insert({55, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::STORAGE_TEXEL_BUFFER, 60)});
        bindings_1.insert({16, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::UNIFORM_BUFFER, 70)});
        bindings_1.insert({67, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::STORAGE_BUFFER, 80)});
        bindings_1.insert({88, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::UNIFORM_BUFFER_DYNAMIC, 90)});
        bindings_1.insert({99, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::STORAGE_BUFFER_DYNAMIC, 100)});
        bindings_1.insert({100, Turbo::Core::TDescriptor(Turbo::Core::TDescriptor::TType::INPUT_ATTACHMENT, 2000)});

        Turbo::Core::TRefPtr<Turbo::Core::TDescriptorSetLayout> descriptor_set_layout_0 = new Turbo::Core::TDescriptorSetLayout(device, bindings_0);
        if (descriptor_set_layout_0.Valid())
        {
            std::cout << "[OK] DescriptorSetLayout 0 create successed!" << std::endl;
        }
        else
        {
            std::cout << "[Error] DescriptorSetLayout 0 create failed!" << std::endl;
        }

        Turbo::Core::TDescriptorSetLayout::TLayout layout_1(bindings_1);
        Turbo::Core::TRefPtr<Turbo::Core::TDescriptorSetLayout> descriptor_set_layout_1 = new Turbo::Core::TDescriptorSetLayout(device, layout_1);
        if (descriptor_set_layout_1.Valid())
        {
            std::cout << "[OK] DescriptorSetLayout 1 create successed!" << std::endl;
        }
        else
        {
            std::cout << "[Error] DescriptorSetLayout 1 create failed!" << std::endl;
        }

        layout_1.Merge(bindings_0);
        Turbo::Core::TRefPtr<Turbo::Core::TDescriptorSetLayout> descriptor_set_layout_2 = new Turbo::Core::TDescriptorSetLayout(device, layout_1);
        if (descriptor_set_layout_2.Valid())
        {
            std::cout << "[OK] DescriptorSetLayout 2 create successed!" << std::endl;
        }
        else
        {
            std::cout << "[Error] DescriptorSetLayout 2 create failed!" << std::endl;
        }
    }

    return 0;
}