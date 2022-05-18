#pragma once
#ifndef TAPPLICATION_H
#define TAPPLICATION_H
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
#endif // TAPPLICATION_H