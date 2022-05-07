#include "TSwapchain.h"
#include "TDevice.h"
#include "TException.h"
#include "TPhysicalDevice.h"
#include "TSurface.h"

Turbo::Core::TSwapchain::TSwapchain(TDevice *device, TSurface *surface)
{
    // if (device != nullptr && surface != nullptr && device->GetPhysicalDevice() != nullptr && surface->GetPhysicalDevice() != nullptr && device->GetPhysicalDevice()->GetVkPhysicalDevice() != VK_NULL_HANDLE && surface->GetPhysicalDevice()->GetVkPhysicalDevice() != VK_NULL_HANDLE && surface->GetVkSurfaceKHR() != VK_NULL_HANDLE)
    // {
    //     if (device->GetPhysicalDevice()->GetVkPhysicalDevice() == surface->GetPhysicalDevice()->GetVkPhysicalDevice() && device->IsEnabledExtension(TExtensionType::VK_KHR_SWAPCHAIN))
    //     {
    //         VkSwapchainCreateInfoKHR swapchain_create_info = {};
    //         swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    //         swapchain_create_info.pNext = nullptr;
    //         swapchain_create_info.flags = 0;
    //         swapchain_create_info.surface = surface->GetVkSurfaceKHR();
    //         if (surface->GetMaxImageCount() >= 3)
    //         {
    //             swapchain_create_info.minImageCount = 3;
    //         }
    //         else
    //         {
    //             swapchain_create_info.minImageCount = surface->GetMinImageCount();
    //         }

    //         VkFormat target_surface_format = VkFormat::VK_FORMAT_UNDEFINED;
    //         VkColorSpaceKHR target_surface_color_space = VkColorSpaceKHR::VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    //         std::vector<Turbo::Core::TSurfaceFormat> surface_formats = surface->GetSurfaceFormats();
    //         size_t surface_format_count = surface_formats.size();
    //         for (size_t surface_format_index = 0; surface_format_index < surface_format_count; surface_format_index++)
    //         {
    //             VkFormat format = surface_formats[surface_format_index].GetFormat().GetVkFormat();
    //             if (format == VkFormat::VK_FORMAT_R8G8B8A8_SRGB)
    //             {
    //                 target_surface_format = VkFormat::VK_FORMAT_R8G8B8A8_SRGB;
    //                 target_surface_color_space = surface_formats[surface_format_index].GetColorSpace().GetVkColorSpaceKHR();
    //                 break;
    //             }
    //             else if (format == VkFormat::VK_FORMAT_B8G8R8A8_SRGB)
    //             {
    //                 target_surface_format = VkFormat::VK_FORMAT_B8G8R8A8_SRGB;
    //                 target_surface_color_space = surface_formats[surface_format_index].GetColorSpace().GetVkColorSpaceKHR();
    //                 break;
    //             }
    //             else if (format == VkFormat::VK_FORMAT_R8G8B8A8_UNORM)
    //             {
    //                 target_surface_format = VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
    //                 target_surface_color_space = surface_formats[surface_format_index].GetColorSpace().GetVkColorSpaceKHR();
    //                 break;
    //             }
    //             else if (format == VkFormat::VK_FORMAT_B8G8R8A8_UNORM)
    //             {
    //                 target_surface_format = VkFormat::VK_FORMAT_B8G8R8A8_UNORM;
    //                 target_surface_color_space = surface_formats[surface_format_index].GetColorSpace().GetVkColorSpaceKHR();
    //                 break;
    //             }
    //         }

    //         if (target_surface_format != VkFormat::VK_FORMAT_UNDEFINED && target_surface_color_space == VkColorSpaceKHR::VK_COLORSPACE_SRGB_NONLINEAR_KHR)
    //         {
    //             swapchain_create_info.imageFormat = surface->GetSurfaceFormats()[0].GetFormat().GetVkFormat();
    //             swapchain_create_info.imageColorSpace = target_surface_color_space;
    //         }
    //         else
    //         {
    //             throw TException(TResult::UNSUPPORTED);
    //         }

    //         swapchain_create_info.imageExtent = surface->GetCurrentExtent();

    //         if (surface->GetMaxImageArrayLayers() >= 1)
    //         {
    //             swapchain_create_info.imageArrayLayers = 1;
    //         }
    //         else
    //         {
    //             throw TException(TResult::UNSUPPORTED);
    //         }

    //         if (surface->GetSupportedUsageFlags() & VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
    //         {
    //             swapchain_create_info.imageUsage = VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    //         }
    //         else
    //         {
    //             throw TException(TResult::UNSUPPORTED);
    //         }

    //         swapchain_create_info.imageSharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
    //         swapchain_create_info.queueFamilyIndexCount = 0;
    //         swapchain_create_info.pQueueFamilyIndices = nullptr;
    //         swapchain_create_info.preTransform = surface->GetCurrentTransform();

    //         if (surface->GetSupportedCompositeAlpha() & VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
    //         {
    //             swapchain_create_info.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    //         }
    //         else
    //         {
    //             throw TException(TResult::UNSUPPORTED);
    //         }

    //         if (surface->IsSupportPresentModeFifoRelaxed())
    //         {
    //             swapchain_create_info.presentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_RELAXED_KHR;
    //         }
    //         else if (surface->IsSupportPresentModeFifo())
    //         {
    //             swapchain_create_info.presentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
    //         }
    //         else if (surface->IsSupportPresentModeMailbox())
    //         {
    //             swapchain_create_info.presentMode = VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR;
    //         }
    //         else if (surface->IsSupportPresentModeImmediate())
    //         {
    //             swapchain_create_info.presentMode = VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR;
    //         }
    //         else
    //         {
    //             throw TException(TResult::UNSUPPORTED);
    //         }

    //         swapchain_create_info.clipped = VK_TRUE;
    //         swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

    //         VkResult result = vkCreateSwapchainKHR(device->GetVkDevice(), &swapchain_create_info, nullptr, &this->vkSwapchainKHR);
    //         if (result == VK_SUCCESS)
    //         {
    //             this->device = device;
    //         }
    //         else
    //         {
    //             throw TException(VkResultToTResult(result));
    //         }
    //     }
    //     else
    //     {
    //         throw TException(TResult::INVALID_PARAMETER);
    //     }
    // }
    // else
    // {
    //     throw TException(TResult::INVALID_PARAMETER);
    // }
}

Turbo::Core::TSwapchain::~TSwapchain()
{
    // if (this->device != nullptr && this->vkSwapchainKHR != VK_NULL_HANDLE)
    // {
    //     vkDestroySwapchainKHR(this->device->GetVkDevice(), this->vkSwapchainKHR, nullptr);
    // }
}

std::string Turbo::Core::TSwapchain::ToString()
{
    return std::string();
}
