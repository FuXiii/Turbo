#include <jni.h>

#include <android/log.h>
#include <game-activity/GameActivity.cpp>
#include <game-text-input/gametextinput.cpp>

extern "C"
{
#include <game-activity/native_app_glue/android_native_app_glue.c>
}

#include <dlfcn.h> //读取Vulkan动态库的头文件和Linux是一样的
#include <vulkan/vulkan.h>

#if defined(ANDROID) || defined(__ANDROID__)
#define TURBO_ANDROID_PLATFORM
#endif

#if defined(TURBO_ANDROID_PLATFORM)
#define ANDROID_IS_ANDROID_STR "Is ANDROID"
#else
#define ANDROID_IS_ANDROID_STR "Not ANDROID"
#endif

#ifndef VK_USE_PLATFORM_ANDROID_KHR
#include <vulkan/vulkan_android.h>
#endif
/*!
 * Handles commands sent to this Android application
 * @param pApp the app the commands are coming from
 * @param cmd the command to handle
 */
void handle_cmd(android_app *pApp, int32_t cmd)
{
    switch (cmd)
    {
    case APP_CMD_INIT_WINDOW: //该窗体被打开
    {
        LOGI("handle_cmd::APP_CMD_INIT_WINDOW");
        void *library = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
        if (library)
        {
            auto vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)dlsym(library, "vkGetInstanceProcAddr");
            if (vkGetInstanceProcAddr)
            {
                auto vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
                auto vkCreateInstance = (PFN_vkCreateInstance)vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkCreateInstance");

                if (vkEnumerateInstanceVersion)
                {
                    uint32_t api_version = 0;
                    VkResult enumerate_instance_version_result = vkEnumerateInstanceVersion(&api_version);
                    if (enumerate_instance_version_result == VkResult::VK_SUCCESS)
                    {
                        uint32_t major = VK_API_VERSION_MAJOR(api_version);
                        uint32_t minor = VK_API_VERSION_MINOR(api_version);
                        uint32_t patch = VK_API_VERSION_PATCH(api_version);

                        LOGI("Vulkan Version::%x.%x.%x", major, minor, patch);

                        uint32_t header_major = VK_API_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE);
                        uint32_t header_minor = VK_API_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE);
                        uint32_t header_patch = VK_API_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE);

                        LOGI("Vulkan Header Version::%x.%x.%x", header_major, header_minor, header_patch);
                    }
                }
                else
                {
                    LOGE("Can not load vkEnumerateInstanceVersion");
                }

                if (vkCreateInstance)
                {
                    VkApplicationInfo application_info = {};
                    application_info.sType = VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO;
                    application_info.pNext = nullptr;
                    application_info.pApplicationName = nullptr;
                    application_info.applicationVersion = 0;
                    application_info.pEngineName = nullptr;
                    application_info.engineVersion = 0;
                    application_info.apiVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);

                    VkInstanceCreateInfo instance_create_info = {};
                    instance_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                    instance_create_info.pNext = nullptr;
                    instance_create_info.flags = 0;
                    instance_create_info.pApplicationInfo = &application_info;
                    instance_create_info.enabledLayerCount = 0;
                    instance_create_info.ppEnabledLayerNames = nullptr;
                    instance_create_info.enabledExtensionCount = 0;
                    instance_create_info.ppEnabledExtensionNames = nullptr;

                    VkInstance vk_instance = VK_NULL_HANDLE;
                    VkResult instance_create_result = vkCreateInstance(&instance_create_info, nullptr, &vk_instance);
                    if (instance_create_result == VkResult::VK_SUCCESS)
                    {

                        LOGI("Create VkInstance successed");

                        auto vkDestroyInstance = (PFN_vkDestroyInstance)vkGetInstanceProcAddr(vk_instance, "vkDestroyInstance");

                        if (vkDestroyInstance)
                        {
                            vkDestroyInstance(vk_instance, nullptr);
                        }
                        else
                        {
                            LOGE("Can not load vkDestroyInstance");
                        }

                        VkAndroidSurfaceCreateInfoKHR adsa;
                    }
                }
                else
                {
                    LOGE("Can not load vkCreateInstance");
                }
            }
            else
            {
                LOGE("Can not load vkGetInstanceProcAddr");
            }
        }
        else
        {
            LOGE("Can not load Vulkan dynamic lib");
        }
    }
    break;
    case APP_CMD_TERM_WINDOW: //窗体被隐藏或是关闭了
    {
        LOGE("handle_cmd::APP_CMD_TERM_WINDOW");
    }
    break;
    default:
        break;
    }
}

/*!
 * Enable the motion events you want to handle; not handled events are
 * passed back to OS for further processing. For this example case,
 * only pointer and joystick devices are enabled.
 *
 * @param motionEvent the newly arrived GameActivityMotionEvent.
 * @return true if the event is from a pointer or joystick device,
 *         false for all other input devices.
 */
bool motion_event_filter_func(const GameActivityMotionEvent *motionEvent)
{
    auto sourceClass = motionEvent->source & AINPUT_SOURCE_CLASS_MASK;
    return (sourceClass == AINPUT_SOURCE_CLASS_POINTER || sourceClass == AINPUT_SOURCE_CLASS_JOYSTICK);
}

/*!
 * This the main entry point for a native activity
 */
void android_main(struct android_app *pApp)
{
    // Can be removed, useful to ensure your code is running
    LOGI("Welcome to android_main");
    LOGI(ANDROID_IS_ANDROID_STR);

    // Register an event handler for Android events
    pApp->onAppCmd = handle_cmd;

    // Set input event filters (set it to NULL if the app wants to process all inputs).
    // Note that for key inputs, this example uses the default default_key_filter()
    // implemented in android_native_app_glue.c.
    android_app_set_motion_event_filter(pApp, motion_event_filter_func);

    // This sets up a typical game/event loop. It will run until the app is destroyed.
    int events;
    android_poll_source *pSource;
    do
    {
        // Process all pending events before running game logic.
        if (ALooper_pollAll(0, nullptr, &events, (void **)&pSource) >= 0)
        {
            if (pSource)
            {
                pSource->process(pApp, pSource);
            }
        }

        // Check if any user data is associated. This is assigned in handle_cmd
        if (true)
        {
            /*一帧渲染*/
        }
    } while (!pApp->destroyRequested);
}
