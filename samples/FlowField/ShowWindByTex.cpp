#include <iostream>
#include <TInstance.h>
// #include <TExtensionInfo.h>

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
                enable_layers.push_back(layer);
                break;
            }
        }
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_extensions;
    {
        auto instance_support_extensions = Turbo::Core::TExtensionInfo::GetInstanceExtensions();
        for (auto &extension : instance_support_extensions)
        {
            switch (extension.GetExtensionType())
            {
            case Turbo::Core::TExtensionType::VK_KHR_SURFACE: {
                enable_extensions.push_back(extension);
            }
            break;
            case Turbo::Core::TExtensionType::VK_KHR_WIN32_SURFACE: {
                enable_extensions.push_back(extension);
            }
            break;
            case Turbo::Core::TExtensionType::VK_KHR_WAYLAND_SURFACE: {
                enable_extensions.push_back(extension);
            }
            break;
            case Turbo::Core::TExtensionType::VK_KHR_XCB_SURFACE: {
                enable_extensions.push_back(extension);
            }
            break;
            case Turbo::Core::TExtensionType::VK_KHR_XLIB_SURFACE: {
                enable_extensions.push_back(extension);
            }
            break;
            default: {
            }
            break;
            }
        }
    }

    Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance = new Turbo::Core::TInstance(enable_layers, enable_extensions, vulkan_verssion);
    
    return 0;
}