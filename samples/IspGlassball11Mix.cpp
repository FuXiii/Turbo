#include <TDevice.h>
#include <TDeviceQueue.h>
#include <TEngine.h>
#include <TPhysicalDevice.h>
#include <TVulkanAllocator.h>

#include <TBuffer.h>
#include <TCommandBuffer.h>
#include <TCommandBufferPool.h>
#include <TImage.h>
#include <TImageView.h>

#include <TShader.h>

#include <TAttachment.h>
#include <TGraphicsPipeline.h>
#include <TRenderPass.h>
#include <TSubpass.h>

#include <TDescriptorPool.h>
#include <TDescriptorSet.h>
#include <TDescriptorSetLayout.h>
#include <TFramebuffer.h>

#include <TFence.h>
#include <TSemaphore.h>

#include <fstream>

#include <GLFW/glfw3.h>

#include <TSurface.h>
#include <TSwapchain.h>

#include <math.h>

#include <TPipelineDescriptorSet.h>
#include <TSampler.h>

#include <glm/ext.hpp>

#include <TVulkanLoader.h>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tiny_gltf.h>

#include <ktx.h>

#include <imgui.h>

std::string asset_root(TURBO_ASSET_ROOT);

static bool g_MouseJustPressed[ImGuiMouseButton_COUNT] = {};
static GLFWcursor *g_MouseCursors[ImGuiMouseCursor_COUNT] = {};

const std::string IMGUI_VERT_SHADER_STR = "#version 450\n"
                                          "layout (location = 0) in vec2 inPos;\n"
                                          "layout (location = 1) in vec2 inUV;\n"
                                          "layout (location = 2) in vec4 inColor;\n"
                                          "layout (push_constant) uniform PushConstants {\n"
                                          "	vec2 scale;\n"
                                          "	vec2 translate;\n"
                                          "} pushConstants;\n"
                                          "layout (location = 0) out vec2 outUV;\n"
                                          "layout (location = 1) out vec4 outColor;\n"
                                          "out gl_PerVertex \n"
                                          "{\n"
                                          "	vec4 gl_Position;   \n"
                                          "};\n"
                                          "void main() \n"
                                          "{\n"
                                          "	outUV = inUV;\n"
                                          "	outColor = inColor;\n"
                                          "	gl_Position = vec4(inPos * pushConstants.scale + pushConstants.translate, 0.0, 1.0);\n"
                                          "}\n";

const std::string IMGUI_FRAG_SHADER_STR = "#version 450\n"
                                          "layout (binding = 0) uniform sampler2D fontSampler;\n"
                                          "layout (location = 0) in vec2 inUV;\n"
                                          "layout (location = 1) in vec4 inColor;\n"
                                          "layout (location = 0) out vec4 outColor;\n"
                                          "layout (location = 1) out vec4 outCustomColor;\n"
                                          "void main() \n"
                                          "{\n"
                                          "	outColor = inColor * texture(fontSampler, inUV);\n"
                                          "	outCustomColor = outColor;\n"
                                          "}";

const std::string MY_VERT_SHADER_STR = "#version 450\n"
                                       "out gl_PerVertex {\n"
                                       "	vec4 gl_Position;\n"
                                       "};\n"
                                       "layout (location = 0) out vec2 uv;\n"
                                       "void main() \n"
                                       "{\n"
                                       "    uv = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);\n"
                                       "	gl_Position = vec4(uv* 2.0f - 1.0f, 0.0f, 1.0f);\n"
                                       "    uv.y = 1-uv.y;\n"
                                       "}";

const std::string MY_FRAG_SHADER_STR = "#version 450\n"
                                       "#extension GL_EXT_samplerless_texture_functions : enable\n"
                                       "layout (push_constant) uniform my_push_constants_t\n"
                                       "{"
                                       "   float time;\n"
                                       "   float resolutionX;\n"
                                       "   float resolutionY;\n"
                                       "   float mouseX;\n"
                                       "   float mouseY;\n"
                                       "   bool isClick;\n"
                                       "   uint frame;\n"
                                       "   float radius;\n"
                                       "} my_push_constants;\n"
                                       "layout (set = 0, binding = 0) uniform samplerCube cubeMap;\n"
                                       "//layout (set = 0, binding = 1) uniform texture2D previousFrame;\n"
                                       "//layout (set = 0, binding = 2) uniform sampler mySampler;\n"
                                       "layout (location = 0) in vec2 uv;\n"
                                       "layout (location = 0) out vec4 outColor;\n"
                                       "#define PI 3.1415926\n"
                                       "#define ZERO 0.\n"
                                       "uint seed = 0u;\n"
                                       "uint randu() { return seed = seed * 1664525u + 1013904223u; }\n"
                                       "float rand01() { return float(randu()) * (1./4294967296.); }\n"
                                       "vec3 light(vec3 rd) {\n"
                                       "    vec3 col =texture(cubeMap, rd.yzx, 0).xyz;// vec3(1.,1.,1.)*0.54\n"
                                       "    vec3 bri = vec3(1.0) + vec3(1.0) * pow(max(dot(rd, normalize(vec3(-0.2, -0.5, 0.5))), 0.), 4.);\n"
                                       "    return col * bri;\n"
                                       "}\n"
                                       "bool intersectSphere(vec3 o, float r, vec3 ro, vec3 rd,\n"
                                       "        inout float t, inout vec3 n) {\n"
                                       "    ro -= o;\n"
                                       "    float b = -dot(ro, rd), c = dot(ro, ro) - r * r;\n"
                                       "    float delta = b * b - c;\n"
                                       "    if (delta < 0.0) return false;\n"
                                       "    delta = sqrt(delta);\n"
                                       "    float t1 = b - delta, t2 = b + delta;\n"
                                       "    if (t1 > t2) t = t1, t1 = t2, t2 = t;\n"
                                       "    if (t1 > t || t2 < 0.) return false;\n"
                                       "    t = t1 > 0. ? t1 : t2;\n"
                                       "    n = normalize(ro + rd * t);\n"
                                       "    return true;\n"
                                       "}\n"
                                       "vec3 sampleCosWeighted(vec3 n) {\n"
                                       "    vec3 u = normalize(cross(n, vec3(1.2345, 2.3456, -3.4561)));\n"
                                       "    vec3 v = cross(u, n);\n"
                                       "    float rn = rand01();\n"
                                       "    float an = 2.0*PI*rand01();\n"
                                       "    vec2 rh = sqrt(rn) * vec2(cos(an), sin(an));\n"
                                       "    float rz = sqrt(1. - rn);\n"
                                       "    return rh.x * u + rh.y * v + rz * n;\n"
                                       "}\n"
                                       "vec3 sampleFresnelDielectric(vec3 rd, vec3 n, float n1, float n2) {\n"
                                       "    float eta = n1 / n2;\n"
                                       "    float ci = -dot(n, rd);\n"
                                       "    if (ci < 0.0) ci = -ci, n = -n;\n"
                                       "    float ct = 1.0 - eta * eta * (1.0 - ci * ci);\n"
                                       "    if (ct < 0.0) return rd + 2.0*ci*n;\n"
                                       "    ct = sqrt(ct);\n"
                                       "    float Rs = (n1 * ci - n2 * ct) / (n1 * ci + n2 * ct);\n"
                                       "    float Rp = (n1 * ct - n2 * ci) / (n1 * ct + n2 * ci);\n"
                                       "    float R = 0.5 * (Rs * Rs + Rp * Rp);\n"
                                       "    return rand01() > R ?\n"
                                       "        rd * eta + n * (eta * ci - ct)\n"
                                       "        : rd + 2.0*ci*n;\n"
                                       "}\n"
                                       "vec3 sampleUniformSphere() {\n"
                                       "    float u = 2.0*PI*rand01();\n"
                                       "    float v = 2.0*rand01()-1.0;\n"
                                       "    return vec3(vec2(cos(u), sin(u))*sqrt(1.0-v*v), v);\n"
                                       "}\n"
                                       "vec3 sampleHenyeyGreenstein(vec3 wi, float g) {\n"
                                       "    if (g == 0.0) return sampleUniformSphere();\n"
                                       "    if (g >= 1.0) return wi;\n"
                                       "    if (g <= -1.0) return -wi;\n"
                                       "    float us = rand01();\n"
                                       "    float vs = 2.0*PI*rand01();\n"
                                       "    float z = (1.0+g*g-pow((1.0-g*g)/(2.0*g*(us+(1.0-g)/(2.0*g))),2.0))/(2.0*g);\n"
                                       "    vec2 xy = vec2(cos(vs), sin(vs)) * sqrt(1.0-z*z);\n"
                                       "    vec3 u = normalize(cross(wi, vec3(1.2345, 2.3456, -3.4561)));\n"
                                       "    vec3 v = cross(u, wi);\n"
                                       "    vec3 wo = normalize(xy.x*u + xy.y*v + z*wi);\n"
                                       "    return wo;\n"
                                       "}\n"
                                       "const int mat_none = -1;\n"
                                       "const int mat_background = 0;\n"
                                       "const int mat_lambertian = 1;\n"
                                       "const int mat_refractive = 2;\n"
                                       "vec3 hsl2rgb(float h, float s, float l) {\n"
                                       "    vec3 rgb = clamp(abs(mod(h*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0);\n"
                                       "    return l + s * (rgb-0.5)*(1.0-abs(2.0*l-1.0));\n"
                                       "}\n"
                                       "vec3 ryb2rgb(vec3 ryb) {\n"
                                       "    const vec3 ryb000 = vec3(1, 1, 1);\n"
                                       "    const vec3 ryb001 = vec3(0.163, 0.373, 0.6);\n"
                                       "    const vec3 ryb010 = vec3(1, 1, 0);\n"
                                       "    const vec3 ryb100 = vec3(1, 0, 0);\n"
                                       "    const vec3 ryb011 = vec3(0, 0.66, 0.2);\n"
                                       "    const vec3 ryb101 = vec3(0.5, 0, 0.5);\n"
                                       "    const vec3 ryb110 = vec3(1, 0.5, 0);\n"
                                       "    const vec3 ryb111 = vec3(0, 0, 0);\n"
                                       "    return mix(mix(\n"
                                       "        mix(ryb000, ryb001, ryb.z),\n"
                                       "        mix(ryb010, ryb011, ryb.z),\n"
                                       "        ryb.y), mix(\n"
                                       "        mix(ryb100, ryb101, ryb.z),\n"
                                       "        mix(ryb110, ryb111, ryb.z),\n"
                                       "        ryb.y), ryb.x);\n"
                                       "}\n"
                                       "void calcAbsorb(in vec3 p, out vec3 emi, out vec3 tabs, out vec3 sabs, out float k, out float g) {\n"
                                       "    p -= vec3(0, 0, 1);  // center of sphere\n"
                                       "    vec3 q = p; q.xz = mat2(0.9,0.2,-0.2,0.9)*q.xz;\n"
                                       "    emi = (abs(q.z+0.01)<0.02 && max(length(q.xy)-0.6, sin(7.0*PI*length(q.xy)))<0.) || length(abs(q)-vec3(0,0,0.25))<0.1 ?\n"
                                       "         8.0*ryb2rgb(hsl2rgb(atan(q.y,q.x)/(2.0*PI)+0.5, 1.0, 0.5)) : vec3(0.0);\n"
                                       "    tabs = vec3(0.0);\n"
                                       "    sabs = vec3(0.9, 0.95, 0.99);\n"
                                       "    k = clamp(-100.0*p.z+0.5, 0., 20.);\n"
                                       "    g = 0.5;\n"
                                       "}\n"
                                       "void calcScatter(in vec3 ro, inout vec3 rd,\n"
                                       "        inout float mt, inout vec3 m_col, out vec3 m_emi, out int material, inout vec3 min_n) {\n"
                                       "    float p = 1.0, hit_p = rand01();\n"
                                       "    float dt = 0.01;\n"
                                       "    m_emi = vec3(0.0);\n"
                                       "    for (float t = 1e-4; t < mt-dt; t += dt) {\n"
                                       "        vec3 emi, tabs, sabs; float k, g;\n"
                                       "        calcAbsorb(ro + rd * (t+0.5*dt), emi, tabs, sabs, k, g);\n"
                                       "        float dp = exp(-k*dt);\n"
                                       "        if (p * dp < hit_p) {\n"
                                       "            dt *= log(p/hit_p)/k;\n"
                                       "            mt = t + dt;\n"
                                       "            rd = sampleHenyeyGreenstein(rd, g);\n"
                                       "            m_col *= sabs * exp(-tabs*dt);\n"
                                       "            material = mat_none;\n"
                                       "            min_n = vec3(0.0);\n"
                                       "            return;\n"
                                       "        }\n"
                                       "        p *= dp;\n"
                                       "        m_col *= exp(-tabs*dt);\n"
                                       "        m_emi += m_col * emi * dt;\n"
                                       "    }\n"
                                       "    material = mat_refractive;\n"
                                       "}\n"
                                       "vec3 mainRender(vec3 ro, vec3 rd) {\n"
                                       "\n"
                                       "    vec3 m_col = vec3(1.0), t_col = vec3(0.0), col;\n"
                                       "    bool is_inside = false;\n"
                                       "    for (int iter = int(ZERO); iter < 128; iter++) {\n"
                                       "        ro += 1e-4f*rd;\n"
                                       "        if (is_inside != (length(ro-vec3(0,0,1))<1.0)) return vec3(1, 0, 0);\n"
                                       "        vec3 n, min_n;\n"
                                       "        float t, min_t = 1e12;\n"
                                       "        vec3 min_ro = ro, min_rd = rd;\n"
                                       "        vec3 min_emi = vec3(0.0);\n"
                                       "        int material = mat_background;\n"
                                       "        t = -ro.z / rd.z;\n"
                                       "        if (t > 0.0) {\n"
                                       "            min_t = t, min_n = vec3(0, 0, 1);\n"
                                       "            min_ro = ro + rd * t, min_rd = rd;\n"
                                       "            col = vec3(0.9, 0.95, 0.98);\n"
                                       "            material = mat_lambertian;\n"
                                       "        }\n"
                                       "        t = min_t;\n"
                                       "        if (intersectSphere(vec3(0.0, 0.0, 1.0), 1.0, ro, rd, t, n)) {\n"
                                       "            min_t = t, min_n = n;\n"
                                       "            if (is_inside) {\n"
                                       "                col = vec3(1.0);\n"
                                       "                min_rd = rd;\n"
                                       "                calcScatter(ro, min_rd, min_t, col, min_emi, material, min_n);\n"
                                       "                min_ro = ro + rd * min_t;\n"
                                       "            }\n"
                                       "            else {\n"
                                       "                min_ro = ro + rd * t, min_rd = rd;\n"
                                       "                col = vec3(1.0);\n"
                                       "                material = mat_refractive;\n"
                                       "            }\n"
                                       "        }\n"
                                       "        if (material == mat_background) {\n"
                                       "            col = light(rd);\n"
                                       "            return m_col * col + t_col;\n"
                                       "        }\n"
                                       "        ro = min_ro, rd = min_rd;\n"
                                       "        min_n = dot(rd, min_n) < 0. ? min_n : -min_n;\n"
                                       "        if (material == mat_lambertian) {\n"
                                       "            rd = sampleCosWeighted(min_n);\n"
                                       "        }\n"
                                       "        else if (material == mat_refractive) {\n"
                                       "            vec2 eta = is_inside ? vec2(1.5, 1.0) : vec2(1.0, 1.5);\n"
                                       "            rd = sampleFresnelDielectric(rd, min_n, eta.x, eta.y);\n"
                                       "        }\n"
                                       "        m_col = m_col * col;\n"
                                       "        t_col += min_emi;\n"
                                       "        if (dot(rd, min_n) < 0.0) {\n"
                                       "            is_inside = !is_inside;\n"
                                       "        }\n"
                                       "    }\n"
                                       "    return m_col + t_col;\n"
                                       "}\n"
                                       "void main() {\n"
                                       "    float iTime = my_push_constants.time;\n"
                                       "    float is_click = my_push_constants.isClick?1.0:0.0;\n"
                                       "    vec3 iMouse = vec3(my_push_constants.mouseX, my_push_constants.mouseY,is_click);\n"
                                       "    vec2 iResolution = vec2(my_push_constants.resolutionX,my_push_constants.resolutionY);\n"
                                       "    vec2 fragCoord = vec2(uv.x*iResolution.x,uv.y*iResolution.y);\n"
                                       "    uint iFrame = my_push_constants.frame;\n"
                                       "    seed = uint(fragCoord.x)*uint(fragCoord.y)*uint(iFrame+1);\n"
                                       "    seed = randu() + 161u*uint(fragCoord.y);\n"
                                       "    seed = randu() + 239u*uint(fragCoord.x);\n"
                                       "    seed = randu() + 197u*uint(iFrame+1);\n"
                                       "    float rx = iMouse.z==0.?0.3:2.0*(iMouse.y/iResolution.y)-0.5;\n"
                                       "    float rz = iMouse.z==0.?-1.2:-iMouse.x/iResolution.x*4.0*3.14;\n"
                                       "    vec3 w = vec3(cos(rx)*vec2(cos(rz),sin(rz)), sin(rx));\n"
                                       "    vec3 u = vec3(-sin(rz),cos(rz),0);\n"
                                       "    vec3 v = cross(w,u);\n"
                                       "    vec3 ro = my_push_constants.radius*w + vec3(0, 0, 0.7);\n"
                                       "    vec2 uv = 2.0*(fragCoord.xy+vec2(rand01(),rand01())-0.5)/iResolution.xy - vec2(1.0);\n"
                                       "    vec3 rd = mat3(u,v,-w)*vec3(uv*iResolution.xy, 3.0*length(iResolution.xy));\n"
                                       "    rd = normalize(rd);\n"
                                       "    vec3 col = mainRender(ro, rd);\n"
                                       "    outColor = vec4(col,1.0);\n"
                                       "}\n";

typedef struct POSITION
{
    float x;
    float y;
    float z;
} POSITION;

typedef struct NORMAL
{
    float x;
    float y;
    float z;
} NORMAL;

typedef struct TEXCOORD
{
    float u;
    float v;
} TEXCOORD;

struct MY_PUSH_CONSTANTS_DATA
{
    float time;
    float resolutionX;
    float resolutionY;
    float mouseX;
    float mouseY;
    bool isClick;
    uint32_t frame;
    float radius;
};

int main()
{
    std::cout << "Vulkan Version:" << Turbo::Core::TVulkanLoader::Instance()->GetVulkanVersion().ToString() << std::endl;

    std::vector<Turbo::Core::TLayerInfo> support_layers;
    std::vector<Turbo::Core::TExtensionInfo> instance_support_extensions;
    {
        Turbo::Core::TRefPtr<Turbo::Core::TInstance> temp_instance = new Turbo::Core::TInstance();
        support_layers = temp_instance->GetSupportLayers();
        instance_support_extensions = temp_instance->GetSupportExtensions();
    }

    Turbo::Core::TLayerInfo khronos_validation;
    for (Turbo::Core::TLayerInfo &layer : support_layers)
    {
        if (layer.GetLayerType() == Turbo::Core::TLayerType::VK_LAYER_KHRONOS_VALIDATION)
        {
            khronos_validation = layer;
            break;
        }
    }

    std::vector<Turbo::Core::TLayerInfo> enable_layer;
    if (khronos_validation.GetLayerType() != Turbo::Core::TLayerType::UNDEFINED)
    {
        enable_layer.push_back(khronos_validation);
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_instance_extensions;
    for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_WIN32_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_WAYLAND_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XCB_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XLIB_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
    }

    Turbo::Core::TVersion instance_version(1, 2, 0, 0);
    Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance = new Turbo::Core::TInstance(&enable_layer, &enable_instance_extensions, &instance_version);
    Turbo::Core::TRefPtr<Turbo::Core::TPhysicalDevice> physical_device = instance->GetBestPhysicalDevice();

    if (!glfwInit())
        return -1;
    GLFWwindow *window;
    int window_width = 1920 / 2.0;
    int window_height = 1080 / 2.0;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(window_width, window_height, "Turbo", NULL, NULL);
    VkSurfaceKHR vk_surface_khr = VK_NULL_HANDLE;
    VkInstance vk_instance = instance->GetVkInstance();
    glfwCreateWindowSurface(vk_instance, window, NULL, &vk_surface_khr);

    Turbo::Core::TPhysicalDeviceFeatures physical_device_features = {};
    physical_device_features.sampleRateShading = true;

    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = physical_device->GetSupportExtensions();
    for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
        {
            enable_device_extensions.push_back(extension);
        }
    }

    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device = new Turbo::Core::TDevice(physical_device, nullptr, &enable_device_extensions, &physical_device_features);
    Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> queue = device->GetBestGraphicsQueue();

    Turbo::Core::TRefPtr<Turbo::Extension::TSurface> surface = new Turbo::Extension::TSurface(device, nullptr, vk_surface_khr);
    uint32_t max_image_count = surface->GetMaxImageCount();
    uint32_t min_image_count = surface->GetMinImageCount();

    uint32_t swapchain_image_count = max_image_count <= min_image_count ? min_image_count : max_image_count - 1;

    Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> swapchain = new Turbo::Extension::TSwapchain(surface, swapchain_image_count, Turbo::Core::TFormatType::B8G8R8A8_SRGB, 1, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST | Turbo::Core::TImageUsageBits::IMAGE_SAMPLED, true);

    std::vector<Turbo::Core::TImage *> swapchain_images = swapchain->GetImages();

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
    for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
    {
        Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        swapchain_image_views.push_back(swapchain_view);
    }

    Turbo::Core::TRefPtr<Turbo::Core::TCommandBufferPool> command_pool = new Turbo::Core::TCommandBufferPool(queue);
    Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> command_buffer = command_pool->Allocate();

    Turbo::Core::TRefPtr<Turbo::Core::TImage> ktx_sky_cube_image = nullptr;
    //<KTX SkyCube Texture>
    {
        ktxResult result;
        ktxTexture *ktx_texture;

        result = ktxTexture_CreateFromNamedFile((asset_root + "/images/cubemap_yokohama_rgba.ktx").c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktx_texture);
        uint32_t ktx_texture_width = ktx_texture->baseWidth;
        uint32_t ktx_texture_height = ktx_texture->baseHeight;
        uint32_t ktx_texture_mip_levels = ktx_texture->numLevels;

        ktx_uint8_t *ktx_texture_data = ktx_texture->pData;
        ktx_size_t ktx_texture_size = ktx_texture->dataSize;

        Turbo::Core::TRefPtr<Turbo::Core::TBuffer> ktx_staging_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, ktx_texture_size);
        void *ktx_ptr = ktx_staging_buffer->Map();
        memcpy(ktx_ptr, ktx_texture_data, ktx_texture_size);
        ktx_staging_buffer->Unmap();

        ktx_sky_cube_image = new Turbo::Core::TImage(device, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT /*for cubemap*/, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R8G8B8A8_UNORM, ktx_texture_width, ktx_texture_height, 1, ktx_texture_mip_levels, 6 /*for cubemap six faces*/, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST | Turbo::Core::TImageUsageBits::IMAGE_SAMPLED, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);

        Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> ktx_command_buffer = command_pool->Allocate();
        ktx_command_buffer->Begin();
        ktx_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::HOST_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::HOST_WRITE_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, ktx_sky_cube_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, ktx_texture_mip_levels, 0, 6 /*for cubemap six faces*/);
        for (uint32_t face = 0; face < 6; face++)
        {
            for (uint32_t mip_index = 0; mip_index < ktx_texture_mip_levels; mip_index++)
            {
                uint32_t copy_width = ktx_texture_width >> mip_index;
                uint32_t copy_height = ktx_texture_height >> mip_index;
                uint32_t copy_mip_level = mip_index;
                uint32_t copy_base_array_layer = face;
                ktx_size_t copy_buffer_offset = 0;
                KTX_error_code ret = ktxTexture_GetImageOffset(ktx_texture, mip_index, 0, face, &copy_buffer_offset);
                assert(ret == KTX_SUCCESS);
                ktx_command_buffer->CmdCopyBufferToImage(ktx_staging_buffer, ktx_sky_cube_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, copy_buffer_offset, copy_width, copy_height, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, copy_mip_level, copy_base_array_layer, 1, 0, 0, 0, copy_width, copy_height, 1);
            }
        }
        ktx_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::FRAGMENT_SHADER_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TAccessBits::SHADER_READ_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::SHADER_READ_ONLY_OPTIMAL, ktx_sky_cube_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, ktx_texture_mip_levels, 0, 6 /*for cubemap six faces*/);
        ktx_command_buffer->End();

        Turbo::Core::TRefPtr<Turbo::Core::TFence> ktx_fence = new Turbo::Core::TFence(device);

        queue->Submit(ktx_command_buffer, ktx_fence);

        ktx_fence->WaitUntil();

        command_pool->Free(ktx_command_buffer);
        ktxTexture_Destroy(ktx_texture);
    }
    //</KTX SkyCube Texture>
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> ktx_sky_cube_image_view = new Turbo::Core::TImageView(ktx_sky_cube_image, Turbo::Core::TImageViewType::IMAGE_VIEW_CUBE, ktx_sky_cube_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, ktx_sky_cube_image->GetMipLevels(), 0, 6 /*for cubemap six faces*/);
    Turbo::Core::TRefPtr<Turbo::Core::TSampler> sky_cube_sampler = new Turbo::Core::TSampler(device, Turbo::Core::TFilter::LINEAR, Turbo::Core::TFilter::LINEAR, Turbo::Core::TMipmapMode::LINEAR, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TBorderColor::FLOAT_OPAQUE_WHITE, 0.0f, 0.0f, ktx_sky_cube_image->GetMipLevels());

    Turbo::Core::TRefPtr<Turbo::Core::TSampler> my_sampler = new Turbo::Core::TSampler(device, Turbo::Core::TFilter::LINEAR, Turbo::Core::TFilter::LINEAR, Turbo::Core::TMipmapMode::LINEAR, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TBorderColor::FLOAT_OPAQUE_WHITE, 0.0f, 0.0f, ktx_sky_cube_image->GetMipLevels());

    Turbo::Core::TRefPtr<Turbo::Core::TImage> depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

    Turbo::Core::TRefPtr<Turbo::Core::TVertexShader> my_vertex_shader = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, MY_VERT_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TFragmentShader> my_fragment_shader = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, MY_FRAG_SHADER_STR);

    std::cout << my_vertex_shader->ToString() << std::endl;
    std::cout << my_fragment_shader->ToString() << std::endl;

    std::vector<Turbo::Core::TDescriptorSize> descriptor_sizes;
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::UNIFORM_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::COMBINED_IMAGE_SAMPLER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::SAMPLER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::SAMPLED_IMAGE, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_IMAGE, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::UNIFORM_TEXEL_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_TEXEL_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::UNIFORM_BUFFER_DYNAMIC, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_BUFFER_DYNAMIC, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::INPUT_ATTACHMENT, 1000));

    Turbo::Core::TRefPtr<Turbo::Core::TDescriptorPool> descriptor_pool = new Turbo::Core::TDescriptorPool(device, descriptor_sizes.size() * 1000, descriptor_sizes);

    Turbo::Core::TSubpass subpass(Turbo::Core::TPipelineType::Graphics);
    subpass.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);                // swapchain color image
    subpass.SetDepthStencilAttachmentReference(1, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL); // depth image

    Turbo::Core::TSubpass subpass1(Turbo::Core::TPipelineType::Graphics);
    subpass1.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL); // swapchain color image

    std::vector<Turbo::Core::TSubpass> subpasses;
    subpasses.push_back(subpass);  // subpass 0
    subpasses.push_back(subpass1); // subpass 1

    Turbo::Core::TAttachment swapchain_color_attachment(swapchain_images[0]->GetFormat(), swapchain_images[0]->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::PRESENT_SRC_KHR);
    Turbo::Core::TAttachment depth_attachment(depth_image->GetFormat(), depth_image->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    std::vector<Turbo::Core::TAttachment> attachemts;
    attachemts.push_back(swapchain_color_attachment);
    attachemts.push_back(depth_attachment);

    Turbo::Core::TRefPtr<Turbo::Core::TRenderPass> render_pass = new Turbo::Core::TRenderPass(device, attachemts, subpasses);

    Turbo::Core::TVertexBinding position_binding(0, sizeof(POSITION), Turbo::Core::TVertexRate::VERTEX);
    position_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32B32_SFLOAT, 0); // position
    Turbo::Core::TVertexBinding normal_binding(1, sizeof(NORMAL), Turbo::Core::TVertexRate::VERTEX);
    normal_binding.AddAttribute(1, Turbo::Core::TFormatType::R32G32B32_SFLOAT, 0); // normal
    Turbo::Core::TVertexBinding texcoord_binding(2, sizeof(TEXCOORD), Turbo::Core::TVertexRate::VERTEX);
    texcoord_binding.AddAttribute(2, Turbo::Core::TFormatType::R32G32_SFLOAT, 0); // texcoord/uv

    std::vector<Turbo::Core::TVertexBinding> vertex_bindings;

    Turbo::Core::TViewport viewport(0, 0, surface->GetCurrentWidth(), surface->GetCurrentHeight(), 0, 1);
    Turbo::Core::TScissor scissor(0, 0, surface->GetCurrentWidth(), surface->GetCurrentHeight());

    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> graphics_pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 0, vertex_bindings, my_vertex_shader, my_fragment_shader, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, true, true, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, 0, 0, false, Turbo::Core::TLogicOp::NO_OP, true, Turbo::Core::TBlendFactor::SRC_ALPHA, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendOp::ADD, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendFactor::ZERO, Turbo::Core::TBlendOp::ADD);

    std::vector<std::pair<Turbo::Core::TRefPtr<Turbo::Core::TImageView>, Turbo::Core::TRefPtr<Turbo::Core::TSampler>>> sky_cube_combined_images;
    sky_cube_combined_images.push_back(std::make_pair(ktx_sky_cube_image_view, sky_cube_sampler));

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TSampler>> my_samplers;
    my_samplers.push_back(my_sampler);

    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> graphics_pipeline_descriptor_set = descriptor_pool->Allocate(graphics_pipeline->GetPipelineLayout());
    graphics_pipeline_descriptor_set->BindData(0, 0, 0, sky_cube_combined_images);
    // graphics_pipeline_descriptor_set->BindData(0, 2, 0, my_samplers);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer>> swpachain_framebuffers;
    for (Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_image_view_item : swapchain_image_views)
    {
        std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> image_views;
        image_views.push_back(swapchain_image_view_item);
        image_views.push_back(depth_image_view);

        Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, image_views);
        swpachain_framebuffers.push_back(swapchain_framebuffer);
    }

    //<IMGUI>
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    Turbo::Core::TRefPtr<Turbo::Core::TSampler> imgui_sampler = new Turbo::Core::TSampler(device);

    Turbo::Core::TRefPtr<Turbo::Core::TShader> imgui_vertex_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::VERTEX, Turbo::Core::TShaderLanguage::GLSL, IMGUI_VERT_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TShader> imgui_fragment_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::FRAGMENT, Turbo::Core::TShaderLanguage::GLSL, IMGUI_FRAG_SHADER_STR);

    Turbo::Core::TVertexBinding imgui_vertex_binding(0, sizeof(ImDrawVert), Turbo::Core::TVertexRate::VERTEX);
    imgui_vertex_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32_SFLOAT, IM_OFFSETOF(ImDrawVert, pos));  // position
    imgui_vertex_binding.AddAttribute(1, Turbo::Core::TFormatType::R32G32_SFLOAT, IM_OFFSETOF(ImDrawVert, uv));   // uv
    imgui_vertex_binding.AddAttribute(2, Turbo::Core::TFormatType::R8G8B8A8_UNORM, IM_OFFSETOF(ImDrawVert, col)); // color

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TShader>> imgui_shaders;
    imgui_shaders.push_back(imgui_vertex_shader);
    imgui_shaders.push_back(imgui_fragment_shader);

    std::vector<Turbo::Core::TVertexBinding> imgui_vertex_bindings;
    imgui_vertex_bindings.push_back(imgui_vertex_binding);

    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> imgui_pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 1, imgui_vertex_bindings, imgui_shaders, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, false, false, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, 0, 0, false, Turbo::Core::TLogicOp::NO_OP, true, Turbo::Core::TBlendFactor::SRC_ALPHA, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendOp::ADD, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendFactor::ZERO, Turbo::Core::TBlendOp::ADD);

    unsigned char *imgui_font_pixels;
    int imgui_font_width, imgui_font_height;
    io.Fonts->GetTexDataAsRGBA32(&imgui_font_pixels, &imgui_font_width, &imgui_font_height);
    size_t imgui_upload_size = imgui_font_width * imgui_font_height * 4 * sizeof(char);

    Turbo::Core::TRefPtr<Turbo::Core::TImage> imgui_font_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R8G8B8A8_UNORM, imgui_font_width, imgui_font_height, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_SAMPLED | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> imgui_font_image_view = new Turbo::Core::TImageView(imgui_font_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, imgui_font_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
    {
        Turbo::Core::TRefPtr<Turbo::Core::TBuffer> imgui_font_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, imgui_upload_size);
        void *imgui_font_ptr = imgui_font_buffer->Map();
        memcpy(imgui_font_ptr, imgui_font_pixels, imgui_upload_size);
        imgui_font_buffer->Unmap();

        Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> imgui_copy_command_buffer = command_pool->Allocate();
        imgui_copy_command_buffer->Begin();
        imgui_copy_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::HOST_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::HOST_WRITE_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, imgui_font_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        imgui_copy_command_buffer->CmdCopyBufferToImage(imgui_font_buffer, imgui_font_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, 0, imgui_font_width, imgui_font_height, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1, 0, 0, 0, imgui_font_width, imgui_font_height, 1);
        imgui_copy_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::FRAGMENT_SHADER_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TAccessBits::SHADER_READ_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::SHADER_READ_ONLY_OPTIMAL, imgui_font_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        imgui_copy_command_buffer->End();

        Turbo::Core::TRefPtr<Turbo::Core::TFence> imgui_font_copy_fence = new Turbo::Core::TFence(device);
        queue->Submit(imgui_copy_command_buffer, imgui_font_copy_fence);

        imgui_font_copy_fence->WaitUntil();
    }

    std::vector<std::pair<Turbo::Core::TRefPtr<Turbo::Core::TImageView>, Turbo::Core::TRefPtr<Turbo::Core::TSampler>>> imgui_combined_image_samplers;
    imgui_combined_image_samplers.push_back(std::make_pair(imgui_font_image_view, imgui_sampler));

    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> imgui_pipeline_descriptor_set = descriptor_pool->Allocate(imgui_pipeline->GetPipelineLayout());
    imgui_pipeline_descriptor_set->BindData(0, 0, 0, imgui_combined_image_samplers);

    io.Fonts->TexID = (ImTextureID)(intptr_t)(imgui_font_image->GetVkImage());

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> imgui_vertex_buffer = nullptr;
    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> imgui_index_buffer = nullptr;
    //</IMGUI>

    bool show_demo_window = true;
    MY_PUSH_CONSTANTS_DATA my_push_constants_data;
    my_push_constants_data.time = 0;
    my_push_constants_data.resolutionX = 1;
    my_push_constants_data.resolutionY = 1;
    my_push_constants_data.isClick = false;
    my_push_constants_data.radius = 10.;

    Turbo::Core::TRefPtr<Turbo::Core::TImageView> pervious_frame = nullptr;
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> current_frame = nullptr;

    bool is_first = true;
    uint32_t _frame = 0;

    float _time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        //<Begin Rendering>
        uint32_t current_image_index = UINT32_MAX;
        Turbo::Core::TRefPtr<Turbo::Core::TSemaphore> wait_image_ready = new Turbo::Core::TSemaphore(device, Turbo::Core::TPipelineStageBits::COLOR_ATTACHMENT_OUTPUT_BIT);
        Turbo::Core::TResult result = swapchain->AcquireNextImageUntil(wait_image_ready, nullptr, &current_image_index);
        switch (result)
        {
        case Turbo::Core::TResult::SUCCESS: {
            // successed get image and go on rendering

            if (!is_first)
            {
                pervious_frame = current_frame;
                current_frame = swapchain_image_views[current_image_index];
            }
            else
            {
                pervious_frame = swapchain_image_views[current_image_index];
                current_frame = swapchain_image_views[current_image_index];

                is_first = false;
            }

            // because we just have one command buffer, so we should reset the command buffer for each frame
            // If we create command buffer for each swapchain image, we don't need to reset it each frame

            //<IMGUI Update>
            int window_w, window_h;
            int display_w, display_h;
            glfwGetWindowSize(window, &window_w, &window_h);
            glfwGetFramebufferSize(window, &display_w, &display_h);
            io.DisplaySize = ImVec2((float)window_w, (float)window_h);
            if (window_w > 0 && window_h > 0)
            {
                io.DisplayFramebufferScale = ImVec2((float)display_w / window_w, (float)display_h / window_h);
            }
            double current_time = glfwGetTime();
            io.DeltaTime = _time > 0.0 ? (float)(current_time - _time) : (float)(1.0f / 60.0f);
            _time = current_time;

            // UpdateMousePosAndButtons
            {
                // Update buttons
                ImGuiIO &io = ImGui::GetIO();
                for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
                {
                    // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
                    io.MouseDown[i] = g_MouseJustPressed[i] || glfwGetMouseButton(window, i) != 0;
                    g_MouseJustPressed[i] = false;
                }

                // Update mouse position
                const ImVec2 mouse_pos_backup = io.MousePos;
                io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

                const bool focused = glfwGetWindowAttrib(window, GLFW_FOCUSED) != 0;
                if (focused)
                {
                    if (io.WantSetMousePos)
                    {
                        glfwSetCursorPos(window, (double)mouse_pos_backup.x, (double)mouse_pos_backup.y);
                    }
                    else
                    {
                        double mouse_x, mouse_y;
                        glfwGetCursorPos(window, &mouse_x, &mouse_y);
                        io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
                    }
                }
            }

            // IUpdateMouseCursor
            {
                ImGuiIO &io = ImGui::GetIO();
                if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
                    break;

                ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
                if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
                {
                    // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                }
                else
                {
                    // Show OS mouse cursor
                    // FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with GLFW 3.2, but 3.3 works here.
                    glfwSetCursor(window, g_MouseCursors[imgui_cursor] ? g_MouseCursors[imgui_cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
            }

            ImGui::NewFrame();

            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin(TURBO_PROJECT_NAME); // Create a window called "Hello, world!" and append into it.
                ImGui::SliderFloat("Radius", &my_push_constants_data.radius, 5, 20);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }
            //</IMGUI Update>

            Turbo::Core::TViewport frame_viewport(0, 0, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight(), 0, 1);
            Turbo::Core::TScissor frame_scissor(0, 0, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight());

            std::vector<Turbo::Core::TViewport> frame_viewports;
            frame_viewports.push_back(frame_viewport);

            std::vector<Turbo::Core::TScissor> frame_scissors;
            frame_scissors.push_back(frame_scissor);

            command_buffer->Begin();
            command_buffer->CmdBeginRenderPass(render_pass, swpachain_framebuffers[current_image_index]);

            // std::vector<Turbo::Core::TImageView *> pervious_frames;
            // pervious_frames.push_back(pervious_frame);
            //  graphics_pipeline_descriptor_set->BindData(0, 1, 0, pervious_frames);

            // Draw
            command_buffer->CmdBindPipeline(graphics_pipeline);
            command_buffer->CmdSetViewport(frame_viewports);
            command_buffer->CmdSetScissor(frame_scissors);
            command_buffer->CmdBindPipelineDescriptorSet(graphics_pipeline_descriptor_set);
            my_push_constants_data.time = _time;
            my_push_constants_data.resolutionX = swapchain->GetWidth();
            my_push_constants_data.resolutionY = swapchain->GetHeight();
            auto mouse_click = io.MouseDown[0];
            auto mouse_pos = io.MousePos;
            my_push_constants_data.mouseX = mouse_pos.x;
            my_push_constants_data.mouseY = mouse_pos.y;
            my_push_constants_data.isClick = mouse_click;
            my_push_constants_data.frame = _frame;
            command_buffer->CmdPushConstants(0, sizeof(my_push_constants_data), &my_push_constants_data);
            command_buffer->CmdDraw(3, 1, 0, 0);

            command_buffer->CmdNextSubpass();

            //<IMGUI Rendering>
            ImGui::Render();
            ImDrawData *draw_data = ImGui::GetDrawData();
            const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

            if (!is_minimized)
            {
                // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
                int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
                int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
                if (fb_width <= 0 || fb_height <= 0)
                {
                    break;
                }

                if (draw_data->TotalVtxCount > 0)
                {
                    size_t vertex_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
                    size_t index_size = draw_data->TotalIdxCount * sizeof(ImDrawIdx);

                    if (imgui_vertex_buffer != nullptr)
                    {
                        imgui_vertex_buffer = nullptr;
                    }

                    if (imgui_index_buffer != nullptr)
                    {
                        imgui_index_buffer = nullptr;
                    }

                    imgui_vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, vertex_size);
                    imgui_index_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_INDEX_BUFFER, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, index_size);

                    ImDrawVert *vtx_dst = (ImDrawVert *)imgui_vertex_buffer->Map();
                    ImDrawIdx *idx_dst = (ImDrawIdx *)imgui_index_buffer->Map();
                    for (int n = 0; n < draw_data->CmdListsCount; n++)
                    {
                        const ImDrawList *cmd_list = draw_data->CmdLists[n];
                        memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
                        memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
                        vtx_dst += cmd_list->VtxBuffer.Size;
                        idx_dst += cmd_list->IdxBuffer.Size;
                    }
                    imgui_vertex_buffer->Unmap();
                    imgui_index_buffer->Unmap();

                    command_buffer->CmdBindPipeline(imgui_pipeline);
                    command_buffer->CmdBindPipelineDescriptorSet(imgui_pipeline_descriptor_set);

                    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> imgui_vertex_buffers;
                    imgui_vertex_buffers.push_back(imgui_vertex_buffer);
                    command_buffer->CmdBindVertexBuffers(imgui_vertex_buffers);
                    command_buffer->CmdBindIndexBuffer(imgui_index_buffer, 0, sizeof(ImDrawIdx) == 2 ? Turbo::Core::TIndexType::UINT16 : Turbo::Core::TIndexType::UINT32);

                    float scale[2];
                    scale[0] = 2.0f / draw_data->DisplaySize.x;
                    scale[1] = 2.0f / draw_data->DisplaySize.y;
                    float translate[2];
                    translate[0] = -1.0f - draw_data->DisplayPos.x * scale[0];
                    translate[1] = -1.0f - draw_data->DisplayPos.y * scale[1];

                    command_buffer->CmdPushConstants(sizeof(float) * 0, sizeof(float) * 2, scale);
                    command_buffer->CmdPushConstants(sizeof(float) * 2, sizeof(float) * 2, translate);

                    ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
                    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

                    int global_vtx_offset = 0;
                    int global_idx_offset = 0;

                    for (int n = 0; n < draw_data->CmdListsCount; n++)
                    {
                        const ImDrawList *cmd_list = draw_data->CmdLists[n];
                        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
                        {
                            const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];
                            if (pcmd->UserCallback != NULL)
                            {
                                // User callback, registered via ImDrawList::AddCallback()
                                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                                {
                                    command_buffer->CmdBindPipeline(imgui_pipeline);
                                    command_buffer->CmdBindPipelineDescriptorSet(imgui_pipeline_descriptor_set);

                                    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> __imgui_vertex_buffers;
                                    __imgui_vertex_buffers.push_back(imgui_vertex_buffer);
                                    command_buffer->CmdBindVertexBuffers(imgui_vertex_buffers);
                                    command_buffer->CmdBindIndexBuffer(imgui_index_buffer, 0, sizeof(ImDrawIdx) == 2 ? Turbo::Core::TIndexType::UINT16 : Turbo::Core::TIndexType::UINT32);

                                    float __scale[2];
                                    __scale[0] = 2.0f / draw_data->DisplaySize.x;
                                    __scale[1] = 2.0f / draw_data->DisplaySize.y;
                                    float __translate[2];
                                    __translate[0] = -1.0f - draw_data->DisplayPos.x * scale[0];
                                    __translate[1] = -1.0f - draw_data->DisplayPos.y * scale[1];

                                    command_buffer->CmdPushConstants(sizeof(float) * 0, sizeof(float) * 2, __scale);
                                    command_buffer->CmdPushConstants(sizeof(float) * 2, sizeof(float) * 2, __translate);
                                }
                                else
                                    pcmd->UserCallback(cmd_list, pcmd);
                            }
                            else
                            {
                                // Project scissor/clipping rectangles into framebuffer space
                                ImVec4 clip_rect;
                                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                                {
                                    // Negative offsets are illegal for vkCmdSetScissor
                                    if (clip_rect.x < 0.0f)
                                        clip_rect.x = 0.0f;
                                    if (clip_rect.y < 0.0f)
                                        clip_rect.y = 0.0f;

                                    // Apply scissor/clipping rectangle
                                    VkRect2D scissor;
                                    scissor.offset.x = (int32_t)(clip_rect.x);
                                    scissor.offset.y = (int32_t)(clip_rect.y);
                                    scissor.extent.width = (uint32_t)(clip_rect.z - clip_rect.x);
                                    scissor.extent.height = (uint32_t)(clip_rect.w - clip_rect.y);

                                    Turbo::Core::TScissor imgui_scissor(scissor.offset.x, scissor.offset.y, scissor.extent.width, scissor.extent.height);
                                    std::vector<Turbo::Core::TScissor> imgui_scissors;
                                    imgui_scissors.push_back(imgui_scissor);
                                    command_buffer->CmdSetScissor(imgui_scissors);

                                    // Draw
                                    command_buffer->CmdDrawIndexed(pcmd->ElemCount, 1, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset, 0);
                                }
                            }
                        }
                        global_idx_offset += cmd_list->IdxBuffer.Size;
                        global_vtx_offset += cmd_list->VtxBuffer.Size;
                    }
                }
            }
            //</IMGUI Rendering>

            command_buffer->CmdEndRenderPass();
            command_buffer->End();

            Turbo::Core::TRefPtr<Turbo::Core::TFence> fence = new Turbo::Core::TFence(device);
            std::vector<Turbo::Core::TRefPtr<Turbo::Core::TSemaphore>> wait_semaphores;
            wait_semaphores.push_back(wait_image_ready);

            queue->Submit(wait_semaphores, {}, command_buffer, fence);

            fence->WaitUntil(); // or you can use semaphore to wait for get higher performance

            command_buffer->Reset(); // you can create an command buffer each for one swapchain image,for now just one command buffer

            Turbo::Core::TResult present_result = queue->Present(swapchain, current_image_index);
            switch (present_result)
            {
            case Turbo::Core::TResult::MISMATCH: {
                // the size of the window had changed you need to recreate swapchain

                // waite for idle
                device->WaitIdle();

                // destroy related resource
                // clear old swapchain image
                swapchain_images.clear();

                // destroy swapchain image views
                swapchain_image_views.clear();

                // destroy depth image and view
                // destroy framebuffer
                swpachain_framebuffers.clear();

                // recreate swapchain
                Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> new_swapchain = new Turbo::Extension::TSwapchain(swapchain);
                swapchain = new_swapchain;

                // recreate swapchain image views
                swapchain_images = swapchain->GetImages();
                for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
                {
                    Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                    swapchain_image_views.push_back(swapchain_view);
                }

                // recreate depth image and view
                depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
                depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

                // recreate framebuffer
                for (Turbo::Core::TRefPtr<Turbo::Core::TImageView> image_view_item : swapchain_image_views)
                {
                    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
                    swapchain_image_views.push_back(image_view_item);
                    swapchain_image_views.push_back(depth_image_view);

                    Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, swapchain_image_views);
                    swpachain_framebuffers.push_back(swapchain_framebuffer);
                }
            }
            break;
            default: {
                //
            }
            break;
            }
        }
        break;
        case Turbo::Core::TResult::TIMEOUT: {
            // you need to wait, or do something else
        }
        break;
        case Turbo::Core::TResult::NOT_READY: {
            // you need to wait, or do something else
        }
        break;
        case Turbo::Core::TResult::MISMATCH: {
            // the size of the window had changed you need to recreate swapchain

            // waite for idle
            device->WaitIdle();

            // destroy related resource
            // clear old swapchain image
            swapchain_images.clear();

            // destroy swapchain image views
            swapchain_image_views.clear();

            // destroy depth image and view
            // destroy framebuffer
            swpachain_framebuffers.clear();

            // recreate swapchain
            Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> new_swapchain = new Turbo::Extension::TSwapchain(swapchain);
            swapchain = new_swapchain;

            // recreate swapchain image views
            swapchain_images = swapchain->GetImages();
            for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
            {
                Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                swapchain_image_views.push_back(swapchain_view);
            }

            // recreate depth image and view
            depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
            depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

            // recreate framebuffer
            for (Turbo::Core::TRefPtr<Turbo::Core::TImageView> image_view_item : swapchain_image_views)
            {
                std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
                swapchain_image_views.push_back(image_view_item);
                swapchain_image_views.push_back(depth_image_view);

                Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, swapchain_image_views);
                swpachain_framebuffers.push_back(swapchain_framebuffer);
            }
        }
        break;
        default: {
            //
        }
        break;
        }
        _frame = _frame + 1;
        //</End Rendering>
    }

    descriptor_pool->Free(graphics_pipeline_descriptor_set);
    descriptor_pool->Free(imgui_pipeline_descriptor_set);
    command_pool->Free(command_buffer);

    glfwTerminate();

    return 0;
}
