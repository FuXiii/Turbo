#pragma once
#ifndef TALLOCATOR_H
#define TALLOCATOR_H
#include "TObject.h"

namespace Turbo
{
	namespace Core
	{
		class TAllocator : public TObject
		{
		public:
			explicit TAllocator();
			~TAllocator();

		public:
			static void *VKAPI_PTR Allocate(size_t size, size_t alignment);
			static void *VKAPI_PTR Reallocate(void *pOriginal, size_t size, size_t alignment);
			static void VKAPI_PTR Free(void *pMemory);

			virtual std::string ToString() override;
		};
	}
}
#endif // !TALLOCATOR_H