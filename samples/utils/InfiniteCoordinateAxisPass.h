#ifndef TURBO_SAMPLES_UTILS_INFINITECOORDINATEAXISPASS_H
#define TURBO_SAMPLES_UTILS_INFINITECOORDINATEAXISPASS_H
#include <TDevice.h>
#include <TRenderPass.h>
#include <TImageView.h>
#include <TSampler.h>
#include <TGraphicsPipeline.h>
#include <TDescriptorPool.h>
#include <TPipelineDescriptorSet.h>

#include <Camera.h>

class InfiniteCoordinateAxisPass : public Turbo::Core::TObject
{
  private:
    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device;
    Turbo::Core::TRefPtr<Turbo::Core::TDescriptorPool> descriptorPool;
    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> pipeline;
    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> pipelineDescriptorSet;

  private:
    void Init(Turbo::Core::TCommandBuffer *commandBuffer, Camera *camera);

  protected:
    ~InfiniteCoordinateAxisPass();

  public:
    InfiniteCoordinateAxisPass();

    void Draw(Turbo::Core::TCommandBuffer *commandBuffer, Camera *camera);

  public:
    std::string ToString() const override;
};

#endif