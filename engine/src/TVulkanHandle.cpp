#include "TVulkanHandle.h"

Turbo::Core::TVulkanHandle::TVulkanHandle() : Turbo::Core::TObject()
{
}

Turbo::Core::TVulkanHandle::~TVulkanHandle()
{
}

void Turbo::Core::TVulkanHandle ::InternalRebuild()
{
    this->InternalDestroy();
    this->InternalCreate();
}
