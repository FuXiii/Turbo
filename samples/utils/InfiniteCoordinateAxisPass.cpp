#include "InfiniteCoordinateAxisPass.h"
#include <TCommandBuffer.h>
#include <ReadFile.h>

InfiniteCoordinateAxisPass::InfiniteCoordinateAxisPass()
{
}

InfiniteCoordinateAxisPass::~InfiniteCoordinateAxisPass()
{
}

void InfiniteCoordinateAxisPass::Init(Turbo::Core::TCommandBuffer *commandBuffer, Camera *camera)
{
    if (Turbo::Core::TReferenced::Valid(commandBuffer) && Turbo::Core::TReferenced::Valid(camera))
    {
        auto current_render_pass = commandBuffer->GetCurrentRenderPass();
        if (Turbo::Core::TReferenced::Valid(current_render_pass))
        {
            auto command_pool = commandBuffer->GetCommandBufferPool();
            auto queue = command_pool->GetDeviceQueue();

            {
                this->device = queue->GetDevice();
            }

            Turbo::Core::TRefPtr<Turbo::Core::TVertexShader> vertex_shader = new Turbo::Core::TVertexShader(this->device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(std::string(TURBO_ASSET_ROOT) + "/shaders/InfiniteCoordinateAxis.vert"));
            Turbo::Core::TRefPtr<Turbo::Core::TFragmentShader> fragment_shader = new Turbo::Core::TFragmentShader(this->device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(std::string(TURBO_ASSET_ROOT) + "/shaders/InfiniteCoordinateAxis.frag"));

            Turbo::Core::TPipelineLayout::TLayout layout;
            layout << *vertex_shader << *fragment_shader;

            Turbo::Core::TVertexShaderStage *vertex_shader_stage = new Turbo::Core::TVertexShaderStage(vertex_shader);
            Turbo::Core::TFragmentShaderStage *fragment_shader_stage = new Turbo::Core::TFragmentShaderStage(fragment_shader);

            this->pipeline = new Turbo::Core::TGraphicsPipeline(layout, current_render_pass, 0, {}, vertex_shader_stage, fragment_shader_stage, Turbo::Core::TTopologyType::LINE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_NONE, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, false, false, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, 0, 0, false, Turbo::Core::TLogicOp::NO_OP, true, Turbo::Core::TBlendFactor::SRC_ALPHA, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendOp::ADD, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendFactor::ZERO, Turbo::Core::TBlendOp::ADD);

            {
                std::vector<Turbo::Core::TDescriptorSize> descriptor_sizes;
                descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptor::TType::UNIFORM_BUFFER, 1));
                this->descriptorPool = new Turbo::Core::TDescriptorPool(device, descriptor_sizes.size() * 1000, descriptor_sizes);
            }

            this->pipelineDescriptorSet = this->descriptorPool->Allocate(this->pipeline->GetPipelineLayout());
            this->pipelineDescriptorSet->BindData(0, 0, camera->GetViewProjectionBuffer());
        }
    }
}

void InfiniteCoordinateAxisPass::Draw(Turbo::Core::TCommandBuffer *commandBuffer, Camera *camera)
{
    if (!this->pipeline.Valid())
    {
        this->Init(commandBuffer, camera);
    }

    if (this->pipeline->Valid() && Turbo::Core::TReferenced::Valid(commandBuffer) && Turbo::Core::TReferenced::Valid(camera))
    {
        commandBuffer->CmdBindPipeline(this->pipeline);
        commandBuffer->CmdBindPipelineDescriptorSet(this->pipelineDescriptorSet);
        commandBuffer->CmdSetLineWidth(1);
        commandBuffer->CmdDraw(2, 6, 0, 0);
    }
}

std::string InfiniteCoordinateAxisPass::ToString() const
{
    return "InfiniteCoordinateAxisPass";
}