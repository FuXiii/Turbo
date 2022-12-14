#pragma once
#ifndef TURBO_RENDER_TSURFACE_H
#define TURBO_RENDER_TSURFACE_H

#ifndef VULKAN_CORE_H_
#include <core/include/vulkan/vulkan_core.h>
#endif

namespace Turbo
{
namespace Core
{
} // namespace Core
} // namespace Turbo

namespace Turbo
{
namespace Render
{
class TSurface
{
  private:
    VkSurfaceKHR vkSurfaceKHR = VK_NULL_HANDLE;

  public:
    TSurface(uint32_t width, uint32_t height, uint32_t layer, uint32_t imageCount); // 对应着[虚拟Surface]
    TSurface(VkSurfaceKHR vkSurfaceKHR);                                            // 对应着[真实Surface]

    bool IsVirtualSurface();
    VkSurfaceKHR GetVkSurfaceKHR();
};
} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TSURFACE_H