
#include <TFrameGraph.hpp>
#include <iostream>

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

    void Create(const std::string &name, const Descriptor &)
    {
        // custom create
    }

    void Destroy()
    {
        // custom destroy
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
            data.foo = builder.Write(data.foo);

            data.bar = builder.Create<CustomTexture>("bar", {256, 256, CustomTexture::Depth});
            data.bar = builder.Write(data.bar);
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
Depth Pass
    ↓
Depth Texture.
    ↓
    ↓        ↗ Depth Texture. ↘
GBuffer Pass → Normal Texture. → Lighting Pass → Light Texture.
             ↘ Color Texture. ↗                       ↓
                                                   Post Pass
                                                       ↓
                                                RenderTarget Texture.
                                                       ↓
                                                  Present Pass
*/
int test2()
{
    TFrameGraph fg;

    struct DepthPassData
    {
        TResource depthTexture;
    };

    fg.AddPass<DepthPassData>(
        "Depth Pass",
        [&](TFrameGraph::TBuilder &builder, DepthPassData &data) {
            data.depthTexture = builder.Create<CustomTexture>("Depth Texture", {128, 128, CustomTexture::Depth});
            data.depthTexture = builder.Write(data.depthTexture);

            fg.GetBlackboard()["Depth Texture"] = data.depthTexture;
        },
        [=](const DepthPassData &data, const TResources &resources, void *context) {
            CustomTexture &depth_texture = resources.Get<CustomTexture>(data.depthTexture);
            uint32_t depth_texture_value = depth_texture.value;

            /*Some Rendering Command...*/
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
            data.depthTexture = builder.Read(data.depthTexture);
            data.depthTexture = builder.Write(data.depthTexture);

            data.normalTexture = builder.Create<CustomTexture>("Normal Texture", {128, 128, CustomTexture::Normal});
            data.normalTexture = builder.Write(data.normalTexture);

            data.colorTexture = builder.Create<CustomTexture>("Normal Texture", {128, 128, CustomTexture::Color});
            data.colorTexture = builder.Write(data.colorTexture);

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

            /*Some Rendering Command...*/
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
            data.depthTexture = fg.GetBlackboard()["Depth Texture"];
            data.depthTexture = builder.Read(data.depthTexture);

            data.normalTexture = fg.GetBlackboard()["Normal Texture"];
            data.normalTexture = builder.Read(data.normalTexture);

            data.colorTexture = fg.GetBlackboard()["Color Texture"];
            data.colorTexture = builder.Read(data.colorTexture);

            data.lightTexture = builder.Create<CustomTexture>("Light Texture", {128, 128, CustomTexture::Shadow});
            data.lightTexture = builder.Write(data.lightTexture);

            fg.GetBlackboard()["Light Texture"] = data.lightTexture;
        },
        [=](const LightPassData &data, const TResources &resources, void *context) {

        });

    struct PostPassData
    {
        TResource lightTexture;
        TResource renderTargetTexture;
    };

    fg.AddPass<PostPassData>(
        "Post Pass",
        [&](TFrameGraph::TBuilder &builder, PostPassData &data) {
            data.lightTexture = fg.GetBlackboard()["Light Texture"];
            data.lightTexture = builder.Read(data.lightTexture);

            data.renderTargetTexture = builder.Create<CustomTexture>("RenderTarget Texture", {128, 128, CustomTexture::Shadow});
            data.renderTargetTexture = builder.Write(data.renderTargetTexture);

            fg.GetBlackboard()["RenderTarget Texture"] = data.renderTargetTexture;
        },
        [=](const PostPassData &data, const TResources &resources, void *context) {

        });

    struct PresentPassData
    {
        TResource renderTargetTexture;
    };

    fg.AddPass<PresentPassData>(
        "Present Pass",
        [&](TFrameGraph::TBuilder &builder, PresentPassData &data) {
            data.renderTargetTexture = fg.GetBlackboard()["RenderTarget Texture"];
            data.renderTargetTexture = builder.Read(data.renderTargetTexture);
        },
        [=](const PresentPassData &data, const TResources &resources, void *context) {

        });

    fg.Compile();
    fg.Execute();

    return 0;
}

int main()
{
    // test0();
    test1();
    return 0;
}