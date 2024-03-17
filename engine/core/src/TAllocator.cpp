#include "TAllocator.h"

#if defined(TURBO_PLATFORM_WINDOWS)
#include <malloc.h>
#elif defined(TURBO_PLATFORM_LINUX)
#include <malloc.h>
#elif defined(TURBO_PLATFORM_ANDROID)
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include <algorithm>

void *TGetAlignedMeta(void *alignedPtr)
{
    if (alignedPtr)
    {
        return ((void **)alignedPtr)[-1];
    }
    return nullptr;
}

size_t TGetAlignedSize(void *alignedPtr)
{
    if (alignedPtr)
    {
        return *(size_t *)((uintptr_t)alignedPtr - sizeof(void *) - sizeof(size_t));
    }
    return 0;
}

void *TAlignedMalloc(size_t size, size_t alignment)
{
    size_t meta_point_size = sizeof(void *);
    size_t aligned_size = sizeof(size_t);
    size_t meta_size = aligned_size + meta_point_size + alignment - 1 + size;

    void *meta = malloc(meta_size);

    uintptr_t start = (uintptr_t)meta + aligned_size + meta_point_size;

    void *aligned_meta = (void *)Turbo::Core::TAllocator::AlignUp(start, alignment);

    *(void **)((uintptr_t)aligned_meta - meta_point_size) = meta;
    *(size_t *)((uintptr_t)aligned_meta - (meta_point_size + aligned_size)) = size;

    return aligned_meta;
}

void TAlignedFree(void *alignedPtr)
{
    if (alignedPtr)
    {
        free(TGetAlignedMeta(alignedPtr));
    }
}

void *TAlignedRealloc(void *origin, size_t size, size_t alignment)
{
    void *new_meta = TAlignedMalloc(size, alignment);
    memcpy(new_meta, origin, std::min(size, TGetAlignedSize(origin)));
    TAlignedFree(origin);
    return new_meta;
}

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
#elif defined(TURBO_PLATFORM_LINUX)
    return memalign(alignment, size);
#elif defined(TURBO_PLATFORM_ANDROID)
    if (alignment < sizeof(void *))
    {
        alignment = sizeof(void *);
    }
    return memalign(alignment, size);
#else
    return TAlignedMalloc(size, alignment);
#endif
}

void *VKAPI_PTR Turbo::Core::TAllocator::Reallocate(void *pOriginal, size_t size, size_t alignment)
{
    // std::cout << "TAllocator::Reallocation" << std::endl;
#if defined(TURBO_PLATFORM_WINDOWS)
    return _aligned_realloc(pOriginal, size, alignment);
#elif defined(TURBO_PLATFORM_LINUX)
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
    return TAlignedRealloc(pOriginal, size, alignment);
#endif
}

void VKAPI_PTR Turbo::Core::TAllocator::Free(void *pMemory)
{
    // std::cout << "TAllocator::Free::" << pMemory << std::endl;
#if defined(TURBO_PLATFORM_WINDOWS)
    return _aligned_free(pMemory);
#elif defined(TURBO_PLATFORM_LINUX)
    free(pMemory);
#elif defined(TURBO_PLATFORM_ANDROID)
    free(pMemory);
#else
    return TAlignedRealloc(pOriginal, size, alignment);
#endif
}

std::string Turbo::Core::TAllocator::ToString() const
{
    return std::string();
}
