#pragma once
#ifndef TURBO_CORE_TIMAGE_H
#define TURBO_CORE_TIMAGE_H
#include "TFormatInfo.h"
#include "TMemoryTypeInfo.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Extension
{
class TSwapchain;
}
} // namespace Turbo

namespace Turbo
{
namespace Core
{
class TDevice;
class TDeviceMemory;

typedef enum TImageTiling
{
    OPTIMAL = 0,
    LINEAR = 1
} TImageTiling;

typedef enum TSampleCountBits
{
    SAMPLE_1_BIT = 0x00000001,
    SAMPLE_2_BIT = 0x00000002,
    SAMPLE_4_BIT = 0x00000004,
    SAMPLE_8_BIT = 0x00000008,
    SAMPLE_16_BIT = 0x00000010,
    SAMPLE_32_BIT = 0x00000020,
    SAMPLE_64_BIT = 0x00000040,
} TSampleCountBits;
typedef VkFlags TSampleCounts;

typedef enum TImageType
{
    DIMENSION_1D = 0,
    DIMENSION_2D = 1,
    DIMENSION_3D = 2,
} TImageType;

typedef enum TImageUsageBits
{
    IMAGE_TRANSFER_SRC = 0x00000001,
    IMAGE_TRANSFER_DST = 0x00000002,
    IMAGE_SAMPLED = 0x00000004,
    IMAGE_STORAGE = 0x00000008,
    IMAGE_COLOR_ATTACHMENT = 0x00000010,
    IMAGE_DEPTH_STENCIL_ATTACHMENT = 0x00000020,
    IMAGE_TRANSIENT_ATTACHMENT = 0x00000040,
    IMAGE_INPUT_ATTACHMENT = 0x00000080,
} TImageUsageBits;
typedef VkFlags TImageUsages;

typedef enum class TImageLayout
{
    UNDEFINED = 0,
    GENERAL = 1,
    COLOR_ATTACHMENT_OPTIMAL = 2,
    DEPTH_STENCIL_ATTACHMENT_OPTIMAL = 3,
    DEPTH_STENCIL_READ_ONLY_OPTIMAL = 4,
    SHADER_READ_ONLY_OPTIMAL = 5,
    TRANSFER_SRC_OPTIMAL = 6,
    TRANSFER_DST_OPTIMAL = 7,
    PREINITIALIZED = 8,
    DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL = 1000117000,
    DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL = 1000117001,
    DEPTH_ATTACHMENT_OPTIMAL = 1000241000,
    DEPTH_READ_ONLY_OPTIMAL = 1000241001,
    STENCIL_ATTACHMENT_OPTIMAL = 1000241002,
    STENCIL_READ_ONLY_OPTIMAL = 1000241003,
    READ_ONLY_OPTIMAL = 1000314000,
    ATTACHMENT_OPTIMAL = 1000314001,
    PRESENT_SRC_KHR = 1000001002,
} TImageLayout;

class TImage : public Turbo::Core::TVulkanHandle
{
  public:
    friend class Turbo::Extension::TSwapchain;

  private:
    // OLD:T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device = nullptr;
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device;
    T_VULKAN_HANDLE_HANDLE VkImage vkImage = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_HANDLE void *vmaAllocation = nullptr;
    T_VULKAN_HANDLE_HANDLE void *vmaAllocationInfo = nullptr;
    T_VULKAN_HANDLE_CHILDREN;

    T_VULKAN_HANDLE_DATA VkImageCreateFlags imageFlags;
    T_VULKAN_HANDLE_DATA TMemoryFlags memoryFlags = 0;
    T_VULKAN_HANDLE_DATA TImageType type;
    T_VULKAN_HANDLE_DATA TFormatInfo format;
    T_VULKAN_HANDLE_DATA VkExtent3D extent;
    T_VULKAN_HANDLE_DATA uint32_t mipLevels;
    T_VULKAN_HANDLE_DATA uint32_t arrayLayers;
    T_VULKAN_HANDLE_DATA TSampleCountBits samples;
    T_VULKAN_HANDLE_DATA TImageTiling tiling;
    T_VULKAN_HANDLE_DATA TImageUsages usages;
    T_VULKAN_HANDLE_DATA TImageLayout layout;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  private:
    TImage(const TRefPtr<TDevice> &device, VkImage vkImage, VkImageCreateFlags imageFlags, TImageType type, TFormatInfo format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, TSampleCountBits samples, TImageTiling tiling, TImageUsages usages, TImageLayout layout); // for swapchain

  public:
    [[deprecated]] explicit TImage(const TRefPtr<TDevice> &device, VkImageCreateFlags imageFlags, TImageType type, TFormatInfo format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, TSampleCountBits samples, TImageTiling tiling, TImageUsages usages, TMemoryFlags memoryFlags, TImageLayout layout = TImageLayout::UNDEFINED);
    explicit TImage(const TRefPtr<TDevice> &device, VkImageCreateFlags imageFlags, TImageType type, TFormatType formatType, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, TSampleCountBits samples, TImageTiling tiling, TImageUsages usages, TMemoryFlags memoryFlags, TImageLayout layout = TImageLayout::UNDEFINED);

  protected:
    virtual ~TImage();

  public:
    VkImage GetVkImage();
    TFormatInfo GetFormat() const;
    const TRefPtr<TDevice> &GetDevice();

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    uint32_t GetDepth() const;

    TSampleCountBits GetSampleCountBits() const;

    TImageUsages GetUsages() const;
    uint32_t GetMipLevels() const;
    uint32_t GetArrayLayers() const;

    TMemoryTypeInfo GetMemoryTypeInfo() const;

    bool IsMappable() const;
    void *Map();
    void Unmap();

    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TIMAGE_H