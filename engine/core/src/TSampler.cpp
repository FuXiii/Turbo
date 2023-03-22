#include "TSampler.h"
#include "TDevice.h"
#include "TException.h"
#include "TPhysicalDevice.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

void Turbo::Core::TSampler::InternalCreate()
{
    VkSamplerCreateInfo vk_sampler_create_info = {};
    vk_sampler_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    vk_sampler_create_info.pNext = nullptr;
    vk_sampler_create_info.flags = 0;
    vk_sampler_create_info.magFilter = (VkFilter)this->magFilter;
    vk_sampler_create_info.minFilter = (VkFilter)this->minFilter;
    vk_sampler_create_info.mipmapMode = (VkSamplerMipmapMode)this->mipmapMode;
    vk_sampler_create_info.addressModeU = (VkSamplerAddressMode)this->addressModeU;
    vk_sampler_create_info.addressModeV = (VkSamplerAddressMode)this->addressModeV;
    vk_sampler_create_info.addressModeW = (VkSamplerAddressMode)this->addressModeW;
    vk_sampler_create_info.mipLodBias = this->mipLodBias;
    vk_sampler_create_info.anisotropyEnable = VK_FALSE;
    if (this->anisotropyEnable)
    {
        vk_sampler_create_info.anisotropyEnable = VK_TRUE;
    }
    vk_sampler_create_info.maxAnisotropy = this->maxAnisotropy;
    vk_sampler_create_info.compareEnable = VK_FALSE;
    vk_sampler_create_info.compareOp = VkCompareOp::VK_COMPARE_OP_NEVER;
    vk_sampler_create_info.minLod = this->minLod;
    vk_sampler_create_info.maxLod = this->maxLod;
    vk_sampler_create_info.borderColor = (VkBorderColor)this->borderColor;
    vk_sampler_create_info.unnormalizedCoordinates = VK_FALSE;

    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = this->device->GetDeviceDriver()->vkCreateSampler(vk_device, &vk_sampler_create_info, allocator, &this->vkSampler);
    if (result != VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TSampler::InternalCreate::vkCreateSampler");
    }
}

void Turbo::Core::TSampler::InternalDestroy()
{
    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    this->device->GetDeviceDriver()->vkDestroySampler(vk_device, this->vkSampler, allocator);
}

Turbo::Core::TSampler::TSampler(TDevice *device, TFilter minFilter, TFilter magFilter, TMipmapMode mipmapMode, TAddressMode addressModeU, TAddressMode addressModeV, TAddressMode addressModeW, TBorderColor borderColor, float mipLodBias, float minLod, float maxLod)
{
    if (device != nullptr)
    {
        this->device = device;

        this->minFilter = minFilter;
        this->magFilter = magFilter;
        this->mipmapMode = mipmapMode;
        this->addressModeU = addressModeU;
        this->addressModeV = addressModeV;
        this->addressModeW = addressModeW;
        this->borderColor = borderColor;
        this->mipLodBias = mipLodBias;
        this->minLod = minLod;
        this->maxLod = maxLod;

        TPhysicalDevice *physical_device = this->device->GetPhysicalDevice();

        if (physical_device->GetDeviceFeatures().samplerAnisotropy == true)
        {
            if (this->device->GetEnableDeviceFeatures().samplerAnisotropy == true)
            {
                this->anisotropyEnable = true;
                this->maxAnisotropy = physical_device->GetDeviceLimits().maxSamplerAnisotropy;
            }
            else
            {
                this->anisotropyEnable = false;
                this->maxAnisotropy = 1.0f;
            }
        }
        else
        {
            this->anisotropyEnable = false;
            this->maxAnisotropy = 1.0f;
        }

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TSampler::TSampler");
    }
}

Turbo::Core::TSampler::~TSampler()
{
    this->InternalDestroy();
}

VkSampler Turbo::Core::TSampler::GetVkSampler()
{
    return this->vkSampler;
}

std::string Turbo::Core::TSampler::ToString()
{
    return std::string();
}
