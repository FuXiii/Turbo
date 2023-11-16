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
    int temp_reference_count = std::numeric_limits<uint32_t>().max();

    bool need_delete = false;

    if (this->referenceCount == 0)
    {
        need_delete = true;
        temp_reference_count = 0;
    }
    else
    {
        temp_reference_count = --this->referenceCount;

        if (this->referenceCount == 0)
        {
            need_delete = true;
        }
    }

    if (need_delete)
    {
        delete this;
    }

    return temp_reference_count;
}