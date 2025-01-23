#ifndef TURBO_SAMPLES_UTILS_IMGUIPASS_H
#define TURBO_SAMPLES_UTILS_IMGUIPASS_H
#include <TDevice.h>
#include <TRenderPass.h>
#include <TImageView.h>
#include <TSampler.h>
#include <TGraphicsPipeline.h>
#include <TDescriptorPool.h>
#include <TFramebuffer.h>

#include <imgui.h>
#include <GLFW/glfw3.h>

class ImGuiPass : public Turbo::Core::TObject
{
  private:
    GLFWwindow *window;

    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device;
    Turbo::Core::TRefPtr<Turbo::Core::TDescriptorPool> descriptorPool;
    Turbo::Core::TRefPtr<Turbo::Core::TRenderPass> renderPass;

    Turbo::Core::TRefPtr<Turbo::Core::TImageView> font;
    Turbo::Core::TRefPtr<Turbo::Core::TSampler> sampler;
    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> pipeline;
    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> pipelineDescriptorSet;

    Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> framebuffer;

  private:
    void Init(Turbo::Core::TCommandBuffer *commandBuffer, Turbo::Core::TImageView *imageView);
    void DrawImGui(float deltaTime, Turbo::Core::TCommandBuffer *commandBuffer, Turbo::Core::TImageView *imageView);

  protected:
    ~ImGuiPass();

  public:
    ImGuiPass(GLFWwindow *window);

    template <typename OnGUI>
    void Draw(float deltaTime, Turbo::Core::TCommandBuffer *commandBuffer, Turbo::Core::TImageView *imageView, OnGUI &&onGUI)
    {
        if (!renderPass.Valid())
        {
            this->Init(commandBuffer, imageView);
        }

        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(imageView->GetImage()->GetWidth(), imageView->GetImage()->GetHeight());
        io.DeltaTime = deltaTime;

        ImGui::NewFrame();
        onGUI();
        ImGui::Render();

        // TODO: Draw ImGui
        this->DrawImGui(deltaTime, commandBuffer, imageView);
    }

  public:
    std::string ToString() const override;
};

#endif /* TURBO_SAMPLES_UTILS_IMGUIPASS_H */
