#include "TAllocator.h"

#if defined(TURBO_PLATFORM_WINDOWS)
#include <malloc.h>
#elif defined(TURBO_PLATFORM_LINUX) || defined(TURBO_PLATFORM_OPEN_HARMONY)
#include <malloc.h>
#elif defined(TURBO_PLATFORM_ANDROID)
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include <algorithm>

namespace Turbo
{
namespace Core
{
struct NAlignedMemoryHeader
{
    void *meta = nullptr;
    size_t alignedSize = 0;
};

Turbo::Core::NAlignedMemoryHeader GetAlignedMemoryHeader(void *alignedPtr)
{
    size_t aligned_memory_header_size = sizeof(Turbo::Core::NAlignedMemoryHeader);
    Turbo::Core::NAlignedMemoryHeader *aligned_memory_header = (Turbo::Core::NAlignedMemoryHeader *)((uintptr_t)alignedPtr - aligned_memory_header_size);
    return *aligned_memory_header;
}

void *GetAlignedMeta(void *alignedPtr)
{
    if (alignedPtr)
    {
        return GetAlignedMemoryHeader(alignedPtr).meta;
    }
    return nullptr;
}

size_t GetAlignedSize(void *alignedPtr)
{
    if (alignedPtr)
    {
        return GetAlignedMemoryHeader(alignedPtr).alignedSize;
    }
    return 0;
}

void *AlignedMalloc(size_t size, size_t alignment)
{
    size_t aligned_memory_header_size = sizeof(Turbo::Core::NAlignedMemoryHeader);
    size_t meta_size = aligned_memory_header_size + alignment - 1 + size;

    void *meta = malloc(meta_size);

    uintptr_t start = (uintptr_t)meta + aligned_memory_header_size;

    void *aligned_meta = (void *)Turbo::Core::TAllocator::AlignUp(start, alignment);

    {
        Turbo::Core::NAlignedMemoryHeader *aligned_memory_header = (Turbo::Core::NAlignedMemoryHeader *)((uintptr_t)aligned_meta - aligned_memory_header_size);
        aligned_memory_header->meta = meta;
        aligned_memory_header->alignedSize = size;
    }

    return aligned_meta;
}

void AlignedFree(void *alignedPtr)
{
    if (alignedPtr)
    {
        free(GetAlignedMeta(alignedPtr));
    }
}

void *AlignedRealloc(void *origin, size_t size, size_t alignment)
{
    void *new_meta = AlignedMalloc(size, alignment);
    memcpy(new_meta, origin, std::min(size, GetAlignedSize(origin)));
    AlignedFree(origin);
    return new_meta;
}

void *aligned_alloc(size_t alignment, size_t size)
{
    return Turbo::Core::AlignedMalloc(size, alignment);
}

void *aligned_ralloc(void *ptr, size_t alignment, size_t size)
{
    return Turbo::Core::AlignedRealloc(ptr, size, alignment);
}

void aligned_free(void *ptr)
{
    Turbo::Core::AlignedFree(ptr);
}
} // namespace Core
} // namespace Turbo

Turbo::Core::TAllocator::TAllocator()
{
}

Turbo::Core::TAllocator::~TAllocator()
{
}

void *VKAPI_PTR Turbo::Core::TAllocator::Allocate(size_t size, size_t alignment)
{
    // std::cout << "TAllocator::Allocation" << std::endl;
#if defined(TURBO_PLATFORM_WINDOWS)
    return _aligned_malloc(size, alignment);
#elif defined(TURBO_PLATFORM_LINUX) || defined(TURBO_PLATFORM_OPEN_HARMONY)
    return memalign(alignment, size);
#elif defined(TURBO_PLATFORM_ANDROID)
    if (alignment < sizeof(void *))
    {
        alignment = sizeof(void *);
    }
    return memalign(alignment, size);
#else
    return Turbo::Core::aligned_alloc(alignment, size);
#endif
}

void *VKAPI_PTR Turbo::Core::TAllocator::Reallocate(void *pOriginal, size_t size, size_t alignment)
{
    // std::cout << "TAllocator::Reallocation" << std::endl;
#if defined(TURBO_PLATFORM_WINDOWS)
    return _aligned_realloc(pOriginal, size, alignment);
#elif defined(TURBO_PLATFORM_LINUX) || defined(TURBO_PLATFORM_OPEN_HARMONY)
    void *new_memory = memalign(alignment, size);
    if (new_memory)
    {
        memcpy(new_memory, pOriginal, std::min(malloc_usable_size(pOriginal), size));
        free(pOriginal);
        return new_memory;
    }
    return pOriginal;
#elif defined(TURBO_PLATFORM_ANDROID)
    void *new_memory = memalign(alignment, size);
    if (new_memory)
    {
        memcpy(new_memory, pOriginal, std::min(malloc_usable_size(pOriginal), size));
        free(pOriginal);
        return new_memory;
    }
    return pOriginal;
#else
    return Turbo::Core::aligned_ralloc(pOriginal, alignment, size);
#endif
}

void VKAPI_PTR Turbo::Core::TAllocator::Free(void *pMemory)
{
    // std::cout << "TAllocator::Free::" << pMemory << std::endl;
#if defined(TURBO_PLATFORM_WINDOWS)
    return _aligned_free(pMemory);
#elif defined(TURBO_PLATFORM_LINUX) || defined(TURBO_PLATFORM_OPEN_HARMONY)
    free(pMemory);
#elif defined(TURBO_PLATFORM_ANDROID)
    free(pMemory);
#else
    return Turbo::Core::aligned_free(pMemory);
#endif
}

std::string Turbo::Core::TAllocator::ToString() const
{
    return std::string();
}
