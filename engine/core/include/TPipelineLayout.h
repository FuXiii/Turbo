#pragma once
#ifndef TURBO_CORE_TPIPELINELAYOUT_H
#define TURBO_CORE_TPIPELINELAYOUT_H
#include "TVulkanHandle.h"
#include <unordered_map>
#include "TDescriptorSetLayout.h"
#include "TShader.h"
namespace Turbo
{
namespace Core
{
class TDescriptorSetLayout;
class TDevice;
class TPushConstantDescriptor;

class TPipelineLayout : public Turbo::Core::TVulkanHandle
{
  public:
    class TLayout
    {
      public:
        using TSet = std::size_t;
        using TSets = std::unordered_map<TSet, Turbo::Core::TDescriptorSetLayout::TLayout>;

        // NOTE: Only one push_constant block is allowed per stage
        // NOTE: VkPipelineLayoutCreateInfo::pPushConstantRanges : Any two elements of pPushConstantRanges must not include the same stage in stageFlags
        class TPushConstants
        {
          public:
            using TOffset = uint32_t;
            using TSize = uint32_t;
            // using TConstants = std::unordered_map<TOffset, std::unordered_map<TSize, VkShaderStageFlags>>;
            //  NOTE: One push constant pure shader type
            using TConstants = std::unordered_map<Turbo::Core::TShaderType, std::pair<TOffset, TSize>>; // TODO: New standard

          private:
            TPushConstants::TConstants constants;

          public:
            TPushConstants() = default;
            TPushConstants(const Turbo::Core::TShaderType &shaderType, const TPushConstants::TOffset &offset, const TPushConstants::TSize &size);
            TPushConstants(const TPushConstants::TConstants &constants);
            TPushConstants(TPushConstants::TConstants &&constants);

            const TPushConstants::TConstants &GetConstants() const;
            bool Empty() const;

            void Merge(const Turbo::Core::TShaderType &shaderType, const TPushConstants::TOffset &offset, const TPushConstants::TSize &size);
            void Merge(const TPushConstants &pushConstants);
            void Merge(const Turbo::Core::TShader::TLayout::TPushConstant &pushConstant);

            std::string ToString() const;
        };

      private:
        TPipelineLayout::TLayout::TSets sets;
        TPipelineLayout::TLayout::TPushConstants pushConstants;

      public:
        TLayout() = default;
        TLayout(const TPipelineLayout::TLayout::TSets &sets, const TPipelineLayout::TLayout::TPushConstants &pushConstants);
        TLayout(const TPipelineLayout::TLayout::TSets &sets);
        TLayout(const TPipelineLayout::TLayout::TPushConstants &pushConstants);
        TLayout(TPipelineLayout::TLayout::TSets &&sets, TPipelineLayout::TLayout::TPushConstants &&pushConstants);

        const TPipelineLayout::TLayout::TSets &GetSets() const;
        const TPipelineLayout::TLayout::TPushConstants &GetPushConstants() const;

        void Merge(const TPipelineLayout::TLayout::TSets &sets);
        void Merge(const TPipelineLayout::TLayout::TPushConstants &pushConstants);
        void Merge(const Turbo::Core::TShader::TLayout &layout);

        std::string ToString() const;
    };

  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device;
    T_VULKAN_HANDLE_HANDLE VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

    T_VULKAN_HANDLE_DATA std::vector<TRefPtr<TDescriptorSetLayout>> descriptorSetLayouts;
    T_VULKAN_HANDLE_DATA std::vector<TPushConstantDescriptor *> pushConstantDescriptors; // FIXME: check it

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TPipelineLayout(TDevice *device, const std::vector<TDescriptorSetLayout *> &descriptorSetLayouts, std::vector<TPushConstantDescriptor *> &pushConstantDescriptors /*FIXME: check it*/);
    TPipelineLayout(TDevice *device, const std::vector<TRefPtr<TDescriptorSetLayout>> &descriptorSetLayouts, std::vector<TPushConstantDescriptor *> &pushConstantDescriptors /*FIXME: check it*/);

    TPipelineLayout(TDevice *device, const TPipelineLayout::TLayout &layout); // TODO: Implemented

  protected:
    virtual ~TPipelineLayout();

  public:
    std::vector<TDescriptorSetLayout *> GetDescriptorSetLayouts();
    const std::vector<TPushConstantDescriptor *> &GetPushConstantDescriptors(); // FIXME: check it

    VkPipelineLayout GetVkPipelineLayout();

    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};

} // namespace Core
} // namespace Turbo

namespace std
{
template <>
class hash<Turbo::Core::TPipelineLayout::TLayout>
{
  public:
    std::size_t operator()(const Turbo::Core::TPipelineLayout::TLayout &layout) const
    {
        class TLayoutHasher
        {
          private:
            std::string *str = nullptr;

          public:
            TLayoutHasher(const Turbo::Core::TPipelineLayout::TLayout &layout)
            {
                this->str = new std::string();
                for (auto &set_item : layout.GetSets())
                {
                    auto set = set_item.first;
                    this->str->append(reinterpret_cast<const std::string::value_type *>(&set), sizeof(set));
                    for (auto &binding_item : set_item.second)
                    {
                        auto &binding = binding_item.first;
                        auto &descriptor_type = binding_item.second.GetType();
                        auto &descriptor_count = binding_item.second.GetCount();

                        this->str->append(reinterpret_cast<const std::string::value_type *>(&binding), sizeof(binding));
                        this->str->append(reinterpret_cast<const std::string::value_type *>(&descriptor_type), sizeof(descriptor_type));
                        this->str->append(reinterpret_cast<const std::string::value_type *>(&descriptor_count), sizeof(descriptor_count));
                    }
                }

                for (auto &constant_item : layout.GetPushConstants().GetConstants())
                {
                    auto shader_type = constant_item.first;
                    this->str->append(reinterpret_cast<const std::string::value_type *>(&shader_type), sizeof(shader_type));

                    auto offset = constant_item.second.first;
                    auto size = constant_item.second.second;
                    this->str->append(reinterpret_cast<const std::string::value_type *>(&offset), sizeof(offset));
                    this->str->append(reinterpret_cast<const std::string::value_type *>(&size), sizeof(size));
                }
            }

            ~TLayoutHasher()
            {
                if (this->str != nullptr)
                {
                    delete this->str;
                    this->str = nullptr;
                }
            }

            std::size_t Hash() const
            {
                return std::hash<std::string>{}(*(this->str));
            }
        };

        return TLayoutHasher(layout).Hash();
    }
};
} // namespace std
#endif // !TURBO_CORE_TPIPELINELAYOUT_H