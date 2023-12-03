#include "TEngine.h"
#include "TException.h"
#include "TPhysicalDevice.h"

void Turbo::Core::TEngine::VerificationInitVulkan()
{
    if (!TInstance::IsSupportVulkan())
    {
        throw Turbo::Core::TException(TResult::UNSUPPORTED, "Turbo::Core::TEngine::VerificationInitVulkan", "Not support Vulkan");
    }

    this->instance = new TInstance();

    TPhysicalDevice *best_physical_device = this->instance->GetBestPhysicalDevice();

    TVersion vulkan_instance_version = TInstance::GetVulkanInstanceVersion();

    if (vulkan_instance_version.GetValidVulkanVersion() < TVersion(1, 1, 0, 0))
    {
        this->InitVulkan(TVersion(1, 0, 0, 0));
    }
    else
    {
        if (best_physical_device->GetVkPhysicalDevice() != VK_NULL_HANDLE)
        {
            if (best_physical_device->GetDeviceApiVersion().GetValidVulkanVersion() >= vulkan_instance_version.GetValidVulkanVersion())
            {
                // delete this->instance;
                this->instance = nullptr;
                this->InitVulkan(vulkan_instance_version.GetValidVulkanVersion());
            }
            else
            {
                // delete this->instance;
                this->instance = nullptr;
                this->InitVulkan(best_physical_device->GetDeviceApiVersion().GetValidVulkanVersion());
            }
        }
        else
        {
            // delete this->instance;
            this->instance = nullptr;
            this->InitVulkan(TVersion(1, 0, 0, 0));
        }
    }
}

void Turbo::Core::TEngine::InitVulkan(TVersion version)
{
    this->instance = new TInstance(nullptr, nullptr, &version);
}

Turbo::Core::TEngine::TEngine()
{
    try
    {
        this->VerificationInitVulkan();
    }
    catch (const std::exception &)
    {
    }
}

Turbo::Core::TEngine::~TEngine()
{
    if (this->instance.Valid())
    {
        // delete this->instance;
        this->instance = nullptr;
    }
}

const Turbo::Core::TRefPtr<Turbo::Core::TInstance> &Turbo::Core::TEngine::GetInstance()
{
    return this->instance;
}

std::string Turbo::Core::TEngine::ToString()
{
    return std::string();
}