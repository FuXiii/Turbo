#include "TPlatform.h"

Turbo::Core::TPlatformType Turbo::Core::TPlatform::GetCurrentPlatform()
{
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
	return TPlatformType::TWINDOWS;
#elif defined(__APPLE__)
	return TPlatformType::TAPPLE;
#elif defined(ANDROID) || defined(__ANDROID__) 
	return TPlatformType::TANDROID;
#elif defined(__linux) || defined(__linux__)
	return TPlatformType::TLINUX;
#elif defined(__unix) || defined(__unix__)
	return TPlatformType::TUNIX;
#else
	return TPlatformType::UNDEFINED;
#endif
}

uint32_t Turbo::Core::TPlatform::GetCurrentPlatformBits()
{
#if defined(_WIN64)
	return 64;
#elif defined(_WIN32)
	return 32;
#elif defined(_WIN16)
	return 16;
#elif defined(__APPLE__)
#if defined(__LP64__)
	return 64;
#else
	return 32;
#endif
#elif defined(__linux)
#if defined(__LP64__)
	return 64;
#else
	return 32;
#endif
#elif defined(__unix)
#if defined(__LP64__)
	return 64;
#else
	return 32;
#endif
#elif defined(ANDROID)
#if defined(__LP64__)
	return 64;
#else
	return 32;
#endif
#else
	return 0;
#endif
}