#include "TBuffer.h"
#include "TResourceAllocator.h"
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
        if (memory_type_info.IsHostVisible())
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

                // TODO: 需要优化，这里需要Context中的CommandPool，而不是手动创建
                Turbo::Core::TCommandBufferPool *command_buffer_pool = new Turbo::Core::TCommandBufferPool(queue);
                Turbo::Core::TCommandBuffer *command_buffer = command_buffer_pool->Allocate();
                command_buffer->Begin();
                command_buffer->CmdCopyBuffer(temp_buffer.buffer, this->buffer, 0, 0, copy_size);
                command_buffer->End();

                Turbo::Core::TFence *copy_fence = new Turbo::Core::TFence(context->GetDevice());
                queue->Submit(nullptr, nullptr, command_buffer, copy_fence);
                copy_fence->WaitUntil();

                delete copy_fence;
                command_buffer_pool->Free(command_buffer);
                delete command_buffer_pool;
            }

            temp_buffer.Destroy(this->allocator);
        }
    }
}
