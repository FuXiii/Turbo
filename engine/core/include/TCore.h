#pragma once
#ifndef TURBO_H
#define TURBO_H

#include <utility>
#include <vector>

#include "vulkan/vulkan.h"
//<Test Delete>
#include <iostream>
//</Test Delete>

#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#define TURBO_PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define TURBO_PLATFORM_APPLE
#elif defined(ANDROID) || defined(__ANDROID__)
#define TURBO_PLATFORM_ANDROID
#elif defined(__linux) || defined(__linux__)
#define TURBO_PLATFORM_LINUX
#elif defined(__unix) || defined(__unix__)
#define TURBO_PLATFORM_UNIX
#else
#define TURBO_PLATFORM_UNDEFINED
#endif

namespace Turbo
{
namespace Core
{
// Turbo Version
#define TURBO_MAKE_VERSION(major, minor, patch) (((major) << 22) | ((minor) << 10) | (patch))
#define TURBO_VERSION_MAJOR(version) ((uint32_t)version >> 22)
#define TURBO_VERSION_MINOR(version) ((uint32_t)version >> 10 & 0xfff)
#define TURBO_VERSION_PATCH(version) ((uint32_t)version & 0x3ff)

#define TURBO_VERSION_MAJOR_MAX ((~(uint32_t)0) >> 22)
#define TURBO_VERSION_MINOR_MAX ((~(uint32_t)0) >> 10 & 0xfff)
#define TURBO_VERSION_PATCH_MAX ((~(uint32_t)0) & 0x3ff)

// Version major/minor/patch
#define TURBO_DEFAULT_APPLICATION_NAME "Turbo"
#define TURBO_DEFAULT_ENGINE_NAME "Turbo"
#define TURBO_APPLICATION_VERSION_0_0 TURBO_MAKE_VERSION(0, 0, 0)
#define TURBO_ENGINE_VERSION_0_0 TURBO_MAKE_VERSION(0, 0, 0)
#define TYRBO_DEFAULT_SHADER_ENTRY_POINT "main"

#define VULKAN_GLOBAL_API
#define VULKAN_INSTANCE_API
#define VULKAN_DEVICE_API

#define VULKAN_CORE
#define VULKAN_EXTENSION

typedef VkDeviceSize TDeviceSize;

typedef enum class TResult : int32_t
{
    UNIMPLEMENTED = -17,
    SHADER_LINK_FAILED = -16,
    SHADER_PARSE_FAILED = -15,
    RESOURCE_STARVATION = -14,
    MEMORY_MAP_FAILED = -13,
    OUT_OF_RANGE = -12,
    INVALID_PARAMETER = -11,
    INVALID_EXTERNAL_HANDLE = -10,

    DEVICE_LOST = -9,
    TOO_MANY_OBJECTS = -8,
    EXTENSION_NOT_PRESENT = -7,
    LAYER_NOT_PRESENT = -6,
    INITIALIZATION_FAILED = -5,
    OUT_OF_DEVICE_MEMORY = -4,
    OUT_OF_HOST_MEMORY = -3,
    UNSUPPORTED = -2,
    FAIL = -1,
    UNDEFINED = 0,
    SUCCESS = 1,
    TIMEOUT = 2,
    NOT_READY = 3,
    MISMATCH = 4
} TResultEnum;

typedef enum TMemoryFlagsBits
{
    DEDICATED_MEMORY = 0x00000001,
    NEVER_ALLOCATE = 0x00000002,
    MAPPED = 0x00000004,
    USER_DATA_COPY_STRING = 0x00000020,
    UPPER_ADDRESS = 0x00000040,
    DONT_BIND = 0x00000080,
    WITHIN_BUDGET = 0x00000100,
    CAN_ALIAS_BIT = 0x00000200,
    HOST_ACCESS_SEQUENTIAL_WRITE = 0x00000400,
    HOST_ACCESS_RANDOM = 0x00000800,
    HOST_ACCESS_ALLOW_TRANSFER_INSTEAD = 0x00001000,
    STRATEGY_MIN_MEMORY = 0x00010000,
    STRATEGY_MIN_TIME = 0x00020000,
    STRATEGY_MIN_OFFSET = 0x00040000,
    STRATEGY_BEST_FIT = STRATEGY_MIN_MEMORY,
    STRATEGY_FIRST_FIT = STRATEGY_MIN_TIME,
    STRATEGY_MASK = STRATEGY_MIN_MEMORY | STRATEGY_MIN_TIME | STRATEGY_MIN_OFFSET,
    FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
} TMemoryFlagsBits;
typedef VkFlags TMemoryFlags;

typedef enum TAccessBits
{
    ACCESS_NONE = 0,
    INDIRECT_COMMAND_READ_BIT = 0x00000001,
    INDEX_READ_BIT = 0x00000002,
    VERTEX_ATTRIBUTE_READ_BIT = 0x00000004,
    UNIFORM_READ_BIT = 0x00000008,
    INPUT_ATTACHMENT_READ_BIT = 0x00000010,
    SHADER_READ_BIT = 0x00000020,
    SHADER_WRITE_BIT = 0x00000040,
    COLOR_ATTACHMENT_READ_BIT = 0x00000080,
    COLOR_ATTACHMENT_WRITE_BIT = 0x00000100,
    DEPTH_STENCIL_ATTACHMENT_READ_BIT = 0x00000200,
    DEPTH_STENCIL_ATTACHMENT_WRITE_BIT = 0x00000400,
    TRANSFER_READ_BIT = 0x00000800,
    TRANSFER_WRITE_BIT = 0x00001000,
    HOST_READ_BIT = 0x00002000,
    HOST_WRITE_BIT = 0x00004000,
    MEMORY_READ_BIT = 0x00008000,
    MEMORY_WRITE_BIT = 0x00010000,
} TAccessBits;
typedef VkFlags TAccess;

TResult VkResultToTResult(VkResult vkResult);

#define TURBO_WHOLE_EXTENT UINT32_MAX

typedef VkExtent2D TExtent2D;
typedef VkExtent3D TExtent3D;
typedef VkDeviceSize TDeviceSize;
typedef VkMemoryHeapFlags TMemoryHeapFlags;
typedef VkQueueFlags TQueueFlags;
typedef VkMemoryMapFlags TMemoryMapFlags;

#define TURBO_UUID_SIZE VK_UUID_SIZE

typedef VkPhysicalDeviceLimits TPhysicalDeviceLimits;
typedef VkPhysicalDeviceSparseProperties TPhysicalDeviceSparseProperties;
typedef VkPhysicalDeviceFeatures TPhysicalDeviceFeatures;

typedef VkBufferCreateFlags TBufferCreateFlags;
typedef VkBufferUsageFlags TBufferUsageFlags;
typedef VkSharingMode TSharingMode;

typedef uint32_t TQueueFamilyIndex;
typedef size_t TMemoryTypeIndex;

typedef VkImageCreateFlags TImageCreateFlags;

typedef VkImageViewCreateFlags TImageViewCreateFlags;
typedef VkImageAspectFlags TImageAspectFlags;
} // namespace Core
} // namespace Turbo
#endif // !TURBO_H