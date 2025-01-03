#include <iostream>
#include <TInstance.h>
//#include <TExtensionInfo.h>

int main()
{
    auto vulkan_verssion = Turbo::Core::TInstance::GetVulkanInstanceVersion();

    auto instance_support_layers = Turbo::Core::TLayerInfo::GetInstanceLayers();
    auto instance_support_extensions = Turbo::Core::TExtensionInfo::GetInstanceExtensions();

    std::vector<Turbo::Core::TLayerInfo> enable_layers;
    {
        for (Turbo::Core::TLayerInfo &layer : instance_support_layers)
        {
            if (layer.GetLayerType() == Turbo::Core::TLayerType::VK_LAYER_KHRONOS_VALIDATION)
            {
                enable_layers.push_back(layer);
                break;
            }
        }
    }

    Turbo::Core::TInstance *instance = new Turbo::Core::TInstance();

    std::cout << "Hello World" << std::endl;
    return 0;
}