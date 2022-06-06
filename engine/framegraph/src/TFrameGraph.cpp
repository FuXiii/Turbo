#include "TFrameGraph.hpp"

Turbo::FrameGraph::TNode::TNode(const std::string &name)
{
    this->name = name;
    this->refCount = 0;
}

Turbo::FrameGraph::TPassNode::TPassNode(const std::string &name, TPass pass, TAgency *agency) : TNode(name)
{
    this->pass = pass;
    this->agency = agency;
}

Turbo::FrameGraph::TFrameGraph::TBuilder::TBuilder(TFrameGraph &frameGraph, TPassNode &passNode) : frameGraph{frameGraph}, passNode{passNode}
{
}

Turbo::FrameGraph::TFrameGraph::TFrameGraph()
{
    this->passNodes = new std::vector<TPassNode>();
    this->resourceNodes = new std::vector<TResourceNode>();
    this->agencys = new std::vector<TAgency>();
}

Turbo::FrameGraph::TFrameGraph::~TFrameGraph()
{
    delete this->passNodes;
    delete this->resourceNodes;
    delete this->agencys;
}

Turbo::FrameGraph::TPassNode &Turbo::FrameGraph::TFrameGraph::CreatePassNode(std::string &name, TAgency *agency)
{
    TPass pass;
    pass.id = static_cast<uint32_t>(this->passNodes->size());
    return this->passNodes->emplace_back(TPassNode(name, pass, agency));
};
