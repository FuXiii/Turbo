#include <GLFW/glfw3.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <iostream>

#include <webgpu/webgpu.h>
#include <webgpu/webgpu_cpp.h>

uint32_t kWidth = 512;
uint32_t kHeight = 512;

wgpu::Instance INSTANCE;
wgpu::Device DEVICE;
wgpu::SwapChain SWAP_CHAIN;
wgpu::RenderPipeline PIPELINE;

const char shaderCode[] = R"(
    @vertex fn vertexMain(@builtin(vertex_index) i : u32) ->
      @builtin(position) vec4f {
        const pos = array(vec2f(0, 1), vec2f(-1, -1), vec2f(1, -1));
        return vec4f(pos[i], 0, 1);
    }
    @fragment fn fragmentMain() -> @location(0) vec4f {
        return vec4f(1, 0, 0, 1);
    }
)";

void GetDevice(void (*callback)(wgpu::Device))
{
    INSTANCE.RequestAdapter(
        nullptr,
        [](WGPURequestAdapterStatus status, WGPUAdapter cAdapter, const char *message, void *userdata) {
            if (status != WGPURequestAdapterStatus_Success)
            {
                exit(0);
            }
            wgpu::Adapter adapter = wgpu::Adapter::Acquire(cAdapter);

            // {
            //     wgpu::AdapterProperties adapter_properties = {};
            //     adapter.GetProperties(&adapter_properties);

            //     std::cout << "vendorID::" << adapter_properties.vendorID << std::endl;
            //     std::cout << "vendorName::" << adapter_properties.vendorName << std::endl;
            //     std::cout << "architecture::" << adapter_properties.architecture << std::endl;
            //     std::cout << "deviceID::" << adapter_properties.deviceID << std::endl;
            //     std::cout << "name::" << adapter_properties.name << std::endl;
            //     std::cout << "driverDescription::" << adapter_properties.driverDescription << std::endl;

            //     switch (adapter_properties.adapterType)
            //     {
            //     case wgpu::AdapterType::DiscreteGPU: {
            //         std::cout << "adapterType::DiscreteGPU" << std::endl;
            //     }
            //     break;
            //     case wgpu::AdapterType::IntegratedGPU: {
            //         std::cout << "adapterType::IntegratedGPU" << std::endl;
            //     }
            //     break;
            //     case wgpu::AdapterType::CPU: {
            //         std::cout << "adapterType::CPU" << std::endl;
            //     }
            //     break;
            //     case wgpu::AdapterType::Unknown: {
            //         std::cout << "adapterType::Unknown" << std::endl;
            //     }
            //     break;
            //     }

            //     switch (adapter_properties.backendType)
            //     {
            //     case wgpu::BackendType::Undefined: {
            //         std::cout << "backendType::Undefined" << std::endl;
            //     }
            //     break;
            //     case wgpu::BackendType::Null: {
            //         std::cout << "backendType::Null" << std::endl;
            //     }
            //     break;
            //     case wgpu::BackendType::WebGPU: {
            //         std::cout << "backendType::WebGPU" << std::endl;
            //     }
            //     break;
            //     case wgpu::BackendType::D3D11: {
            //         std::cout << "backendType::D3D11" << std::endl;
            //     }
            //     break;
            //     case wgpu::BackendType::D3D12: {
            //         std::cout << "backendType::D3D12" << std::endl;
            //     }
            //     break;
            //     case wgpu::BackendType::Metal: {
            //         std::cout << "backendType::Metal" << std::endl;
            //     }
            //     break;
            //     case wgpu::BackendType::Vulkan: {
            //         std::cout << "backendType::Vulkan" << std::endl;
            //     }
            //     break;
            //     case wgpu::BackendType::OpenGL: {
            //         std::cout << "backendType::OpenGL" << std::endl;
            //     }
            //     break;
            //     case wgpu::BackendType::OpenGLES: {
            //         std::cout << "backendType::OpenGLES" << std::endl;
            //     }
            //     break;
            //     }
            //     std::cout << "compatibilityMode::" << adapter_properties.compatibilityMode << std::endl;
            // }

            adapter.RequestDevice(
                nullptr,
                [](WGPURequestDeviceStatus status, WGPUDevice cDevice, const char *message, void *userdata) {
                    wgpu::Device device = wgpu::Device::Acquire(cDevice);
                    reinterpret_cast<void (*)(wgpu::Device)>(userdata)(device);
                },
                userdata);
        },
        reinterpret_cast<void *>(callback));
}

void SetupSwapChain(wgpu::Surface surface)
{
    wgpu::SwapChainDescriptor scDesc{.usage = wgpu::TextureUsage::RenderAttachment,
                                     .format = wgpu::TextureFormat::BGRA8Unorm,
                                     .width = kWidth,
                                     .height = kHeight,
                                     .presentMode = wgpu::PresentMode::Fifo};
    SWAP_CHAIN = DEVICE.CreateSwapChain(surface, &scDesc);
}

void CreateRenderPipeline()
{
    wgpu::ShaderModuleWGSLDescriptor wgslDesc{};
    wgslDesc.code = shaderCode;

    wgpu::ShaderModuleDescriptor shaderModuleDescriptor{.nextInChain = &wgslDesc};
    wgpu::ShaderModule shaderModule = DEVICE.CreateShaderModule(&shaderModuleDescriptor);

    wgpu::ColorTargetState colorTargetState{.format = wgpu::TextureFormat::BGRA8Unorm};

    wgpu::FragmentState fragmentState{
        .module = shaderModule, .entryPoint = "fragmentMain", .targetCount = 1, .targets = &colorTargetState};

    wgpu::RenderPipelineDescriptor descriptor{.vertex = {.module = shaderModule, .entryPoint = "vertexMain"},
                                              .fragment = &fragmentState};
    PIPELINE = DEVICE.CreateRenderPipeline(&descriptor);
}

void InitGraphics(wgpu::Surface surface)
{
    SetupSwapChain(surface);
    CreateRenderPipeline();
}

void Render()
{
    double time = glfwGetTime();
    double coefficient = (sin(time) + 1) * 0.5;
    wgpu::Color clear_color = {.r = coefficient, .g = coefficient, .b = coefficient, .a = 1};
    wgpu::RenderPassColorAttachment attachment{.view = SWAP_CHAIN.GetCurrentTextureView(),
                                               .loadOp = wgpu::LoadOp::Clear,
                                               .storeOp = wgpu::StoreOp::Store,
                                               .clearValue = clear_color};

    wgpu::RenderPassDescriptor renderpass{.colorAttachmentCount = 1, .colorAttachments = &attachment};

    wgpu::CommandEncoder encoder = DEVICE.CreateCommandEncoder();
    wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&renderpass);
    pass.SetPipeline(PIPELINE);
    pass.Draw(3);
    pass.End();
    wgpu::CommandBuffer commands = encoder.Finish();
    DEVICE.GetQueue().Submit(1, &commands);
}

EM_BOOL EMSCRIPTEN_ANIMATION_FRAME(double time, void *userData)
{
    GLFWwindow *window = static_cast<GLFWwindow *>(userData);
    if (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        Render();
    }
    return EM_TRUE;
}

void Start()
{
    if (!glfwInit())
    {
        return;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(kWidth, kHeight, "WebGPU Test", nullptr, nullptr);

    wgpu::SurfaceDescriptorFromCanvasHTMLSelector canvas_desc{};
    canvas_desc.selector = "#canvas";

    wgpu::SurfaceDescriptor surface_desc{.nextInChain = &canvas_desc};
    wgpu::Surface surface = INSTANCE.CreateSurface(&surface_desc);

    InitGraphics(surface);

    emscripten_request_animation_frame_loop(EMSCRIPTEN_ANIMATION_FRAME, window);
}

int main()
{
    INSTANCE = wgpu::CreateInstance();
    GetDevice([](wgpu::Device device) {
        DEVICE = device;
        Start();
    });
}