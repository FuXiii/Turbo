#ifndef TURBO_SAMPLES_UTILS_CAMERA_H
#define TURBO_SAMPLES_UTILS_CAMERA_H
#include <glm/ext.hpp>
#include <TDevice.h>
#include <TBuffer.h>

class Camera : public Turbo::Core::TObject
{
  public:
    typedef struct ViewProjectionMatrixs
    {
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 viewInverse;
        glm::mat4 projectionInverse;
    } matrixs;

  private:
    glm::vec3 position = glm::vec3(0, 0, -1);

    float horizontal = 0; // radian
    float vertical = 0;   // radian

    float speed = 1;

    ViewProjectionMatrixs viewProjectionMatrixs;
    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> viewProjectionBuffer;

  protected:
    ~Camera() = default;

  public:
    Camera(Turbo::Core::TDevice *device);

    void Update();
    void SetSpeed(float speed);

    Turbo::Core::TBuffer *GetViewProjectionBuffer();

  public:
    std::string ToString() const override;
};

#endif