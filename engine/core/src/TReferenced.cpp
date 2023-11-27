#include "TReferenced.h"
#include <limits>

Turbo::Core::TReferenced::TReferenced()
{
    this->referenceCount = 0;
}

Turbo::Core::TReferenced::~TReferenced()
{
}

uint32_t Turbo::Core::TReferenced::Reference() const
{
    return ++this->referenceCount;
}

uint32_t Turbo::Core::TReferenced::UnReference() const
{
    if (this->referenceCount == 0)
    {
        delete this;
        return 0;
    }

    bool need_delete = false;

    uint32_t temp_reference_count = --this->referenceCount;

    if (this->referenceCount == 0)
    {
        need_delete = true;
    }

    if (need_delete)
    {
        this->Release();
    }

    return temp_reference_count;
}

uint32_t Turbo::Core::TReferenced::GetReferenceCount() const
{
    return this->referenceCount;
}

void Turbo::Core::TReferenced::Release() const
{
    delete this;
}

bool Turbo::Core::TReferenced::Valid() const
{
    return true;
}