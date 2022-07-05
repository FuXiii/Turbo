
#include <TFrameGraph.hpp>
#include <iostream>

using namespace Turbo::FrameGraph;

struct CustomTexture
{
    enum Usage
    {
        Color,
        Depth,
        Normal
    };

    struct Descriptor
    {
        uint32_t width;
        uint32_t height;
        Usage usage;
    };

    uint32_t value;

    CustomTexture() = default;

    void create(const Descriptor &, void *)
    {
        // custom create
    }

    void destroy(const Descriptor &, void *)
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
        [=](const CustomPassData &data, TResources &resources, void *context) {

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
        [=](const CustomPassData &data, TResources &resources, void *context) {
            int32_t foo_value = resources.Get<CustomTexture>(data.foo).value;
            int32_t bar_value = resources.Get<CustomTexture>(data.bar).value;
        });

    fg.Compile();
    fg.Execute();

    return 0;
}

int main()
{
    return test0();
}