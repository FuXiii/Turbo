#pragma once
#ifndef TURBO_CORE_TDESCRIPTORSETLAYOUT_H
#define TURBO_CORE_DESCRIPTORSETLAYOUT_H
#include "TDescriptor.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDevice;

class TDescriptorSetLayout : public Turbo::Core::TVulkanHandle
{
  public:
    class Layout
    {
      private:
        TBindings bindings;

      public:
        Layout() = default;
        Layout(const TBindings &bindings);

        std::size_t GetCount() const;

        TBindings::const_iterator begin() const;
        TBindings::const_iterator end() const;

        // TODO: hash for key

        bool operator==(const Layout &other) const;
        bool operator!=(const Layout &other) const;
    };

  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device;
    T_VULKAN_HANDLE_HANDLE VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;

    T_VULKAN_HANDLE_DATA std::vector<TDescriptor *> descriptors;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    // TDescriptorSetLayout(const TRefPtr<TDevice> &device, std::vector<TDescriptor *> &descriptors);
    TDescriptorSetLayout(TDevice *device, const std::vector<TDescriptor *> &descriptors);
    // TDescriptorSetLayout(TDevice *device, const Layout &layout); // FIXME: implement it!!!

  protected:
    virtual ~TDescriptorSetLayout();

  public:
    uint32_t GetSet() const;
    VkDescriptorSetLayout GetVkDescriptorSetLayout();
    TDescriptor::Type GetDescriptorType(uint32_t binding) const;

    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};

} // namespace Core
} // namespace Turbo

namespace std
{
template <>
class hash<Turbo::Core::TDescriptorSetLayout::Layout>
{
  public:
    std::size_t operator()(const Turbo::Core::TDescriptorSetLayout::Layout &layout) const
    {
        class TLayoutHasher
        {
          private:
            std::string *str = nullptr;

          public:
            TLayoutHasher(const Turbo::Core::TDescriptorSetLayout::Layout &layout)
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
#endif // !TURBO_CORE_TDESCRIPTORSETLAYOUT_H