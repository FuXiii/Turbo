#pragma once
#ifndef TURBO_RENDER_TCONTEXT_H
#define TURBO_RENDER_TCONTEXT_H
#include "TBuffer.h"
#include "TImage.h"
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
class TBuffer;
class TCommandBufferPool;
class TCommandBuffer;
class TFence;
class TRenderPass;
class TFramebuffer;
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

    TRenderPassPool *renderPassPool = nullptr;

  public:
    TContext();
    ~TContext();

    Turbo::Core::TImage *CreateImage(const TImage::Descriptor &descriptor);
    void DestroyImage(Turbo::Core::TImage *image);
    // Turbo::Core::TImage *CreateCubeImage(uint32_t width, uint32_t height,uint32_t depth,uint32_t layer/*, flags,TImage*/);
    Turbo::Core::TBuffer *CreateBuffer(const TBuffer::Descriptor &descriptor);
    void DestroyBuffer(Turbo::Core::TBuffer *buffer);

    Turbo::Core::TCommandBuffer *AllocateCommandBuffer();
    void FreeCommandBuffer(Turbo::Core::TCommandBuffer *commandBuffer);

    void ClearTexture(Turbo::Render::TTexture2D &texture2D, float r = 0, float g = 0, float b = 0, float a = 0);

    /*TODO: will delete*/ [[deprecated]] void BeginRenderPass(const Turbo::FrameGraph::TRenderPass &renderPass);
    bool BeginRenderPass(Turbo::Render::TRenderPass &renderPass);
    void EndRenderPass();

    void BindPipeline(const Turbo::Render::TComputePipeline &computePipeline);
    void BindPipeline(const Turbo::Render::TGraphicsPipeline &graphicsPipeline);

    void Flush();
    bool Wait(uint64_t timeout);

    Turbo::Core::TInstance *GetInstance();
    Turbo::Core::TPhysicalDevice *GetPhysicalDevice();
    Turbo::Core::TDevice *GetDevice();
    Turbo::Core::TDeviceQueue *GetDeviceQueue();
    Turbo::Render::TCommandBuffer GetCommandBuffer();

    /*Just For Test*/ Turbo::Core::TImage *GetTextureImage(Turbo::Render::TTexture2D texture2d);
};
} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TCONTEXT_H