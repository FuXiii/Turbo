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

        class TPushConstants // FIXME: Move it to Pipeline Layout!!!
        {
          public:
            using TOffset = uint32_t;
            using TSize = uint32_t;
            using TConstants = std::unordered_map<TOffset, std::unordered_map<TSize, VkShaderStageFlags>>;

          private:
            TPushConstants::TConstants constants;

          public:
            TPushConstants() = default;
            TPushConstants(const TPushConstants::TConstants &constants);
            TPushConstants(TPushConstants::TConstants &&constants);

            const TPushConstants::TConstants &GetConstants() const;
            bool Empty() const;

            void Merge(TPushConstants::TOffset offset, TPushConstants::TSize size, VkShaderStageFlags flags);
            void Merge(const TPushConstants &pushConstants);

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

        void Merge(const TPipelineLayout::TLayout::TSets &sets);
        void Merge(const TPipelineLayout::TLayout::TPushConstants &pushConstants);
        void Merge(const TShader::TLayout::TPushConstant &pushConstant);

        void Merge(const Turbo::Core::TShader::TLayout &layout);
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

    TPipelineLayout(TDevice *device, const TPipelineLayout::TLayout &layout);

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
#endif // !TURBO_CORE_TPIPELINELAYOUT_H