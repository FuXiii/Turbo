#include <iostream>
#include <TInstance.h>
// #include <TExtensionInfo.h>
#include <TPhysicalDevice.h>
#include <TVulkanLoader.h>
#include <array>

struct FormatRange
{
    VkFormat first_format;
    VkFormat last_format;
};

int main()
{
    auto vulkan_verssion = Turbo::Core::TInstance::GetVulkanInstanceVersion();

    std::vector<Turbo::Core::TLayerInfo> enable_layers;
    {
        auto instance_support_layers = Turbo::Core::TLayerInfo::GetInstanceLayers();
        for (auto &layer : instance_support_layers)
        {
            if (layer.GetLayerType() == Turbo::Core::TLayerType::VK_LAYER_KHRONOS_VALIDATION)
            {
                //enable_layers.push_back(layer);
                break;
            }
        }
    }

    Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance = new Turbo::Core::TInstance(enable_layers, {}, vulkan_verssion);

    auto physical_device = instance->GetBestPhysicalDevice();
    auto vk_physical_device = physical_device->GetVkPhysicalDevice();

    auto format_ranges = std::array{
        FormatRange{static_cast<VkFormat>(0), static_cast<VkFormat>(184)},
        FormatRange{static_cast<VkFormat>(1000156000), static_cast<VkFormat>(1000156033)},
        FormatRange{static_cast<VkFormat>(1000330000), static_cast<VkFormat>(1000330003)},
        FormatRange{static_cast<VkFormat>(1000340000), static_cast<VkFormat>(1000340001)},
        FormatRange{static_cast<VkFormat>(1000066000), static_cast<VkFormat>(1000066013)},
        FormatRange{static_cast<VkFormat>(1000470000), static_cast<VkFormat>(1000470001)},
        FormatRange{static_cast<VkFormat>(1000054000), static_cast<VkFormat>(1000054007)},
        FormatRange{static_cast<VkFormat>(1000464000), static_cast<VkFormat>(1000464000)},
    };

    for (auto &item : format_ranges)
    {
        for (uint32_t format = item.first_format; format <= item.last_format; format++)
        {
            VkFormatProperties vk_format_properties = {};
            physical_device->GetPhysicalDeviceDriver()->vkGetPhysicalDeviceFormatProperties(vk_physical_device, static_cast<VkFormat>(format), &vk_format_properties);

            if (vk_format_properties.bufferFeatures != 0 || vk_format_properties.linearTilingFeatures != 0 || vk_format_properties.optimalTilingFeatures != 0)
            {
                std::cout << "Support: " << format << std::endl;
            }
            else
            {
                std::cout << "-Unsupport: " << format << std::endl;
            }
        }
    }

    return 0;
}