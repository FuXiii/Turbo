#pragma once
#ifndef TURBO_RENDER_TCONTEXT_H
#define TURBO_RENDER_TCONTEXT_H
#include "TBuffer.h"
#include "TImage.h"
#include "TSampler.h"
#include <map>
#include <stdint.h>
#include <vector>

namespace Turbo
{
namespace Core
{
class TInstance;
class TPhysicalDevice;
class TDevice;
class TDeviceQueue;
class TImage;
class TImageView;
class TSampler;
class TBuffer;
class TCommandBufferPool;
class TCommandBuffer;
class TFence;
class TRenderPass;
class TFramebuffer;
class TVertexBinding;
} // namespace Core
} // namespace Turbo

namespace Turbo
{
namespace FrameGraph
{
class TRenderPass;
} // namespace FrameGraph
} // namespace Turbo

namespace Turbo
{
namespace Render
{

class TRenderPass;
class TComputePipeline;
class TGraphicsPipeline;
class TContext;

class TFramebufferPool
{
  private:
    TContext *context = nullptr;

  private:
    std::vector<Turbo::Core::TFramebuffer *> framebuffers;

  private:
    void CreateFramebuffer(Turbo::Render::TRenderPass &renderPass);
    bool Find(Turbo::Render::TRenderPass &renderPass);

  public:
    TFramebufferPool(TContext *context);
    ~TFramebufferPool();

    bool Allocate(Turbo::Render::TRenderPass &renderPass);
    void Free(Turbo::Render::TRenderPass &renderPass);
};

class TRenderPassPool
{
  private:
    TContext *context = nullptr;
    TFramebufferPool *framebufferPool = nullptr;

  private:
    std::vector<Turbo::Core::TRenderPass *> renderPasses;

  private:
    void CreateRenderPass(Turbo::Render::TRenderPass &renderPass);
    // void DestroyRenderPass(Turbo::Render::TRenderPass &renderPass);

    bool Find(Turbo::Render::TRenderPass &renderPass);

  public:
    TRenderPassPool(TContext *context);
    ~TRenderPassPool();

    bool Allocate(Turbo::Render::TRenderPass &renderPass);
    void Free(Turbo::Render::TRenderPass &renderPass);
};

typedef struct TCommandBuffer
{
    Turbo::Core::TCommandBuffer *commandBuffer = nullptr;
    Turbo::Core::TFence *fence = nullptr;
} TCommandBuffer;

class TContext
{
  private:
    Turbo::Core::TInstance *instance = nullptr;
    Turbo::Core::TPhysicalDevice *physicalDevice = nullptr;
    Turbo::Core::TDevice *device = nullptr;
    Turbo::Core::TDeviceQueue *graphicsQueue = nullptr;
    Turbo::Core::TCommandBufferPool *commandBufferPool = nullptr;

    Turbo::Render::TCommandBuffer currentCommandBuffer;
    std::vector<Turbo::Render::TCommandBuffer> commandBuffers;

    // for BeginRenderPass(...)
    TRenderPassPool *renderPassPool = nullptr;

    // for BindVeretxAttribute(...)
    std::vector<Turbo::Core::TVertexBinding *> vertexBindings;
    std::vector<Turbo::Core::TBuffer *> vertexBuffers;

    // for BindDescriptor(...)
    using TSetID = uint32_t;
    using TBindingID = uint32_t;
    typedef enum class TDescriptorMapType
    {
        UNDEFINED,
        UNIFROM_BUFFER_MAP,
        COMBINED_IMAGE_SAMPLER_MAP,
        SAMPLED_IMAGE_MAP,
        SAMPLER_MAP,
    } TDescriptorMapType;

    typedef struct TDescriptorID
    {
        TSetID set = std::numeric_limits<uint32_t>::max();
        TBindingID binding = std::numeric_limits<uint32_t>::max();

        /*
          struct TDescriptorIDCmp
          {
                bool operator()(const TDescriptorID &lhs, const TDescriptorID &rhs) const
              {
                    if (lhs.set != rhs.set)
                  {
                        return lhs.set < rhs.set;
                  }

          if (lhs.binding != rhs.binding)
                  {
                        return lhs.binding < rhs.binding;
                  }
                  return false;
              }
          };
        */

        bool operator()(const TDescriptorID &lhs, const TDescriptorID &rhs) const
        {
            if (lhs.set != rhs.set)
            {
                return lhs.set < rhs.set;
            }

            if (lhs.binding != rhs.binding)
            {
                return lhs.binding < rhs.binding;
            }

            return false;
        }
    } TDescriptorID;

    std::map<TDescriptorID, std::vector<Turbo::Core::TBuffer *>, TDescriptorID> uniformBufferMap;
    std::map<TDescriptorID, std::vector<std::pair<Turbo::Core::TImageView *, Turbo::Core::TSampler *>>, TDescriptorID> combinedImageSamplerMap;
    std::map<TDescriptorID, std::vector<Turbo::Core::TImageView *>, TDescriptorID> sampledImageMap;
    std::map<TDescriptorID, std::vector<Turbo::Core::TSampler *>, TDescriptorID> samplerMap;

    std::map<TSetID, std::map<TBindingID, TDescriptorMapType>> descriptorMap;

  public:
    TContext();
    ~TContext();

    Turbo::Core::TImage *CreateImage(const Turbo::Render::TImage::Descriptor &descriptor);
    void DestroyImage(Turbo::Core::TImage *image);
    // Turbo::Core::TImage *CreateCubeImage(uint32_t width, uint32_t height,uint32_t depth,uint32_t layer/*, flags,TImage*/);
    Turbo::Core::TBuffer *CreateBuffer(const Turbo::Render::TBuffer::Descriptor &descriptor);
    void DestroyBuffer(Turbo::Core::TBuffer *buffer);

    Turbo::Core::TCommandBuffer *AllocateCommandBuffer();
    void FreeCommandBuffer(Turbo::Core::TCommandBuffer *commandBuffer);

    Turbo::Core::TSampler *CreateSampler(const Turbo::Render::TSampler::Descriptor &descriptor);
    void DestroySampler(Turbo::Core::TSampler *sampler);

    void ClearTexture(Turbo::Render::TTexture2D &texture2D, float r = 0, float g = 0, float b = 0, float a = 0);

    /*TODO: will delete*/ [[deprecated]] void BeginRenderPass(const Turbo::FrameGraph::TRenderPass &renderPass);
    bool BeginRenderPass(Turbo::Render::TRenderPass &renderPass);

    void BindVeretxAttribute(const Turbo::Render::TVertexBuffer &vertexBuffer, Turbo::Render::TAttributeID attributeID, uint32_t location);

    void BindPipeline(const Turbo::Render::TComputePipeline &computePipeline);
    void BindPipeline(const Turbo::Render::TGraphicsPipeline &graphicsPipeline);

    void BindDescriptor(TSetID set, TBindingID binding, const std::vector<Turbo::Render::TTexture2D> &texture2Ds);
    void BindDescriptor(TSetID set, TBindingID binding, const Turbo::Render::TTexture2D &texture2D);
    void BindDescriptor(TSetID set, TBindingID binding, const std::vector<Turbo::Render::TTexture3D> &texture3Ds);
    void BindDescriptor(TSetID set, TBindingID binding, const Turbo::Render::TTexture3D &texture3D);
    template <typename T>
    void BindDescriptor(TSetID set, TBindingID binding, const std::vector<Turbo::Render::TUniformBuffer<T>> &uniformBuffers)
    {
        auto set_map = this->descriptorMap.find(set);
        if (set_map != this->descriptorMap.end())
        {
            auto binding_map = set_map->second.find(binding);
            if (binding_map != set_map->second.end())
            {
                // TODO:更新Set和Binding对应的缓存
                // 说明之前已有描述符资源绑定到此，需要在之前的【描述符资源数组】中移除并更新到目标【描述符资源数组】中
                TDescriptorMapType descriptor_map_type = binding_map->second;
                switch (descriptor_map_type)
                {
                case TDescriptorMapType::UNDEFINED: {
                }
                break;
                case TDescriptorMapType::UNIFROM_BUFFER_MAP: {
                    this->uniformBufferMap.at({set, binding}).clear();
                }
                break;
                case TDescriptorMapType::COMBINED_IMAGE_SAMPLER_MAP: {
                    this->combinedImageSamplerMap.at({set, binding}).clear();
                }
                break;
                case TDescriptorMapType::SAMPLED_IMAGE_MAP: {
                    this->sampledImageMap.at({set, binding}).clear();
                }
                break;
                case TDescriptorMapType::SAMPLER_MAP: {
                    this->samplerMap.at({set, binding}).clear();
                }
                break;
                }

                // TODO:更新到目标【描述符资源数组】中
                std::vector<Turbo::Core::TBuffer *> &uniform_buffers = this->uniformBufferMap.at({set, binding});
                for (const Turbo::Render::TUniformBuffer<T> &uniform_buffer_item : uniformBuffers)
                {
                    uniform_buffers.push_back(uniform_buffer_item.buffer);
                }

                // 更新绑定类型
                binding_map->second = TDescriptorMapType::UNIFROM_BUFFER_MAP;
            }
            else
            {
                // TODO:增加新的Binding（BindingMap增加新项目）。并将std::vector<各种uinform资源类型>存入相应缓存
                // 说明找到了Set，但没有Binding
                std::vector<Turbo::Core::TBuffer *> &uniform_buffers = this->uniformBufferMap[{set, binding}];
                for (const Turbo::Render::TUniformBuffer<T> &uniform_buffer_item : uniformBuffers)
                {
                    uniform_buffers.push_back(uniform_buffer_item.buffer);
                }

                set_map->second[binding] = TDescriptorMapType::UNIFROM_BUFFER_MAP;
            }
        }
        else
        {
            // TODO:增加新的Set，Binding映射（SetMap中增加新项）。并将std::vector<各种uinform资源类型>存入相应缓存
            std::vector<Turbo::Core::TBuffer *> &uniform_buffers = this->uniformBufferMap[{set, binding}];
            for (const Turbo::Render::TUniformBuffer<T> &uniform_buffer_item : uniformBuffers)
            {
                uniform_buffers.push_back(uniform_buffer_item.buffer);
            }
            this->descriptorMap[set][binding] = TDescriptorMapType::UNIFROM_BUFFER_MAP;
        }
    }
    template <typename T>
    void BindDescriptor(TSetID set, TBindingID binding, const Turbo::Render::TUniformBuffer<T> &uniformBuffer)
    {
        std::vector<Turbo::Render::TUniformBuffer<T>> uniform_buffers;
        uniform_buffers.push_back(uniformBuffer);
        this->BindDescriptor(set, binding, uniform_buffers);
    }

    void EndRenderPass();

    void Flush();
    bool Wait(uint64_t timeout);

    Turbo::Core::TInstance *GetInstance();
    Turbo::Core::TPhysicalDevice *GetPhysicalDevice();
    Turbo::Core::TDevice *GetDevice();
    Turbo::Core::TDeviceQueue *GetDeviceQueue();
    Turbo::Render::TCommandBuffer GetCommandBuffer();

    /*FIXME:Just For Test*/ [[deprecated]] Turbo::Core::TImage *GetTextureImage(Turbo::Render::TTexture2D texture2d);
};
} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TCONTEXT_H