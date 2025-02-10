#include "Camera.h"
#include <imgui.h>

glm::quat CalAttitude(float vertical, float horizontal)
{
    return glm::quat(glm::radians(glm::vec3(-vertical, -horizontal, 0)));
}

Camera::Camera(Turbo::Core::TDevice *device)
{
    if (Turbo::Core::TReferenced::Valid(device))
    {
        this->viewProjectionBuffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(ViewProjectionMatrixs));
    }
    else
    {
        throw std::runtime_error("[Error] Camera::Camera(...) device invalid!");
    }
}

void Camera::Update()
{
    if (ImGui::GetCurrentContext() != nullptr)
    {
        ImGuiIO &io = ImGui::GetIO();
        auto mouse_delta = io.MouseDelta;
        if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
        {
            this->horizontal -= mouse_delta.x * 0.1f;
            this->vertical -= mouse_delta.y * 0.1f;
        }

        auto attitude = CalAttitude(this->vertical, this->horizontal);
        auto forward = attitude * glm::vec3(0, 0, 1) * 0.1f;
        auto right = attitude * glm::vec3(1, 0, 0) * 0.1f;

        if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_W))
        {
            this->position += forward * this->speed;
        }
        if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_A))
        {
            this->position -= right * this->speed;
        }
        if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_S))
        {
            this->position -= forward * this->speed;
        }
        if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_D))
        {
            this->position += right * this->speed;
        }

        this->position += forward * io.MouseWheel;

        // Update view projection buffer
        {
            auto cal_look_at = [this]() -> glm::mat4 {
                auto attitude = CalAttitude(this->vertical, this->horizontal);
                auto forward = attitude * glm::vec3(0, 0, 1);

                auto eye = this->position;
                auto center = this->position + forward;
                auto up = attitude * glm::vec3(0, -1, 0);

                return glm::lookAt(eye, center, up);
            };

            this->viewProjectionMatrixs.view = cal_look_at();
            this->viewProjectionMatrixs.projection = glm::perspective(glm::radians(45.0f), io.DisplaySize.x / io.DisplaySize.y, 0.1f, 10000.0f);
            {
                void *projection_view_matrix_ptr = this->viewProjectionBuffer->Map();
                memcpy(projection_view_matrix_ptr, &(this->viewProjectionMatrixs), sizeof(matrixs));
                this->viewProjectionBuffer->Unmap();
            }
        }
    }
}

void Camera::SetSpeed(float speed)
{
    this->speed = speed;
}

std::string Camera::ToString() const
{
    return "Camera";
}

Turbo::Core::TBuffer *Camera::GetViewProjectionBuffer()
{
    return this->viewProjectionBuffer;
}
