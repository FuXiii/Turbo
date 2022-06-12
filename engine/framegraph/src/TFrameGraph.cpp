#include "TFrameGraph.hpp"
#include <cassert>

Turbo::FrameGraph::TNode::TNode(const std::string &name)
{
    this->name = name;
    this->refCount = 0;
}

const std::string &Turbo::FrameGraph::TNode::GetName()
{
    return this->name;
}

Turbo::FrameGraph::TPassNode::TPassNode(const std::string &name, TPass pass, TAgency *agency) : TNode(name)
{
    this->pass = pass;
    this->agency = agency;
}

Turbo::FrameGraph::TPassNode::~TPassNode()
{
    delete this->agency;
}

Turbo::FrameGraph::TResource Turbo::FrameGraph::TPassNode::AddCreate(TResource resource)
{
    return this->creates.emplace_back(resource);
}

Turbo::FrameGraph::TResource Turbo::FrameGraph::TPassNode::AddRead(TResource resource)
{
    assert(!this->IsCreate(resource) && !this->IsWrite(resource));
    return this->IsRead(resource) ? resource : this->reads.emplace_back(resource);
}

Turbo::FrameGraph::TResource Turbo::FrameGraph::TPassNode::AddWrite(TResource resource)
{
    return this->IsWrite(resource) ? resource : this->writes.emplace_back(resource);
}

bool Turbo::FrameGraph::TPassNode::IsCreate(TResource resource)
{
    for (TResource &resource_item : this->creates)
    {
        if (resource.id == resource_item.id)
        {
            return true;
        }
    }

    return false;
}

bool Turbo::FrameGraph::TPassNode::IsWrite(TResource resource)
{
    for (TResource &resource_item : this->writes)
    {
        if (resource.id == resource_item.id)
        {
            return true;
        }
    }

    return false;
}

bool Turbo::FrameGraph::TPassNode::IsRead(TResource resource)
{
    for (TResource &resource_item : this->reads)
    {
        if (resource.id == resource_item.id)
        {
            return true;
        }
    }

    return false;
}

Turbo::FrameGraph::TResourceNode::TResourceNode(const std::string &name, TResource resource, uint32_t agencyID, uint32_t version) : TNode(name)
{
    this->resource = resource;
    this->agencyID = agencyID;
    this->version = version;
}

Turbo::FrameGraph::TResource Turbo::FrameGraph::TResourceNode::GetResource()
{
    return this->resource;
}

uint32_t Turbo::FrameGraph::TResourceNode::GetAgencyID()
{
    return this->agencyID;
}

uint32_t Turbo::FrameGraph::TResourceNode::GetVersion()
{
    return this->version;
}

Turbo::FrameGraph::TFrameGraph::TBuilder::TBuilder(TFrameGraph &frameGraph, TPassNode &passNode) : frameGraph{frameGraph}, passNode{passNode}
{
}

Turbo::FrameGraph::TResource Turbo::FrameGraph::TFrameGraph::TBuilder::Read(TResource resource)
{
    assert(this->frameGraph.IsValid(resource));
    return this->passNode.AddRead(resource);
}

Turbo::FrameGraph::TResource Turbo::FrameGraph::TFrameGraph::TBuilder::Write(TResource resource)
{
    assert(this->frameGraph.IsValid(resource));
    if (this->passNode.IsCreate(resource))
    {
        return this->passNode.AddWrite(resource);
    }
    else
    {
        TResourceNode &clone_resource_node = this->frameGraph.CloneResourceNode(resource);
        TResource resource = clone_resource_node.GetResource();
        return this->passNode.AddWrite(resource);
    }
}

Turbo::FrameGraph::TFrameGraph::TFrameGraph()
{
    this->passNodes = new std::vector<TPassNode>();
    this->resourceNodes = new std::vector<TResourceNode>();
    this->agencys = new std::vector<TAgency *>();
}

Turbo::FrameGraph::TFrameGraph::~TFrameGraph()
{
    delete this->passNodes;
    delete this->resourceNodes;

    for (TAgency *agency_item : *this->agencys)
    {
        delete agency_item;
    }

    delete this->agencys;
}

Turbo::FrameGraph::TPassNode &Turbo::FrameGraph::TFrameGraph::CreatePassNode(const std::string &name, TAgency *agency)
{
    TPass pass;
    pass.id = static_cast<uint32_t>(this->passNodes->size());
    return this->passNodes->emplace_back(TPassNode(name, pass, agency));
};

Turbo::FrameGraph::TResourceNode &Turbo::FrameGraph::TFrameGraph::CreateResourceNode(const std::string &name, uint32_t agencyID)
{
    TResource resource;
    resource.id = static_cast<uint32_t>(this->resourceNodes->size());
    TResourceNode resource_node(name, resource, agencyID, 1);
    return this->resourceNodes->emplace_back(resource_node);
}

Turbo::FrameGraph::TResourceNode &Turbo::FrameGraph::TFrameGraph::CloneResourceNode(TResource resource)
{
    assert(resource.id < this->resourceNodes->size());
    TResourceNode &resource_node = (*this->resourceNodes)[resource.id];
    uint32_t agency_id = resource_node.GetAgencyID();
    assert(agency_id < this->agencys->size());

    const uint32_t clone_resource_id = static_cast<uint32_t>(this->resourceNodes->size());
    TResource clone_resource;
    clone_resource.id = clone_resource_id;
    TResourceNode clone_resource_node(resource_node.GetName(), clone_resource, agency_id, resource_node.GetVersion() + 1);
    return this->resourceNodes->emplace_back(clone_resource_node);
}

bool Turbo::FrameGraph::TFrameGraph::IsValid(TResource resource)
{
    if (resource.id < this->resourceNodes->size())
    {
        return true;
    }

    return false;

    // resource.version and agency.version
}

template <typename Virtualizable>
Virtualizable &Turbo::FrameGraph::TFrameGraph::GetResourceAgency(TResource resource)
{
    assert(resource.id < this->resourceNodes->size());
    TResourceNode &resource_node = (*this->resourceNodes)[resource.id];
    uint32_t agency_id = resource_node.GetAgencyID();
    TResourceAgency<Virtualizable> *agency = dynamic_cast<TResourceAgency<Virtualizable> *>((*this->agencys)[agency_id]);
    return agency->GetResoureceAgency();
}

Turbo::FrameGraph::TResources::TResources(TFrameGraph &frameGraph, TPassNode &passNode) : frameGraph(frameGraph), passNode(passNode)
{
}

template <typename Virtualizable>
Virtualizable &Turbo::FrameGraph::TResources::Get(TResource resource)
{
    assert(this->passNode.IsCreate(resource) || this->passNode.IsRead(resource) || this->passNode.IsWrite(resource));
    return this->frameGraph.GetResourceAgency<Virtualizable>(resource);
}

void Turbo::FrameGraph::TFrameGraph::Compile()
{
}

void Turbo::FrameGraph::TFrameGraph::Execute()
{
}
