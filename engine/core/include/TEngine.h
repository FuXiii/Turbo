#pragma once
#ifndef TURBO_CORE_TAPPLICATION_H
#define TURBO_CORE_TAPPLICATION_H
#include "TInstance.h"
#include "TObject.h"


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

  protected:
    virtual ~TEngine();

  public:
    TInstance &GetInstance();

    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // TURBO_CORE_TAPPLICATION_H