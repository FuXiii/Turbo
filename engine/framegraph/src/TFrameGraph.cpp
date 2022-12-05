#include "TFrameGraph.hpp"
#include <cassert>
#include <stack>

Turbo::FrameGraph::TNode::TNode()
{
}

Turbo::FrameGraph::TPassNode::TPassNode(const std::string &name, TPass pass, std::unique_ptr<TPassExecutorProxy> &&passExecutorProxy) : TNode(), name(name), pass(pass), passExecutorProxy(std::move(passExecutorProxy))
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

Turbo::FrameGraph::TVirtualResourceProxy::TVirtualResourceProxy(const std::string &name) : TProxy(), name(name), version(0)
{
}

const std::string &Turbo::FrameGraph::TVirtualResourceProxy::GetName()
{
    return this->name;
}

Turbo::FrameGraph::TVersion Turbo::FrameGraph::TVirtualResourceProxy::GetVersion()
{
    return this->version;
}

Turbo::FrameGraph::TResourceNode::TResourceNode(TResource resource, TVirtualResourceProxy *virtualResourceProxy, uint32_t version) : TNode(), resource(resource), virtualResourceProxy(virtualResourceProxy), version(version)
{
}

Turbo::FrameGraph::TResource Turbo::FrameGraph::TResourceNode::GetResource()
{
    return this->resource;
}

Turbo::FrameGraph::TVirtualResourceProxy *Turbo::FrameGraph::TResourceNode::GetResourceProxy()
{
    return this->virtualResourceProxy;
}

const std::string &Turbo::FrameGraph::TResourceNode::GetName()
{
    assert(this->virtualResourceProxy != nullptr);
    return this->virtualResourceProxy->GetName();
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
    // FIXME: 同TFrameGraph::TBuilder::Write(...)问题
    // FIXME: 详情见Issue文档
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

    // FIXME: 运行到此处说明resource对应的资源不是当前Pass创建的，需要找是否已经创建
    // FIXME: 此处的前提是resource已经通过TFrameGraph::TBuilder::Create(...)被创建（别的Pass创建的）
    // FIXME: 此时resource在当前pass中还没有创建，应该看一下Fg中是否有该资源节点:this->frameGraph.GetResourceNode(resource)
    // FIXME: this->frameGraph.GetResourceNode(resource)中也没找到该资源节点的话，说明资源未被创建，应有的策略是直接终止程序
    // FIXME: 详情见Issue文档
    TResourceNode &resource_node = this->frameGraph.GetResourceNode(resource);

    TVirtualResourceProxy *resource_proxy = resource_node.GetResourceProxy();
    resource_proxy->version += 1;

    TResourceNode &new_resource_node = this->frameGraph.CreateResourceNode(resource_proxy);
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
    this->resourceProxys = new std::vector<TVirtualResourceProxy *>();
}

Turbo::FrameGraph::TFrameGraph::~TFrameGraph()
{
    delete this->passNodes;
    delete this->resourceNodes;

    for (TVirtualResourceProxy *virtual_resource_proxy_item : *this->resourceProxys)
    {
        delete virtual_resource_proxy_item;
    }

    delete this->resourceProxys;
}

Turbo::FrameGraph::TPassNode &Turbo::FrameGraph::TFrameGraph::CreatePassNode(const std::string &name, std::unique_ptr<TPassExecutorProxy> &&passExecutorProxy)
{
    TPass pass;
    pass.id = static_cast<uint32_t>(this->passNodes->size());
    return this->passNodes->emplace_back(TPassNode(name, pass, std::move(passExecutorProxy)));
};

Turbo::FrameGraph::TResourceNode &Turbo::FrameGraph::TFrameGraph::CreateResourceNode(TVirtualResourceProxy *virtualResourceProxy)
{
    TResource resource;
    resource.id = static_cast<uint32_t>(this->resourceNodes->size());
    TResourceNode resource_node(resource, virtualResourceProxy, virtualResourceProxy->GetVersion());
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

void Turbo::FrameGraph::TFrameGraph::Compile()
{
    // compute passes and resource reference counts
    for (TPassNode &pass_node_item : *this->passNodes)
    {
        pass_node_item.refCount = pass_node_item.writes.size();
        if (pass_node_item.sideEffect)
        {
            pass_node_item.refCount += 1;
        }

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
                TVirtualResourceProxy *virtual_resource = resource_node.virtualResourceProxy;

                virtual_resource->firstUser = virtual_resource->firstUser.id != TURBO_INVALID_ID ? virtual_resource->firstUser : pass_node_item.pass;
                virtual_resource->lastUser = pass_node_item.pass;
            }

            for (TResource &write_resource_item : pass_node_item.writes)
            {
                TResourceNode &resource_node = this->resourceNodes->at(write_resource_item.id);
                TVirtualResourceProxy *virtual_resource = resource_node.virtualResourceProxy;

                virtual_resource->firstUser = virtual_resource->firstUser.id != TURBO_INVALID_ID ? virtual_resource->firstUser : pass_node_item.pass;
                virtual_resource->lastUser = pass_node_item.pass;
            }
        }
    }

    // compile the resource creater and destorier
    for (TVirtualResourceProxy *virtual_resource_proxy_item : *this->resourceProxys)
    {
        TPass first_user = virtual_resource_proxy_item->firstUser;
        TPass last_user = virtual_resource_proxy_item->lastUser;

        if (first_user.id != TURBO_INVALID_ID && last_user.id != TURBO_INVALID_ID)
        {
            TPassNode &first_user_pass_node = this->passNodes->at(first_user.id);
            TPassNode &last_user_pass_node = this->passNodes->at(last_user.id);

            first_user_pass_node.devirtualizes.push_back(virtual_resource_proxy_item);
            last_user_pass_node.destroies.push_back(virtual_resource_proxy_item);
        }
    }
}

void Turbo::FrameGraph::TFrameGraph::Execute(void *context)
{
    for (TPassNode &pass_node_item : *this->passNodes)
    {
        TResources resources(*this, pass_node_item);

        for (TVirtualResourceProxy *virtual_resource_item : pass_node_item.devirtualizes)
        {
            virtual_resource_item->Create(/*TODO: we need a allocator/context*/);
        }

        if (pass_node_item.refCount > 0)
        {
            pass_node_item.passExecutorProxy->Executor(resources, context);
        }

        for (TVirtualResourceProxy *virtual_resource_item : pass_node_item.destroies)
        {
            // FIXME: 此处不应该真的去销毁相应的资源，正常应该标记对应资源需要被回收，进而异步的回收资源
            // FIXME: 详情见Issue文档
            virtual_resource_item->Destroy(/*TODO: we need a allocator/context*/);
        }
    }
}

Turbo::FrameGraph::TBlackboard &Turbo::FrameGraph::TFrameGraph::GetBlackboard()
{
    return this->blackboard;
}
