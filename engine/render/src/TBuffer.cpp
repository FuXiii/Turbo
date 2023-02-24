#include "render/include/TBuffer.h"
#include "render/include/TResourceAllocator.h"
#include <core/include/TBuffer.h>
#include <core/include/TCommandBuffer.h>
#include <core/include/TCommandBufferPool.h>
#include <core/include/TDeviceQueue.h>
#include <core/include/TException.h>
#include <core/include/TFence.h>
#include <core/include/TPhysicalDevice.h>

void Turbo::Render::TBuffer::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    if (allocator != nullptr)
    {
        Turbo::Render::TResourceAllocator *resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
        this->buffer = resource_allocator->CreateBuffer(descriptor);
        this->allocator = allocator;
        this->descriptor = descriptor;
    }
}

void Turbo::Render::TBuffer::Destroy(void *allocator)
{
    if (allocator != nullptr)
    {
        Turbo::Render::TResourceAllocator *resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
        resource_allocator->DestroyBuffer(this->buffer);
    }
}

Turbo::Render::TBufferUsages Turbo::Render::TBuffer::GetUsages()
{
    return this->descriptor.usages;
}

uint64_t Turbo::Render::TBuffer::GetSize()
{
    return this->descriptor.size;
}

Turbo::Render::TDomain Turbo::Render::TBuffer::GetDomain()
{
    return this->descriptor.domain;
}

void Turbo::Render::TBuffer::Copy(void *src, uint64_t size)
{
    if (this->buffer != nullptr && this->buffer->GetVkBuffer() != VK_NULL_HANDLE)
    {
        uint64_t copy_size = this->descriptor.size < size ? this->descriptor.size : size;

        Turbo::Core::TMemoryTypeInfo memory_type_info = this->buffer->GetMemoryTypeInfo();
        if (memory_type_info.IsHostVisible() && this->buffer->IsMappable())
        {
            void *buffer_ptr = this->buffer->Map();
            memcpy(buffer_ptr, src, copy_size);
            this->buffer->Unmap();
        }
        else
        {
            Turbo::Render::TBuffer::Descriptor temp_buffer_descriptor = {};
            temp_buffer_descriptor.usages = Turbo::Render::TBufferUsageBits::BUFFER_TRANSFER_SRC;
            temp_buffer_descriptor.size = this->descriptor.size;
            temp_buffer_descriptor.domain = Turbo::Render::TDomainBits::CPU;

            Turbo::Render::TBuffer temp_buffer;
            temp_buffer.Create("temp_buffer", temp_buffer_descriptor, this->allocator);
            temp_buffer.Copy(src, size);

            {
                Turbo::Render::TResourceAllocator *resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
                Turbo::Render::TContext *context = resource_allocator->GetContext();
                Turbo::Core::TDeviceQueue *queue = context->GetDeviceQueue();

                Turbo::Core::TCommandBuffer *command_buffer = resource_allocator->AllocateCommandBuffer();
                command_buffer->Begin();
                command_buffer->CmdCopyBuffer(temp_buffer.buffer, this->buffer, 0, 0, copy_size);
                command_buffer->End();

                Turbo::Core::TFence *copy_fence = new Turbo::Core::TFence(context->GetDevice());
                queue->Submit(nullptr, nullptr, command_buffer, copy_fence);
                copy_fence->WaitUntil();

                delete copy_fence;
                resource_allocator->FreeCommandBuffer(command_buffer);
            }

            temp_buffer.Destroy(this->allocator);
        }
    }
}

void Turbo::Render::TBuffer::Copy(TBuffer *src, uint64_t srcOffset, uint64_t size)
{
    if (this->buffer != nullptr && this->buffer->GetVkBuffer() != VK_NULL_HANDLE && src != nullptr)
    {
        uint64_t copy_size = src->descriptor.size < size ? src->descriptor.size : size;
        copy_size = copy_size < this->descriptor.size ? copy_size : this->descriptor.size;

        Turbo::Render::TResourceAllocator *resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
        Turbo::Render::TContext *context = resource_allocator->GetContext();
        Turbo::Core::TDeviceQueue *queue = context->GetDeviceQueue();

        Turbo::Core::TCommandBuffer *command_buffer = resource_allocator->AllocateCommandBuffer();
        command_buffer->Begin();
        command_buffer->CmdCopyBuffer(src->buffer, this->buffer, srcOffset, 0, copy_size);
        command_buffer->End();

        Turbo::Core::TFence *copy_fence = new Turbo::Core::TFence(context->GetDevice());
        queue->Submit(nullptr, nullptr, command_buffer, copy_fence);
        copy_fence->WaitUntil();

        delete copy_fence;
        resource_allocator->FreeCommandBuffer(command_buffer);
    }
}

bool Turbo::Render::TBuffer::IsValid() const
{
    if (this->buffer != nullptr && this->buffer->GetVkBuffer() != VK_NULL_HANDLE)
    {
        return true;
    }

    return false;
}

Turbo::Render::TVertexBuffer::TAttribute::TAttribute(Turbo::Render::TFormat format, uint32_t offset)
{
    this->format = format;
    this->offset = offset;
}

Turbo::Render::TFormat Turbo::Render::TVertexBuffer::TAttribute::GetFormat()
{
    return this->format;
}

uint32_t Turbo::Render::TVertexBuffer::TAttribute::GetOffset()
{
    return this->offset;
}

bool Turbo::Render::TVertexBuffer::TAttribute::IsValid() const
{
    if (this->format != Turbo::Render::TFormat::UNDEFINED && this->offset != std::numeric_limits<uint32_t>::max())
    {
        return true;
    }
    return false;
}

void Turbo::Render::TVertexBuffer::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    Turbo::Render::TBuffer::Descriptor buffer_descriptor = {};
    buffer_descriptor.usages = Turbo::Render::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Render::TBufferUsageBits::BUFFER_TRANSFER_SRC | Turbo::Render::TBufferUsageBits::BUFFER_TRANSFER_DST;
    buffer_descriptor.size = descriptor.size;
    buffer_descriptor.domain = descriptor.domain;

    this->stride = descriptor.stride;
    this->rate = descriptor.rate;

    Turbo::Render::TBuffer::Create(name, buffer_descriptor, allocator);
}

Turbo::Render::TAttributeID Turbo::Render::TVertexBuffer::AddAttribute(Turbo::Render::TFormat format, uint32_t offset)
{
    this->attributes.push_back({format, offset});
    return this->attributes.size() - 1;
}

Turbo::Render::TVertexBuffer::TAttribute Turbo::Render::TVertexBuffer::GetAttribute(TAttributeID id) const
{
    Turbo::Render::TVertexBuffer::TAttribute result{};
    if (id > (this->attributes.size() - 1))
    {
        return result;
    }

    return this->attributes[id];
}

const std::vector<Turbo::Render::TVertexBuffer::TAttribute> &Turbo::Render::TVertexBuffer::GetAttributes()
{
    return this->attributes;
}

uint32_t Turbo::Render::TVertexBuffer::GetStride() const
{
    return this->stride;
}

Turbo::Render::TVertexBuffer::TRate Turbo::Render::TVertexBuffer::GetRate() const
{
    return this->rate;
}
