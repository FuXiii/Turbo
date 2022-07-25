#include <TFrameGraph.hpp>

int main()
{
    std::string pass_node_name = "custom_pass_node";

    Turbo::FrameGraph::TPass pass;
    pass.id = 0;

    Turbo::FrameGraph::TPassNode pass_node(pass_node_name, pass, nullptr);
}