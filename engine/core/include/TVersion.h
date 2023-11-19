#pragma once
#ifndef TURBO_CORE_TVERSION_H
#define TURBO_CORE_TVERSION_H
#include "TInfo.h"
#include <stdint.h>

namespace Turbo
{
	namespace Core
	{
		class TVersion :public TInfo
		{
		private:
			uint32_t major;
			uint32_t minor;
			uint32_t patch;
			uint32_t develop;

		public:
			static TVersion TurboVersion;

		public:
			explicit TVersion();
			explicit TVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t develop);
			TVersion(const TVersion& version);
			~TVersion();

		public:
			uint32_t GetVulkanVersion();
			TVersion GetValidVulkanVersion();

			uint32_t GetMajor();
			uint32_t GetMinor();
			uint32_t GetPatch();
			uint32_t GetDevelop();

			TVersion& operator =(const TVersion& version);
			bool operator == (const TVersion& version)const;
			bool operator != (const TVersion& version)const;
			bool operator > (const TVersion& version)const;
			bool operator < (const TVersion& version)const;
			bool operator <= (const TVersion& version)const;
			bool operator >= (const TVersion& version)const;

			virtual std::string ToString() override;
		};
	}
}
#endif // !TURBO_CORE_TVERSION_H