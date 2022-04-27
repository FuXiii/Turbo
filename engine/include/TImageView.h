#pragma once
#ifndef TIMAGEVIEW_H
#define TIMAGEVIEW_H
#include "TFormatInfo.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TImage;

class TImageView : public TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TImage *image = nullptr;
    T_VULKAN_HANDLE_HANDLE VkImageView vkImageView = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN;

    T_VULKAN_HANDLE_DATA VkImageViewType viewType;
    T_VULKAN_HANDLE_DATA TFormatInfo format;
    T_VULKAN_HANDLE_DATA VkImageAspectFlags aspectMask;
    T_VULKAN_HANDLE_DATA uint32_t baseMipLevel;
    T_VULKAN_HANDLE_DATA uint32_t levelCount;
    T_VULKAN_HANDLE_DATA uint32_t baseArrayLayer;
    T_VULKAN_HANDLE_DATA uint32_t layerCount;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TImageView(TImage *image, VkImageViewType viewType, TFormatInfo format, VkImageAspectFlags aspectMask, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    ~TImageView();

    TImage *GetImage();

    VkImageView GetVkImageView();

    // Inherited via TObject
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TIMAGEVIEW_H