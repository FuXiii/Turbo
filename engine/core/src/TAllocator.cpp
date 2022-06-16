#include "TAllocator.h"

#if defined(ANDROID) || defined(__ANDROID__)
#include <cstdlib>
#elif defined(__APPLE__) || defined(__ANDROID__) || (defined(__linux__) && defined(__GLIBCXX__) && !defined(_GLIBCXX_HAVE_ALIGNED_ALLOC))
#include <cstdlib>
#if defined(__APPLE__)
#include <AvailabilityMacros.h>
#endif
#else
// #include <malloc.h>
#include <cstdlib>
#endif

Turbo::Core::TAllocator::TAllocator()
{
}

Turbo::Core::TAllocator::~TAllocator()
{
}

void *VKAPI_PTR Turbo::Core::TAllocator::Allocate(size_t size, size_t alignment)
{
    // std::cout << "TAllocator::Allocation" << std::endl;
#if defined(ANDROID) || defined(__ANDROID__)
    if (alignment < sizeof(void *))
    {
        alignment = sizeof(void *);
    }

    return memalign(alignment, size);
#elif defined(__APPLE__) || defined(__ANDROID__) || (defined(__linux__) && defined(__GLIBCXX__) && !defined(_GLIBCXX_HAVE_ALIGNED_ALLOC))
#if defined(__APPLE__) && (defined(MAC_OS_X_VERSION_10_16) || defined(__IPHONE_14_0))
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_16 || __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_14_0
    // For C++14, usr/include/malloc/_malloc.h declares aligned_alloc()) only
    // with the MacOSX11.0 SDK in Xcode 12 (which is what adds
    // MAC_OS_X_VERSION_10_16), even though the function is marked
    // availabe for 10.15. That's why the preprocessor checks for 10.16 but
    // the __builtin_available checks for 10.15.
    // People who use C++17 could call aligned_alloc with the 10.15 SDK already.
    if (__builtin_available(macOS 10.15, iOS 13, *))
        return aligned_alloc(alignment, size);
#endif
#endif
    if (alignment < sizeof(void *))
    {
        alignment = sizeof(void *);
    }

    void *pointer;
    if (posix_memalign(&pointer, alignment, size) == 0)
        return pointer;
    return nullptr;
#elif defined(TURBO_PLATFORM_WINDOWS)
    return _aligned_malloc(size, alignment);
#elif defined(TURBO_PLATFORM_LINUX)
    return aligned_alloc(alignment, size);
#else
    return aligned_alloc(alignment, size);
#endif
}

void *VKAPI_PTR Turbo::Core::TAllocator::Reallocate(void *pOriginal, size_t size, size_t alignment)
{
    // std::cout << "TAllocator::Reallocation" << std::endl;
#if defined(TURBO_PLATFORM_WINDOWS)
    return _aligned_realloc(pOriginal, size, alignment);
#endif
    return realloc(pOriginal, size);
}

void VKAPI_PTR Turbo::Core::TAllocator::Free(void *pMemory)
{
    // std::cout << "TAllocator::Free" << std::endl;
#if defined(TURBO_PLATFORM_WINDOWS)
    return _aligned_free(pMemory);
#else
    return free(pMemory);
#endif
}

std::string Turbo::Core::TAllocator::ToString()
{
    return std::string();
}
