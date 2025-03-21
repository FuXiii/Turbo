#include <iostream>
#include <TReferenced.h>
#include <TRefPtr.h>
#include <vulkan/vulkan_core.h>

#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>

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

class DescriptorSetLayout : public Turbo::Core::TReferenced // OK
{
  public:
    class Layout
    {
      private:
        Bindings bindings;
    };

  private:
    VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;

    Bindings bindings;

  protected:
    ~DescriptorSetLayout()
    {
    }

  public:
    DescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout, const Bindings &bindings)
    {
        this->vkDescriptorSetLayout = descriptorSetLayout;
        this->bindings = bindings;
    }
};

using Set = size_t;
using Sets = std::unordered_map<Set, Turbo::Core::TRefPtr<DescriptorSetLayout>>;

using Offset = uint32_t;
using Size = uint32_t;
using PushConstants = std::unordered_map<Offset, std::unordered_map<Size, VkShaderStageFlags>>;

class PipelineLayout : public Turbo::Core::TReferenced // OK
{
  private:
    VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

    Sets sets;
    PushConstants pushConstants;

  protected:
    ~PipelineLayout()
    {
    }

  public:
    PipelineLayout(VkPipelineLayout pipelineLayout, const Sets &sets, const PushConstants &pushConstants)
    {
        this->vkPipelineLayout = pipelineLayout;
        this->sets = sets;
        this->pushConstants = pushConstants;
    }
};

int main()
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
    return 0;
}