
#include <TFrameGraph.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

void WriteTextFile(const std::string &text, const std::string &filename)
{
    std::ofstream out_stream;
    out_stream.open(filename, std::ios::out | std::ios::trunc);

    if (out_stream.good())
    {
        out_stream << text;
    }

    out_stream.close();
}

using namespace Turbo::FrameGraph;

struct CustomTexture
{
    enum Usage
    {
        Color,
        Depth,
        Normal,
        Shadow
    };

    struct Descriptor
    {
        uint32_t width;
        uint32_t height;
        Usage usage;
    };

    uint32_t value;

    CustomTexture() = default;

    std::string name;

    void Create(const std::string &name, const Descriptor &)
    {
        // custom create
        std::cout << "CustomTexture::Create::" << name << std::endl;
        this->name = name;
    }

    void Destroy()
    {
        // custom destroy
        std::cout << "CustomTexture::Destroy::" << name << std::endl;
    }
};

struct CustomPassData
{
    TResource foo;
    TResource bar;
};

int test0()
{
    TFrameGraph fg;

    fg.AddPass<CustomPassData>(
        "CustomPass",
        [&](TFrameGraph::TBuilder &builder, CustomPassData &data) {
            std::cout << "Pass::Setup" << std::endl;
            std::cout << "CustomPassData::Setup::" << data.bar.id << std::endl;
            std::cout << "CustomPassData::Setup::" << data.foo.id << std::endl;
        },
        [=](const CustomPassData &data, const TResources &resources, void *context) {

        });

    fg.Compile();
    fg.Execute();

    return 0;
}

int test1()
{
    TFrameGraph fg;

    fg.AddPass<CustomPassData>(
        "CustomPass",
        [&](TFrameGraph::TBuilder &builder, CustomPassData &data) {
            data.foo = builder.Create<CustomTexture>("foo", {128, 128, CustomTexture::Color});

            TFrameGraph::TBuilder::TSubpass subpass = builder.CreateSubpass();
            data.foo = subpass.Write(data.foo);

            data.bar = builder.Create<CustomTexture>("bar", {256, 256, CustomTexture::Depth});
            data.bar = subpass.Write(data.bar);
        },
        [=](const CustomPassData &data, const TResources &resources, void *context) {
            int32_t foo_value = resources.Get<CustomTexture>(data.foo).value;
            int32_t bar_value = resources.Get<CustomTexture>(data.bar).value;
        });

    fg.Compile();
    fg.Execute();

    return 0;
}

/*
[Depth Pass]
    ↓
Depth Texture
    ↓
    ↓          ↗ Depth Texture ↘
[GBuffer Pass] → Normal Texture → [Lighting Pass] → Light Texture
               ↘ Color Texture ↗                       ↓
                                                   [Post Pass]
                                                       ↓
                                                RenderTarget Texture
                                                       ↓
                                                  [Present Pass]
*/
int test2()
{
    std::cout << "==========Test FrameGraph Subpass==========" << std::endl;

    TFrameGraph fg;

    struct DepthPassData
    {
        TResource depthTexture;
    };

    fg.AddPass<DepthPassData>(
        "Depth Pass",
        [&](TFrameGraph::TBuilder &builder, DepthPassData &data) {
            data.depthTexture = builder.Create<CustomTexture>("Depth Texture", {128, 128, CustomTexture::Depth});

            TFrameGraph::TBuilder::TSubpass subpass = builder.CreateSubpass();
            data.depthTexture = subpass.Write(data.depthTexture);

            fg.GetBlackboard()["Depth Texture"] = data.depthTexture;
        },
        [=](const DepthPassData &data, const TResources &resources, void *context) {
            CustomTexture &depth_texture = resources.Get<CustomTexture>(data.depthTexture);
            uint32_t depth_texture_value = depth_texture.value;

            std::cout << "Depth Pass Execute" << std::endl;

            /*<Some Rendering/Compute Command...>*/
            /*...*/
            /*</Some Rendering/Compute Command...>*/
        });

    struct GBufferPassData
    {
        TResource depthTexture;
        TResource normalTexture;
        TResource colorTexture;
    };

    fg.AddPass<GBufferPassData>(
        "GBuffer Pass",
        [&](TFrameGraph::TBuilder &builder, GBufferPassData &data) {
            data.depthTexture = fg.GetBlackboard()["Depth Texture"];

            TFrameGraph::TBuilder::TSubpass subpass = builder.CreateSubpass();
            data.depthTexture = subpass.Read(data.depthTexture);
            data.depthTexture = subpass.Write(data.depthTexture);

            data.normalTexture = builder.Create<CustomTexture>("Normal Texture", {128, 128, CustomTexture::Normal});
            data.normalTexture = subpass.Write(data.normalTexture);

            data.colorTexture = builder.Create<CustomTexture>("Color Texture", {128, 128, CustomTexture::Color});
            data.colorTexture = subpass.Write(data.colorTexture);

            //<MultiSubpass>
            TFrameGraph::TBuilder::TSubpass subpass1 = builder.CreateSubpass();
            data.depthTexture = subpass1.Write(data.depthTexture);
            data.normalTexture = subpass1.Write(data.normalTexture);
            data.colorTexture = subpass1.Write(data.colorTexture);

            TFrameGraph::TBuilder::TSubpass subpass2 = builder.CreateSubpass();
            data.depthTexture = subpass2.Write(data.depthTexture);
            data.normalTexture = subpass2.Write(data.normalTexture);
            data.colorTexture = subpass2.Write(data.colorTexture);

            TFrameGraph::TBuilder::TSubpass subpass3 = builder.CreateSubpass();
            data.depthTexture = subpass3.Write(data.depthTexture);
            data.normalTexture = subpass3.Write(data.normalTexture);
            data.colorTexture = subpass3.Write(data.colorTexture);
            //</MultiSubpass>

            fg.GetBlackboard()["Depth Texture"] = data.depthTexture;
            fg.GetBlackboard()["Normal Texture"] = data.normalTexture;
            fg.GetBlackboard()["Color Texture"] = data.colorTexture;
        },
        [=](const GBufferPassData &data, const TResources &resources, void *context) {
            CustomTexture &depth_texture = resources.Get<CustomTexture>(data.depthTexture);
            CustomTexture &normal_texture = resources.Get<CustomTexture>(data.normalTexture);
            CustomTexture &color_texture = resources.Get<CustomTexture>(data.colorTexture);
            uint32_t depth_texture_value = depth_texture.value;
            uint32_t normal_texture_value = normal_texture.value;
            uint32_t color_texture_value = color_texture.value;

            std::cout << "GBuffer Pass Execute" << std::endl;

            /*<Some Rendering/Compute Command...>*/
            /*...*/
            /*</Some Rendering/Compute Command...>*/
        });

    struct LightPassData
    {
        TResource depthTexture;
        TResource normalTexture;
        TResource colorTexture;
        TResource lightTexture;
    };

    fg.AddPass<LightPassData>(
        "Light Pass",
        [&](TFrameGraph::TBuilder &builder, LightPassData &data) {
            TFrameGraph::TBuilder::TSubpass subpass = builder.CreateSubpass();

            data.depthTexture = fg.GetBlackboard()["Depth Texture"];
            data.depthTexture = subpass.Read(data.depthTexture);

            data.normalTexture = fg.GetBlackboard()["Normal Texture"];
            data.normalTexture = subpass.Read(data.normalTexture);

            data.colorTexture = fg.GetBlackboard()["Color Texture"];
            data.colorTexture = subpass.Read(data.colorTexture);

            data.lightTexture = builder.Create<CustomTexture>("Light Texture", {128, 128, CustomTexture::Shadow});
            data.lightTexture = subpass.Write(data.lightTexture);

            fg.GetBlackboard()["Light Texture"] = data.lightTexture;
        },
        [=](const LightPassData &data, const TResources &resources, void *context) {
            CustomTexture &depth_texture = resources.Get<CustomTexture>(data.depthTexture);
            CustomTexture &light_texture = resources.Get<CustomTexture>(data.lightTexture);
            CustomTexture &color_texture = resources.Get<CustomTexture>(data.colorTexture);
            CustomTexture &normal_texture = resources.Get<CustomTexture>(data.normalTexture);

            std::cout << "Light Pass Execute" << std::endl;

            /*<Some Rendering/Compute Command...>*/
            /*...*/
            /*</Some Rendering/Compute Command...>*/
        });

    struct PostPassData
    {
        TResource lightTexture;
        TResource renderTargetTexture;
    };

    fg.AddPass<PostPassData>(
        "Post Pass",
        [&](TFrameGraph::TBuilder &builder, PostPassData &data) {
            TFrameGraph::TBuilder::TSubpass subpass = builder.CreateSubpass();

            data.lightTexture = fg.GetBlackboard()["Light Texture"];
            data.lightTexture = subpass.Read(data.lightTexture);

            data.renderTargetTexture = builder.Create<CustomTexture>("RenderTarget Texture", {128, 128, CustomTexture::Shadow});
            data.renderTargetTexture = subpass.Write(data.renderTargetTexture);

            fg.GetBlackboard()["RenderTarget Texture"] = data.renderTargetTexture;
        },
        [=](const PostPassData &data, const TResources &resources, void *context) {
            CustomTexture &light_texture = resources.Get<CustomTexture>(data.lightTexture);
            CustomTexture &render_target_texture = resources.Get<CustomTexture>(data.renderTargetTexture);

            std::cout << "Post Pass Execute" << std::endl;

            /*<Some Rendering/Compute Command...>*/
            /*...*/
            /*</Some Rendering/Compute Command...>*/
        });

    struct PresentPassData
    {
        TResource renderTargetTexture;

        // TResource testTexture;
    };

    fg.AddPass<PresentPassData>(
        "Present Pass",
        [&](TFrameGraph::TBuilder &builder, PresentPassData &data) {
            TFrameGraph::TBuilder::TSubpass subpass = builder.CreateSubpass();

            data.renderTargetTexture = fg.GetBlackboard()["RenderTarget Texture"];
            data.renderTargetTexture = subpass.Read(data.renderTargetTexture);

            builder.SideEffect();
        },
        [=](const PresentPassData &data, const TResources &resources, void *context) {
            CustomTexture &render_target_texture = resources.Get<CustomTexture>(data.renderTargetTexture);

            std::cout << "Present Pass Execute" << std::endl;

            /*<Some Rendering/Compute Command...>*/
            /*...*/
            /*</Some Rendering/Compute Command...>*/
        });

    fg.Compile();
    std::string mermaid = fg.ToMermaid();
    fg.Execute();

    std::stringstream ss;

    ss << "```mermaid" << std::endl << mermaid << "```" << std::endl;

    WriteTextFile(ss.str(), "./Fg.md");

    return 0;
}

int main()
{
    // test0();
    // test1();
    test2();
    return 0;
}