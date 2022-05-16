#include "TQueueFamilyInfo.h"
#include "TAllocator.h"

void Turbo::Core::TQueueFamilyInfo::CalculatePerformanceScore()
{
	if (this->IsSupportGraphics())
	{
		this->score += 5;
	}

	if (this->IsSupportCompute())
	{
		this->score += 4;
	}

	if (this->IsSupportTransfer())
	{
		this->score += 3;
	}

	if (this->IsSupportSparse())
	{
		this->score += 2;
	}

	if (this->IsSupportPresent())
	{
		this->score += 1;
	}
}

Turbo::Core::TQueueFamilyInfo::TQueueFamilyInfo() : Turbo::Core::TInfo()
{
	this->queueFlags = 0;
	this->queueCount = 0;
	this->timestampValidBits = 0;
	this->minImageTransferGranularity.width = 0;
	this->minImageTransferGranularity.height = 0;
	this->minImageTransferGranularity.depth = 0;
	this->index = UINT32_MAX;
	this->score = 0;
}

Turbo::Core::TQueueFamilyInfo::TQueueFamilyInfo(const TQueueFamilyInfo &obj)
{
	this->queueFlags = obj.queueFlags;
	this->queueCount = obj.queueCount;
	this->timestampValidBits = obj.timestampValidBits;
	this->minImageTransferGranularity.width = obj.minImageTransferGranularity.width;
	this->minImageTransferGranularity.height = obj.minImageTransferGranularity.height;
	this->minImageTransferGranularity.depth = obj.minImageTransferGranularity.depth;
	this->index = obj.index;
	this->score = obj.score;
}

Turbo::Core::TQueueFamilyInfo::~TQueueFamilyInfo()
{
}

bool Turbo::Core::TQueueFamilyInfo::IsSupportGraphics()
{
	if (this->queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TQueueFamilyInfo::IsSupportCompute()
{
	if (this->queueFlags & VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TQueueFamilyInfo::IsSupportTransfer()
{
	if (this->queueFlags & VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TQueueFamilyInfo::IsSupportSparse()
{
	if (this->queueFlags & VkQueueFlagBits::VK_QUEUE_SPARSE_BINDING_BIT)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TQueueFamilyInfo::IsSupportProtected()
{
	if (this->queueFlags & VkQueueFlagBits::VK_QUEUE_PROTECTED_BIT)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TQueueFamilyInfo::IsSupportPresent()
{
	if (this->IsSupportGraphics() || this->IsSupportCompute())
	{
		return true;
	}

	return false;
}

uint32_t Turbo::Core::TQueueFamilyInfo::GetQueueCount()
{
	return this->queueCount;
}

uint32_t Turbo::Core::TQueueFamilyInfo::GetTimestampValidBits()
{
	return this->timestampValidBits;
}

VkExtent3D Turbo::Core::TQueueFamilyInfo::GetMinImageTransferGranularity()
{
	return this->minImageTransferGranularity;
}

uint32_t Turbo::Core::TQueueFamilyInfo::GetIndex()
{
	return this->index;
}

uint32_t Turbo::Core::TQueueFamilyInfo::GetPerformanceScore()
{
	return this->score;
}

bool Turbo::Core::TQueueFamilyInfo::operator==(const TQueueFamilyInfo &queueFamily) const
{
	return this->index == queueFamily.index;
}

bool Turbo::Core::TQueueFamilyInfo::operator!=(const TQueueFamilyInfo &queueFamily) const
{
	return this->index != queueFamily.index;
}

bool Turbo::Core::TQueueFamilyInfo::operator<(const TQueueFamilyInfo &queueFamily) const
{
	return this->index < queueFamily.index;
}

std::string Turbo::Core::TQueueFamilyInfo::ToString()
{
	return std::string();
}
