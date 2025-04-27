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

template <typename T, typename = std::enable_if_t<std::is_integral<T>::value || std::is_enum<T>::value>>
class TFlags
{
  private:
    std::array<char, sizeof(T)> flags = {0};

  public:
    TFlags() = default;

    TFlags(const T &flags)
    {
        std::memcpy(this->flags.data(), &flags, sizeof(T));
    }

    operator const T &() const
    {
        return this->flags;
    }

    bool Has(const T &flags)
    {
        const char *compare_flags = (const char *)(&flags);
        for (std::size_t i = 0; i < this->flags.size(); i++)
        {
            const char current_byte = *(compare_flags + i);
            if ((this->flags[i] & current_byte) != current_byte)
            {
                return false;
            }
        }
        return true;
    }

    TFlags &operator|=(const TFlags &other)
    {
        for (std::size_t i = 0; i < this->flags.size(); i++)
        {
            this->flags[i] |= other.flags[i];
        }

        return *this;
    }

    TFlags operator|(const TFlags &other)
    {
        TFlags temp_flags = *this;
        temp_flags |= other;
        return temp_flags;
    }

    std::string ToString() const
    {
        std::string str;
        for (auto riter = this->flags.rbegin(); riter != this->flags.rend(); riter++) // 大字头、小字头
        {
            str += std::bitset<8 * sizeof(char)>(*riter).to_string() + " ";
        }
        str.pop_back();
        return str;
    }
};

template <typename T>
TFlags<T> operator|(const T &left, const T &right)
{
    TFlags<T> flags;
    flags |= left;
    flags |= right;
    return flags;
}

template <typename T>
TFlags<T> operator|(const T &left, const TFlags<T> &right)
{
    TFlags<T> flags;
    flags |= left;
    flags |= right;
    return flags;
}

// template <typename T>
// TFlags<T> operator|(const TFlags<T> &left, const T &right)
//{
//     TFlags<T> flags = left;
//     flags |= right;
//     return flags;
// }
//
// template <typename T>
// TFlags<T> operator|(const T &left, const TFlags<T> &right)
//{
//     TFlags<T> flags = left;
//     flags |= right;
//     return flags;
// }

template <typename T>
std::ostream &operator<<(std::ostream &os, const TFlags<T> &flags)
{
    return os << flags.ToString();
}

// template <typename T>
// TFlags<T> &operator|(const T &left, const T &right)
//{
//     // std::bitset<sizeof(T)> bit_set_left=left;
//     return left | right;
// }

void FlagsTest()
{
    // TFlags flags = TestFlagBits::_1 | TestFlagBits::_10;
    // std::cout << "flags: " << flags << std::endl;

    TFlags flags = TestFlagBits::_10000 | TestFlagBits::_1000 | TestFlagBits::_10;
    flags |= TestFlagBits::_100;
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

int main()
{
    VulkanContext vulkan_context;

    Turbo::Core::TInstance *instance = vulkan_context.Instance();
    Turbo::Core::TDevice *device = vulkan_context.Device();
    Turbo::Core::TDeviceQueue *queue = vulkan_context.Queue();

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
        Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TConstants constants_0;
        constants_0[0][512] = 0b00001;
        constants_0[10][123] = 0b00011;
        constants_0[21][456] = 0b00101;

        Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TConstants constants_1;
        constants_1[0][512] = 0b00110;
        constants_1[10][123] = 0b00100;
        constants_1[21][456] = 0b10001;
        constants_1[21][455] = 0b01111;
        constants_1[33][789] = 0b00001;

        Turbo::Core::TPipelineLayout::TLayout::TPushConstants push_constants_0(constants_0);
        std::cout << push_constants_0.ToString() << std::endl;

        Turbo::Core::TPipelineLayout::TLayout::TPushConstants push_constants_1(constants_1);
        std::cout << push_constants_1.ToString() << std::endl;

        push_constants_0.Merge(push_constants_1);
        std::cout << push_constants_0.ToString() << std::endl;
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
        Turbo::Core::TShader::TLayout::TPushConstant push_constant_0(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, 0, 512);
        Turbo::Core::TShader::TLayout::TPushConstant push_constant_1(VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, 0, 512);

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

    FlagsTest();

    return 0;
}