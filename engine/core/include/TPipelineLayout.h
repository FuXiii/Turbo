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
            // NOTE: One push constant pure shader type
            // NOTE: Both offset and size are in units of bytes and must be a multiple of 4.
            using TConstants = std::unordered_map<Turbo::Core::TShaderType, std::pair<TPushConstants::TOffset, TPushConstants::TSize>>; // TODO: New standard

            class TOffsets
            {
              public:
                using TOffsetsMap = std::unordered_map<Turbo::Core::TShaderType, TPushConstants::TOffset>; // TODO: New standard

              private:
                TOffsetsMap offsets;

              public:
                TOffsetsMap::const_iterator begin() const;
                TOffsetsMap::const_iterator end() const;

                void Merge(const Turbo::Core::TShaderType &shaderType, const TPushConstants::TOffset &offset);
            };

          private:
            TPushConstants::TConstants constants;

          private:
            // void RefreshPushConstantsOffset(const Turbo::Core::TShaderType &startShaderType);

          public:
            TPushConstants() = default;
            TPushConstants(const Turbo::Core::TShaderType &shaderType, const TPushConstants::TOffset &offset, const TPushConstants::TSize &size);
            TPushConstants(const TPushConstants::TConstants &constants);
            TPushConstants(TPushConstants::TConstants &&constants);

            const TPushConstants::TConstants &GetConstants() const;
            bool Empty() const;

            void Merge(const Turbo::Core::TShaderType &shaderType, const TPushConstants::TOffset &offset, const TPushConstants::TSize &size);
            void Merge(const Turbo::Core::TShaderType &shaderType, const TPushConstants::TOffset &offset);
            void Merge(const TPushConstants &pushConstants);
            void Merge(const Turbo::Core::TShader::TLayout::TPushConstant &pushConstant);

            const Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TSize &GetConstantSize(const Turbo::Core::TShaderType &shaderType) const;
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
        const TPipelineLayout::TLayout::TPushConstants::TSize &GetPushConstantSize(const Turbo::Core::TShaderType &shaderType) const;

        bool Empty() const;

        void Merge(const TPipelineLayout::TLayout::TSets &sets);
        void Merge(const TPipelineLayout::TLayout::TPushConstants &pushConstants);
        void Merge(const Turbo::Core::TShader::TLayout &layout);
        void Merge(const Turbo::Core::TShaderType &shaderType, const TPushConstants::TOffset &offset);

        std::size_t Hash() const;

        std::string ToString() const;
    };

  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device;
    T_VULKAN_HANDLE_HANDLE VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

    //[[deprecated]] T_VULKAN_HANDLE_DATA std::vector<TRefPtr<TDescriptorSetLayout>> descriptorSetLayouts;
    T_VULKAN_HANDLE_DATA std::unordered_map<TPipelineLayout::TLayout::TSet, TRefPtr<TDescriptorSetLayout>> descriptorSetLayouts;

    [[deprecated]] T_VULKAN_HANDLE_DATA std::vector<TPushConstantDescriptor *> pushConstantDescriptors; // FIXME: check it

    TPipelineLayout::TLayout layout;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    //[[deprecated]] TPipelineLayout(TDevice *device, const std::vector<TDescriptorSetLayout *> &descriptorSetLayouts, std::vector<TPushConstantDescriptor *> &pushConstantDescriptors /*FIXME: check it*/);
    //[[deprecated]] TPipelineLayout(TDevice *device, const std::vector<TRefPtr<TDescriptorSetLayout>> &descriptorSetLayouts, std::vector<TPushConstantDescriptor *> &pushConstantDescriptors /*FIXME: check it*/);

    TPipelineLayout(TDevice *device, const TPipelineLayout::TLayout &layout); // TODO: Implemented

  protected:
    virtual ~TPipelineLayout();

  public:
    //[[deprecated]] std::vector<TDescriptorSetLayout *> GetDescriptorSetLayouts();
    std::unordered_map<TPipelineLayout::TLayout::TSet, TDescriptorSetLayout *> GetDescriptorSetLayouts();
    [[deprecated]] const std::vector<TPushConstantDescriptor *> &GetPushConstantDescriptors(); // FIXME: check it

    const TPipelineLayout::TLayout &GetLayout() const;

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
    std::string ToHashString(const Turbo::Core::TPipelineLayout::TLayout &layout) const
    {
        std::string result;
        if (!layout.Empty())
        {
            auto &sets = layout.GetSets();
            std::vector<Turbo::Core::TPipelineLayout::TLayout::TSet> ordered_sets;
            {
                for (auto &set_item : sets)
                {
                    ordered_sets.push_back(set_item.first);
                }
                std::sort(ordered_sets.begin(), ordered_sets.end());
            }

            for (auto &set : ordered_sets)
            {
                result.append(reinterpret_cast<const std::string::value_type *>(&set), sizeof(set));

                auto &descriptor_set_layout = sets.at(set);

                result.append(std::hash<Turbo::Core::TDescriptorSetLayout::TLayout>{}.ToHashString(descriptor_set_layout));
            }

            auto &constants = layout.GetPushConstants().GetConstants();
            std::vector<Turbo::Core::TShaderType> ordered_shader_types;
            {
                for (auto &constant_item : constants)
                {
                    ordered_shader_types.push_back(constant_item.first);
                }
                std::sort(ordered_shader_types.begin(), ordered_shader_types.end());
            }

            for (auto &shader_type : ordered_shader_types)
            {
                auto &constant_item = constants.at(shader_type);
                result.append(reinterpret_cast<const std::string::value_type *>(&shader_type), sizeof(shader_type));

                auto offset = constant_item.first;
                auto size = constant_item.second;
                result.append(reinterpret_cast<const std::string::value_type *>(&offset), sizeof(offset));
                result.append(reinterpret_cast<const std::string::value_type *>(&size), sizeof(size));
            }
        }
        return result;
    }

    std::size_t operator()(const Turbo::Core::TPipelineLayout::TLayout &layout) const
    {
        return std::hash<std::string>{}(ToHashString(layout));
    }
};
} // namespace std

Turbo::Core::TPipelineLayout::TLayout &operator<<(Turbo::Core::TPipelineLayout::TLayout &layout, const Turbo::Core::TShader &shader);
Turbo::Core::TPipelineLayout::TLayout &operator<<(Turbo::Core::TPipelineLayout::TLayout &layout, const Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TOffsets &offsets);
#endif // !TURBO_CORE_TPIPELINELAYOUT_H