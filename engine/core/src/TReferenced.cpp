#include "TReferenced.h"

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
    bool need_delete = false;

    if (this->referenceCount == 0)
    {
        need_delete = true;
    }
    else
    {
        --this->referenceCount;

        if (this->referenceCount == 0)
        {
            need_delete = true;
        }
    }

    if (need_delete)
    {
        delete this;
    }

    return this->referenceCount;
}