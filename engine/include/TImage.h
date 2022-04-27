#pragma once
#ifndef TIMAGE_H
#define TIMAGE_H
#include "TFormatInfo.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDevice;
class TDeviceMemory;

class TImage : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkImage vkImage = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_HANDLE void *vmaAllocation = nullptr;
    T_VULKAN_HANDLE_CHILDREN;

    T_VULKAN_HANDLE_DATA VkImageCreateFlags imageFlags;
    T_VULKAN_HANDLE_DATA TMemoryFlags memoryFlags = 0;
    T_VULKAN_HANDLE_DATA VkImageType type;
    T_VULKAN_HANDLE_DATA TFormatInfo format;
    T_VULKAN_HANDLE_DATA VkExtent3D extent;
    T_VULKAN_HANDLE_DATA uint32_t mipLevels;
    T_VULKAN_HANDLE_DATA uint32_t arrayLayers;
    T_VULKAN_HANDLE_DATA VkSampleCountFlagBits samples;
    T_VULKAN_HANDLE_DATA VkImageTiling tiling;
    T_VULKAN_HANDLE_DATA VkImageUsageFlags usage;
    T_VULKAN_HANDLE_DATA VkImageLayout layout;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TImage(TDevice *device, VkImageCreateFlags imageFlags, VkImageType type, TFormatInfo format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage, TMemoryFlags memoryFlags, VkImageLayout layout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED);
    ~TImage();

  public:
    VkImage GetVkImage();
    TFormatInfo GetFormat();
    TDevice *GetDevice();

    uint32_t GetWidth();
    uint32_t GetHeight();
    uint32_t GetDepth();

    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // TIMAGE_H