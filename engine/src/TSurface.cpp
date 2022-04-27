#include "TSurface.h"
#include "TException.h"
#include "TInstance.h"
#include "TPhysicalDevice.h"

#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
Turbo::Core::TSurface::TSurface(TPhysicalDevice* physicalDevice, HINSTANCE hinstance, HWND hwnd)
{
	if (physicalDevice != nullptr && physicalDevice->GetVkPhysicalDevice() != VK_NULL_HANDLE)
	{
		TInstance* instance = physicalDevice->GetInstance();
		if (instance != nullptr && instance->GetVkInstance() != VK_NULL_HANDLE && instance->IsEnabledExtension(TExtensionType::VK_KHR_SURFACE))
		{
			VkWin32SurfaceCreateInfoKHR win32_surface_create_info = {};
			win32_surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			win32_surface_create_info.pNext = nullptr;
			win32_surface_create_info.flags = 0;
			win32_surface_create_info.hinstance = hinstance;
			win32_surface_create_info.hwnd = hwnd;

			VkResult result = vkCreateWin32SurfaceKHR(instance->GetVkInstance(), &win32_surface_create_info, nullptr, &this->vkSurfaceKHR);
			if (result != VK_SUCCESS)
			{
				throw TException(VkResultToTResult(result));
			}

			this->physicalDevice = physicalDevice;

			this->GetSurfaceSupportQueueFamilys();
			this->GetSurfaceCapabilities();
			this->GetSurfaceSupportSurfaceFormats();
			this->GetSurfaceSupportPresentationMode();
		}
		else
		{
			throw TException(TResult::INVALID_PARAMETER);
		}
	}
	else
	{
		throw TException(TResult::INVALID_PARAMETER);
	}
}
#endif

Turbo::Core::TSurface::~TSurface()
{
	if (this->physicalDevice != nullptr)
	{
		TInstance* instance = physicalDevice->GetInstance();
		if (instance != nullptr && instance->GetVkInstance() != VK_NULL_HANDLE && this->vkSurfaceKHR != VK_NULL_HANDLE)
		{
			vkDestroySurfaceKHR(this->physicalDevice->GetInstance()->GetVkInstance(), this->vkSurfaceKHR, nullptr);
		}
	}
}

void Turbo::Core::TSurface::GetSurfaceSupportQueueFamilys()
{
	size_t queue_family_count = this->physicalDevice->GetQueueFamilyCount();
	for (size_t queue_family_index = 0; queue_family_index < queue_family_count; queue_family_index++)
	{
		TQueueFamilyInfo queue_family = this->physicalDevice->GetQueueFamilyByIndex(static_cast<uint32_t>(queue_family_index));

		VkBool32 is_support = VK_FALSE;

		VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(this->physicalDevice->GetVkPhysicalDevice(), queue_family.GetIndex(), this->vkSurfaceKHR, &is_support);
		if (result == VK_SUCCESS)
		{
			if (is_support == VK_TRUE)
			{
				this->supportQueueFamilys.push_back(queue_family);
			}
		}
		else
		{
			throw TException(VkResultToTResult(result));
		}
	}

	//��ǰ�豸��֧�ָ�Surface
	if (this->supportQueueFamilys.size() == 0)
	{
		throw TException(TResult::UNSUPPORTED);
	}
}

void Turbo::Core::TSurface::GetSurfaceCapabilities()
{
	VkSurfaceCapabilitiesKHR surface_capanilities;
	VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(this->physicalDevice->GetVkPhysicalDevice(), this->vkSurfaceKHR, &surface_capanilities);
	if (result == VK_SUCCESS)
	{
		this->minImageCount = surface_capanilities.minImageCount;
		this->maxImageCount = surface_capanilities.maxImageCount;
		this->currentExtent = surface_capanilities.currentExtent;
		this->minImageExtent = surface_capanilities.minImageExtent;
		this->maxImageExtent = surface_capanilities.maxImageExtent;
		this->maxImageArrayLayers = surface_capanilities.maxImageArrayLayers;

		this->supportedTransforms = surface_capanilities.supportedTransforms;
		this->currentTransform = surface_capanilities.currentTransform;
		this->supportedCompositeAlpha = surface_capanilities.supportedCompositeAlpha;
		this->supportedUsageFlags = surface_capanilities.supportedUsageFlags;
	}
	else
	{
		throw TException(VkResultToTResult(result));
	}
}

void Turbo::Core::TSurface::GetSurfaceSupportSurfaceFormats()
{
	uint32_t surface_format_count = 0;
	VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(this->physicalDevice->GetVkPhysicalDevice(), this->vkSurfaceKHR, &surface_format_count, nullptr);
	if (result == VK_SUCCESS && surface_format_count > 0)
	{
		std::vector<VkSurfaceFormatKHR> surface_formats;
		surface_formats.resize(surface_format_count);

		result = vkGetPhysicalDeviceSurfaceFormatsKHR(this->physicalDevice->GetVkPhysicalDevice(), this->vkSurfaceKHR, &surface_format_count, surface_formats.data());
		if (result == VK_SUCCESS)
		{
			std::vector<TFormatInfo> support_formats = this->physicalDevice->GetSupportFormats();
			size_t support_formats_size = support_formats.size();

			for (size_t surface_format_index = 0; surface_format_index < surface_format_count; surface_format_index++)
			{
				if (support_formats_size > 0)
				{
					for (size_t support_format_index = 0; support_format_index < support_formats_size; support_format_index++)
					{
						if (support_formats[support_format_index].GetVkFormat() == surface_formats[surface_format_index].format)
						{
							TSurfaceFormat surface_format;
							surface_format.format = support_formats[support_format_index];
							surface_format.colorSpace = TColorSpace(static_cast<TColorSpaceType>(surface_formats[surface_format_index].colorSpace));

							this->surfaceFormats.push_back(surface_format);
							break;
						}
					}
				}
				else
				{
					throw TException(TResult::INVALID_PARAMETER);
				}
			}

			if (this->surfaceFormats.size() == 0)
			{
				throw TException(TResult::UNSUPPORTED);
			}
		}
		else
		{
			throw TException(VkResultToTResult(result));
		}
	}
	else
	{
		throw TException(VkResultToTResult(result));
	}
}

void Turbo::Core::TSurface::GetSurfaceSupportPresentationMode()
{
	this->presentModeFlags = 0;

	uint32_t present_mode_count = 0;
	VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(this->physicalDevice->GetVkPhysicalDevice(), this->vkSurfaceKHR, &present_mode_count, nullptr);
	if (result == VK_SUCCESS)
	{
		if (present_mode_count > 0)
		{
			this->presentModes.resize(present_mode_count);
			result = vkGetPhysicalDeviceSurfacePresentModesKHR(this->physicalDevice->GetVkPhysicalDevice(), this->vkSurfaceKHR, &present_mode_count, this->presentModes.data());
			if (result == VK_SUCCESS)
			{
				for (size_t present_mode_index = 0; present_mode_index < present_mode_count; present_mode_index++)
				{
					if (!(this->presentModeFlags & 0x001))
					{
						if (presentModes[present_mode_index] == VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR)
						{
							this->presentModeFlags |= 0x001;
						}
					}

					if (!(this->presentModeFlags & 0x002))
					{
						if (presentModes[present_mode_index] == VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR)
						{
							this->presentModeFlags |= 0x002;
						}
					}

					if (!(this->presentModeFlags & 0x004))
					{
						if (presentModes[present_mode_index] == VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR)
						{
							this->presentModeFlags |= 0x004;
						}
					}

					if (!(this->presentModeFlags & 0x008))
					{
						if (presentModes[present_mode_index] == VkPresentModeKHR::VK_PRESENT_MODE_FIFO_RELAXED_KHR)
						{
							this->presentModeFlags |= 0x008;
						}
					}
				}
			}
			else
			{
				throw TException(TResult::UNSUPPORTED);
			}
		}
		else
		{
			throw TException(TResult::UNSUPPORTED);
		}
	}
	else
	{
		throw TException(VkResultToTResult(result));
	}
}

VkSurfaceKHR Turbo::Core::TSurface::GetVkSurfaceKHR()
{
	return this->vkSurfaceKHR;
}

uint32_t Turbo::Core::TSurface::GetMinImageCount()
{
	return this->minImageCount;
}

uint32_t Turbo::Core::TSurface::GetMaxImageCount()
{
	return this->maxImageCount;
}

Turbo::Core::TExtent2D Turbo::Core::TSurface::GetCurrentExtent()
{
	return this->currentExtent;
}

Turbo::Core::TExtent2D Turbo::Core::TSurface::GetMinImageExtent()
{
	return this->minImageExtent;
}

Turbo::Core::TExtent2D Turbo::Core::TSurface::GetMaxImageExtent()
{
	return this->maxImageExtent;
}

uint32_t Turbo::Core::TSurface::GetMaxImageArrayLayers()
{
	return this->maxImageArrayLayers;
}

bool Turbo::Core::TSurface::IsSupportIdentityTransform()
{
	if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportRotate90Transform()
{
	if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportRotate180Transform()
{
	if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportRotate270Transform()
{
	if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportHorizontalMirrorTransform()
{
	if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportHorizontalMirrorRotate90Transform()
{
	if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportHorizontalMirrorRotate180Transform()
{
	if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportHorizontalMirrorRotate270Transform()
{
	if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportInheritTransform()
{
	if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportCompositeAlphaOpaque()
{
	if (this->supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportCompositeAlphaPreMultiplied()
{
	if (this->supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportCompositeAlphaPostMultiplied()
{
	if (this->supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportCompositeAlphaInherit()
{
	if (this->supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportPresentModeImmediate()
{
	if (this->presentModeFlags & 0x001)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportPresentModeMailbox()
{
	if (this->presentModeFlags & 0x002)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportPresentModeFifo()
{
	if (this->presentModeFlags & 0x004)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TSurface::IsSupportPresentModeFifoRelaxed()
{
	if (this->presentModeFlags & 0x008)
	{
		return true;
	}

	return false;
}

std::vector<Turbo::Core::TSurfaceFormat> Turbo::Core::TSurface::GetSurfaceFormats()
{
	return this->surfaceFormats;
}

std::vector<Turbo::Core::TPresentMode> Turbo::Core::TSurface::GetPresentModes()
{
	return this->presentModes;
}

Turbo::Core::TSurfaceTransformFlagsKHR Turbo::Core::TSurface::GetSupportedTransforms()
{
	return this->supportedTransforms;
}

Turbo::Core::TSurfaceTransformFlagBitsKHR Turbo::Core::TSurface::GetCurrentTransform()
{
	return this->currentTransform;
}

Turbo::Core::TCompositeAlphaFlagsKHR Turbo::Core::TSurface::GetSupportedCompositeAlpha()
{
	return this->supportedCompositeAlpha;
}

Turbo::Core::TImageUsageFlags Turbo::Core::TSurface::GetSupportedUsageFlags()
{
	return this->supportedUsageFlags;
}

Turbo::Core::TPhysicalDevice * Turbo::Core::TSurface::GetPhysicalDevice()
{
	return this->physicalDevice;
}

std::string Turbo::Core::TSurface::ToString()
{
	return std::string();
}
