#pragma once
#ifndef TURBO_CORE_TVULKANHANDLE_H
#define TURBO_CORE_TVULKANHANDLE_H
#include "TObject.h"

#define T_VULKAN_HANDLE_PARENT

#define T_VULKAN_HANDLE_HANDLE

#define T_VULKAN_HANDLE_CHILDREN

#define T_VULKAN_HANDLE_DATA

#define T_VULKAN_HANDLE_REFRESH_DATA

namespace Turbo
{
namespace Core
{
class TVulkanHandle : public Turbo::Core::TObject
{
  public:
    TVulkanHandle();
    ~TVulkanHandle();

  protected:
    // virtual void AddChildHandle(TVulkanHandle* handle);
    // virtual void RemoveChildHandle(TVulkanHandle* handle);
    virtual void InternalRebuild();
    virtual void InternalCreate() = 0;
    virtual void InternalDestroy() = 0;

  public:
    // disallow copy and assignment
    TVulkanHandle(TVulkanHandle const &) = delete;
    TVulkanHandle(TVulkanHandle &&) = delete;
    TVulkanHandle &operator=(TVulkanHandle const &) = delete;
    TVulkanHandle &operator=(TVulkanHandle &&) = delete;
};

} // namespace Core
} // namespace Turbo

#endif // !TURBO_CORE_TVULKANHANDLE_H