#include <GLFW/glfw3.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgpu.h>
#include <iostream>

#include <webgpu/webgpu.h>

#include <vector>

struct WGPUContext
{
    WGPUInstance instance = nullptr;
    WGPUAdapter adapter = nullptr;
    WGPUDevice device = nullptr;
    WGPUSurface surface = nullptr;
    WGPUSwapChain swapchain = nullptr;
};

EM_BOOL EMSCRIPTEN_REQUEST_ANIMATION_FRAME_LOOP_CALLBACK(double time, void *userData)
{
    WGPUContext *wgpu_context = static_cast<WGPUContext *>(userData);

    std::cout << time / 1000 << std::endl;

    WGPUTextureView current_texture_view = wgpuSwapChainGetCurrentTextureView(wgpu_context->swapchain);
    if (!current_texture_view)
    {
        std::cerr << "Cannot acquire next swap chain texture" << std::endl;

        return EM_FALSE;
    }

    // wgpuSwapChainPresent(wgpu_context->swapchain);

    return EM_TRUE;
}

void Test()
{
    WGPUContext *context = new WGPUContext();

    WGPUInstanceDescriptor wgpu_instance_descriptor = {};
    wgpu_instance_descriptor.nextInChain = nullptr;
    WGPUInstance wgpu_instance = wgpuCreateInstance(&wgpu_instance_descriptor);

    context->instance = wgpu_instance;

    WGPURequestAdapterCallback wgpu_request_adapter_callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const *message, void *userdata) {
        switch (status)
        {
        case WGPURequestAdapterStatus::WGPURequestAdapterStatus_Success: {

            std::cout << "Begin::Set wgpu_adapter" << std::endl;
            WGPUContext *wgpu_context = static_cast<WGPUContext *>(userdata);
            wgpu_context->adapter = adapter;
            std::cout << "End::Set wgpu_adapter" << std::endl;

            {
                size_t feature_count = wgpuAdapterEnumerateFeatures(adapter, nullptr);
                std::vector<WGPUFeatureName> features(feature_count);
                wgpuAdapterEnumerateFeatures(adapter, features.data());

                for (WGPUFeatureName &featre_name_item : features)
                {
                    std::cout << "WGPUFeatureName:" << featre_name_item << std::endl;
                }
            }

            WGPURequestDeviceCallback wgpu_request_device_callback = [](WGPURequestDeviceStatus status, WGPUDevice device, char const *message, void *userdata) {
                switch (status)
                {
                case WGPURequestDeviceStatus::WGPURequestDeviceStatus_Success: {

                    WGPUContext *wgpu_context = static_cast<WGPUContext *>(userdata);
                    wgpu_context->device = device;

                    WGPUDevice wgpu_device = device;
                    WGPUErrorCallback wgpu_error_callback = [](WGPUErrorType type, char const *message, void *userdata) {
                        std::cout << "Error:" << type;
                        if (message)
                        {
                            std::cout << ":" << message;
                        }
                        std::cout << std::endl;
                    };

                    wgpuDeviceSetUncapturedErrorCallback(wgpu_device, wgpu_error_callback, nullptr);

                    WGPUQueue wgpu_queue = wgpuDeviceGetQueue(wgpu_device);

                    WGPUQueueWorkDoneCallback wgpu_queue_work_done_callback = [](WGPUQueueWorkDoneStatus status, void *userdata) { std::cout << "Queued work finished with status: " << status << std::endl; };
                    wgpuQueueOnSubmittedWorkDone(wgpu_queue, 0, wgpu_queue_work_done_callback, nullptr);

                    WGPUSurfaceDescriptorFromCanvasHTMLSelector wgpu_surface_descriptor_from_canvas_html_selector = {};
                    wgpu_surface_descriptor_from_canvas_html_selector.chain.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector;
                    wgpu_surface_descriptor_from_canvas_html_selector.chain.next = nullptr;
                    wgpu_surface_descriptor_from_canvas_html_selector.selector = "canvas";

                    WGPUSurfaceDescriptor wgpu_surface_descriptor = {};
                    wgpu_surface_descriptor.nextInChain = reinterpret_cast<WGPUChainedStruct *>(&wgpu_surface_descriptor_from_canvas_html_selector);
                    wgpu_surface_descriptor.label = nullptr;

                    WGPUSurface wgpu_surface = wgpuInstanceCreateSurface(wgpu_context->instance, &wgpu_surface_descriptor);

                    wgpu_context->surface = wgpu_surface;

                    WGPUTextureFormat surface_preferred_format = wgpuSurfaceGetPreferredFormat(wgpu_surface, wgpu_context->adapter);
                    std::cout << "surface_preferred_format:" << surface_preferred_format << std::endl;

                    WGPUSwapChainDescriptor wgpu_swap_chain_descriptor = {};
                    wgpu_swap_chain_descriptor.nextInChain = nullptr;
                    wgpu_swap_chain_descriptor.label = nullptr;
                    wgpu_swap_chain_descriptor.usage = WGPUTextureUsage::WGPUTextureUsage_RenderAttachment;
                    wgpu_swap_chain_descriptor.format = surface_preferred_format; // WGPUTextureFormat_BGRA8Unorm
                    wgpu_swap_chain_descriptor.width = 1920;
                    wgpu_swap_chain_descriptor.height = 1080;
                    wgpu_swap_chain_descriptor.presentMode = WGPUPresentMode::WGPUPresentMode_Fifo;

                    WGPUSwapChain wgpu_swap_chain = wgpuDeviceCreateSwapChain(device, wgpu_surface, &wgpu_swap_chain_descriptor);

                    wgpu_context->swapchain = wgpu_swap_chain;

                    emscripten_request_animation_frame_loop(EMSCRIPTEN_REQUEST_ANIMATION_FRAME_LOOP_CALLBACK, wgpu_context);

                    WGPUCommandEncoderDescriptor wgpu_command_encoder_descriptor = {};
                    wgpu_command_encoder_descriptor.nextInChain = nullptr;
                    wgpu_command_encoder_descriptor.label = "Command Encoder";
                    WGPUCommandEncoder wgpu_command_encoder = wgpuDeviceCreateCommandEncoder(wgpu_device, &wgpu_command_encoder_descriptor);

                    // wgpuCommandEncoderBeginRenderPass(WGPUCommandEncoder commandEncoder, const WGPURenderPassDescriptor *descriptor);

                    WGPUCommandBufferDescriptor wgpu_command_buffer_descriptor = {};
                    wgpu_command_buffer_descriptor.nextInChain = nullptr;
                    wgpu_command_buffer_descriptor.label = "Command Buffer";
                    WGPUCommandBuffer wgpu_command_buffer = wgpuCommandEncoderFinish(wgpu_command_encoder, &wgpu_command_buffer_descriptor);

                    wgpuQueueSubmit(wgpu_queue, 1, &wgpu_command_buffer);

                    // TODO: start emscripten loop callback
                }
                break;
                case WGPURequestDeviceStatus::WGPURequestDeviceStatus_Error:
                case WGPURequestDeviceStatus::WGPURequestDeviceStatus_Unknown:
                default: {
                    throw std::runtime_error(std::string("Could not get WebGPU device: ") + std::string(message));
                }
                break;
                }
            };

            WGPUDeviceDescriptor wgpu_device_descriptor = {};
            wgpuAdapterRequestDevice(adapter, &wgpu_device_descriptor, wgpu_request_device_callback, wgpu_context);
        }
        break;
        case WGPURequestAdapterStatus::WGPURequestAdapterStatus_Unavailable:
        case WGPURequestAdapterStatus::WGPURequestAdapterStatus_Error:
        case WGPURequestAdapterStatus::WGPURequestAdapterStatus_Unknown:
        default: {
            throw std::runtime_error(std::string("Unsupport WebGPU"));
        }
        break;
        }
    };

    WGPURequestAdapterOptions wgpu_request_adapter_options = {};
    wgpuInstanceRequestAdapter(wgpu_instance, &wgpu_request_adapter_options, wgpu_request_adapter_callback, context);

    if (context->adapter != nullptr)
    {
        std::cout << "wgpu_adapter not nullptr" << std::endl;
    }
    else
    {
        std::cout << "wgpu_adapter is nullptr" << std::endl;
    }
}

int main()
{
    Test();

    return 0;
}