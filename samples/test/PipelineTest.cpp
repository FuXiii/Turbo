#include <iostream>
#include <TReferenced.h>
#include <TRefPtr.h>
#include <vulkan/vulkan_core.h>

#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <array>

#include <TDevice.h>
#include <TDeviceQueue.h>
#include <TVulkanHandle.h>
#include <TVulkanAllocator.h>
#include <TVulkanLoader.h>
#include <TDescriptor.h>
#include <TDescriptorSetLayout.h>
#include <TShader.h>
#include <TDescriptorPool.h>

#include <TPipelineLayout.h>

#include <ReadFile.h>
#include <TPipeline.h>

std::string asset_root(TURBO_ASSET_ROOT);

class VulkanContext
{
  private:
    Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance;
    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device;
    Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> queue;

  public:
    VulkanContext()
    {
        auto vulkan_verssion = Turbo::Core::TInstance::GetVulkanInstanceVersion();
        std::cout << "vulkan verssion: " << vulkan_verssion.ToString() << std::endl;
        std::vector<Turbo::Core::TLayerInfo> enable_layers;
        {
            auto instance_support_layers = Turbo::Core::TLayerInfo::GetInstanceLayers();
            for (auto &layer : instance_support_layers)
            {
                switch (layer.GetLayerType())
                {
                case Turbo::Core::TLayerType::VK_LAYER_KHRONOS_VALIDATION: {
                    enable_layers.push_back(layer);
                }
                break;
                default: {
                }
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

        this->instance = new Turbo::Core::TInstance(enable_layers, enable_extensions, vulkan_verssion);

        auto physical_device = this->instance->GetBestPhysicalDevice();

        std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
        {
            std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = physical_device->GetSupportExtensions();
            for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
            {
                switch (extension.GetExtensionType())
                {
                case Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN: {
                    enable_device_extensions.push_back(extension);
                }
                break;
                default: {
                }
                break;
                }
            }
        }

        this->device = new Turbo::Core::TDevice(physical_device, {}, enable_device_extensions, {});
        this->queue = device->GetBestGraphicsQueue();
    }

    Turbo::Core::TInstance *Instance()
    {
        return this->instance;
    }

    Turbo::Core::TDevice *Device()
    {
        return this->device;
    }

    Turbo::Core::TDeviceQueue *Queue()
    {
        return this->queue;
    }
};

void Test_TSpecializationConstants(Turbo::Core::TInstance *instance, Turbo::Core::TDevice *device, Turbo::Core::TDeviceQueue *queue)
{
    Turbo::Core::TPipeline::TSpecializationConstants scs;
    scs.Merge(0, 0.2f);
    scs.Merge(1, 1.0);
    scs.Merge(3, 2);
    scs.Merge(6, 3ui32);
    scs.Merge(20, 4);
    scs.Merge(2880, true);

    std::cout << scs.ToString() << std::endl;
}

// void Test_(Turbo::Core::TInstance *instance, Turbo::Core::TDevice *device, Turbo::Core::TDeviceQueue *queue)

int main()
{
    VulkanContext vulkan_context;

    Turbo::Core::TInstance *instance = vulkan_context.Instance();
    Turbo::Core::TDevice *device = vulkan_context.Device();
    Turbo::Core::TDeviceQueue *queue = vulkan_context.Queue();

    Test_TSpecializationConstants(instance, device, queue);
}