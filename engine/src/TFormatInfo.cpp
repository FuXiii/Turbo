#include "TFormatInfo.h"
#include "TPhysicalDevice.h"

std::vector<VkFormat> TAllFormats = {
	VK_FORMAT_R4G4_UNORM_PACK8,
	VK_FORMAT_R4G4B4A4_UNORM_PACK16,
	VK_FORMAT_B4G4R4A4_UNORM_PACK16,
	VK_FORMAT_R5G6B5_UNORM_PACK16,
	VK_FORMAT_B5G6R5_UNORM_PACK16,
	VK_FORMAT_R5G5B5A1_UNORM_PACK16,
	VK_FORMAT_B5G5R5A1_UNORM_PACK16,
	VK_FORMAT_A1R5G5B5_UNORM_PACK16,
	VK_FORMAT_R8_UNORM,
	VK_FORMAT_R8_SNORM,
	VK_FORMAT_R8_USCALED,
	VK_FORMAT_R8_SSCALED,
	VK_FORMAT_R8_UINT,
	VK_FORMAT_R8_SINT,
	VK_FORMAT_R8_SRGB,
	VK_FORMAT_R8G8_UNORM,
	VK_FORMAT_R8G8_SNORM,
	VK_FORMAT_R8G8_USCALED,
	VK_FORMAT_R8G8_SSCALED,
	VK_FORMAT_R8G8_UINT,
	VK_FORMAT_R8G8_SINT,
	VK_FORMAT_R8G8_SRGB,
	VK_FORMAT_R8G8B8_UNORM,
	VK_FORMAT_R8G8B8_SNORM,
	VK_FORMAT_R8G8B8_USCALED,
	VK_FORMAT_R8G8B8_SSCALED,
	VK_FORMAT_R8G8B8_UINT,
	VK_FORMAT_R8G8B8_SINT,
	VK_FORMAT_R8G8B8_SRGB,
	VK_FORMAT_B8G8R8_UNORM,
	VK_FORMAT_B8G8R8_SNORM,
	VK_FORMAT_B8G8R8_USCALED,
	VK_FORMAT_B8G8R8_SSCALED,
	VK_FORMAT_B8G8R8_UINT,
	VK_FORMAT_B8G8R8_SINT,
	VK_FORMAT_B8G8R8_SRGB,
	VK_FORMAT_R8G8B8A8_UNORM,
	VK_FORMAT_R8G8B8A8_SNORM,
	VK_FORMAT_R8G8B8A8_USCALED,
	VK_FORMAT_R8G8B8A8_SSCALED,
	VK_FORMAT_R8G8B8A8_UINT,
	VK_FORMAT_R8G8B8A8_SINT,
	VK_FORMAT_R8G8B8A8_SRGB,
	VK_FORMAT_B8G8R8A8_UNORM,
	VK_FORMAT_B8G8R8A8_SNORM,
	VK_FORMAT_B8G8R8A8_USCALED,
	VK_FORMAT_B8G8R8A8_SSCALED,
	VK_FORMAT_B8G8R8A8_UINT,
	VK_FORMAT_B8G8R8A8_SINT,
	VK_FORMAT_B8G8R8A8_SRGB,
	VK_FORMAT_A8B8G8R8_UNORM_PACK32,
	VK_FORMAT_A8B8G8R8_SNORM_PACK32,
	VK_FORMAT_A8B8G8R8_USCALED_PACK32,
	VK_FORMAT_A8B8G8R8_SSCALED_PACK32,
	VK_FORMAT_A8B8G8R8_UINT_PACK32,
	VK_FORMAT_A8B8G8R8_SINT_PACK32,
	VK_FORMAT_A8B8G8R8_SRGB_PACK32,
	VK_FORMAT_A2R10G10B10_UNORM_PACK32,
	VK_FORMAT_A2R10G10B10_SNORM_PACK32,
	VK_FORMAT_A2R10G10B10_USCALED_PACK32,
	VK_FORMAT_A2R10G10B10_SSCALED_PACK32,
	VK_FORMAT_A2R10G10B10_UINT_PACK32,
	VK_FORMAT_A2R10G10B10_SINT_PACK32,
	VK_FORMAT_A2B10G10R10_UNORM_PACK32,
	VK_FORMAT_A2B10G10R10_SNORM_PACK32,
	VK_FORMAT_A2B10G10R10_USCALED_PACK32,
	VK_FORMAT_A2B10G10R10_SSCALED_PACK32,
	VK_FORMAT_A2B10G10R10_UINT_PACK32,
	VK_FORMAT_A2B10G10R10_SINT_PACK32,
	VK_FORMAT_R16_UNORM,
	VK_FORMAT_R16_SNORM,
	VK_FORMAT_R16_USCALED,
	VK_FORMAT_R16_SSCALED,
	VK_FORMAT_R16_UINT,
	VK_FORMAT_R16_SINT,
	VK_FORMAT_R16_SFLOAT,
	VK_FORMAT_R16G16_UNORM,
	VK_FORMAT_R16G16_SNORM,
	VK_FORMAT_R16G16_USCALED,
	VK_FORMAT_R16G16_SSCALED,
	VK_FORMAT_R16G16_UINT,
	VK_FORMAT_R16G16_SINT,
	VK_FORMAT_R16G16_SFLOAT,
	VK_FORMAT_R16G16B16_UNORM,
	VK_FORMAT_R16G16B16_SNORM,
	VK_FORMAT_R16G16B16_USCALED,
	VK_FORMAT_R16G16B16_SSCALED,
	VK_FORMAT_R16G16B16_UINT,
	VK_FORMAT_R16G16B16_SINT,
	VK_FORMAT_R16G16B16_SFLOAT,
	VK_FORMAT_R16G16B16A16_UNORM,
	VK_FORMAT_R16G16B16A16_SNORM,
	VK_FORMAT_R16G16B16A16_USCALED,
	VK_FORMAT_R16G16B16A16_SSCALED,
	VK_FORMAT_R16G16B16A16_UINT,
	VK_FORMAT_R16G16B16A16_SINT,
	VK_FORMAT_R16G16B16A16_SFLOAT,
	VK_FORMAT_R32_UINT,
	VK_FORMAT_R32_SINT,
	VK_FORMAT_R32_SFLOAT,
	VK_FORMAT_R32G32_UINT,
	VK_FORMAT_R32G32_SINT,
	VK_FORMAT_R32G32_SFLOAT,
	VK_FORMAT_R32G32B32_UINT,
	VK_FORMAT_R32G32B32_SINT,
	VK_FORMAT_R32G32B32_SFLOAT,
	VK_FORMAT_R32G32B32A32_UINT,
	VK_FORMAT_R32G32B32A32_SINT,
	VK_FORMAT_R32G32B32A32_SFLOAT,
	VK_FORMAT_R64_UINT,
	VK_FORMAT_R64_SINT,
	VK_FORMAT_R64_SFLOAT,
	VK_FORMAT_R64G64_UINT,
	VK_FORMAT_R64G64_SINT,
	VK_FORMAT_R64G64_SFLOAT,
	VK_FORMAT_R64G64B64_UINT,
	VK_FORMAT_R64G64B64_SINT,
	VK_FORMAT_R64G64B64_SFLOAT,
	VK_FORMAT_R64G64B64A64_UINT,
	VK_FORMAT_R64G64B64A64_SINT,
	VK_FORMAT_R64G64B64A64_SFLOAT,
	VK_FORMAT_B10G11R11_UFLOAT_PACK32,
	VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
	VK_FORMAT_D16_UNORM,
	VK_FORMAT_X8_D24_UNORM_PACK32,
	VK_FORMAT_D32_SFLOAT,
	VK_FORMAT_S8_UINT,
	VK_FORMAT_D16_UNORM_S8_UINT,
	VK_FORMAT_D24_UNORM_S8_UINT,
	VK_FORMAT_D32_SFLOAT_S8_UINT,
	VK_FORMAT_BC1_RGB_UNORM_BLOCK,
	VK_FORMAT_BC1_RGB_SRGB_BLOCK,
	VK_FORMAT_BC1_RGBA_UNORM_BLOCK,
	VK_FORMAT_BC1_RGBA_SRGB_BLOCK,
	VK_FORMAT_BC2_UNORM_BLOCK,
	VK_FORMAT_BC2_SRGB_BLOCK,
	VK_FORMAT_BC3_UNORM_BLOCK,
	VK_FORMAT_BC3_SRGB_BLOCK,
	VK_FORMAT_BC4_UNORM_BLOCK,
	VK_FORMAT_BC4_SNORM_BLOCK,
	VK_FORMAT_BC5_UNORM_BLOCK,
	VK_FORMAT_BC5_SNORM_BLOCK,
	VK_FORMAT_BC6H_UFLOAT_BLOCK,
	VK_FORMAT_BC6H_SFLOAT_BLOCK,
	VK_FORMAT_BC7_UNORM_BLOCK,
	VK_FORMAT_BC7_SRGB_BLOCK,
	VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK,
	VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK,
	VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,
	VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,
	VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK,
	VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK,
	VK_FORMAT_EAC_R11_UNORM_BLOCK,
	VK_FORMAT_EAC_R11_SNORM_BLOCK,
	VK_FORMAT_EAC_R11G11_UNORM_BLOCK,
	VK_FORMAT_EAC_R11G11_SNORM_BLOCK,
	VK_FORMAT_ASTC_4x4_UNORM_BLOCK,
	VK_FORMAT_ASTC_4x4_SRGB_BLOCK,
	VK_FORMAT_ASTC_5x4_UNORM_BLOCK,
	VK_FORMAT_ASTC_5x4_SRGB_BLOCK,
	VK_FORMAT_ASTC_5x5_UNORM_BLOCK,
	VK_FORMAT_ASTC_5x5_SRGB_BLOCK,
	VK_FORMAT_ASTC_6x5_UNORM_BLOCK,
	VK_FORMAT_ASTC_6x5_SRGB_BLOCK,
	VK_FORMAT_ASTC_6x6_UNORM_BLOCK,
	VK_FORMAT_ASTC_6x6_SRGB_BLOCK,
	VK_FORMAT_ASTC_8x5_UNORM_BLOCK,
	VK_FORMAT_ASTC_8x5_SRGB_BLOCK,
	VK_FORMAT_ASTC_8x6_UNORM_BLOCK,
	VK_FORMAT_ASTC_8x6_SRGB_BLOCK,
	VK_FORMAT_ASTC_8x8_UNORM_BLOCK,
	VK_FORMAT_ASTC_8x8_SRGB_BLOCK,
	VK_FORMAT_ASTC_10x5_UNORM_BLOCK,
	VK_FORMAT_ASTC_10x5_SRGB_BLOCK,
	VK_FORMAT_ASTC_10x6_UNORM_BLOCK,
	VK_FORMAT_ASTC_10x6_SRGB_BLOCK,
	VK_FORMAT_ASTC_10x8_UNORM_BLOCK,
	VK_FORMAT_ASTC_10x8_SRGB_BLOCK,
	VK_FORMAT_ASTC_10x10_UNORM_BLOCK,
	VK_FORMAT_ASTC_10x10_SRGB_BLOCK,
	VK_FORMAT_ASTC_12x10_UNORM_BLOCK,
	VK_FORMAT_ASTC_12x10_SRGB_BLOCK,
	VK_FORMAT_ASTC_12x12_UNORM_BLOCK,
	VK_FORMAT_ASTC_12x12_SRGB_BLOCK,
	VK_FORMAT_G8B8G8R8_422_UNORM,
	VK_FORMAT_B8G8R8G8_422_UNORM,
	VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM,
	VK_FORMAT_G8_B8R8_2PLANE_420_UNORM,
	VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM,
	VK_FORMAT_G8_B8R8_2PLANE_422_UNORM,
	VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM,
	VK_FORMAT_R10X6_UNORM_PACK16,
	VK_FORMAT_R10X6G10X6_UNORM_2PACK16,
	VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16,
	VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,
	VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,
	VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
	VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
	VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
	VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
	VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
	VK_FORMAT_R12X4_UNORM_PACK16,
	VK_FORMAT_R12X4G12X4_UNORM_2PACK16,
	VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16,
	VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,
	VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,
	VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,
	VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,
	VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,
	VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,
	VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,
	VK_FORMAT_G16B16G16R16_422_UNORM,
	VK_FORMAT_B16G16R16G16_422_UNORM,
	VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM,
	VK_FORMAT_G16_B16R16_2PLANE_420_UNORM,
	VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM,
	VK_FORMAT_G16_B16R16_2PLANE_422_UNORM,
	VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM,
	VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG,
	VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG,
	VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG,
	VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG,
	VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG,
	VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG,
	VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG,
	VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG,
	VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT,
	VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT,
	VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT,
	VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT,
	VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT,
	VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT,
	VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT,
	VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT,
	VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT,
	VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT,
	VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT,
	VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT,
	VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT,
	VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT,
	VK_FORMAT_G8B8G8R8_422_UNORM_KHR,
	VK_FORMAT_B8G8R8G8_422_UNORM_KHR,
	VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR,
	VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR,
	VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR,
	VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR,
	VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR,
	VK_FORMAT_R10X6_UNORM_PACK16_KHR,
	VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR,
	VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR,
	VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR,
	VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR,
	VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR,
	VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR,
	VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR,
	VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR,
	VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR,
	VK_FORMAT_R12X4_UNORM_PACK16_KHR,
	VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR,
	VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR,
	VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR,
	VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR,
	VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR,
	VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR,
	VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR,
	VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR,
	VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR,
	VK_FORMAT_G16B16G16R16_422_UNORM_KHR,
	VK_FORMAT_B16G16R16G16_422_UNORM_KHR,
	VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR,
	VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR,
	VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR,
	VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR,
	VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR
};

std::vector<Turbo::Core::TFormatInfo> Turbo::Core::TFormatInfo::GetSupportFormats(TPhysicalDevice* physicalDevice)
{
	std::vector<TFormatInfo> formats;
	if (physicalDevice != nullptr && physicalDevice->GetVkPhysicalDevice() != VK_NULL_HANDLE)
	{
		size_t all_formats_count = TAllFormats.size();
		for (size_t format_index = 0; format_index < all_formats_count; format_index++)
		{
			VkFormatProperties format_properties = {};
			format_properties.bufferFeatures = 0;
			format_properties.linearTilingFeatures = 0;
			format_properties.optimalTilingFeatures = 0;

			vkGetPhysicalDeviceFormatProperties(physicalDevice->GetVkPhysicalDevice(), TAllFormats[format_index], &format_properties);
			if (format_properties.bufferFeatures || format_properties.linearTilingFeatures || format_properties.optimalTilingFeatures)
			{
				TFormatInfo format(static_cast<TFormatType>(TAllFormats[format_index]));
				format.linearTilingFeatures = format_properties.linearTilingFeatures;
				format.optimalTilingFeatures = format_properties.optimalTilingFeatures;
				format.bufferFeatures = format_properties.bufferFeatures;
				formats.push_back(format);
			}
		}
	}

	return formats;
}

bool Turbo::Core::TFormatInfo::IsSupportFormat(TPhysicalDevice * physicalDevice, TFormatType formatType)
{
	if (physicalDevice != nullptr && physicalDevice->GetVkPhysicalDevice() != VK_NULL_HANDLE)
	{
		std::vector<TFormatInfo> support_formats = TFormatInfo::GetSupportFormats(physicalDevice);
		size_t support_format_count = support_formats.size();
		for (size_t support_format_index = 0; support_format_index < support_format_count; support_format_index++)
		{
			if (support_formats[support_format_index].formatType == formatType)
			{
				return true;
			}
		}
	}

	return false;
}

Turbo::Core::TFormatInfo::TFormatInfo(TFormatType formatType)
{
	this->formatType = formatType;
}

Turbo::Core::TFormatInfo::~TFormatInfo()
{
}

Turbo::Core::TFormatType Turbo::Core::TFormatInfo::GetFormatType()
{
	return this->formatType;
}

VkFormat Turbo::Core::TFormatInfo::GetVkFormat()
{
	return static_cast<VkFormat>(this->formatType);
}

bool Turbo::Core::TFormatInfo::operator==(const TFormatInfo & format) const
{
	if (format.formatType == this->formatType)
	{
		return true;
	}
	return false;
}

bool Turbo::Core::TFormatInfo::operator!=(const TFormatInfo & format) const
{
	if (format.formatType != this->formatType)
	{
		return true;
	}
	return false;
}

std::string Turbo::Core::TFormatInfo::ToString()
{
	/*switch (this->formatType)
	{
	case Turbo::Core::TFormatType::UNDEFINED:
	{}
	break;
	case Turbo::Core::TFormatType::R4G4_UNORM_PACK8:
	{}
	break;
	case Turbo::Core::TFormatType::R4G4B4A4_UNORM_PACK16:
	{}
	break;
	case Turbo::Core::TFormatType::B4G4R4A4_UNORM_PACK16:
		break;
	case Turbo::Core::TFormatType::R5G6B5_UNORM_PACK16:
		break;
	case Turbo::Core::TFormatType::B5G6R5_UNORM_PACK16:
		break;
	case Turbo::Core::TFormatType::R5G5B5A1_UNORM_PACK16:
		break;
	case Turbo::Core::TFormatType::B5G5R5A1_UNORM_PACK16:
		break;
	case Turbo::Core::TFormatType::A1R5G5B5_UNORM_PACK16:
		break;
	case Turbo::Core::TFormatType::R8_UNORM:
		break;
	case Turbo::Core::TFormatType::R8_SNORM:
		break;
	case Turbo::Core::TFormatType::R8_USCALED:
		break;
	case Turbo::Core::TFormatType::R8_SSCALED:
		break;
	case Turbo::Core::TFormatType::R8_UINT:
		break;
	case Turbo::Core::TFormatType::R8_SINT:
		break;
	case Turbo::Core::TFormatType::R8_SRGB:
		break;
	case Turbo::Core::TFormatType::R8G8_UNORM:
		break;
	case Turbo::Core::TFormatType::R8G8_SNORM:
		break;
	case Turbo::Core::TFormatType::R8G8_USCALED:
		break;
	case Turbo::Core::TFormatType::R8G8_SSCALED:
		break;
	case Turbo::Core::TFormatType::R8G8_UINT:
		break;
	case Turbo::Core::TFormatType::R8G8_SINT:
		break;
	case Turbo::Core::TFormatType::R8G8_SRGB:
		break;
	case Turbo::Core::TFormatType::R8G8B8_UNORM:
		break;
	case Turbo::Core::TFormatType::R8G8B8_SNORM:
		break;
	case Turbo::Core::TFormatType::R8G8B8_USCALED:
		break;
	case Turbo::Core::TFormatType::R8G8B8_SSCALED:
		break;
	case Turbo::Core::TFormatType::R8G8B8_UINT:
		break;
	case Turbo::Core::TFormatType::R8G8B8_SINT:
		break;
	case Turbo::Core::TFormatType::R8G8B8_SRGB:
		break;
	case Turbo::Core::TFormatType::B8G8R8_UNORM:
		break;
	case Turbo::Core::TFormatType::B8G8R8_SNORM:
		break;
	case Turbo::Core::TFormatType::B8G8R8_USCALED:
		break;
	case Turbo::Core::TFormatType::B8G8R8_SSCALED:
		break;
	case Turbo::Core::TFormatType::B8G8R8_UINT:
		break;
	case Turbo::Core::TFormatType::B8G8R8_SINT:
		break;
	case Turbo::Core::TFormatType::B8G8R8_SRGB:
		break;
	case Turbo::Core::TFormatType::R8G8B8A8_UNORM:
		break;
	case Turbo::Core::TFormatType::R8G8B8A8_SNORM:
		break;
	case Turbo::Core::TFormatType::R8G8B8A8_USCALED:
		break;
	case Turbo::Core::TFormatType::R8G8B8A8_SSCALED:
		break;
	case Turbo::Core::TFormatType::R8G8B8A8_UINT:
		break;
	case Turbo::Core::TFormatType::R8G8B8A8_SINT:
		break;
	case Turbo::Core::TFormatType::R8G8B8A8_SRGB:
		break;
	case Turbo::Core::TFormatType::B8G8R8A8_UNORM:
		break;
	case Turbo::Core::TFormatType::B8G8R8A8_SNORM:
		break;
	case Turbo::Core::TFormatType::B8G8R8A8_USCALED:
		break;
	case Turbo::Core::TFormatType::B8G8R8A8_SSCALED:
		break;
	case Turbo::Core::TFormatType::B8G8R8A8_UINT:
		break;
	case Turbo::Core::TFormatType::B8G8R8A8_SINT:
		break;
	case Turbo::Core::TFormatType::B8G8R8A8_SRGB:
		break;
	case Turbo::Core::TFormatType::A8B8G8R8_UNORM_PACK32:
		break;
	case Turbo::Core::TFormatType::A8B8G8R8_SNORM_PACK32:
		break;
	case Turbo::Core::TFormatType::A8B8G8R8_USCALED_PACK32:
		break;
	case Turbo::Core::TFormatType::A8B8G8R8_SSCALED_PACK32:
		break;
	case Turbo::Core::TFormatType::A8B8G8R8_UINT_PACK32:
		break;
	case Turbo::Core::TFormatType::A8B8G8R8_SINT_PACK32:
		break;
	case Turbo::Core::TFormatType::A8B8G8R8_SRGB_PACK32:
		break;
	case Turbo::Core::TFormatType::A2R10G10B10_UNORM_PACK32:
		break;
	case Turbo::Core::TFormatType::A2R10G10B10_SNORM_PACK32:
		break;
	case Turbo::Core::TFormatType::A2R10G10B10_USCALED_PACK32:
		break;
	case Turbo::Core::TFormatType::A2R10G10B10_SSCALED_PACK32:
		break;
	case Turbo::Core::TFormatType::A2R10G10B10_UINT_PACK32:
		break;
	case Turbo::Core::TFormatType::A2R10G10B10_SINT_PACK32:
		break;
	case Turbo::Core::TFormatType::A2B10G10R10_UNORM_PACK32:
		break;
	case Turbo::Core::TFormatType::A2B10G10R10_SNORM_PACK32:
		break;
	case Turbo::Core::TFormatType::A2B10G10R10_USCALED_PACK32:
		break;
	case Turbo::Core::TFormatType::A2B10G10R10_SSCALED_PACK32:
		break;
	case Turbo::Core::TFormatType::A2B10G10R10_UINT_PACK32:
		break;
	case Turbo::Core::TFormatType::A2B10G10R10_SINT_PACK32:
		break;
	case Turbo::Core::TFormatType::R16_UNORM:
		break;
	case Turbo::Core::TFormatType::R16_SNORM:
		break;
	case Turbo::Core::TFormatType::R16_USCALED:
		break;
	case Turbo::Core::TFormatType::R16_SSCALED:
		break;
	case Turbo::Core::TFormatType::R16_UINT:
		break;
	case Turbo::Core::TFormatType::R16_SINT:
		break;
	case Turbo::Core::TFormatType::R16_SFLOAT:
		break;
	case Turbo::Core::TFormatType::R16G16_UNORM:
		break;
	case Turbo::Core::TFormatType::R16G16_SNORM:
		break;
	case Turbo::Core::TFormatType::R16G16_USCALED:
		break;
	case Turbo::Core::TFormatType::R16G16_SSCALED:
		break;
	case Turbo::Core::TFormatType::R16G16_UINT:
		break;
	case Turbo::Core::TFormatType::R16G16_SINT:
		break;
	case Turbo::Core::TFormatType::R16G16_SFLOAT:
		break;
	case Turbo::Core::TFormatType::R16G16B16_UNORM:
		break;
	case Turbo::Core::TFormatType::R16G16B16_SNORM:
		break;
	case Turbo::Core::TFormatType::R16G16B16_USCALED:
		break;
	case Turbo::Core::TFormatType::R16G16B16_SSCALED:
		break;
	case Turbo::Core::TFormatType::R16G16B16_UINT:
		break;
	case Turbo::Core::TFormatType::R16G16B16_SINT:
		break;
	case Turbo::Core::TFormatType::R16G16B16_SFLOAT:
		break;
	case Turbo::Core::TFormatType::R16G16B16A16_UNORM:
		break;
	case Turbo::Core::TFormatType::R16G16B16A16_SNORM:
		break;
	case Turbo::Core::TFormatType::R16G16B16A16_USCALED:
		break;
	case Turbo::Core::TFormatType::R16G16B16A16_SSCALED:
		break;
	case Turbo::Core::TFormatType::R16G16B16A16_UINT:
		break;
	case Turbo::Core::TFormatType::R16G16B16A16_SINT:
		break;
	case Turbo::Core::TFormatType::R16G16B16A16_SFLOAT:
		break;
	case Turbo::Core::TFormatType::R32_UINT:
		break;
	case Turbo::Core::TFormatType::R32_SINT:
		break;
	case Turbo::Core::TFormatType::R32_SFLOAT:
		break;
	case Turbo::Core::TFormatType::R32G32_UINT:
		break;
	case Turbo::Core::TFormatType::R32G32_SINT:
		break;
	case Turbo::Core::TFormatType::R32G32_SFLOAT:
		break;
	case Turbo::Core::TFormatType::R32G32B32_UINT:
		break;
	case Turbo::Core::TFormatType::R32G32B32_SINT:
		break;
	case Turbo::Core::TFormatType::R32G32B32_SFLOAT:
		break;
	case Turbo::Core::TFormatType::R32G32B32A32_UINT:
		break;
	case Turbo::Core::TFormatType::R32G32B32A32_SINT:
		break;
	case Turbo::Core::TFormatType::R32G32B32A32_SFLOAT:
	{

	}
		break;
	case Turbo::Core::TFormatType::R64_UINT:
	{

	}
		break;
	case Turbo::Core::TFormatType::R64_SINT:
	{

	}
		break;
	case Turbo::Core::TFormatType::R64_SFLOAT:
	{

	}
		break;
	case Turbo::Core::TFormatType::R64G64_UINT:
	{

	}
		break;
	case Turbo::Core::TFormatType::R64G64_SINT:
	{

	}
		break;
	case Turbo::Core::TFormatType::R64G64_SFLOAT:
	{

	}
		break;
	case Turbo::Core::TFormatType::R64G64B64_UINT:
	{

	}
		break;
	case Turbo::Core::TFormatType::R64G64B64_SINT:
	{

	}
		break;
	case Turbo::Core::TFormatType::R64G64B64_SFLOAT:
	{

	}
		break;
	case Turbo::Core::TFormatType::R64G64B64A64_UINT:
	{

	}
		break;
	case Turbo::Core::TFormatType::R64G64B64A64_SINT:
	{

	}
		break;
	case Turbo::Core::TFormatType::R64G64B64A64_SFLOAT:
	{

	}
		break;
	case Turbo::Core::TFormatType::B10G11R11_UFLOAT_PACK32:
	{

	}
		break;
	case Turbo::Core::TFormatType::E5B9G9R9_UFLOAT_PACK32:
	{

	}
		break;
	case Turbo::Core::TFormatType::D16_UNORM:
	{

	}
		break;
	case Turbo::Core::TFormatType::X8_D24_UNORM_PACK32:
	{

	}
		break;
	case Turbo::Core::TFormatType::D32_SFLOAT:
	{

	}
		break;
	case Turbo::Core::TFormatType::S8_UINT:
	{

	}
		break;
	case Turbo::Core::TFormatType::D16_UNORM_S8_UINT:
	{

	}
		break;
	case Turbo::Core::TFormatType::D24_UNORM_S8_UINT:
	{

	}
		break;
	case Turbo::Core::TFormatType::D32_SFLOAT_S8_UINT:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC1_RGB_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC1_RGB_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC1_RGBA_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC1_RGBA_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC2_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC2_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC3_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC3_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC4_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC4_SNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC5_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC5_SNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC6H_UFLOAT_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC6H_SFLOAT_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC7_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::BC7_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ETC2_R8G8B8_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ETC2_R8G8B8_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ETC2_R8G8B8A1_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ETC2_R8G8B8A1_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ETC2_R8G8B8A8_UNORM_BLOCK:
	{

	}
	{

	}
		break;
	case Turbo::Core::TFormatType::ETC2_R8G8B8A8_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::EAC_R11_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::EAC_R11_SNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::EAC_R11G11_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::EAC_R11G11_SNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_4x4_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_4x4_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_5x4_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_5x4_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_5x5_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_5x5_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_6x5_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_6x5_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_6x6_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_6x6_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_8x5_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_8x5_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_8x6_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_8x6_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_8x8_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_8x8_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_10x5_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_10x5_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_10x6_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_10x6_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_10x8_UNORM_BLOCK:
	{

	}
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_10x8_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_10x10_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_10x10_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_12x10_UNORM_BLOCK:
	{

	}
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_12x10_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_12x12_UNORM_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::ASTC_12x12_SRGB_BLOCK:
	{

	}
		break;
	case Turbo::Core::TFormatType::G8B8G8R8_422_UNORM:
	{

	}
		break;
	case Turbo::Core::TFormatType::B8G8R8G8_422_UNORM:
	{

	}
		break;
	case Turbo::Core::TFormatType::G8_B8_R8_3PLANE_420_UNORM:
	{

	}
		break;
	case Turbo::Core::TFormatType::G8_B8R8_2PLANE_420_UNORM:
	{

	}
		break;
	case Turbo::Core::TFormatType::G8_B8_R8_3PLANE_422_UNORM:
	{

	}
		break;
	case Turbo::Core::TFormatType::G8_B8R8_2PLANE_422_UNORM:
	{

	}
		break;
	case Turbo::Core::TFormatType::G8_B8_R8_3PLANE_444_UNORM:
	{

	}
		break;
	case Turbo::Core::TFormatType::R10X6_UNORM_PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::R10X6G10X6_UNORM_2PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::R10X6G10X6B10X6A10X6_UNORM_4PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::R12X4_UNORM_PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::R12X4G12X4_UNORM_2PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::R12X4G12X4B12X4A12X4_UNORM_4PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
	{

	}
		break;
	case Turbo::Core::TFormatType::B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
	{

	}

	break;
	case Turbo::Core::TFormatType::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
	{}
	break;
	case Turbo::Core::TFormatType::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
	{}
	break;
	case Turbo::Core::TFormatType::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
	{}
	break;
	case Turbo::Core::TFormatType::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
	{}
	break;
	case Turbo::Core::TFormatType::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
	{}
	break;
	case Turbo::Core::TFormatType::G16B16G16R16_422_UNORM:
	{}
	break;
	case Turbo::Core::TFormatType::B16G16R16G16_422_UNORM:
	{}
	break;
	case Turbo::Core::TFormatType::G16_B16_R16_3PLANE_420_UNORM:
	{}
	break;
	case Turbo::Core::TFormatType::G16_B16R16_2PLANE_420_UNORM:
	{}
	break;
	case Turbo::Core::TFormatType::G16_B16_R16_3PLANE_422_UNORM:
	{}
	break;
	case Turbo::Core::TFormatType::G16_B16R16_2PLANE_422_UNORM:
	{}
	break;
	case Turbo::Core::TFormatType::G16_B16_R16_3PLANE_444_UNORM:
	{}
	break;
	default:
		break;
	}*/
	return std::string();
}
