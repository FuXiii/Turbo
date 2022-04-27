#pragma once
#ifndef TPHYSICALDEVICEINFO_H
#define TPHYSICALDEVICEINFO_H

#include "TInfo.h"
#include "TVersion.h"
#include "TVendorInfo.h"
#include "TLayerInfo.h"
#include "TExtensionInfo.h"
#include "TQueueFamilyInfo.h"
#include "TFormatInfo.h"
#include "TMemoryTypeInfo.h"

namespace Turbo
{
    namespace Core
    {
        typedef enum class TPhysicalDeviceType
        {
            UNDEFINED = VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER,
            OTHER = UNDEFINED,
            INTEGRATED_GPU = VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
            DISCRETE_GPU = VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
            VIRTUAL_GPU = VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
            CPU = VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU
        } TPhysicalDeviceTypeEnum;

        typedef struct TPipelineCacheUUID
        {
            uint8_t uuid[TURBO_UUID_SIZE];
        } TPipelineCacheUUID;

        class TPhysicalDeviceInfo : public Turbo::Core::TInfo
        {
        public:
            std::string name;
            TVersion apiVersion;
            uint32_t driverVersion;
            TVendorInfo vendor;
            uint32_t id;
            TPhysicalDeviceType type;
            TPipelineCacheUUID pipelineCacheUUID;
            VkPhysicalDeviceLimits limits;
            VkPhysicalDeviceSparseProperties sparseProperties;
            TPhysicalDeviceFeatures features;

            std::vector<TLayerInfo> supportLayers;
            std::vector<TExtensionInfo> supportExtensions;

            std::vector<TQueueFamilyInfo> queueFamilys;

            std::vector<TMemoryTypeInfo> memoryTypes;

            std::vector<TFormatInfo> supportFormats;

        public:
            TPhysicalDeviceInfo();
            ~TPhysicalDeviceInfo();

        public:
            virtual std::string ToString() override;
        };
    }
} // namespace Turbo

#endif // !TPHYSICALDEVICEINFO_H