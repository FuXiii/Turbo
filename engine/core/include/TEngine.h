#pragma once
#ifndef TURBO_CORE_TAPPLICATION_H
#define TURBO_CORE_TAPPLICATION_H
#include "TObject.h"
#include "TInstance.h"

namespace Turbo
{
	namespace Core
	{
		class TPhysicalDevice;
		class TDevice;

		class TEngine : public TObject
		{
		private:
			TInstance *instance = nullptr;

		private:
			void VerificationInitVulkan();
			void InitVulkan(TVersion version);

		public:
			explicit TEngine();
			~TEngine();
  
		public:
			TInstance &GetInstance();

			virtual std::string ToString() override;
		};
	}
}
#endif // TURBO_CORE_TAPPLICATION_H