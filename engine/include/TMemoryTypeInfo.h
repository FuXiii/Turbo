#pragma once
#ifndef TMEMORYTYPE_H
#define TMEMORYTYPE_H
#include "TInfo.h"
#include "TMemoryHeapInfo.h"

namespace Turbo
{
	namespace Core
	{
		typedef VkMemoryPropertyFlags TMemoryPropertyFlags;

		class TMemoryTypeInfo :public TInfo
		{
		public:
			friend class TPhysicalDevice;

		private:
			TMemoryPropertyFlags memoryPropertyFlags;
			TMemoryHeapInfo memoryHeap;

			uint32_t index;

		public:
			explicit TMemoryTypeInfo();
			~TMemoryTypeInfo();

		public:
			TMemoryHeapInfo GetMemoryHeap();

			uint32_t GetIndex();

			bool IsDeviceLocal();
			bool IsHostVisible();
			bool IsHostCoherent();
			bool IsHostCached();
			bool IsLazilyAllocated();
			bool IsProtected();

			virtual std::string ToString() override;
		};
	}
}
#endif // !TMEMORYTYPE_H