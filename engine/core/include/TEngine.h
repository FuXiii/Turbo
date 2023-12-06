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

// FIXME: Delete this TEngine class
class TEngine : public TObject
{
  private:
    TRefPtr<TInstance> instance;

  private:
    void VerificationInitVulkan();
    void InitVulkan(TVersion version);

  public:
    explicit TEngine();

  protected:
    virtual ~TEngine();

  public:
    const TRefPtr<TInstance> &GetInstance();

    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};
} // namespace Core
} // namespace Turbo
#endif // TURBO_CORE_TAPPLICATION_H