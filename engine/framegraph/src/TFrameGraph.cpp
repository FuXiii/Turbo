#include "TFrameGraph.hpp"
#include <cassert>
#include <stack>

Turbo::FrameGraph::TNode::TNode()
{
}

Turbo::FrameGraph::TPassNode::TPassNode(const std::string &name, TPass pass, std::unique_ptr<TPassExecutorAgency> &&agency) : TNode(), name(name), pass(pass), agency(std::move(agency))
{
}

Turbo::FrameGraph::TResource Turbo::FrameGraph::TPassNode::AddCreate(TResource resource)
{
    return this->IsCreate(resource) ? resource : this->creates.emplace_back(resource);
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

const std::string &Turbo::FrameGraph::TPassNode::GetName()
{
    return this->name;
}

Turbo::FrameGraph::TVirtualResourceAgency::TVirtualResourceAgency(const std::string &name) : TAgency(), name(name)
{
}

const std::string &Turbo::FrameGraph::TVirtualResourceAgency::GetName()
{
    return this->name;
}

Turbo::FrameGraph::TVersion Turbo::FrameGraph::TVirtualResourceAgency::GetVersion()
{
    return this->version;
}

void Turbo::FrameGraph::TVirtualResourceAgency::SetVersion(TVersion version)
{
    this->version = version;
}

Turbo::FrameGraph::TResourceNode::TResourceNode(TResource resource, TVirtualResourceAgency *resourceAgency, uint32_t version) : TNode(), resource(resource), resourceAgency(resourceAgency), version(version)
{
}

Turbo::FrameGraph::TResource Turbo::FrameGraph::TResourceNode::GetResource()
{
    return this->resource;
}

Turbo::FrameGraph::TVirtualResourceAgency *Turbo::FrameGraph::TResourceNode::GetResourceAgency()
{
    return this->resourceAgency;
}

const std::string &Turbo::FrameGraph::TResourceNode::GetName()
{
    assert(this->resourceAgency != nullptr);
    return this->resourceAgency->GetName();
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
    if (this->passNode.IsWrite(resource))
    {
        return resource;
    }

    if (this->passNode.IsCreate(resource))
    {
        return this->passNode.AddWrite(resource);
    }

    TResourceNode &resource_node = this->frameGraph.GetResourceNode(resource);

    TVirtualResourceAgency *resource_agency = resource_node.GetResourceAgency();
    TVersion resource_agency_version = resource_agency->GetVersion();
    ++resource_agency_version;
    resource_agency->SetVersion(resource_agency_version);

    TResourceNode &new_resource_node = this->frameGraph.CreateResourceNode(resource_agency);
    TResource new_resource = new_resource_node.GetResource();
    return this->passNode.AddWrite(new_resource);
}

Turbo::FrameGraph::TFrameGraph::TBuilder &Turbo::FrameGraph::TFrameGraph::TBuilder::SideEffect()
{
    this->passNode.sideEffect = true;
    return *this;
}

Turbo::FrameGraph::TFrameGraph::TFrameGraph()
{
    this->passNodes = new std::vector<TPassNode>();
    this->resourceNodes = new std::vector<TResourceNode>();
    this->resourceAgencys = new std::vector<TVirtualResourceAgency *>();
}

Turbo::FrameGraph::TFrameGraph::~TFrameGraph()
{
    delete this->passNodes;
    delete this->resourceNodes;

    for (TVirtualResourceAgency *virtual_resourceagency_item : *this->resourceAgencys)
    {
        delete virtual_resourceagency_item;
    }

    delete this->resourceAgencys;
}

Turbo::FrameGraph::TPassNode &Turbo::FrameGraph::TFrameGraph::CreatePassNode(const std::string &name, std::unique_ptr<TPassExecutorAgency> &&agency)
{
    TPass pass;
    pass.id = static_cast<uint32_t>(this->passNodes->size());
    return this->passNodes->emplace_back(TPassNode(name, pass, std::move(agency)));
};

Turbo::FrameGraph::TResourceNode &Turbo::FrameGraph::TFrameGraph::CreateResourceNode(TVirtualResourceAgency *virtualResourceAgency)
{
    TResource resource;
    resource.id = static_cast<uint32_t>(this->resourceNodes->size());
    TResourceNode resource_node(resource, virtualResourceAgency, virtualResourceAgency->GetVersion());
    return this->resourceNodes->emplace_back(resource_node);
}

bool Turbo::FrameGraph::TFrameGraph::IsValid(TResource resource)
{
    if (resource.id < this->resourceNodes->size())
    {
        return true;
    }

    return false;
}

bool Turbo::FrameGraph::TFrameGraph::IsValid(TPass pass)
{
    if (pass.id < this->passNodes->size())
    {
        return true;
    }

    return false;
}

Turbo::FrameGraph::TResourceNode &Turbo::FrameGraph::TFrameGraph::GetResourceNode(TResource resource)
{
    assert(this->IsValid(resource));
    return this->resourceNodes->at(resource.id);
}

Turbo::FrameGraph::TResources::TResources(TFrameGraph &frameGraph, TPassNode &passNode) : frameGraph(frameGraph), passNode(passNode)
{
}

#include <iostream>
void Turbo::FrameGraph::TFrameGraph::Compile()
{
    // compute passes and resource reference counts
    for (TPassNode &pass_node_item : *this->passNodes)
    {
        pass_node_item.refCount = pass_node_item.writes.size();

        for (TResource resource_item : pass_node_item.reads)
        {
            this->resourceNodes->at(resource_item.id).refCount++;
        }

        for (TResource resource_item : pass_node_item.writes)
        {
            this->resourceNodes->at(resource_item.id).writer = pass_node_item.pass;
        }
    }

    //<Test output>
    {
        for (auto &pass_node_item : *this->passNodes)
        {
            std::cout << "PassNode::" << pass_node_item.GetName() << "::" << pass_node_item.refCount << std::endl;
        }

        for (auto &resource_node_item : *this->resourceNodes)
        {
            std::cout << "ResourceNode::" << resource_node_item.GetName() << "::" << resource_node_item.refCount << "::writer::" << this->passNodes->at(resource_node_item.writer.id).name << std::endl;
        }
    }
    //</Test output>

    // cull passes and resources unreference
    std::stack<TResource> unref_resources;
    for (TResourceNode &resource_node_item : *this->resourceNodes)
    {
        if (resource_node_item.refCount == 0)
        {
            unref_resources.push(resource_node_item.resource);
        }
    }

    while (!unref_resources.empty())
    {
        TResource resource = unref_resources.top();
        unref_resources.pop();

        TResourceNode &resource_node = this->resourceNodes->at(resource.id);
        TPassNode &writer = this->passNodes->at(resource_node.writer.id);
        if (--writer.refCount == 0)
        {
            for (TResource &read_resource_item : writer.reads)
            {
                TResourceNode &read_resource_node = this->resourceNodes->at(read_resource_item.id);
                if (--read_resource_node.refCount == 0)
                {
                    unref_resources.push(read_resource_item);
                }
            }
        }
        std::cout << "UnRef::" << resource_node.GetName() << "::Writer::" << writer.name << ":" << writer.refCount << std::endl;
    }

    // compute resource first/last users for active passes
    for (TPassNode &pass_node_item : *this->passNodes)
    {
        if (pass_node_item.refCount > 0)
        {
            for (TResource &read_resource_item : pass_node_item.reads)
            {
                TResourceNode &resource_node = this->resourceNodes->at(read_resource_item.id);
                TVirtualResourceAgency *virtual_resource = resource_node.resourceAgency;

                virtual_resource->firstUser = virtual_resource->firstUser.id != TURBO_NVALID_ID ? virtual_resource->firstUser : pass_node_item.pass;
                virtual_resource->lastUser = pass_node_item.pass;
            }

            for (TResource &write_resource_item : pass_node_item.writes)
            {
                TResourceNode &resource_node = this->resourceNodes->at(write_resource_item.id);
                TVirtualResourceAgency *virtual_resource = resource_node.resourceAgency;

                virtual_resource->firstUser = virtual_resource->firstUser.id != TURBO_NVALID_ID ? virtual_resource->firstUser : pass_node_item.pass;
                virtual_resource->lastUser = pass_node_item.pass;
            }
        }
    }

    // compile the resource creater and destorier
    for (TVirtualResourceAgency *virtual_resource_agency_item : *this->resourceAgencys)
    {
        TPass first_user = virtual_resource_agency_item->firstUser;
        TPass last_user = virtual_resource_agency_item->lastUser;

        if (first_user.id != TURBO_NVALID_ID && last_user.id != TURBO_NVALID_ID)
        {
            TPassNode &first_user_pass_node = this->passNodes->at(first_user.id);
            TPassNode &last_user_pass_node = this->passNodes->at(last_user.id);

            first_user_pass_node.devirtualizes.push_back(virtual_resource_agency_item);
            last_user_pass_node.destroies.push_back(virtual_resource_agency_item);
        }
    }
}

void Turbo::FrameGraph::TFrameGraph::Execute(void *context)
{
    for (TPassNode &pass_node_item : *this->passNodes)
    {

        TResources resources(*this, pass_node_item);

        for (TVirtualResourceAgency *virtual_resource_item : pass_node_item.devirtualizes)
        {
            virtual_resource_item->Create();
        }

        if (pass_node_item.refCount > 0 || pass_node_item.sideEffect)
        {
            pass_node_item.agency->Executor(resources, context);
        }

        for (TVirtualResourceAgency *virtual_resource_item : pass_node_item.destroies)
        {
            virtual_resource_item->Destroy();
        }
    }
}

Turbo::FrameGraph::TBlackboard &Turbo::FrameGraph::TFrameGraph::GetBlackboard()
{
    return this->blackboard;
}
