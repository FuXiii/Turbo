#pragma once
#ifndef TMEMORYHEAP_H
#define TMEMORYHEAP_H
#include "TInfo.h"

namespace Turbo
{
	namespace Core
	{
		class TMemoryHeapInfo :public TInfo
		{
		public:
			friend class TPhysicalDevice;

		private:
			TMemoryHeapFlags memoryHeapFlags;
			TDeviceSize size;

		public:
			explicit TMemoryHeapInfo();
			~TMemoryHeapInfo();

		public:
			bool IsDeviceLocal();
			bool IsMultiInstance();

			TDeviceSize GetByteSize();

			// Inherited via TObject
			virtual std::string ToString() override;
		};
	}
}
#endif // !TMEMORYHEAP_H