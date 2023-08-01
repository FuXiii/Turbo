#include <GLFW/glfw3.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgpu.h>
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
    wgpu::SwapChainDescriptor scDesc{.usage = wgpu::TextureUsage::RenderAttachment, .format = wgpu::TextureFormat::BGRA8Unorm, .width = kWidth, .height = kHeight, .presentMode = wgpu::PresentMode::Fifo};
    SWAP_CHAIN = DEVICE.CreateSwapChain(surface, &scDesc);
}

void CreateRenderPipeline()
{
    wgpu::ShaderModuleWGSLDescriptor wgslDesc{};
    wgslDesc.code = shaderCode;

    wgpu::ShaderModuleDescriptor shaderModuleDescriptor{.nextInChain = &wgslDesc};
    wgpu::ShaderModule shaderModule = DEVICE.CreateShaderModule(&shaderModuleDescriptor);

    wgpu::ColorTargetState colorTargetState{.format = wgpu::TextureFormat::BGRA8Unorm};

    wgpu::FragmentState fragmentState{.module = shaderModule, .entryPoint = "fragmentMain", .targetCount = 1, .targets = &colorTargetState};

    wgpu::RenderPipelineDescriptor descriptor{.vertex = {.module = shaderModule, .entryPoint = "vertexMain"}, .fragment = &fragmentState};
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
    wgpu::RenderPassColorAttachment attachment{.view = SWAP_CHAIN.GetCurrentTextureView(), .loadOp = wgpu::LoadOp::Clear, .storeOp = wgpu::StoreOp::Store, .clearValue = clear_color};

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

#include <future>
#include <thread>

int Test()
{
    WGPUInstanceDescriptor wgpu_instance_descriptor = {};
    wgpu_instance_descriptor.nextInChain = nullptr;
    WGPUInstance wgpu_instance = wgpuCreateInstance(&wgpu_instance_descriptor);

    if (wgpu_instance != nullptr)
    {
        std::cout << "WGPUInstance create success" << std::endl;
    }
    else
    {
        std::cout << "WGPUInstance create failed" << std::endl;
        return -1;
    }

    WGPUAdapter wgpu_adapter = nullptr;

    //std::future<WGPUAdapter> f2 = std::async(std::launch::async, [] { WGPUAdapter return_wgpu_adapter = nullptr });

    WGPURequestAdapterCallback wgpu_request_adapter_callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const *message, void *userdata) {
        switch (status)
        {
        case WGPURequestAdapterStatus::WGPURequestAdapterStatus_Success: {
            WGPUAdapter *temp_wgpu_adapter = reinterpret_cast<WGPUAdapter *>(userdata);
            *temp_wgpu_adapter = adapter;
            std::cout << "WGPURequestAdapterCallback Success" << std::endl;

            if (adapter != nullptr)
            {
                std::cout << "adapter Success" << std::endl;
            }
            else
            {
                std::cout << "adapter nullptr" << std::endl;
            }
        }
        break;
        case WGPURequestAdapterStatus::WGPURequestAdapterStatus_Unavailable:
        case WGPURequestAdapterStatus::WGPURequestAdapterStatus_Error:
        case WGPURequestAdapterStatus::WGPURequestAdapterStatus_Unknown: {
            throw std::runtime_error(std::string("Unsupport WebGPU"));
        }
        break;
        }
    };

    WGPURequestAdapterOptions wgpu_request_adapter_options = {};
    wgpu_request_adapter_options.nextInChain = nullptr;
    wgpu_request_adapter_options.compatibleSurface = nullptr;
    wgpu_request_adapter_options.powerPreference = WGPUPowerPreference::WGPUPowerPreference_LowPower;
    wgpu_request_adapter_options.backendType = WGPUBackendType::WGPUBackendType_WebGPU;
    wgpu_request_adapter_options.forceFallbackAdapter = false;
    wgpu_request_adapter_options.compatibilityMode = false;

    wgpuInstanceRequestAdapter(wgpu_instance, &wgpu_request_adapter_options, wgpu_request_adapter_callback, (void *)&wgpu_adapter);

    // while (wgpu_adapter == nullptr)
    // {
    //     std::cout << "waite for wgpu_adapter" << std::endl;
    // }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    if (wgpu_adapter != nullptr)
    {
        std::cout << "wgpu_adapter Success" << std::endl;
    }
    else
    {
        std::cout << "wgpu_adapter nullptr" << std::endl;
    }
    wgpuInstanceRelease(wgpu_instance);
    return 0;
}

int main()
{
    Test();

    INSTANCE = wgpu::CreateInstance();

    GetDevice([](wgpu::Device device) {
        DEVICE = device;
        Start();
    });

    return 0;
}