#pragma once
#ifndef TURBO_CORE_TIMAGEVIEW_H
#define TURBO_CORE_TIMAGEVIEW_H
#include "TFormatInfo.h"
#include "TInfo.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TImage;

typedef enum TImageViewType
{
    IMAGE_VIEW_1D = 0,
    IMAGE_VIEW_2D = 1,
    IMAGE_VIEW_3D = 2,
    IMAGE_VIEW_CUBE = 3,
    IMAGE_VIEW_1D_ARRAY = 4,
    IMAGE_VIEW_2D_ARRAY = 5,
    IMAGE_VIEW_CUBE_ARRAY = 6,
} TImageViewType;

typedef enum TImageAspectBits
{
    ASPECT_NONE_KHR = 0,
    ASPECT_COLOR_BIT = 0x00000001,
    ASPECT_DEPTH_BIT = 0x00000002,
    ASPECT_STENCIL_BIT = 0x00000004,
    ASPECT_METADATA_BIT = 0x00000008,
    ASPECT_PLANE_0_BIT = 0x00000010,
    ASPECT_PLANE_1_BIT = 0x00000020,
    ASPECT_PLANE_2_BIT = 0x00000040,
    ASPECT_MEMORY_PLANE_0_BIT_EXT = 0x00000080,
    ASPECT_MEMORY_PLANE_1_BIT_EXT = 0x00000100,
    ASPECT_MEMORY_PLANE_2_BIT_EXT = 0x00000200,
    ASPECT_MEMORY_PLANE_3_BIT_EXT = 0x00000400,
    ASPECT_PLANE_0_BIT_KHR = VK_IMAGE_ASPECT_PLANE_0_BIT,
    ASPECT_PLANE_1_BIT_KHR = VK_IMAGE_ASPECT_PLANE_1_BIT,
    ASPECT_PLANE_2_BIT_KHR = VK_IMAGE_ASPECT_PLANE_2_BIT,
} TImageAspectBits;
typedef VkFlags TImageAspects;

class TImageView : public TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TImage> image = nullptr;
    T_VULKAN_HANDLE_HANDLE VkImageView vkImageView = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN;

    T_VULKAN_HANDLE_DATA TImageViewType viewType;
    T_VULKAN_HANDLE_DATA TFormatInfo format;
    T_VULKAN_HANDLE_DATA TImageAspects aspects;
    T_VULKAN_HANDLE_DATA uint32_t baseMipLevel;
    T_VULKAN_HANDLE_DATA uint32_t levelCount;
    T_VULKAN_HANDLE_DATA uint32_t baseArrayLayer;
    T_VULKAN_HANDLE_DATA uint32_t layerCount;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    [[deprecated]] explicit TImageView(const TRefPtr<TImage> &image, TImageViewType viewType, TFormatInfo format, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    explicit TImageView(const TRefPtr<TImage> &image, TImageViewType viewType, TFormatType formatType, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);

  protected:
    virtual ~TImageView();

  public:
    TRefPtr<TImage> GetImage();

    VkImageView GetVkImageView();
    TImageViewType GetViewType() const;
    TFormatInfo GetFormat() const;
    TImageAspects GetAspects() const;
    uint32_t GetBaseMipLevel() const;
    uint32_t GetLevelCount() const;
    uint32_t GetBaseArrayLayer() const;
    uint32_t GetLayerCount() const;

    // Inherited via TObject
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TIMAGEVIEW_H