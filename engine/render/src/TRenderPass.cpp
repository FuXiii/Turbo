#include "TRenderPass.h"

Turbo::Render::TRenderPass &Turbo::Render::TRenderPass::SetTestValue(uint32_t value)
{
    this->testValue = value;
    return *this;
}

Turbo::Render::TRenderPass &Turbo::Render::TRenderPass::SetTestIsSomething(bool isSomething)
{
    this->testIsSomething = isSomething;
    return *this;
}