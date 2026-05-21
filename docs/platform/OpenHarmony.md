# 开源鸿蒙

鸿蒙端： (Native)XComponent 或 ArkTSXComponent

VK_OHOS_SURFACE_EXTENSION_NAME

vkCreateSurfaceOHOS (VkInstance instance, const VkSurfaceCreateInfoOHOS*pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)

cmake 设置鸿蒙系统宏定义 defined(VK_USE_PLATFORM_OHOS) 或（和） defined(OHOS_PLATFORM) 之后 Turbo 会推导出 TURBO_PLATFORM_OPEN_HARMONY 宏定义

依赖库：

* libace_ndk.z.so
* libnative_window.so
* libvulkan.so （这个不需要，Turbo 采用的手动打开库文件链接方式）

头文件：

```cxx
#include <ace/xcomponent/native_interface_xcomponent.h>
#include <native_window/external_window.h>
#include <vulkan/vulkan.h>
```

创建 Surface

```CXX
VkSurfaceCreateInfoOHOS surfaceCreateInfo{};
surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_SURFACE_CREATE_INFO_OHOS;
surfaceCreateInfo.window = window;//window 为 OnSurfaceCreate回调传入的（）这个是最主要的

OHNativeWindow* VkSurfaceCreateInfoOHOS::window
```

OHNativeWindow 是从 XComponent 组件中获取的

```TS
XComponent({
    id: 'xcomponentId',
    type: 'surface',
    libraryname: 'entry'
})
.margin({ bottom: 20 })
.width(360)
.height(360)
```

```cxx
// XComponent在创建Suface时的回调函数
void OnSurfaceCreatedCB(OH_NativeXComponent *component, void *window) {
    // 在回调函数里可以拿到OHNativeWindow
    OHNativeWindow *nativeWindow = static_cast<OHNativeWindow *>(window);
}

OH_NativeXComponent_Callback callback;
// 注册OnSurfaceCreated回调函数
callback.OnSurfaceCreated = OnSurfaceCreatedCB;
// 将callback注册给nativeXComponent
OH_NativeXComponent_RegisterCallback(nativeXComponent, &callback);
```

XComponent 组件中获取 Native XComponent 之后通过 OH_NativeXComponent_RegisterCallback 注册回调获取 OHNativeWindow

* OHNativeWindow 声明在 <native_window/external_window.h> 中，位于 libnative_window.so 库中

* OH_NativeXComponent_Callback 回调类型用于获取 OH_NativeXComponent 的 原生 window 。声明在 <ace/xcomponent/native_interface_xcomponent.h> 中，位于 libace_ndk.z.so 库中。

```CXX

void(* OH_NativeXComponent_Callback::OnSurfaceCreated) (OH_NativeXComponent *component, void *window);
void(* OH_NativeXComponent_Callback::OnSurfaceChanged) (OH_NativeXComponent *component, void *window);
void(* OH_NativeXComponent_Callback::OnSurfaceDestroyed) (OH_NativeXComponent *component, void *window);
void(* OH_NativeXComponent_Callback::DispatchTouchEvent) (OH_NativeXComponent *component, void *window);

struct OH_NativeXComponent_Callback
{
OnSurfaceCreated OnSurfaceCreated;
OnSurfaceChanged OnSurfaceChanged;
OnSurfaceDestroyed OnSurfaceDestroyed;
DispatchTouchEvent DispatchTouchEvent;
};
```

* OH_NativeXComponent_RegisterCallback 用于 `Native` 向 `鸿蒙` 注册 OH_NativeXComponent_Callback 回调。声明在 <ace/xcomponent/native_interface_xcomponent.h> 中，位于 libace_ndk.z.so 库中。

* napi_define_properties 用于 `Native` 向 `鸿蒙` 注册指定回调接口。声明在 <napi/native_api.h> 中，位于 libace_napi.z.so 库中。

## 鸿蒙窗口

对于管理和维护用于显示渲染结果的 `鸿蒙窗口` 有多种[开发方案](https://developer.huawei.com/consumer/cn/doc/harmonyos-guides/napi-xcomponent-guidelines)可选

## 鸿蒙 API 9 (HarmonyOS 3.1/4.0)

```TS
XComponent(value: {id: string, type: string, libraryname?: string, controller?: XComponentController})
```

> 注：该函数在高版本的鸿蒙中已被弃用

* 其中 `type` 使用 `surface`
* 其中 `libraryname` 必须与在 `native` 侧 `Napi` 模块注册时 `nm_modname` 的名字一致

```TS
XComponent({ id: 'xcomponentId1', type: 'surface', libraryname: 'nativerender' })
  .onLoad((context) => {})
  .onDestroy(() => {})
```

在 `Native` 侧：

* 每一个模块对应一个 `so` 。
* `so` 的命名规则为 `lib{模块名}.so` 。

```CXX
static napi_value Init(napi_env env, napi_value exports)
{
    // 定义暴露在模块上的方法
    napi_property_descriptor desc[] ={
        DECLARE_NAPI_FUNCTION("changeColor", PluginRender::NapiChangeColor),
    };
    // 通过此接口开发者可在exports上挂载native方法（即上面的PluginRender::NapiChangeColor），exports会通过js引擎绑定到js层的一个js对象
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    return exports;
}

static napi_module nativerenderModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init, // 指定加载对应模块时的回调函数
    .nm_modname = "nativerender", // 指定模块名称，对于XComponent相关开发，这个名称必须和ArkTS侧XComponent中libraryname的值保持一致
    .nm_priv = ((void*)0),
    .reserved = { 0 },
};

extern "C" __attribute__((constructor)) void RegisterModule(void)
{
    // 注册so模块
    napi_module_register(&nativerenderModule);
}
```

在模块被加载时的回调内（`Init(napi_env, napi_value)`）解析获得NativeXComponent实例：

```CXX
{
    // ...
    napi_status status;
    napi_value exportInstance = nullptr;
    OH_NativeXComponent *nativeXComponent = nullptr;
    // 用来解析出被wrap了NativeXComponent指针的属性
    status = napi_get_named_property(env, exports, OH_NATIVE_XCOMPONENT_OBJ, &exportInstance);
    if (status != napi_ok) {
        return false;
    }
    // 通过napi_unwrap接口，解析出NativeXComponent的实例指针
    status = napi_unwrap(env, exportInstance, reinterpret_cast<void**>(&nativeXComponent));
    // ...
}
```

获得 `NativeXComponent` 指针后（`OH_NativeXComponent`），通过 `OH_NativeXComponent_RegisterCallback` 接口进行回调注册：

```CXX
  OH_NativeXComponent *nativeXComponent = nullptr;
    // 解析出NativeXComponent实例

    OH_NativeXComponent_Callback callback;
    callback->OnSurfaceCreated = OnSurfaceCreatedCB; // surface创建成功后触发，开发者可以从中获取native window的句柄
    callback->OnSurfaceChanged = OnSurfaceChangedCB; // surface发生变化后触发，开发者可以从中获取native window的句柄以及XComponent的变更信息
    callback->OnSurfaceDestroyed = OnSurfaceDestroyedCB; // surface销毁时触发，开发者可以在此释放资源
    callback->DispatchTouchEvent = DispatchTouchEventCB; // XComponent的touch事件回调接口，开发者可以从中获得此次touch事件的信息

    OH_NativeXComponent_RegisterCallback(nativeXComponent, callback);
```

## 鸿蒙 API 19 (HarmonyOS 5.1.1)

从 `API 8` 开始，开发者可以通过基于 `OH_NativeXComponent` 实例相关的接口进行 `XComponent` 组件 `Surface` 的生命周期监听，在 `API 19` 版本中推荐使用新的 `OH_ArkUI_SurfaceHolder` 管理，其提供了更加安全，丰富的交互与管理。

> 最终目的是获取 `OHNativeWindow* VkSurfaceCreateInfoOHOS::window`

可通过 `OH_NativeXComponent_Callback` 接口获取 `OHNativeWindow`

`XComponent(value: {id: string, type: string, libraryname?: string, controller?: XComponentController}) //从 API 8 开始支持，从 API 12 开始废弃`

建议使用：

`XComponent(value: {id: string, type: XComponentType, libraryname?: string, controller?: XComponentController}) //从 API 10 开始支持，从 API 12 开始不再演进`

建议使用：

`XComponent(options: XComponentOptions) //从 API 12 开始支持`

参数相较于之前版本最主要的区别是去掉了 `id` 和 `libraryname` 参数，增加 `imageAIOptions` 参数。

`XComponent(params: NativeXComponentParameters) //从 API 19 开始支持`

参数相较于之前版本最主要的区别是只有 `type` 和 `imageAIOptions` 参数。

`OH_ArkUI_SurfaceHolder` 从 `API 19` 开始支持。

### Surface 生命周期管理

#### 使用 `XComponentController` 管理 `Surface` 生命周期场景

该场景在 `ArkTS` 侧的 `XComponentController` 获取 `SurfaceId` ，生命周期回调、触摸、鼠标、按键等事件回调等均在 `ArkTS` 侧触发。

适用于视频播放、相机预览等媒体播放类场景，该场景需要在ArkTS侧获取SurfaceId，并将SurfaceId传入对应接口。

* 基于 `ArkTS` 侧获取的 `SurfaceId` ，在 `Native` 侧调用 `OH_NativeWindow_CreateNativeWindowFromSurfaceId` 接口创建出 `NativeWindow` 实例。
* 利用 `NativeWindow` 和 `EGL` 接口开发自定义绘制内容以及申请和提交 `Buffer` 到图形队列。
* `ArkTS` 侧获取生命周期、事件等信息传递到 `Native` 侧处理。

核心的思路是自定义 `XComponentController` 派生类，实现自己的 `Surface` 生命周期回调，并在对应的回调中通知 `C/C++` 层：

```TS
// 重写XComponentController，设置生命周期回调
class MyXComponentController extends XComponentController {
    onSurfaceCreated(surfaceId: string): void {
        console.info(`onSurfaceCreated surfaceId: ${surfaceId}`)
        nativeRender.SetSurfaceId(BigInt(surfaceId));
    }

    onSurfaceChanged(surfaceId: string, rect: SurfaceRect): void {
        console.info(`onSurfaceChanged surfaceId: ${surfaceId}, rect: ${JSON.stringify(rect)}}`)
        // 在onSurfaceChanged中调用ChangeSurface绘制内容
        nativeRender.ChangeSurface(BigInt(surfaceId), rect.surfaceWidth, rect.surfaceHeight)
    }

    onSurfaceDestroyed(surfaceId: string): void {
        console.info(`onSurfaceDestroyed surfaceId: ${surfaceId}`)
        nativeRender.DestroySurface(BigInt(surfaceId))
    }
}

@Entry
@Component
struct Index {
    xComponentController: XComponentController = new MyXComponentController();

    build() {
        Column() {
            XComponent({
                type: XComponentType.SURFACE,
                controller: this.xComponentController //NOTE: 指定自定义控制器
            })
        }
    }
}
```

对于 `Native` 层相关的核心代码如下：

```CXX
// 解析从ArkTS侧传入的surfaceId，此处surfaceId是一个64位int值
int64_t ParseId(napi_env env, napi_callback_info info) {
    if ((env == nullptr) || (info == nullptr)) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "ParseId", "env or info is null");
        return -1;
    }
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    if (napi_ok != napi_get_cb_info(env, info, &argc, args, nullptr, nullptr)) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "ParseId", "GetContext napi_get_cb_info failed");
        return -1;
    }
    int64_t value = 0;
    bool lossless = true;
    if (napi_ok != napi_get_value_bigint_int64(env, args[0], &value, &lossless)) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "ParseId", "Get value failed");
        return -1;
    }
    return value;
}

// 设置SurfaceId，基于SurfaceId完成对NativeWindow的初始化
napi_value PluginManager::SetSurfaceId(napi_env env, napi_callback_info info) {
    int64_t surfaceId = ParseId(env, info);
    OHNativeWindow *nativeWindow;
    PluginRender *pluginRender;
    if (windowMap_.find(surfaceId) == windowMap_.end()) {//查看是否已经存在创建的 OHNativeWindow，没有则新建一个
        OH_NativeWindow_CreateNativeWindowFromSurfaceId(surfaceId, &nativeWindow);//新建 OHNativeWindow
        windowMap_[surfaceId] = nativeWindow;
    } else {
        return nullptr;
    }
    if (pluginRenderMap_.find(surfaceId) == pluginRenderMap_.end()) {//查看是否已经存在对应的渲染器，没有则新建一个
        pluginRender = new PluginRender(surfaceId);
        pluginRenderMap_[surfaceId] = pluginRender;
    }
    pluginRender->InitNativeWindow(nativeWindow);//根据 OHNativeWindow 初始化渲染器（本人意见：该函数调用应该放到上面 if 块中，创建后初始化一次）
    return nullptr;
}

// 根据传入的surfaceId、width、height实现Surface大小的变动
napi_value PluginManager::ChangeSurface(napi_env env, napi_callback_info info) {
    if ((env == nullptr) || (info == nullptr)) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginManager",
                     "ChangeSurface: OnLoad env or info is null");
        return nullptr;
    }
    int64_t surfaceId = 0;
    size_t argc = 3;
    napi_value args[3] = {nullptr};

    if (napi_ok != napi_get_cb_info(env, info, &argc, args, nullptr, nullptr)) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginManager",
                     "ChangeSurface: GetContext napi_get_cb_info failed");
        return nullptr;
    }
    bool lossless = true;
    int index = 0;
    if (napi_ok != napi_get_value_bigint_int64(env, args[index++], &surfaceId, &lossless)) {//获取 surfaceId
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginManager", "ChangeSurface: Get value failed");
        return nullptr;
    }
    double width;
    if (napi_ok != napi_get_value_double(env, args[index++], &width)) {//获取 width
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginManager", "ChangeSurface: Get width failed");
        return nullptr;
    }
    double height;
    if (napi_ok != napi_get_value_double(env, args[index++], &height)) {//获取 height
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginManager", "ChangeSurface: Get height failed");
        return nullptr;
    }
    auto pluginRender = GetPluginRender(surfaceId);//根据 surfaceId 获取对应 渲染器
    if (pluginRender == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginManager", "ChangeSurface: Get pluginRender failed");
        return nullptr;
    }
    pluginRender->UpdateNativeWindowSize(width, height);//通知渲染器画面大小发生变化
    return nullptr;
}

// 销毁Surface
napi_value PluginManager::DestroySurface(napi_env env, napi_callback_info info) {
    int64_t surfaceId = ParseId(env, info);
    auto pluginRenderMapIter = pluginRenderMap_.find(surfaceId);
    if (pluginRenderMapIter != pluginRenderMap_.end()) {//如果找到已存在对应的渲染器，销毁并移除
        delete pluginRenderMapIter->second;
        pluginRenderMap_.erase(pluginRenderMapIter);
    }
    auto windowMapIter = windowMap_.find(surfaceId);
    if (windowMapIter != windowMap_.end()) {//如果找到已存在对应的 OHNativeWindow，销毁并移除
        OH_NativeWindow_DestroyNativeWindow(windowMapIter->second);
        windowMap_.erase(windowMapIter);
    }
    return nullptr;
}
```

#### 使用 `OH_ArkUI_SurfaceHolder` 管理 `Surface` 生命周期场景

该场景根据 `XComponent` 组件对应的 `ArkUI_NodeHandle` 创建 `OH_ArkUI_SurfaceHolder` ，生命周期回调、触摸等事件回调、无障碍和可变帧率回调等均在 `Native` 侧触发。

适用于如下场景：

1. 有较复杂的交互逻辑、对频繁跨语言调用导致性能损耗敏感的场景。
2. 希望能控制 `Surface` 生命周期触发时机的场景。

> XComponent -> ArkUI_NodeHandle -> OH_ArkUI_SurfaceHolder -> 注册Surface生命周期

与使用 `XComponentController` 管理 `Surface` 生命周期场景不同，本场景允许应用根据 `XComponent` 组件对应的 `ArkUI_NodeHandle` 中创建 `OH_ArkUI_SurfaceHolder` ，并通过 `OH_ArkUI_SurfaceHolder` 上的相关接口注册 `Surface` 生命周期，`XComponent` 组件相关的无障碍、可变帧率等能力也可根据 `ArkUI_NodeHandle` 通过相关接口来实现。同时，`XComponent` 组件上的基础/手势事件也可通过 `ArkUI_NodeHandle` 对象使用 `ArkUI NDK` 接口来监听。

* 在 `ArkTS` 侧创建的 `XComponent` 组件可以将其对应的 `FrameNode` 节点传递到 `Native` 侧以获取 `ArkUI_NodeHandle` ，或者在 `Native` 侧直接创建 `XComponent` 组件对应的 `ArkUI_NodeHandle` ，然后调用 `OH_ArkUI_SurfaceHolder_Create` 接口创建 `OH_ArkUI_SurfaceHolder` 实例。
* 基于 `OH_ArkUI_SurfaceHolder` 实例注册相应的生命周期回调、事件回调，获取 `NativeWindow` 实例。
* 利用 `NativeWindow` 和 `EGL` 接口开发自定义绘制内容以及申请和提交 `Buffer` 到图形队列。

`FrameNode` 表示组件树的实体节点。

```TS
import native from 'libnativerender.so';

@Entry
@Component
struct Index {
    xcomponentId: string = 'xcp' + (new Date().getTime());

    build() {
        Column() {
            XComponent({
                type: XComponentType.SURFACE
            })
            .id(this.xcomponentId)//自定义一个id
            .onAttach(() => {//组件挂载到组件树时触发此回调
                let node = this.getUIContext().getFrameNodeById(this.xcomponentId)//通过 id 获取组件的 FrameNode，getFrameNodeById 通过遍历查询对应 id 的节点，性能较差。推荐使用 getAttachedFrameNodeById。
                native.bindNode(this.xcomponentId, node)//将获取到的 FrameNode 传递给 Native 层，进行绑定。
            })
            .onDetach(() => {
                native.unbindNode(this.xcomponentId)//解绑
            })
            .focusable(true)
            .focusOnTouch(true)
            .defaultFocus(true)
        }
    }
}
```

对于 `Native` 层相关的核心代码如下：

```CXX
std::unordered_map<std::string, ArkUI_NodeHandle> PluginManager::nodeHandleMap_;
std::unordered_map<void *, EGLRender *> PluginManager::renderMap_;
std::unordered_map<void *, OH_ArkUI_SurfaceCallback *> PluginManager::callbackMap_;
std::unordered_map<void *, OH_ArkUI_SurfaceHolder *> PluginManager::surfaceHolderMap_;

//获取 Native 侧对应函数接口
ArkUI_NativeNodeAPI_1 *nodeAPI = reinterpret_cast<ArkUI_NativeNodeAPI_1 *>(OH_ArkUI_QueryModuleInterfaceByName(ARKUI_NATIVE_NODE, "ArkUI_NativeNodeAPI_1"));//获取 ArkUI 提供的 Native 侧 Node 类型接口集合

std::string value2String(napi_env env, napi_value value) { // 将napi_value转化为string类型的变量
    size_t stringSize = 0;
    napi_get_value_string_utf8(env, value, nullptr, 0, &stringSize);
    std::string valueString;
    valueString.resize(stringSize);
    napi_get_value_string_utf8(env, value, &valueString[0], stringSize + 1, &stringSize);
    return valueString;
}

napi_value PluginManager::BindNode(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value args[2] = {nullptr};
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    std::string nodeId = value2String(env, args[0]);//获取自定义的 XComponent 的 id

    ArkUI_NodeHandle handle;
    OH_ArkUI_GetNodeHandleFromNapiValue(env, args[1], &handle); // 获取nodeHandle, ArkUI 侧传入的 FrameNode

    OH_ArkUI_SurfaceHolder *holder = OH_ArkUI_SurfaceHolder_Create(handle); // 获取SurfaceHolder
    nodeHandleMap_[nodeId] = handle;
    surfaceHolderMap_[handle] = holder;

    auto callback = OH_ArkUI_SurfaceCallback_Create(); // 创建SurfaceCallback
    callbackMap_[holder] = callback;
    OH_ArkUI_SurfaceCallback_SetSurfaceCreatedEvent(callback, OnSurfaceCreated); // 注册OnSurfaceCreated回调
    OH_ArkUI_SurfaceCallback_SetSurfaceChangedEvent(callback, OnSurfaceChanged); // 注册OnSurfaceChanged回调
    OH_ArkUI_SurfaceCallback_SetSurfaceDestroyedEvent(callback, OnSurfaceDestroyed); // 注册OnSurfaceDestroyed回调
    OH_ArkUI_SurfaceCallback_SetSurfaceShowEvent(callback, OnSurfaceShow); // 注册OnSurfaceShow回调
    OH_ArkUI_SurfaceCallback_SetSurfaceHideEvent(callback, OnSurfaceHide); // 注册OnSurfaceHide回调
    OH_ArkUI_XComponent_RegisterOnFrameCallback(handle, OnFrameCallback); // 注册OnFrameCallback回调

    OH_ArkUI_SurfaceHolder_AddSurfaceCallback(holder, callback); // 注册SurfaceCallback回调

    if (!nodeAPI->addNodeEventReceiver(handle, onEvent)) { // 添加事件监听，返回成功码 0 （该函数添加的监听回调函数触发时机会先于registerNodeEventReceiver注册的全局回调函数。）
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "onBind", "addNodeEventReceiver error");
    }
    if (!nodeAPI->registerNodeEvent(handle, NODE_TOUCH_EVENT, 0, nullptr)) { // 用C接口注册touch事件，返回成功码 0 （这样之前 addNodeEventReceiver 注册的 onEvent 将会在 NODE_TOUCH_EVENT 事件发生时进行回调）
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "onBind", "registerTouchEvent error");
    }
    provider_ = OH_ArkUI_AccessibilityProvider_Create(handle); // 创建一个ArkUI_AccessibilityProvider类型的对象（用于无障碍功能）
    /**
    * 获取ArkUI_AccessibilityProvider后，如果注册无障碍回调函数请参考：
    * https://gitcode.com/openharmony/docs/blob/OpenHarmony-5.1.0-Release/zh-cn/application-dev/ui/ndk-accessibility-xcomponent.md
    * **/
    return nullptr;
}

napi_value PluginManager::UnbindNode(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    std::string nodeId = value2String(env, args[0]);
    auto node = nodeHandleMap_[nodeId];
    OH_ArkUI_XComponent_UnregisterOnFrameCallback(node); // 解注册帧回调
    OH_ArkUI_AccessibilityProvider_Dispose(provider_); // 销毁ArkUI_AccessibilityProvider
    nodeAPI->disposeNode(node); // 销毁nodeHandle
    nodeHandleMap_.erase(nodeId);
    return nullptr;
}

napi_value PluginManager::SetFrameRate(napi_env env, napi_callback_info info)
{
    size_t argc = 4;
    napi_value args[4] = {nullptr};
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    std::string nodeId = value2String(env, args[0]);
    auto node = nodeHandleMap_[nodeId];
    
    int32_t min = 0;
    napi_get_value_int32( env, args[1], &min);

    int32_t max = 0;
    napi_get_value_int32(env, args[2], &max);

    int32_t expected = 0;
    napi_get_value_int32(env, args[3], &expected);
    OH_NativeXComponent_ExpectedRateRange range = {
        .min = min,
        .max = max,
        .expected = expected
    };
    OH_ArkUI_XComponent_SetExpectedFrameRateRange(node, range); // 设置期望帧率
    return nullptr;
}

napi_value PluginManager::SetNeedSoftKeyboard(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value args[2] = {nullptr};
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    std::string nodeId = value2String(env, args[0]);
    auto node = nodeHandleMap_[nodeId];
    
    bool needSoftKeyboard = false;
    napi_get_value_bool( env, args[1], &needSoftKeyboard);
    OH_ArkUI_XComponent_SetNeedSoftKeyboard(node, needSoftKeyboard); // 设置是否需要软键盘
    return nullptr;
}
```

定义 `Surface` 创建成功，发生改变，销毁和事件，可变帧率回调接口。

```CXX
void OnSurfaceCreated(OH_ArkUI_SurfaceHolder *holder) {
    auto window = OH_ArkUI_XComponent_GetNativeWindow(holder); // 获取native window
    auto render = new EGLRender();//创建自己的渲染器
    PluginManager::renderMap_[holder] = render;
    render->SetUpEGLContext(window);//将 NativeWindow 传递给自定义渲染器
}

void OnSurfaceChanged(OH_ArkUI_SurfaceHolder *holder, uint64_t width, uint64_t height) {
    if (PluginManager::renderMap_.count(holder)) {//判断 holder 是否存在。返回拥有与指定实参 key 比较相等的键的元素数，因为此容器不允许重复故为 1 或 0。
        auto render = PluginManager::renderMap_[holder];
        render->SetEGLWindowSize(width, height); // 设置绘制区域大小
        render->DrawStar(true); // 绘制五角星
    }
}

void OnSurfaceDestroyed(OH_ArkUI_SurfaceHolder *holder) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, 0xff00, "onBind", "on destroyed");
    if (PluginManager::renderMap_.count(holder)) { // 销毁render对象
        auto render = PluginManager::renderMap_[holder];
        delete render;
        PluginManager::renderMap_.erase(holder);
    }
    if (PluginManager::callbackMap_.count(holder)) {
        auto callback = PluginManager::callbackMap_[holder];
        OH_ArkUI_SurfaceHolder_RemoveSurfaceCallback(holder, callback); // 移除SurfaceCallback
        OH_ArkUI_SurfaceCallback_Dispose(callback); // 销毁surfaceCallback
        PluginManager::callbackMap_.erase(holder);
    }
    OH_ArkUI_SurfaceHolder_Dispose(holder); // 销毁surfaceHolder
}

void OnSurfaceShow(OH_ArkUI_SurfaceHolder* holder)
{
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "onBind", "on surface show");
}

void OnSurfaceHide(OH_ArkUI_SurfaceHolder* holder)
{
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "onBind", "on surface hide");
}

void onEvent(ArkUI_NodeEvent *event) {
    auto eventType = OH_ArkUI_NodeEvent_GetEventType(event); // 获取组件事件类型
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "onBind", "on event");
    if (eventType == NODE_TOUCH_EVENT) {
        ArkUI_NodeHandle handle = OH_ArkUI_NodeEvent_GetNodeHandle(event); // 获取触发该事件的组件对象
        auto holder = PluginManager::surfaceHolderMap_[handle];
        if (PluginManager::renderMap_.count(holder)) {
            auto render = PluginManager::renderMap_[holder];
            render->DrawStar(false); // 绘制五角星  
        }
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "onBind", "on touch");
    }
}
```

#### 使用 `NativeXComponent` 管理 `Surface` 生命周期场景

该场景在 `native` 层获取 `Native XComponent` 实例，在 `Native` 侧注册 `XComponent` 的生命周期回调，以及触摸、鼠标、按键等事件回调。

与使用 `OH_ArkUI_SurfaceHolder` 管理 `Surface` 生命周期场景类似，但交互事件接口不够丰富，且使用不当容易出现稳定性问题，建议使用 `OH_ArkUI_SurfaceHolder` 的接口。

#### 根据推荐方式使用 `OH_ArkUI_SurfaceHolder` 管理 `Surface` 生命周期场景

## 每帧回调

### `VSync` (从 `API 9` 开始)

```CXX
#include <native_vsync/native_vsync.h>
```

首先需要定义一个 `VSyn` `C`回调函数

```CXX
void RenderEngine::OnVsync(long long timestamp, void *data)
{
    OH_LOG_Print(LOG_APP, LOG_DEBUG, LOG_PRINT_DOMAIN, "RenderEngine", "OnVsync %{public}lld.", timestamp);
    auto renderEngine = reinterpret_cast<RenderEngine *>(data);
    if (renderEngine == nullptr) {
        return;
    }

    renderEngine->vSyncCnt_++;
    renderEngine->wakeUpCond_.notify_one();
}
```

创建 `OH_NativeVSync` 实例

```CXX
const char* demoName = "NativeImageSample";
nativeVsync_ = OH_NativeVSync_Create(demoName, strlen(demoName));
```

通过 `OH_NativeVSync` 实例设置 `VSync` 回调函数。

```CXX
wakeUpCond_.wait(lock, [this]() { return wakeUp_ || vSyncCnt_ > 0; });
wakeUp_ = false;
if (vSyncCnt_ > 0) {
    vSyncCnt_--;
    (void)OH_NativeVSync_RequestFrame(nativeVsync_, &RenderEngine::OnVsync, this);
    OH_NativeVSync_GetPeriod(nativeVsync_, &period);
}
```

在 `NativeVsync` 中可以创建与窗口绑定的OH_NativeVSync实例：

```CXX
OH_NativeVSync* OH_NativeVSync_Create_ForAssociatedWindow(uint64_t windowID, const char* name, unsigned int length)//起始版本 14
```

使用本接口创建出来的 `OH_NativeVSync` 实例的实际 `vsync` 周期与系统 `vsync` 周期不完全一致，系统会根据窗口的状态对实际 `vsync` 周期进行调整。

其中 `windowID` 可以通过 `OH_NativeWindow_GetSurfaceId` 接口获取。

### NativeDisplaySoloist  (从 `API 12` 开始)

如果开发者想在独立线程中实现帧率控制的 `Native` 侧业务，可以通过 `DisplaySoloist` 来实现，如游戏、自绘制 `UI` 框架对接等场景。

开发者可以选择多个 `DisplaySoloist` 实例共享一个线程，也可以选择每个 `DisplaySoloist` 实例独占一个线程。

```TS
XComponent({
  id: 'xcomponentId_120',
  type: XComponentType.SURFACE,
  libraryname: 'entry'
})
  .onLoad((xComponentContext) => {
    this.xComponentContext = xComponentContext as XComponentContext;
  }).width('640px')
```

```CXX
#include <native_display_soloist/native_display_soloist.h>
```

> 注: 部分功能与 `XComponent` 重叠。

```CXX
char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
if (OH_NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize) != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
   SAMPLE_LOGE("NapiRegister: Unable to get XComponent id");
   return nullptr;
}
std::string id(idStr);

g_displaySync[id] = OH_DisplaySoloist_Create(true);//创建 DisplaySoloist

// 设置期望帧率范围
// 此结构体成员变量分别为帧率范围的最小值、最大值以及期望帧率
DisplaySoloist_ExpectedRateRange range;
if (id == "xcomponentId30") {
   // 第一个XComponent期望帧率为30Hz
   range = {30, 120, 30};
}
if (id == "xcomponentId120") {
   // 第二个XComponent期望帧率为120Hz
   range = {30, 120, 120};
}
OH_DisplaySoloist_SetExpectedFrameRateRange(nativeDisplaySoloist, &range);
// 注册回调与使能每帧回调
OH_DisplaySoloist_Start(nativeDisplaySoloist, TestCallback, nativeXComponent);

static void TestCallback(long long timestamp, long long targetTimestamp, void *data) 
{
    //自定义回调内容
}
```

销毁与结束：

```CXX
napi_value SampleXComponent::NapiUnregister(napi_env env, napi_callback_info info)
{
    // ...
    // 取消注册每帧回调
    OH_DisplaySoloist_Stop(g_displaySync[id]);
    // ...
}

napi_value SampleXComponent::NapiDestroy(napi_env env, napi_callback_info info)
{
    // ...
    // 销毁OH_DisplaySoloist实例
    OH_DisplaySoloist_Destroy(g_displaySync[id]);
    g_displaySync.erase(id);       
    // ...
}
```

## NDK 开发

只要通过 `CMake` 开发。主要参数：

```cmd
-D OHOS_STL=c++_shared -D OHOS_ARCH=arm64-v8a -D OHOS_PLATFORM=OHOS
```

* `OHOS_STL` 设置 `C++` 标准库连接方式。可选值为 `c++_shared / c++_static` (默认为 `c++_shared` )。
* `OHOS_ARCH` 设置目标架构。可选值为 `armeabi-v7a / arm64-v8a / x86_64` （ `arm64-v8a` 为主流同时也是默认值）。
* `OHOS_PLATFORM` 目标平台。可选值为 `OHOS` 。

通过设置 `${OHOS_SDK}/native/build/cmake/ohos.toolchain.cmake` 工具链编译：

```cmd
cmake -D OHOS_STL=c++_shared -D OHOS_ARCH=arm64-v8a -D OHOS_PLATFORM=OHOS -D CMAKE_TOOLCHAIN_FILE={ohos-sdk}/linux/native/build/cmake/ohos.toolchain.cmake ..
cmake --build .
```

`ohos.toolchain.cmake` 内部会设置 `set(OHOS OHOS)` 变量，项目可通过该变量判断是否为 `鸿蒙` 平台。

在 `Windows` 平台下还需要设置使用的生成器，官网使用的是 `Ninja` 。

```cmd
 F:\windows\native\build-tools\cmake\bin\cmake.exe -G "Ninja" -D OHOS_STL=c++_shared -D OHOS_ARCH=arm64-v8a -D OHOS_PLATFORM=OHOS -D CMAKE_TOOLCHAIN_FILE=F:\windows\native\build\cmake\ohos.toolchain.cmake ..
```

> 注：如需 `debug` 调试，增加参数 `-D CMAKE_BUILD_TYPE=Debug` 。

引用已经编译好的 `鸿蒙` 平台库：

```cmake
# 比如引入 avcodec_ffmpeg 库
add_library(library SHARED hello.cpp)

add_library(avcodec_ffmpeg SHARED IMPORTED)
set_target_properties(avcodec_ffmpeg
    PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/third_party/FFmpeg/libs/${OHOS_ARCH}/libavcodec_ffmpeg.so)

target_link_libraries(library PUBLIC libace_napi.z.so avcodec_ffmpeg)
```

### sysroot

使用 `ohos.toolchain.cmake` 内部默认配置的是 `${OHOS_SDK_NATIVE}/sysroot` 。

### 选择编译器

使用 `ohos.toolchain.cmake` 内部默认配置的是 `${OHOS_SDK_NATIVE}/llvm/bin/clang` 和 `clang++` 。鸿蒙自己的编译器：毕昇，如果使用毕昇编译的话，最简单的方法就是将：

1. `${OHOS_SDK_NATIVE}/llvm` 的 `llvm` 重命名一个名字，比如 `llvm-bak`
2. 将 `xxx/sdk/default/hms/native/BiSheng` 的 `BiSheng` 文件夹拷贝到 `${OHOS_SDK_NATIVE}` 文件夹下
3. 将拷贝的文件夹重命名为 `llvm`

说白了就是:将 `llvm` 下的内容替换为 `BiSheng` 的内容，这样使用默认的 `llvm` 时，起始使用的是毕昇编译器。

毕昇编译的程序有优化提升，并有 `IClang` 等编译优化手段。

### 已知C++兼容性问题

应用启动或 `dlopen` 时，`hilog` 报错 `symbol not found, s=__emutls_get_address`。原因是 `API9` 及之前版本的 `libc++_shared.so`无此符号，而 `API11` 之后版本的 `libc++_shared.so` 有此符号。解决方法是更新应用或HAR包的SDK版本。

### Node-API 交互

1. `初始化阶段`：当`ArkTS`侧在`import`一个`Native`模块时，`ArkTS`引擎会调用 `ModuleManager` 加载模块对应的 `so` 及其依赖。首次加载时会触发模块的注册，将模块定义的方法属性挂载到 `exports` 对象上并返回该对象。

2. `调用阶段`：当 `ArkTS` 侧通过上述 `import` 返回的对象调用方法时，`ArkTS` 引擎会找到并调用对应的 `C/C++` 方法。

* `ArkTS` 侧：实现 `C++` 方法的调用，通过 `import` 所需的 `so` 库后，可以调用 `C++` 方法。
* `Native` 侧：`.cpp` 文件，实现模块的注册。需要提供注册 `lib` 库的名称，并在注册回调方法中定义接口的映射关系，即 `Native` 方法及对应的 `JS/ArkTS` 接口名称等。

#### 设置模块注册信息

`ArkTS` 侧 `import native` 模块时，会加载其对应的 `so` 。加载 `so` 时，首先会调用 `napi_module_register` 方法，将模块注册到系统中，并调用模块初始化函数。

`napi_module` 有两个关键属性：

1. `.nm_register_func` 定义模块初始化函数。
2. `.nm_modname` 定义模块的名称，也就是 `ArkTS` 侧引入的 `so` 库的名称，模块系统会根据此名称来区分不同的 `so` 。

> 注：注册代码写在 `Native` 项目更目录下的 `napi_init.cpp` 中。

```CXX
// entry/src/main/cpp/napi_init.cpp

// 准备模块加载相关信息，将上述Init函数与本模块名等信息记录下来。
static napi_module demoModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "entry",
    .nm_priv = ((void*)0),
    .reserved = {0},
};

// 加载so时，该函数会自动被调用，将上述demoModule模块注册到系统中。
extern "C" __attribute__((constructor)) void RegisterDemoModule() {
    napi_module_register(&demoModule);
}
```

>注：以上代码无须复制，创建 `Native C++` 工程以后在 `napi_init.cpp` 代码中已配置好。

#### 模块初始化

实现 `ArkTS` 接口与 `C++` 接口的绑定和映射。

> 注：注册代码写在 `Native` 项目更目录下的 `napi_init.cpp` 中。

```CXX
// entry/src/main/cpp/napi_init.cpp
EXTERN_C_START
// 模块初始化
static napi_value Init(napi_env env, napi_value exports) {
    // ArkTS接口与C++接口的绑定和映射
    napi_property_descriptor desc[] = {
        // 注：仅需复制以下两行代码，Init在完成创建Native C++工程以后在napi_init.cpp中已配置好。
        {"callNative", nullptr, CallNative, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"nativeCallArkTS", nullptr, NativeCallArkTS, nullptr, nullptr, nullptr, napi_default, nullptr}
    };
    // 在exports对象上挂载CallNative/NativeCallArkTS两个Native方法
    napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
    return exports;
}
EXTERN_C_END
```

在 `index.d.ts` 文件中，提供 `JS` 侧的接口方法。

```ts
// entry/src/main/cpp/types/libentry/index.d.ts
export const callNative: (a: number, b: number) => number;
export const nativeCallArkTS: (cb: (a: number) => number) => number;
```

在 `oh-package.json5` 文件中将 `index.d.ts` 与 `cpp` 文件关联起来。

```json
// entry/src/main/cpp/types/libentry/oh-package.json5
{
  "name": "libentry.so",
  "types": "./index.d.ts",
  "version": "",
  "description": "Please describe the basic information."
}
```

在 `CMakeLists.txt` 文件中配置 `CMake` 打包参数。

```cmake
# entry/src/main/cpp/CMakeLists.txt
cmake_minimum_required(VERSION 3.4.1)
project(MyApplication2)

set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${NATIVERENDER_ROOT_PATH}
                    ${NATIVERENDER_ROOT_PATH}/include)

# 添加名为entry的库
add_library(entry SHARED napi_init.cpp)
# 构建此可执行文件需要链接的库
target_link_libraries(entry PUBLIC libace_napi.z.so)
```

实现 `Native` 侧的 `CallNative` 以及 `NativeCallArkTS` 接口。具体代码如下：

```CXX
// entry/src/main/cpp/napi_init.cpp
static napi_value CallNative(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    // 声明参数数组
    napi_value args[2] = {nullptr};

    // 获取传入的参数并依次放入参数数组中
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    // 依次获取参数
    double value0;
    napi_get_value_double(env, args[0], &value0);
    double value1;
    napi_get_value_double(env, args[1], &value1);

    // 返回两数相加的结果
    napi_value sum;
    napi_create_double(env, value0 + value1, &sum);
    return sum;
}

static napi_value NativeCallArkTS(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    // 声明参数数组
    napi_value args[1] = {nullptr};

    // 获取传入的参数并依次放入参数数组中
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    // 创建一个int，作为ArkTS的入参
    napi_value argv = nullptr;
    napi_create_int32(env, 2, &argv);

    // 调用传入的callback，并将其结果返回
    napi_value result = nullptr;
    napi_call_function(env, nullptr, args[0], 1, &argv, &result);
    return result;
}
```

#### ArkTS侧调用C/C++方法实现

`ArkTS` 侧通过 `import` 引入 `Native` 侧包含处理逻辑的 `so` 来使用 `C/C++` 的方法。

```ts

// entry/src/main/ets/pages/Index.ets
// 通过import的方式，引入Native能力。
import nativeModule from 'libentry.so'

@Entry
@Component
struct Index {
  @State message: string = 'Test Node-API callNative result: ';
  @State message2: string = 'Test Node-API nativeCallArkTS result: ';
  build() {
    Row() {
      Column() {
        // 第一个按钮，调用callNative方法，对应到Native侧的CallNative方法，进行两数相加。
        Text(this.message)
          .fontSize(50)
          .fontWeight(FontWeight.Bold)
          .onClick(() => {
            this.message += nativeModule.callNative(2, 3);
            })
        // 第二个按钮，调用nativeCallArkTS方法，对应到Native的NativeCallArkTS，在Native调用ArkTS function。
        Text(this.message2)
          .fontSize(50)
          .fontWeight(FontWeight.Bold)
          .onClick(() => {
            this.message2 += nativeModule.nativeCallArkTS((a: number)=> {
                return a * 2;
            });
          })
      }
      .width('100%')
    }
    .height('100%')
  }
}
```

### Node-API的约束限制

#### `SO` 命名规则

导入使用的模块名和注册时的模块名大小写保持一致，如模块名为 `entry` ，则 `so` 的名字为 `libentry.so` ，`napi_module` 中 `nm_modname` 字段应为 `entry` ， `ArkTS` 侧使用时写作：`import xxx from 'libentry.so'` 。

#### 注册建议

* `nm_register_func` 对应的函数（如上述 `Init` 函数）需要加上 `static` ，防止与其他 `so` 里的符号冲突。
* 模块注册的入口，即使用 `__attribute__((constructor))` 修饰的函数的函数名（如上述 `RegisterDemoModule` 函数）需要确保不与其它模块重复。

#### 多线程限制

每个引擎实例对应一个 `ArkTS` 线程，实例上的对象不能跨线程操作，否则会引起应用 `crash` 。使用时需要遵循如下原则：

* `Node-API` 接口只能在 `ArkTS` 线程使用。
* `Native` 接口入参 `env` 与特定 `ArkTS` 线程绑定，只能在创建该 `env` 的线程使用。
* 使用 `Node-API` 接口创建的数据需在 `env` 完全销毁前进行释放，避免内存泄漏。此外，在 `napi_env` 销毁后访问/使用这些数据，可能会导致进程崩溃。
