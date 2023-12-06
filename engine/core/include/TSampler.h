#pragma once
#ifndef TURBO_CORE_TSAMPLER_H
#define TURBO_CORE_TSAMPLER_H
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDevice;

typedef enum class TFilter
{
    NEAREST = 0,
    LINEAR = 1,
} TFilter;

typedef enum class TMipmapMode
{
    NEAREST = 0,
    LINEAR = 1,
} TMipmapMode;

typedef enum class TAddressMode
{
    REPEAT = 0,
    MIRRORED_REPEAT = 1,
    CLAMP_TO_EDGE = 2,
    CLAMP_TO_BORDER = 3,
    MIRROR_CLAMP_TO_EDGE = 4,
} TAddressMode;

typedef enum class TBorderColor
{
    FLOAT_TRANSPARENT_BLACK = 0,
    INT_TRANSPARENT_BLACK = 1,
    FLOAT_OPAQUE_BLACK = 2,
    INT_OPAQUE_BLACK = 3,
    FLOAT_OPAQUE_WHITE = 4,
    INT_OPAQUE_WHITE = 5,
} TBorderColor;

class TSampler : public TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device;
    T_VULKAN_HANDLE_HANDLE VkSampler vkSampler = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN;

    TFilter minFilter;
    TFilter magFilter;
    TMipmapMode mipmapMode;
    TAddressMode addressModeU;
    TAddressMode addressModeV;
    TAddressMode addressModeW;
    TBorderColor borderColor;
    float mipLodBias;
    float minLod;
    float maxLod;

    bool anisotropyEnable;
    float maxAnisotropy;

    // bool compareEnable; //目前为VK_FALSE(直到深度测试完善)

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TSampler(const TRefPtr<TDevice> &device, TFilter minFilter = TFilter::LINEAR, TFilter magFilter = TFilter::LINEAR, TMipmapMode mipmapMode = TMipmapMode::LINEAR, TAddressMode addressModeU = TAddressMode::REPEAT, TAddressMode addressModeV = TAddressMode::REPEAT, TAddressMode addressModeW = TAddressMode::REPEAT, TBorderColor borderColor = TBorderColor::FLOAT_OPAQUE_WHITE, float mipLodBias = 0.0f, float minLod = 0.0f, float maxLod = 0.0f);

  protected:
    virtual ~TSampler();

  public:
    VkSampler GetVkSampler();

  public:
    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TSAMPLER_H