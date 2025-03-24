#include <iostream>
#include <TReferenced.h>
#include <TRefPtr.h>
#include <vulkan/vulkan_core.h>

#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include <TDevice.h>
#include <TDeviceQueue.h>
#include <TVulkanHandle.h>
#include <TVulkanAllocator.h>
#include <TVulkanLoader.h>

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
    Type type = Type::SAMPLER; //描述符类型
    size_t count = 0;          //数量

  public:
    Descriptor() = default;
    Descriptor(Type type, size_t count = 1)
    {
        this->type = type;
        this->count = count;
    }

    Type GetType() const
    {
        return this->type;
    }

    size_t GetCount() const
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
    DescriptorSetLayout(Turbo::Core::TDevice *device, const Bindings &bindings)
    {
        this->device = device;
        this->layout = bindings;

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

//namespace std {
//    template<>
//    class hash<DescriptorSetLayout::Layout>
//    {
//
//    };
//}


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

int main()
{
    auto vulkan_verssion = Turbo::Core::TInstance::GetVulkanInstanceVersion();

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

    Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance = new Turbo::Core::TInstance(enable_layers, enable_extensions, vulkan_verssion);

    auto physical_device = instance->GetBestPhysicalDevice();

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

    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device = new Turbo::Core::TDevice(physical_device, {}, enable_device_extensions, {});
    Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> queue = device->GetBestGraphicsQueue();

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

    if (true)
    {
        std::unordered_map<DescriptorSetLayout::Layout, Turbo::Core::TRefPtr<DescriptorSetLayout>> layout_map;
        if (layout_map.empty())
        {
            std::cout << "layout_map empty" << std::endl;
        }
        else
        {
            std::cout << "layout_map bot empty" << std::endl;
        }
    }

    return 0;
}