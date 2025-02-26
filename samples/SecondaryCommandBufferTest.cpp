#include <TDevice.h>
#include <TDeviceQueue.h>
#include <TEngine.h>
#include <TPhysicalDevice.h>

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

#include <GLFW/glfw3.h>

#include <TSurface.h>
#include <TSwapchain.h>

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

const std::string SKY_VERT_SHADER_STR = "#version 450\n"
                                        "layout(set = 0, binding = 0) uniform UBO{\n"
                                        "    mat4 mvp;\n"
                                        "}ubo;\n"
                                        "layout(location = 0) in vec3 inPos;\n"
                                        "layout(location = 0) out vec3 outUVW;\n"
                                        "void main()\n"
                                        "{\n"
                                        "    outUVW = inPos;\n"
                                        "    // Convert cubemap coordinates into Vulkan coordinate space\n"
                                        "    outUVW.xy *= -1.0;\n"
                                        "    gl_Position = ubo.mvp * vec4(inPos.xyz, 1.0);\n"
                                        "}\n";

const std::string SKY_FRAG_SHADER_STR = "#version 450\n"
                                        "layout(set = 0, binding = 1) uniform samplerCube samplerCubeMap;\n"
                                        "layout(location = 0) in vec3 inUVW;\n"
                                        "layout(location = 0) out vec4 outFragColor;\n"
                                        "layout (location = 1) out vec4 outCustomColor;\n"
                                        "void main()\n"
                                        "{\n"
                                        "    outFragColor = texture(samplerCubeMap, inUVW);\n"
                                        "    outCustomColor = outFragColor;\n"
                                        "}\n";

const std::string VERT_SHADER_STR = "#version 450 core\n"
                                    "layout (set = 0, binding = 0) uniform bufferVals {\n"
                                    "    float value;\n"
                                    "    float camX;\n"
                                    "    float camY;\n"
                                    "    float camZ;\n"
                                    "} myBufferVals;\n"
                                    "layout (set = 1, binding = 0) uniform mvpBuffer {\n"
                                    "    mat4 mvp;\n"
                                    "    mat4 m;\n"
                                    "} mvpBufferVals;\n"
                                    "layout (location = 0) in vec3 pos;\n"
                                    "layout (location = 1) in vec3 normal;\n"
                                    "layout (location = 2) in vec2 inUV;\n"
                                    "layout (location = 0) out vec4 outNormal;\n"
                                    "layout (location = 1) out vec2 outUV;\n"
                                    "layout (location = 2) out float outValue;\n"
                                    "layout (location = 3) out vec4 outSunPosition;\n"
                                    "layout (location = 4) out vec3 outPosition;\n"
                                    "layout (location = 5) out vec4 outCamPos;\n"
                                    "void main() {\n"
                                    "   gl_Position =mvpBufferVals.mvp * vec4(pos.xyz,1);\n"
                                    "   outNormal = mvpBufferVals.m * vec4(normal.xyz,1);\n"
                                    "   outPosition =(mvpBufferVals.m * vec4(pos.xyz,1)).xyz;\n"
                                    "   outUV = inUV;\n"
                                    "   outValue = myBufferVals.value;\n"
                                    "   outSunPosition = /*mvpBufferVals.m **/vec4(-10,-10,-10,1);\n"
                                    "   vec3 camPos=vec3(myBufferVals.camX,myBufferVals.camY,myBufferVals.camZ);\n"
                                    "   outCamPos = /*mvpBufferVals.m **/ vec4(camPos,1);\n"
                                    "}\n";

const std::string FRAG_SHADER_STR = "#version 450 core\n"
                                    "layout (set = 0, binding = 1) uniform texture2D myTexture;\n"
                                    "layout (set = 1, binding = 1) uniform samplerCube samplerColor;\n"
                                    "layout (set = 2, binding = 2) uniform sampler mySampler;\n"
                                    "layout (location = 0) in vec4 normal;\n"
                                    "layout (location = 1) in vec2 uv;\n"
                                    "layout (location = 2) in float inValue;\n"
                                    "layout (location = 3) in vec4 sunPosition;\n"
                                    "layout (location = 4) in vec3 inPosition;\n"
                                    "layout (location = 5) in vec4 inCamPos;\n"
                                    "layout (location = 0) out vec4 outColor;\n"
                                    "layout (location = 1) out vec4 outCustomColor;\n"
                                    "layout (push_constant) uniform my_push_constants_t\n"
                                    "{"
                                    "   float alpha;\n"
                                    "   float metallic;\n"
                                    "   float roughness;\n"
                                    "   float intensity;\n"
                                    "} my_push_constants;\n"
                                    "const float PI = 3.14159265359;\n"
                                    "vec3 materialcolor()\n"
                                    "{\n"
                                    "	return vec3(1, 0, 0);\n"
                                    "}\n"
                                    "float D_GGX(float dotNH, float roughness)\n"
                                    "{\n"
                                    "	float alpha = roughness * roughness;\n"
                                    "	float alpha2 = alpha * alpha;\n"
                                    "	float denom = dotNH * dotNH * (alpha2 - 1.0) + 1.0;\n"
                                    "	return (alpha2)/(PI * denom*denom); \n"
                                    "}\n"
                                    "float G_SchlicksmithGGX(float dotNL, float dotNV, float roughness)\n"
                                    "{\n"
                                    "	float r = (roughness + 1.0);\n"
                                    "	float k = (r*r) / 8.0;\n"
                                    "	float GL = dotNL / (dotNL * (1.0 - k) + k);\n"
                                    "	float GV = dotNV / (dotNV * (1.0 - k) + k);\n"
                                    "	return GL * GV;\n"
                                    "}\n"
                                    "vec3 F_Schlick(float cosTheta, float metallic)\n"
                                    "{\n"
                                    "	vec3 F0 = mix(vec3(0.04), materialcolor(), metallic);\n"
                                    "	vec3 F = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0); \n"
                                    "	return F;\n"
                                    "}\n"
                                    "vec3 BRDF(vec3 L, vec3 V, vec3 N, float metallic, float roughness)\n"
                                    "{\n"
                                    "	vec3 H = normalize (V + L);\n"
                                    "	float dotNV = clamp(dot(N, V), 0.0, 1.0);\n"
                                    "	float dotNL = clamp(dot(N, L), 0.0, 1.0);\n"
                                    "	float dotLH = clamp(dot(L, H), 0.0, 1.0);\n"
                                    "	float dotNH = clamp(dot(N, H), 0.0, 1.0);\n"
                                    "	vec3 lightColor = vec3(1.0);\n"
                                    "	vec3 color = vec3(0.0);\n"
                                    "	if (dotNL > 0.0)\n"
                                    "	{\n"
                                    "		float rroughness = max(0.05, roughness);\n"
                                    "		float D = D_GGX(dotNH, roughness); \n"
                                    "		float G = G_SchlicksmithGGX(dotNL, dotNV, rroughness);\n"
                                    "		vec3 F = F_Schlick(dotNV, metallic);\n"
                                    "		vec3 spec = D * F * G / (4.0 * dotNL * dotNV);\n"
                                    "		color += spec * dotNL * lightColor;\n"
                                    "	}\n"
                                    "	return color;\n"
                                    "}\n"
                                    "void main() {\n"
                                    "	float load_bias = my_push_constants.alpha * 10;\n"
                                    "	vec3 normalize_position = normalize(inPosition);\n"
                                    "	vec3 reflect_dir = reflect(normalize_position, normalize(normal.xyz));\n"
                                    "	vec4 sky_cube_color = texture(samplerColor, reflect_dir, 0);\n"
                                    "	vec3 albedo = /*pow(vec3(1,0,0),vec3(2.2))*//*texture(sampler2D(myTexture, mySampler), uv, 0).xyz**/sky_cube_color.xyz;\n"

                                    "	vec3 N = normalize(normal.xyz);\n"
                                    "	vec3 V = normalize(inCamPos.xyz - inPosition);\n"
                                    "	float roughness = my_push_constants.roughness;\n"

                                    "	vec3 Lo = vec3(0.0);\n"
                                    "   vec3 L=normalize(vec3(1,1,1));\n"
                                    "   Lo += BRDF(L, V, N, my_push_constants.metallic, roughness);\n"

                                    "	vec3 lightIntensity = vec3(my_push_constants.intensity);\n"
                                    "	vec3 radiance = lightIntensity;\n"

                                    "   vec3 ambient = materialcolor()*0.02/**texture(sampler2D(myTexture, mySampler), uv, 0).xyz*/;\n"
                                    "   vec3 _color = ambient + Lo;\n"
                                    "   _color = pow(_color, vec3(0.4545));\n"
                                    "   outCustomColor = vec4(_color,1.0);\n"
                                    "}\n";

const std::string INPUT_ATTACHMENT_VERT_SHADER_STR = "#version 450\n"
                                                     "out gl_PerVertex {\n"
                                                     "	vec4 gl_Position;\n"
                                                     "};\n"
                                                     "void main() \n"
                                                     "{\n"
                                                     "	gl_Position = vec4(vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2) * 2.0f - 1.0f, 0.0f, 1.0f);\n"
                                                     "}";

const std::string INPUT_ATTACHMENT_FRAG_SHADER_STR = "#version 450\n"
                                                     "layout (input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput inputColor;\n"
                                                     "layout (input_attachment_index = 1, set = 0, binding = 1) uniform subpassInput inputDepth;\n"
                                                     "layout (push_constant) uniform my_push_constants_t\n"
                                                     "{"
                                                     "   int isOutputDepth;\n"
                                                     "} my_push_constants;\n"
                                                     "layout (location = 0) out vec4 outColor;\n"
                                                     "void main() {\n"
                                                     "    if(my_push_constants.isOutputDepth == 0)\n"
                                                     "    {\n"
                                                     "        outColor=subpassLoad(inputColor).rgba;\n"
                                                     "    }\n"
                                                     "    else\n"
                                                     "    { \n"
                                                     "        float alpha=subpassLoad(inputColor).a;\n"
                                                     "        float depth=subpassLoad(inputDepth).r;\n"
                                                     "        outColor=vec4(depth,depth,depth,alpha);\n"
                                                     "    }\n"
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

struct PUSH_CONSTANT_DATA
{
    float alpha;
    float metallic;
    float roughness;
    float intensity;
};

struct MY_BUFFER_DATA
{
    float value;
    POSITION camPos;
};

struct MVP_BUFFER_DATA
{
    glm::mat4 mvp;
    glm::mat4 m;
};

int main()
{

    std::cout << "Vulkan Version:" << Turbo::Core::TVulkanLoader::Instance()->GetVulkanVersion().ToString() << std::endl;

    // float value = -10.0f;

    PUSH_CONSTANT_DATA push_constant_data = {};
    push_constant_data.alpha = 1;
    push_constant_data.intensity = 20;

    MY_BUFFER_DATA my_buffer_data = {};
    my_buffer_data.value = -5;

    MVP_BUFFER_DATA mvp_buffer_data = {};

    //<gltf for Suzanne>
    std::vector<POSITION> POSITION_data;
    std::vector<NORMAL> NORMAL_data;
    std::vector<TEXCOORD> TEXCOORD_data;
    std::vector<uint32_t> INDICES_data;

    {
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;

        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, asset_root + "/models/Suzanne.gltf");
        const tinygltf::Scene &scene = model.scenes[model.defaultScene];
        tinygltf::Node &node = model.nodes[scene.nodes[0]];
        tinygltf::Mesh &mesh = model.meshes[node.mesh];
        tinygltf::Primitive &primitive = mesh.primitives[0];
        int mode = primitive.mode;                                          // 4 is triangle
        int position_accesser_index = primitive.attributes["POSITION"];     // 0
        int normal_accesser_index = primitive.attributes["NORMAL"];         // 1
        int texcoord_0_accesser_index = primitive.attributes["TEXCOORD_0"]; // 2
        int indices_accesser_index = primitive.indices;                     // 3
        tinygltf::Accessor &position_accessor = model.accessors[position_accesser_index];
        tinygltf::Accessor &normal_accessor = model.accessors[normal_accesser_index];
        tinygltf::Accessor &texcoord_0_accessor = model.accessors[texcoord_0_accesser_index];
        tinygltf::Accessor &indices_accessor = model.accessors[indices_accesser_index];

        tinygltf::BufferView &position_buffer_view = model.bufferViews[position_accessor.bufferView];
        tinygltf::BufferView &normal_buffer_view = model.bufferViews[normal_accessor.bufferView];
        tinygltf::BufferView &texcoord_0_buffer_view = model.bufferViews[texcoord_0_accessor.bufferView];
        tinygltf::BufferView &indices_buffer_view = model.bufferViews[indices_accessor.bufferView];

        int position_buffer_index = position_buffer_view.buffer;
        size_t position_buffer_byteLength = position_buffer_view.byteLength;
        int position_buffer_byteOffset = position_buffer_view.byteOffset;
        int position_type = position_accessor.type;

        int normal_buffer_index = normal_buffer_view.buffer;
        size_t normal_buffer_byteLength = normal_buffer_view.byteLength;
        int normal_buffer_byteOffset = normal_buffer_view.byteOffset;
        int normal_type = normal_accessor.type;

        int texcoord_0_buffer_index = texcoord_0_buffer_view.buffer;
        size_t texcoord_0_buffer_byteLength = texcoord_0_buffer_view.byteLength;
        int texcoord_0_buffer_byteOffset = texcoord_0_buffer_view.byteOffset;
        int texcoord_0_type = texcoord_0_accessor.type;

        int indices_buffer_index = indices_buffer_view.buffer;
        size_t indices_buffer_byteLength = indices_buffer_view.byteLength;
        int indices_buffer_byteOffset = indices_buffer_view.byteOffset;
        int indices_type = indices_accessor.type;

        tinygltf::Buffer &position_buffer = model.buffers[position_buffer_index];
        tinygltf::Buffer &normal_buffer = model.buffers[normal_buffer_index];
        tinygltf::Buffer &texcoord_0_buffer = model.buffers[texcoord_0_buffer_index];
        tinygltf::Buffer &indices_buffer = model.buffers[indices_buffer_index];

        std::vector<unsigned char> &position_data = position_buffer.data;
        std::vector<unsigned char> &normal_data = normal_buffer.data;
        std::vector<unsigned char> &texcoord_0_data = texcoord_0_buffer.data;
        std::vector<unsigned char> &indices_data = indices_buffer.data;

        std::vector<unsigned short> temp_indices_data;

        POSITION_data.resize(position_buffer_byteLength / sizeof(POSITION));
        NORMAL_data.resize(normal_buffer_byteLength / sizeof(NORMAL));
        TEXCOORD_data.resize(texcoord_0_buffer_byteLength / sizeof(TEXCOORD));
        temp_indices_data.resize(indices_buffer_byteLength / sizeof(unsigned short));

        memcpy(POSITION_data.data(), position_data.data() + position_buffer_byteOffset, position_buffer_byteLength);
        memcpy(NORMAL_data.data(), normal_data.data() + normal_buffer_byteOffset, normal_buffer_byteLength);
        memcpy(TEXCOORD_data.data(), texcoord_0_data.data() + texcoord_0_buffer_byteOffset, texcoord_0_buffer_byteLength);
        memcpy(temp_indices_data.data(), indices_data.data() + indices_buffer_byteOffset, indices_buffer_byteLength);

        for (unsigned short &temp_indices_item : temp_indices_data)
        {
            INDICES_data.push_back(temp_indices_item);
        }
    }
    //</gltf for Suzanne>

    //<gltf for sky cube>
    std::vector<POSITION> SKY_CUBE_POSITION_data;
    std::vector<NORMAL> SKY_CUBE_NORMAL_data;
    std::vector<TEXCOORD> SKY_CUBE_TEXCOORD_data;
    std::vector<uint32_t> SKY_CUBE_INDICES_data;

    {
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;

        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, asset_root + "/models/cube.gltf");
        const tinygltf::Scene &scene = model.scenes[model.defaultScene];
        tinygltf::Node &node = model.nodes[scene.nodes[2]];
        tinygltf::Mesh &mesh = model.meshes[node.mesh];
        tinygltf::Primitive &primitive = mesh.primitives[0];
        int position_accesser_index = primitive.attributes["POSITION"];
        int normal_accesser_index = primitive.attributes["NORMAL"];
        int texcoord_0_accesser_index = primitive.attributes["TEXCOORD_0"];
        int indices_accesser_index = primitive.indices;
        tinygltf::Accessor &position_accessor = model.accessors[position_accesser_index];
        tinygltf::Accessor &normal_accessor = model.accessors[normal_accesser_index];
        tinygltf::Accessor &texcoord_0_accessor = model.accessors[texcoord_0_accesser_index];
        tinygltf::Accessor &indices_accessor = model.accessors[indices_accesser_index];

        tinygltf::BufferView &position_buffer_view = model.bufferViews[position_accessor.bufferView];
        tinygltf::BufferView &normal_buffer_view = model.bufferViews[normal_accessor.bufferView];
        tinygltf::BufferView &texcoord_0_buffer_view = model.bufferViews[texcoord_0_accessor.bufferView];
        tinygltf::BufferView &indices_buffer_view = model.bufferViews[indices_accessor.bufferView];

        int position_buffer_index = position_buffer_view.buffer;
        size_t position_buffer_byteLength = position_buffer_view.byteLength;
        int position_buffer_byteOffset = position_buffer_view.byteOffset;
        int position_type = position_accessor.type;

        int normal_buffer_index = normal_buffer_view.buffer;
        size_t normal_buffer_byteLength = normal_buffer_view.byteLength;
        int normal_buffer_byteOffset = normal_buffer_view.byteOffset;
        int normal_type = normal_accessor.type;

        int texcoord_0_buffer_index = texcoord_0_buffer_view.buffer;
        size_t texcoord_0_buffer_byteLength = texcoord_0_buffer_view.byteLength;
        int texcoord_0_buffer_byteOffset = texcoord_0_buffer_view.byteOffset;
        int texcoord_0_type = texcoord_0_accessor.type;

        int indices_buffer_index = indices_buffer_view.buffer;
        size_t indices_buffer_byteLength = indices_buffer_view.byteLength;
        int indices_buffer_byteOffset = indices_buffer_view.byteOffset;
        int indices_type = indices_accessor.type;

        tinygltf::Buffer &position_buffer = model.buffers[position_buffer_index];
        tinygltf::Buffer &normal_buffer = model.buffers[normal_buffer_index];
        tinygltf::Buffer &texcoord_0_buffer = model.buffers[texcoord_0_buffer_index];
        tinygltf::Buffer &indices_buffer = model.buffers[indices_buffer_index];

        std::vector<unsigned char> &position_data = position_buffer.data;
        std::vector<unsigned char> &normal_data = normal_buffer.data;
        std::vector<unsigned char> &texcoord_0_data = texcoord_0_buffer.data;
        std::vector<unsigned char> &indices_data = indices_buffer.data;

        std::vector<unsigned short> temp_indices_data;

        SKY_CUBE_POSITION_data.resize(position_buffer_byteLength / sizeof(POSITION));
        SKY_CUBE_NORMAL_data.resize(normal_buffer_byteLength / sizeof(NORMAL));
        SKY_CUBE_TEXCOORD_data.resize(texcoord_0_buffer_byteLength / sizeof(TEXCOORD));
        temp_indices_data.resize(indices_buffer_byteLength / sizeof(unsigned short));

        memcpy(SKY_CUBE_POSITION_data.data(), position_data.data() + position_buffer_byteOffset, position_buffer_byteLength);
        memcpy(SKY_CUBE_NORMAL_data.data(), normal_data.data() + normal_buffer_byteOffset, normal_buffer_byteLength);
        memcpy(SKY_CUBE_TEXCOORD_data.data(), texcoord_0_data.data() + texcoord_0_buffer_byteOffset, texcoord_0_buffer_byteLength);
        memcpy(temp_indices_data.data(), indices_data.data() + indices_buffer_byteOffset, indices_buffer_byteLength);

        for (unsigned short &temp_indices_item : temp_indices_data)
        {
            SKY_CUBE_INDICES_data.push_back(temp_indices_item);
        }
    }
    //</gltf for sky cube>

    uint32_t sky_cube_indices_count = SKY_CUBE_INDICES_data.size();

    uint32_t indices_count = INDICES_data.size();

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

    Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> swapchain = new Turbo::Extension::TSwapchain(surface, swapchain_image_count, Turbo::Core::TFormatType::B8G8R8A8_SRGB, 1, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, true);

    std::vector<Turbo::Core::TImage *> swapchain_images = swapchain->GetImages();

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
    for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
    {
        Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        swapchain_image_views.push_back(swapchain_view);
    }

    Turbo::Core::TRefPtr<Turbo::Core::TCommandBufferPool> command_pool = new Turbo::Core::TCommandBufferPool(queue);
    Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> command_buffer = command_pool->Allocate();
    Turbo::Core::TRefPtr<Turbo::Core::TSecondaryCommandBuffer> secoondary_command_buffer = command_pool->AllocateSecondary();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)swapchain->GetWidth() / (float)swapchain->GetHeight(), 0.1f, 100.0f);

    glm::mat4 mvp = projection * view * model;
    glm::mat4 sky_cube_view = glm::mat4(glm::mat3(view));
    glm::mat4 sky_cube_mvp = projection * sky_cube_view * glm::mat4(1.0f);

    mvp_buffer_data.mvp = mvp;
    mvp_buffer_data.m = model;

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> mvp_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(mvp_buffer_data));
    void *mvp_ptr = mvp_buffer->Map();
    memcpy(mvp_ptr, &mvp_buffer_data, sizeof(mvp_buffer_data));
    mvp_buffer->Unmap();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> sky_cube_mvp_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(sky_cube_mvp));
    void *sky_cube_mvp_ptr = sky_cube_mvp_buffer->Map();
    memcpy(sky_cube_mvp_ptr, &sky_cube_mvp, sizeof(sky_cube_mvp));
    sky_cube_mvp_buffer->Unmap();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> value_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(float));
    void *value_ptr = value_buffer->Map();
    memcpy(value_ptr, &my_buffer_data, sizeof(my_buffer_data));
    value_buffer->Unmap();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> position_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION) * POSITION_data.size());
    void *position_buffer_ptr = position_buffer->Map();
    memcpy(position_buffer_ptr, POSITION_data.data(), sizeof(POSITION) * POSITION_data.size());
    position_buffer->Unmap();
    POSITION_data.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> sky_cube_position_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION) * SKY_CUBE_POSITION_data.size());
    void *sky_cube_position_buffer_ptr = sky_cube_position_buffer->Map();
    memcpy(sky_cube_position_buffer_ptr, SKY_CUBE_POSITION_data.data(), sizeof(POSITION) * SKY_CUBE_POSITION_data.size());
    sky_cube_position_buffer->Unmap();
    SKY_CUBE_POSITION_data.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> normal_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(NORMAL) * NORMAL_data.size());
    void *normal_buffer_ptr = normal_buffer->Map();
    memcpy(normal_buffer_ptr, NORMAL_data.data(), sizeof(NORMAL) * NORMAL_data.size());
    normal_buffer->Unmap();
    NORMAL_data.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> sky_cube_normal_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(NORMAL) * SKY_CUBE_NORMAL_data.size());
    void *sky_cube_normal_buffer_ptr = sky_cube_normal_buffer->Map();
    memcpy(sky_cube_normal_buffer_ptr, SKY_CUBE_NORMAL_data.data(), sizeof(NORMAL) * SKY_CUBE_NORMAL_data.size());
    sky_cube_normal_buffer->Unmap();
    SKY_CUBE_NORMAL_data.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> texcoord_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(TEXCOORD) * TEXCOORD_data.size());
    void *texcoord_buffer_ptr = texcoord_buffer->Map();
    memcpy(texcoord_buffer_ptr, TEXCOORD_data.data(), sizeof(TEXCOORD) * TEXCOORD_data.size());
    texcoord_buffer->Unmap();
    TEXCOORD_data.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> sky_cube_texcoord_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(TEXCOORD) * SKY_CUBE_TEXCOORD_data.size());
    void *sky_cube_texcoord_buffer_ptr = sky_cube_texcoord_buffer->Map();
    memcpy(sky_cube_texcoord_buffer_ptr, SKY_CUBE_TEXCOORD_data.data(), sizeof(TEXCOORD) * SKY_CUBE_TEXCOORD_data.size());
    sky_cube_texcoord_buffer->Unmap();
    SKY_CUBE_TEXCOORD_data.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> index_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_INDEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(uint32_t) * INDICES_data.size());
    void *index_buffer_ptr = index_buffer->Map();
    memcpy(index_buffer_ptr, INDICES_data.data(), sizeof(uint32_t) * INDICES_data.size());
    index_buffer->Unmap();
    INDICES_data.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> sky_cube_index_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_INDEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(uint32_t) * SKY_CUBE_INDICES_data.size());
    void *sky_cube_index_buffer_ptr = sky_cube_index_buffer->Map();
    memcpy(sky_cube_index_buffer_ptr, SKY_CUBE_INDICES_data.data(), sizeof(uint32_t) * SKY_CUBE_INDICES_data.size());
    sky_cube_index_buffer->Unmap();
    SKY_CUBE_INDICES_data.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TImage> ktx_image = nullptr;
    //<KTX Texture>
    {
        std::string ktx_filename = asset_root + "/images/metalplate01_rgba.ktx";

        ktxTexture *ktx_texture;
        KTX_error_code ktx_result;

        ktx_result = ktxTexture_CreateFromNamedFile(ktx_filename.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktx_texture);

        if (ktx_texture == nullptr)
        {
            throw std::runtime_error("Couldn't load texture");
        }

        uint32_t ktx_texture_width = ktx_texture->baseWidth;
        uint32_t ktx_texture_height = ktx_texture->baseHeight;
        uint32_t ktx_texture_mip_levels = ktx_texture->numLevels;

        ktx_uint8_t *ktx_texture_data = ktx_texture->pData;
        ktx_size_t ktx_texture_size = ktx_texture->dataSize;

        Turbo::Core::TRefPtr<Turbo::Core::TBuffer> ktx_staging_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, ktx_texture_size);
        void *ktx_ptr = ktx_staging_buffer->Map();
        memcpy(ktx_ptr, ktx_texture_data, ktx_texture_size);
        ktx_staging_buffer->Unmap();

        ktx_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R8G8B8A8_UNORM, ktx_texture_width, ktx_texture_height, 1, ktx_texture_mip_levels, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST | Turbo::Core::TImageUsageBits::IMAGE_SAMPLED, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);

        Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> ktx_command_buffer = command_pool->Allocate();
        ktx_command_buffer->Begin();
        ktx_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::HOST_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::HOST_WRITE_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, ktx_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, ktx_texture_mip_levels, 0, 1);
        for (uint32_t mip_index = 0; mip_index < ktx_texture_mip_levels; mip_index++)
        {
            uint32_t copy_width = ktx_texture_width >> mip_index;
            uint32_t copy_height = ktx_texture_height >> mip_index;
            uint32_t copy_mip_level = mip_index;
            ktx_size_t copy_buffer_offset = 0;
            ktx_result = ktxTexture_GetImageOffset(ktx_texture, mip_index, 0, 0, &copy_buffer_offset);
            ktx_command_buffer->CmdCopyBufferToImage(ktx_staging_buffer, ktx_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, copy_buffer_offset, copy_width, copy_height, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, copy_mip_level, 0, 1, 0, 0, 0, copy_width, copy_height, 1);
        }
        ktx_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::FRAGMENT_SHADER_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TAccessBits::SHADER_READ_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::SHADER_READ_ONLY_OPTIMAL, ktx_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, ktx_texture_mip_levels, 0, 1);
        ktx_command_buffer->End();

        Turbo::Core::TRefPtr<Turbo::Core::TFence> ktx_fence = new Turbo::Core::TFence(device);

        queue->Submit(ktx_command_buffer, ktx_fence);

        ktx_fence->WaitUntil();

        // delete ktx_fence;
        // delete ktx_staging_buffer;
        command_pool->Free(ktx_command_buffer);
        ktxTexture_Destroy(ktx_texture);
    }
    //</KTX Texture>
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> ktx_texture_view = new Turbo::Core::TImageView(ktx_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, ktx_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, ktx_image->GetMipLevels(), 0, 1);
    Turbo::Core::TRefPtr<Turbo::Core::TSampler> sampler = new Turbo::Core::TSampler(device, Turbo::Core::TFilter::LINEAR, Turbo::Core::TFilter::LINEAR, Turbo::Core::TMipmapMode::LINEAR, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TAddressMode::REPEAT, Turbo::Core::TBorderColor::FLOAT_OPAQUE_WHITE, 0.0f, 0.0f, ktx_image->GetMipLevels());

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

    Turbo::Core::TRefPtr<Turbo::Core::TImage> color_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> color_image_view = new Turbo::Core::TImageView(color_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, color_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);

    Turbo::Core::TRefPtr<Turbo::Core::TImage> depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

    Turbo::Core::TRefPtr<Turbo::Core::TShader> vertex_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::VERTEX, Turbo::Core::TShaderLanguage::GLSL, VERT_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TShader> fragment_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::FRAGMENT, Turbo::Core::TShaderLanguage::GLSL, FRAG_SHADER_STR);

    Turbo::Core::TRefPtr<Turbo::Core::TShader> input_attachment_vertex_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::VERTEX, Turbo::Core::TShaderLanguage::GLSL, INPUT_ATTACHMENT_VERT_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TShader> input_attachment_fragment_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::FRAGMENT, Turbo::Core::TShaderLanguage::GLSL, INPUT_ATTACHMENT_FRAG_SHADER_STR);

    Turbo::Core::TRefPtr<Turbo::Core::TShader> sky_vertex_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::VERTEX, Turbo::Core::TShaderLanguage::GLSL, SKY_VERT_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TShader> sky_fragment_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::FRAGMENT, Turbo::Core::TShaderLanguage::GLSL, SKY_FRAG_SHADER_STR);

    std::cout << vertex_shader->ToString() << std::endl;
    std::cout << fragment_shader->ToString() << std::endl;

    std::cout << sky_vertex_shader->ToString() << std::endl;
    std::cout << sky_fragment_shader->ToString() << std::endl;

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

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> buffers;
    buffers.push_back(value_buffer);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> mvp_buffers;
    mvp_buffers.push_back(mvp_buffer);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> sky_cube_mvp_buffers;
    sky_cube_mvp_buffers.push_back(sky_cube_mvp_buffer);

    Turbo::Core::TSubpass subpass(Turbo::Core::TPipelineType::Graphics);
    subpass.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);                // swapchain color image
    subpass.AddColorAttachmentReference(1, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);                // custom color image
    subpass.SetDepthStencilAttachmentReference(2, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL); // depth image

    Turbo::Core::TSubpass subpass1(Turbo::Core::TPipelineType::Graphics);
    subpass1.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL); // swapchain color image
    subpass1.AddInputAttachmentReference(1, Turbo::Core::TImageLayout::SHADER_READ_ONLY_OPTIMAL); // custom color image, input attachment
    subpass1.AddInputAttachmentReference(2, Turbo::Core::TImageLayout::SHADER_READ_ONLY_OPTIMAL); // depth image, input attachment

    std::vector<Turbo::Core::TSubpass> subpasses;
    subpasses.push_back(subpass);  // subpass 0
    subpasses.push_back(subpass1); // subpass 1

    Turbo::Core::TAttachment swapchain_color_attachment(swapchain_images[0]->GetFormat(), swapchain_images[0]->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::PRESENT_SRC_KHR);
    Turbo::Core::TAttachment color_attachment(color_image->GetFormat(), color_image->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::PRESENT_SRC_KHR);
    Turbo::Core::TAttachment depth_attachment(depth_image->GetFormat(), depth_image->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    std::vector<Turbo::Core::TAttachment> attachemts;
    attachemts.push_back(swapchain_color_attachment);
    attachemts.push_back(color_attachment);
    attachemts.push_back(depth_attachment);

    Turbo::Core::TRefPtr<Turbo::Core::TRenderPass> render_pass = new Turbo::Core::TRenderPass(device, attachemts, subpasses);

    Turbo::Core::TVertexBinding position_binding(0, sizeof(POSITION), Turbo::Core::TVertexRate::VERTEX);
    position_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32B32_SFLOAT, 0); // position
    Turbo::Core::TVertexBinding normal_binding(1, sizeof(NORMAL), Turbo::Core::TVertexRate::VERTEX);
    normal_binding.AddAttribute(1, Turbo::Core::TFormatType::R32G32B32_SFLOAT, 0); // normal
    Turbo::Core::TVertexBinding texcoord_binding(2, sizeof(TEXCOORD), Turbo::Core::TVertexRate::VERTEX);
    texcoord_binding.AddAttribute(2, Turbo::Core::TFormatType::R32G32_SFLOAT, 0); // texcoord/uv

    std::vector<Turbo::Core::TVertexBinding> vertex_bindings;
    vertex_bindings.push_back(position_binding);
    vertex_bindings.push_back(normal_binding);
    vertex_bindings.push_back(texcoord_binding);

    Turbo::Core::TViewport viewport(0, 0, surface->GetCurrentWidth(), surface->GetCurrentHeight(), 0, 1);
    Turbo::Core::TScissor scissor(0, 0, surface->GetCurrentWidth(), surface->GetCurrentHeight());

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TShader>> shaders{vertex_shader, fragment_shader};
    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TShader>> sky_cube_shaders{sky_vertex_shader, sky_fragment_shader};
    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TShader>> input_attachment_shaders{input_attachment_vertex_shader, input_attachment_fragment_shader};
    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 0, vertex_bindings, shaders, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, true, true, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, 0, 0, false, Turbo::Core::TLogicOp::NO_OP, true, Turbo::Core::TBlendFactor::SRC_ALPHA, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendOp::ADD, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendFactor::ZERO, Turbo::Core::TBlendOp::ADD);
    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> sky_cube_pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 0, vertex_bindings, sky_cube_shaders, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_FRONT_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, false, false, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false);
    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> input_attachment_pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 1, vertex_bindings, input_attachment_shaders, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, true, false, Turbo::Core::TCompareOp::LESS_OR_EQUAL);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> my_textures;
    my_textures.push_back(/*texture_view*/ ktx_texture_view);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TSampler>> my_samples;
    my_samples.push_back(sampler);

    std::vector<std::pair<Turbo::Core::TRefPtr<Turbo::Core::TImageView>, Turbo::Core::TRefPtr<Turbo::Core::TSampler>>> sky_cube_combined_images;
    sky_cube_combined_images.push_back(std::make_pair(ktx_sky_cube_image_view, sky_cube_sampler));

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> input_attachment_colors;
    input_attachment_colors.push_back(color_image_view);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> input_attachment_depths;
    input_attachment_depths.push_back(depth_image_view);

    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> pipeline_descriptor_set = descriptor_pool->Allocate(pipeline->GetPipelineLayout());
    pipeline_descriptor_set->BindData(0, 0, 0, buffers);
    pipeline_descriptor_set->BindData(0, 1, 0, my_textures);
    pipeline_descriptor_set->BindData(1, 0, 0, mvp_buffers);
    pipeline_descriptor_set->BindData(1, 1, 0, sky_cube_combined_images);
    pipeline_descriptor_set->BindData(2, 2, 0, my_samples);

    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> sky_cube_pipeline_descriptor_set = descriptor_pool->Allocate(sky_cube_pipeline->GetPipelineLayout());
    sky_cube_pipeline_descriptor_set->BindData(0, 0, 0, sky_cube_mvp_buffers);
    sky_cube_pipeline_descriptor_set->BindData(0, 1, 0, sky_cube_combined_images);

    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> input_attachment_pipeline_descriptor_set = descriptor_pool->Allocate(input_attachment_pipeline->GetPipelineLayout());
    input_attachment_pipeline_descriptor_set->BindData(0, 0, 0, input_attachment_colors);
    input_attachment_pipeline_descriptor_set->BindData(0, 1, 0, input_attachment_depths);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> vertex_buffers;
    vertex_buffers.push_back(position_buffer);
    vertex_buffers.push_back(normal_buffer);
    vertex_buffers.push_back(texcoord_buffer);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> sky_cube_vertex_buffers;
    sky_cube_vertex_buffers.push_back(sky_cube_position_buffer);
    sky_cube_vertex_buffers.push_back(sky_cube_normal_buffer);
    sky_cube_vertex_buffers.push_back(sky_cube_texcoord_buffer);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer>> swpachain_framebuffers;
    for (Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_image_view_item : swapchain_image_views)
    {
        std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> image_views;
        image_views.push_back(swapchain_image_view_item);
        image_views.push_back(color_image_view);
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
    bool is_shouw_depth = false;

    float angle = 180;
    float _time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        my_buffer_data.camPos.x = 0;
        my_buffer_data.camPos.y = 0;
        my_buffer_data.camPos.z = -my_buffer_data.value;

        void *_ptr = value_buffer->Map();
        memcpy(_ptr, &my_buffer_data, sizeof(my_buffer_data));
        value_buffer->Unmap();

        model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = model * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, my_buffer_data.value));
        projection = glm::perspective(glm::radians(45.0f), (float)(swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth()) / (float)(swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight()), 0.1f, 300.0f);
        mvp = projection * view * model;

        mvp_buffer_data.mvp = mvp;
        mvp_buffer_data.m = model;

        sky_cube_view = glm::mat4(glm::mat3(view));
        sky_cube_mvp = projection * sky_cube_view /** glm::rotate(glm::mat4(1.0f), glm::radians(_time * 3), glm::vec3(0.0f, 1.0f, 0.0f))*/;

        sky_cube_mvp_ptr = sky_cube_mvp_buffer->Map();
        memcpy(sky_cube_mvp_ptr, &sky_cube_mvp, sizeof(sky_cube_mvp));
        sky_cube_mvp_buffer->Unmap();

        _ptr = mvp_buffer->Map();
        memcpy(_ptr, &mvp_buffer_data, sizeof(mvp_buffer_data));
        mvp_buffer->Unmap();

        //<Begin Rendering>
        uint32_t current_image_index = UINT32_MAX;
        Turbo::Core::TRefPtr<Turbo::Core::TSemaphore> wait_image_ready = new Turbo::Core::TSemaphore(device, Turbo::Core::TPipelineStageBits::COLOR_ATTACHMENT_OUTPUT_BIT);
        Turbo::Core::TResult result = swapchain->AcquireNextImageUntil(wait_image_ready, nullptr, &current_image_index);
        switch (result)
        {
        case Turbo::Core::TResult::SUCCESS: {
            // successed get image and go on rendering

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

                ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)

                ImGui::Checkbox("Is show depth", &is_shouw_depth);

                ImGui::SliderFloat("angle", &angle, 0.0f, 360);                               // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("alpha", &push_constant_data.alpha, 0.0f, 1.0f);           // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("intensity", &push_constant_data.intensity, 0.0f, 100.0f); // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("metallic", &push_constant_data.metallic, 0.0f, 1.0f);     // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("roughness", &push_constant_data.roughness, 0.0f, 1.0f);   // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("value", &my_buffer_data.value, -10.0f, 0.0f);             // Edit 1 float using a slider from 0.0f to 1.0f

                if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

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

            // record secoondary command buffer
            // sky cube
            secoondary_command_buffer->Begin(render_pass, swpachain_framebuffers[current_image_index], 0);
            secoondary_command_buffer->CmdBindPipeline(sky_cube_pipeline);
            secoondary_command_buffer->CmdBindPipelineDescriptorSet(sky_cube_pipeline_descriptor_set);
            secoondary_command_buffer->CmdBindVertexBuffers(sky_cube_vertex_buffers);
            secoondary_command_buffer->CmdSetViewport(frame_viewports);
            secoondary_command_buffer->CmdSetScissor(frame_scissors);
            secoondary_command_buffer->CmdBindIndexBuffer(sky_cube_index_buffer);
            secoondary_command_buffer->CmdDrawIndexed(sky_cube_indices_count, 1, 0, 0, 0);

            // Suzanne
            secoondary_command_buffer->CmdBindPipeline(pipeline);
            secoondary_command_buffer->CmdPushConstants(0, sizeof(push_constant_data), &push_constant_data);
            secoondary_command_buffer->CmdBindPipelineDescriptorSet(pipeline_descriptor_set);
            secoondary_command_buffer->CmdBindVertexBuffers(vertex_buffers);
            secoondary_command_buffer->CmdSetViewport(frame_viewports);
            secoondary_command_buffer->CmdSetScissor(frame_scissors);
            secoondary_command_buffer->CmdBindIndexBuffer(index_buffer);
            secoondary_command_buffer->CmdDrawIndexed(indices_count, 1, 0, 0, 0);
            secoondary_command_buffer->End();

            command_buffer->Begin();
            command_buffer->CmdBeginRenderPass(render_pass, swpachain_framebuffers[current_image_index], Turbo::Core::TSubpassContents::SECONDARY_COMMAND_BUFFERS);
            command_buffer->CmdExecuteCommand(secoondary_command_buffer);
            command_buffer->CmdNextSubpass();
            command_buffer->CmdSetViewport(frame_viewports);
            command_buffer->CmdSetScissor(frame_scissors);
            command_buffer->CmdBindPipeline(input_attachment_pipeline);
            int is_out_put_depth = 0;
            if (is_shouw_depth)
            {
                is_out_put_depth = 1;
                command_buffer->CmdPushConstants(0, sizeof(is_out_put_depth), &is_out_put_depth);
            }
            else
            {
                is_out_put_depth = 0;
                command_buffer->CmdPushConstants(0, sizeof(is_out_put_depth), &is_out_put_depth);
            }
            command_buffer->CmdBindPipelineDescriptorSet(input_attachment_pipeline_descriptor_set);
            command_buffer->CmdDraw(3, 1, 0, 0);

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

            // delete fence;

            command_buffer->Reset(); // you can create an command buffer each for one swapchain image,for now just one command buffer
            secoondary_command_buffer->Reset();

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

                // destroy color image and view

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

                // recreate color image and view
                color_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
                color_image_view = new Turbo::Core::TImageView(color_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, color_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);

                // recreate depth image and view
                depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
                depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

                // recreate framebuffer
                for (Turbo::Core::TRefPtr<Turbo::Core::TImageView> image_view_item : swapchain_image_views)
                {
                    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
                    swapchain_image_views.push_back(image_view_item);
                    swapchain_image_views.push_back(color_image_view);
                    swapchain_image_views.push_back(depth_image_view);

                    Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, swapchain_image_views);
                    swpachain_framebuffers.push_back(swapchain_framebuffer);
                }

                std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> temp_input_attachment_colors;
                temp_input_attachment_colors.push_back(color_image_view);

                std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> temp_input_attachment_depths;
                temp_input_attachment_depths.push_back(depth_image_view);

                input_attachment_pipeline_descriptor_set->BindData(0, 0, 0, temp_input_attachment_colors);
                input_attachment_pipeline_descriptor_set->BindData(0, 1, 0, temp_input_attachment_depths);
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

            // destroy color image and view

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

            // recreate color image and view
            color_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
            color_image_view = new Turbo::Core::TImageView(color_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, color_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);

            // recreate depth image and view
            depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
            depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

            // recreate framebuffer
            for (Turbo::Core::TRefPtr<Turbo::Core::TImageView> image_view_item : swapchain_image_views)
            {
                std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
                swapchain_image_views.push_back(image_view_item);
                swapchain_image_views.push_back(color_image_view);
                swapchain_image_views.push_back(depth_image_view);

                Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, swapchain_image_views);
                swpachain_framebuffers.push_back(swapchain_framebuffer);
            }

            std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> temp_input_attachment_colors;
            temp_input_attachment_colors.push_back(color_image_view);

            std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> temp_input_attachment_depths;
            temp_input_attachment_depths.push_back(depth_image_view);

            input_attachment_pipeline_descriptor_set->BindData(0, 0, 0, temp_input_attachment_colors);
            input_attachment_pipeline_descriptor_set->BindData(0, 1, 0, temp_input_attachment_depths);
        }
        break;
        default: {
            //
        }
        break;
        }

        // delete wait_image_ready;
        //</End Rendering>
    }

    descriptor_pool->Free(imgui_pipeline_descriptor_set);
    descriptor_pool->Free(pipeline_descriptor_set);
    descriptor_pool->Free(sky_cube_pipeline_descriptor_set);
    descriptor_pool->Free(input_attachment_pipeline_descriptor_set);
    command_pool->Free(command_buffer);
    command_pool->Free(secoondary_command_buffer);

    glfwTerminate();

    return 0;
}
