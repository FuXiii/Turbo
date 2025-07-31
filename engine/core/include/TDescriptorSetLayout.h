#pragma once
#ifndef TURBO_CORE_TDESCRIPTORSETLAYOUT_H
#define TURBO_CORE_DESCRIPTORSETLAYOUT_H
#include "TDescriptor.h"
#include "TVulkanHandle.h"

#include <algorithm>

namespace Turbo
{
namespace Core
{
class TDevice;

class TDescriptorSetLayout : public Turbo::Core::TVulkanHandle
{
  public:
    class TLayout
    {
      public:
        using TBinding = std::size_t;
        using TBindings = std::unordered_map<TBinding, TDescriptor>;

      private:
        TBindings bindings;

      public:
        TLayout() = default;
        TLayout(const TBindings &bindings);

        std::size_t GetCount() const;
        bool Empty() const;

        TBindings::const_iterator begin() const;
        TBindings::const_iterator end() const;

        void Merge(TDescriptorSetLayout::TLayout::TBinding binding, const TDescriptor &descriptor);
        void Merge(const TBindings &bindings);
        void Merge(const TDescriptorSetLayout::TLayout &layout);

        bool Has(const TBinding &binding) const;

        // void Override(const TDescriptorSetLayout::TLayout &layout);

        TDescriptor &operator[](TBinding &&binding);
        const TDescriptor &operator[](const TBinding &binding) const;

        bool operator==(const TDescriptorSetLayout::TLayout &other) const;
        bool operator!=(const TDescriptorSetLayout::TLayout &other) const;

        std::size_t Hash() const;

        std::string ToString() const;
    };

  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device;
    T_VULKAN_HANDLE_HANDLE VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;

    [[deprecated]] std::vector<TDescriptor *> descriptors;
    TDescriptorSetLayout::TLayout layout;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    // TDescriptorSetLayout(const TRefPtr<TDevice> &device, std::vector<TDescriptor *> &descriptors);
    TDescriptorSetLayout(TDevice *device, const std::vector<TDescriptor *> &descriptors);
    TDescriptorSetLayout(TDevice *device, const TDescriptorSetLayout::TLayout &layout);

  protected:
    virtual ~TDescriptorSetLayout();

  public:
    //[[deprecated]] uint32_t GetSet() const;

    const TDescriptorSetLayout::TLayout &GetLayout() const;
    VkDescriptorSetLayout GetVkDescriptorSetLayout();
    [[deprecated]] TDescriptor::TType GetDescriptorType(uint32_t binding) const; // FIXME: change to: TDescriptor GetDescriptor(uint32_t binding) const

    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};

} // namespace Core
} // namespace Turbo

namespace std
{
template <>
class hash<Turbo::Core::TDescriptorSetLayout::TLayout>
{
  public:
    std::string ToHashString(const Turbo::Core::TDescriptorSetLayout::TLayout &layout) const
    {
        std::string result;
        if (!layout.Empty())
        {
            std::vector<Turbo::Core::TDescriptorSetLayout::TLayout::TBinding> ordered_bindings;
            {
                for (auto &binding_item : layout)
                {
                    ordered_bindings.push_back(binding_item.first);
                }
                std::sort(ordered_bindings.begin(), ordered_bindings.end());
            }

            // for (auto &binding_item : layout)
            for (auto &binding : ordered_bindings)
            {
                auto &binding_item = layout[binding];
                auto &descriptor_type = binding_item.GetType();
                auto &descriptor_count = binding_item.GetCount();

                result.append(reinterpret_cast<const std::string::value_type *>(&binding), sizeof(binding));
                result.append(reinterpret_cast<const std::string::value_type *>(&descriptor_type), sizeof(descriptor_type));
                result.append(reinterpret_cast<const std::string::value_type *>(&descriptor_count), sizeof(descriptor_count));
            }
        }
        return result;
    }

    std::size_t operator()(const Turbo::Core::TDescriptorSetLayout::TLayout &layout) const
    {
        return std::hash<std::string>{}(ToHashString(layout));
    }
};
} // namespace std
#endif // !TURBO_CORE_TDESCRIPTORSETLAYOUT_H