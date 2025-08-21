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

uint32_t Turbo::Core::TReferenced::UnReferenceWithoutDelete() const
{
    if (this->referenceCount == 0)
    {
        return 0;
    }

    return --this->referenceCount;
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

Turbo::Core::TMemory::TMemory(void *data, const std::size_t &size) : data(data), size(size)
{
}

Turbo::Core::TMemory::TMemory(const std::size_t &size) : size(size)
{
    if (size > 0)
    {
        this->data = malloc(size);
    }
}

Turbo::Core::TMemory::~TMemory()
{
    if (this->data != nullptr)
    {
        free(this->data);
        this->data = nullptr;
        this->size = 0;
    }
}

const std::size_t &Turbo::Core::TMemory::Size() const
{
    return this->size;
}

const void *Turbo::Core::TMemory::Data() const
{
    return this->data;
}

void *Turbo::Core::TMemory::Data()
{
    return this->data;
}

bool Turbo::Core::TMemory::Valid() const
{
    return this->data != nullptr && this->size > 0;
}
