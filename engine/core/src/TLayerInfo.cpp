#include "TLayerInfo.h"
#include "TException.h"
#include "TInstance.h"
#include "TPhysicalDevice.h"
#include "TVulkanLoader.h"
#include <sstream>

size_t Turbo::Core::TLayerInfo::GetInstanceLayerCount()
{
    TVulkanLoader::Instance();

    uint32_t layer_count = 0;
    VkResult result = VkResult::VK_ERROR_UNKNOWN;

    do
    {
        result = Turbo::Core::TVulkanLoader::Instance()->LoadGlobalDriver().vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

        if (result == VkResult::VK_SUCCESS)
        {
            break;
        }
    } while (result == VkResult::VK_INCOMPLETE);

    return layer_count;
}

std::vector<Turbo::Core::TLayerInfo> Turbo::Core::TLayerInfo::GetInstanceLayers()
{
    TVulkanLoader::Instance();

    std::vector<Turbo::Core::TLayerInfo> layers;
    VkResult result = VkResult::VK_ERROR_UNKNOWN;

    do
    {
        uint32_t layer_count = static_cast<uint32_t>(TLayerInfo::GetInstanceLayerCount());
        if (layer_count > 0)
        {
            std::vector<VkLayerProperties> layer_propertiess;
            layer_propertiess.resize(layer_count);
            result = Turbo::Core::TVulkanLoader::Instance()->LoadGlobalDriver().vkEnumerateInstanceLayerProperties(&layer_count, layer_propertiess.data());

            if (result == VkResult::VK_SUCCESS)
            {
                layers.resize(layer_count);

                for (size_t layer_index = 0; layer_index < layer_count; layer_index++)
                {
                    layers[layer_index].name = layer_propertiess[layer_index].layerName;
                    layers[layer_index].layerType = TLayerInfo::GetLayerTypeByLayerName(layers[layer_index].name);
                    uint32_t specification_version = layer_propertiess[layer_index].specVersion;
                    layers[layer_index].specificationVersion = TVersion(VK_VERSION_MAJOR(specification_version), VK_VERSION_MINOR(specification_version), VK_VERSION_PATCH(specification_version), 0);
                    layers[layer_index].implementationVersion = layer_propertiess[layer_index].implementationVersion;
                    layers[layer_index].description = layer_propertiess[layer_index].description;

                    size_t layer_extension_count = TExtensionInfo::GetInstanceExtensionCount(&layers[layer_index]);
                    if (layer_extension_count > 0)
                    {
                        std::vector<TExtensionInfo> extensions = TExtensionInfo::GetInstanceExtensions(&layers[layer_index]);

                        layer_extension_count = extensions.size();

                        layers[layer_index].extensions.resize(layer_extension_count);

                        for (size_t extension_index = 0; extension_index < layer_extension_count; extension_index++)
                        {
                            layers[layer_index].extensions[extension_index].name = extensions[extension_index].name;
                            layers[layer_index].extensions[extension_index].version = extensions[extension_index].version;
                        }
                    }
                }

                break;
            }
        }
        else
        {
            break;
        }
    } while (result == VkResult::VK_INCOMPLETE);

    return layers;
}

size_t Turbo::Core::TLayerInfo::GetPhysicalDeviceLayerCount(const TRefPtr<TPhysicalDevice> &physicalDevice)
{
    uint32_t layer_count = 0;

    if (physicalDevice.Valid())
    {
        VkResult result = VkResult::VK_ERROR_UNKNOWN;
        PFN_vkEnumerateDeviceLayerProperties pfn_vk_enumerate_device_layer_properties = TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkEnumerateDeviceLayerProperties>(physicalDevice->GetInstance(), "vkEnumerateDeviceLayerProperties");

        do
        {
            result = pfn_vk_enumerate_device_layer_properties(physicalDevice->GetVkPhysicalDevice(), &layer_count, nullptr);

            if (result == VkResult::VK_SUCCESS)
            {
                break;
            }
        } while (result == VkResult::VK_INCOMPLETE);
    }

    return layer_count;
}

std::vector<Turbo::Core::TLayerInfo> Turbo::Core::TLayerInfo::GetPhysicalDeviceLayers(const TRefPtr<TPhysicalDevice> &physicalDevice)
{
    std::vector<Turbo::Core::TLayerInfo> layers;
    VkResult result = VkResult::VK_ERROR_UNKNOWN;

    PFN_vkEnumerateDeviceLayerProperties pfn_vk_enumerate_device_layer_properties = TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkEnumerateDeviceLayerProperties>(physicalDevice->GetInstance(), "vkEnumerateDeviceLayerProperties");

    do
    {
        uint32_t layer_count = static_cast<uint32_t>(TLayerInfo::GetPhysicalDeviceLayerCount(physicalDevice));

        if (layer_count > 0)
        {
            std::vector<VkLayerProperties> layer_propertiess;
            layer_propertiess.resize(layer_count);
            result = pfn_vk_enumerate_device_layer_properties(physicalDevice->GetVkPhysicalDevice(), &layer_count, layer_propertiess.data());

            if (result == VkResult::VK_SUCCESS)
            {
                layers.resize(layer_count);

                for (uint32_t layer_index = 0; layer_index < layer_count; layer_index++)
                {
                    layers[layer_index].name = layer_propertiess[layer_index].layerName;
                    layers[layer_index].layerType = TLayerInfo::GetLayerTypeByLayerName(layers[layer_index].name);
                    uint32_t specification_version = layer_propertiess[layer_index].specVersion;
                    layers[layer_index].specificationVersion = TVersion(VK_VERSION_MAJOR(specification_version), VK_VERSION_MINOR(specification_version), VK_VERSION_PATCH(specification_version), 0);
                    layers[layer_index].implementationVersion = layer_propertiess[layer_index].implementationVersion;
                    layers[layer_index].description = layer_propertiess[layer_index].description;

                    size_t layer_extension_count = TExtensionInfo::GetPhysicalDeviceExtensionCount(physicalDevice, &layers[layer_index]);
                    if (layer_extension_count > 0)
                    {
                        std::vector<TExtensionInfo> extensions = TExtensionInfo::GetPhysicalDeviceExtensions(physicalDevice, &layers[layer_index]);

                        layer_extension_count = extensions.size();

                        layers[layer_index].extensions.resize(layer_extension_count);

                        for (uint32_t extension_index = 0; extension_index < layer_extension_count; extension_index++)
                        {
                            layers[layer_index].extensions[extension_index].name = extensions[extension_index].name;
                            layers[layer_index].extensions[extension_index].version = extensions[extension_index].version;
                        }
                    }
                }

                break;
            }
        }
        else
        {
            break;
        }
    } while (result == VkResult::VK_INCOMPLETE);

    return layers;
}

Turbo::Core::TLayerType Turbo::Core::TLayerInfo::GetLayerTypeByLayerName(std::string layerName)
{
    if (layerName == std::string("VK_LAYER_KHRONOS_validation"))
    {
        return TLayerType::VK_LAYER_KHRONOS_VALIDATION;
    }
    else if (layerName == std::string("VK_LAYER_LUNARG_api_dump"))
    {
        return TLayerType::VK_LAYER_LUNARG_API_DUMP;
    }
    else if (layerName == std::string("VK_LAYER_LUNARG_device_simulation"))
    {
        return TLayerType::VK_LAYER_LUNARG_DEVICE_SIMULATION;
    }
    else if (layerName == std::string("VK_LAYER_LUNARG_monitor"))
    {
        return TLayerType::VK_LAYER_LUNARG_MONITOR;
    }
    else if (layerName == std::string("VK_LAYER_LUNARG_screenshot"))
    {
        return TLayerType::VK_LAYER_LUNARG_SCREENSHOT;
    }
    else if (layerName == std::string("VK_LAYER_LUNARG_vktrace"))
    {
        return TLayerType::VK_LAYER_LUNARG_VKTRACE;
    }
    else if (layerName == std::string("VK_LAYER_NV_optimus"))
    {
        return TLayerType::VK_LAYER_NV_OPTIMUS;
    }
    else if (layerName == std::string("VK_LAYER_VALVE_steam_fossilize"))
    {
        return TLayerType::VK_LAYER_VALVE_STEAM_FOSSILIZE;
    }
    else if (layerName == std::string("VK_LAYER_VALVE_steam_overlay"))
    {
        return TLayerType::VK_LAYER_VALVE_STEAM_OVERLAY;
    }

    return TLayerType::UNDEFINED;
}

std::string Turbo::Core::TLayerInfo::GetLayerNameByLayerType(TLayerType layerType)
{
    switch (layerType)
    {
    case Turbo::Core::TLayerType::UNDEFINED:
        break;
    case Turbo::Core::TLayerType::VK_LAYER_KHRONOS_VALIDATION:
        return std::string("VK_LAYER_KHRONOS_validation");
        break;
    case Turbo::Core::TLayerType::VK_LAYER_LUNARG_API_DUMP:
        return std::string("VK_LAYER_LUNARG_api_dump");
        break;
    case Turbo::Core::TLayerType::VK_LAYER_LUNARG_DEVICE_SIMULATION:
        return std::string("VK_LAYER_LUNARG_device_simulation");
        break;
    case Turbo::Core::TLayerType::VK_LAYER_LUNARG_MONITOR:
        return std::string("VK_LAYER_LUNARG_monitor");
        break;
    case Turbo::Core::TLayerType::VK_LAYER_LUNARG_SCREENSHOT:
        return std::string("VK_LAYER_LUNARG_screenshot");
        break;
    case Turbo::Core::TLayerType::VK_LAYER_LUNARG_VKTRACE:
        return std::string("VK_LAYER_LUNARG_vktrace");
        break;
    case Turbo::Core::TLayerType::VK_LAYER_NV_OPTIMUS:
        return std::string("VK_LAYER_NV_optimus");
        break;
    case Turbo::Core::TLayerType::VK_LAYER_VALVE_STEAM_FOSSILIZE:
        return std::string("VK_LAYER_VALVE_steam_fossilize");
        break;
    case Turbo::Core::TLayerType::VK_LAYER_VALVE_STEAM_OVERLAY:
        return std::string("VK_LAYER_VALVE_steam_overlay");
        break;
    default:
        break;
    }
    return std::string();
}

bool Turbo::Core::TLayerInfo::IsSupportInstanceLayer(TLayerType layerType)
{
    if (layerType != TLayerType::UNDEFINED && TLayerInfo::GetInstanceLayerCount() > 0)
    {
        std::vector<TLayerInfo> layers = TLayerInfo::GetInstanceLayers();
        size_t layer_count = layers.size();
        for (size_t layer_index = 0; layer_index < layer_count; layer_index++)
        {
            if (layerType == layers[layer_index].layerType)
            {
                return true;
            }
        }
    }

    return false;
}

Turbo::Core::TLayerInfo::TLayerInfo() : Turbo::Core::TInfo()
{
    this->layerType = TLayerType::UNDEFINED;
    this->name = "";
    this->specificationVersion = TVersion(0, 0, 0, 0);
    this->implementationVersion = 0;
    this->description = "";
}

Turbo::Core::TLayerInfo::~TLayerInfo()
{
}

Turbo::Core::TLayerType Turbo::Core::TLayerInfo::GetLayerType()
{
    return this->layerType;
}

const std::string &Turbo::Core::TLayerInfo::GetName()
{
    return this->name;
}

Turbo::Core::TVersion Turbo::Core::TLayerInfo::GetSpecificationVersion()
{
    return this->specificationVersion;
}

uint32_t Turbo::Core::TLayerInfo::GetImplementationVersion()
{
    return this->implementationVersion;
}

std::string Turbo::Core::TLayerInfo::GetDescription()
{
    return this->description;
}

size_t Turbo::Core::TLayerInfo::GetExtensionCount()
{
    return this->extensions.size();
}

Turbo::Core::TExtensionInfo Turbo::Core::TLayerInfo::GetExtension(uint32_t index)
{
    TExtensionInfo result;
    if (index > this->extensions.size() - 1)
    {
        throw Turbo::Core::TException(TResult::OUT_OF_RANGE, "Turbo::Core::TLayerInfo::GetExtension");
    }

    return this->extensions[index];
}

std::vector<Turbo::Core::TExtensionInfo> Turbo::Core::TLayerInfo::GetExtensions()
{
    return this->extensions;
}

bool Turbo::Core::TLayerInfo::operator==(const TLayerInfo &layer) const
{
    if (this->layerType == layer.layerType)
    {
        return true;
    }
    return false;
}

bool Turbo::Core::TLayerInfo::operator!=(const TLayerInfo &layer) const
{
    if (this->layerType != layer.layerType)
    {
        return true;
    }
    return false;
}

std::string Turbo::Core::TLayerInfo::ToString()
{
    std::stringstream ss;
    ss << this->name << " (" << this->description << ") Vulkan version " << this->specificationVersion.ToString() << ", layer version " << this->implementationVersion;
    return ss.str();
}