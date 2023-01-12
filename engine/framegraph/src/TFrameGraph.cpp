#include "TFrameGraph.hpp"
#include <basetsd.h>
#include <cassert>
#include <regex>
#include <sstream>
#include <stack>
#include <stdint.h>
#include <string>
#include <vector>

void Turbo::FrameGraph::TSubpass::Write(TResource resource)
{
    bool is_found = false;
    for (TResource resource_item : this->writes)
    {
        if (resource_item.id == resource.id)
        {
            is_found = true;
            break;
        }
    }

    if (!is_found)
    {
        this->writes.push_back(resource);
    }
}

void Turbo::FrameGraph::TSubpass::Read(TResource resource)
{
    bool is_found = false;
    for (TResource resource_item : this->reads)
    {
        if (resource_item.id == resource.id)
        {
            is_found = true;
            break;
        }
    }

    if (!is_found)
    {
        this->reads.push_back(resource);
    }
}

void Turbo::FrameGraph::TSubpass::Input(TResource resource)
{
    bool is_found = false;
    for (TResource resource_item : this->inputs)
    {
        if (resource_item.id == resource.id)
        {
            is_found = true;
            break;
        }
    }

    if (!is_found)
    {
        this->reads.push_back(resource);
    }
}

std::vector<Turbo::FrameGraph::TResource> Turbo::FrameGraph::TSubpass::GetWrites()
{
    return this->writes;
}

std::vector<Turbo::FrameGraph::TResource> Turbo::FrameGraph::TSubpass::GetReads()
{
    return this->reads;
}

std::vector<Turbo::FrameGraph::TResource> Turbo::FrameGraph::TSubpass::GetInputs()
{
    return this->inputs;
}

void Turbo::FrameGraph::TRenderPass::AddSubpass(const TSubpass &subpass)
{
    this->subpasses.push_back(subpass);
}

std::vector<Turbo::FrameGraph::TSubpass> Turbo::FrameGraph::TRenderPass::GetSubpasses()
{
    return this->subpasses;
}

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
    // FIXME: 同一个PassNode的多Subpass下经常会发生读取之前写过的资源，此行代码并不适合此种情况，应该删除该assert(...)判断
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
            if (resource.isInput)
            {
                resource_item.isInput = true;
            }

            return true;
        }
    }

    return false;
}

Turbo::FrameGraph::TRenderPass Turbo::FrameGraph::TPassNode::GetRenderPass()
{
    return this->renderPass;
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

Turbo::FrameGraph::TFrameGraph::TBuilder::TSubpass::TSubpass(TBuilder *builder, uint32_t index)
{
    this->builder = builder;
    this->index = index;
}

Turbo::FrameGraph::TResource Turbo::FrameGraph::TFrameGraph::TBuilder::TSubpass::Write(TResource resource)
{
    if (this->builder != nullptr)
    {
        TResource return_resurce = this->builder->Write(resource);
        std::vector<Turbo::FrameGraph::TSubpass> &subpasses = this->builder->passNode.renderPass.subpasses;
        if (this->index != TURBO_INVALID_SUBPASS_INDEX && this->index < subpasses.size())
        {
            Turbo::FrameGraph::TSubpass &subpass = this->builder->passNode.renderPass.subpasses[this->index];
            subpass.Write(return_resurce);
        }

        return return_resurce;
    }

    return Turbo::FrameGraph::TResource();
}

Turbo::FrameGraph::TResource Turbo::FrameGraph::TFrameGraph::TBuilder::TSubpass::Read(TResource resource)
{
    if (this->builder != nullptr)
    {
        resource.isInput = false;
        TResource return_resurce = this->builder->Read(resource);
        std::vector<Turbo::FrameGraph::TSubpass> &subpasses = this->builder->passNode.renderPass.subpasses;
        if (this->index != TURBO_INVALID_SUBPASS_INDEX && this->index < subpasses.size())
        {
            Turbo::FrameGraph::TSubpass &subpass = this->builder->passNode.renderPass.subpasses[this->index];
            subpass.Read(return_resurce);
        }

        return return_resurce;
    }

    return Turbo::FrameGraph::TResource();
}

Turbo::FrameGraph::TResource Turbo::FrameGraph::TFrameGraph::TBuilder::TSubpass::Input(TResource resource)
{
    if (this->builder != nullptr)
    {
        resource.isInput = true;
        TResource return_resurce = this->builder->Read(resource);
        std::vector<Turbo::FrameGraph::TSubpass> &subpasses = this->builder->passNode.renderPass.subpasses;
        if (this->index != TURBO_INVALID_SUBPASS_INDEX && this->index < subpasses.size())
        {
            Turbo::FrameGraph::TSubpass &subpass = this->builder->passNode.renderPass.subpasses[this->index];
            subpass.Input(return_resurce);
        }

        return return_resurce;
    }

    return Turbo::FrameGraph::TResource();
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

Turbo::FrameGraph::TFrameGraph::TBuilder::TSubpass Turbo::FrameGraph::TFrameGraph::TBuilder::CreateSubpass()
{
    Turbo::FrameGraph::TSubpass subpass;
    this->passNode.renderPass.AddSubpass(subpass);

    uint32_t subpass_index = this->passNode.renderPass.subpasses.size() - 1;

    return Turbo::FrameGraph::TFrameGraph::TBuilder::TSubpass(this, subpass_index);
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

void Turbo::FrameGraph::TFrameGraph::Execute(void *context, void *allocator)
{
    for (TPassNode &pass_node_item : *this->passNodes)
    {
        TResources resources(*this, pass_node_item);

        for (TVirtualResourceProxy *virtual_resource_item : pass_node_item.devirtualizes)
        {
            virtual_resource_item->Create(allocator);
        }

        if (pass_node_item.refCount > 0)
        {
            pass_node_item.passExecutorProxy->Executor(resources, context);
        }

        for (TVirtualResourceProxy *virtual_resource_item : pass_node_item.destroies)
        {
            // FIXME: 此处不应该真的去销毁相应的资源，正常应该标记对应资源需要被回收，进而异步的回收资源
            // FIXME: 详情见Issue文档
            virtual_resource_item->Destroy(allocator);
        }
    }
}

std::string Turbo::FrameGraph::TFrameGraph::ToMermaid()
{
    struct linkStyle
    {
        uint32_t index = 0;
        bool isWrite = false;
    };

    std::stringstream mermaid_string_stream;

    std::string write_link_style = "stroke:#a44141,stroke-width:3px";
    std::string read_link_style = "stroke:#95ad5b,stroke-width:3px";

    mermaid_string_stream << "graph LR;" << std::endl;
    mermaid_string_stream << "classDef Resource fill:#608ba3" << std::endl;
    mermaid_string_stream << "classDef Pass fill:#e8924a" << std::endl;
    mermaid_string_stream << "classDef Subpass fill:#8474a0" << std::endl;
    mermaid_string_stream << "classDef Start fill:#95ad5b,stroke:#95ad5b,stroke-width:4px" << std::endl;
    mermaid_string_stream << "classDef End fill:#a44141,stroke:#a44141,stroke-width:4px" << std::endl;

    mermaid_string_stream << "Start((\" \")):::Start" << std::endl;
    mermaid_string_stream << "End((\" \")):::End" << std::endl;

    uint32_t link_index = 0;
    std::vector<linkStyle> link_styles;
    uint32_t pass_node_cout = this->passNodes->size();
    for (uint32_t pass_node_index = 0; pass_node_index < pass_node_cout; pass_node_index++)
    {
        const TPassNode &pass_node_item = (*this->passNodes)[pass_node_index];
        std::string pass_node_key_name = "PassNode" + std::to_string(pass_node_item.pass.id);
        mermaid_string_stream << pass_node_key_name << ":::Pass" << std::endl;

        mermaid_string_stream << "subgraph " << pass_node_key_name << "[\"" << pass_node_item.name << "\"]" << std::endl;
        mermaid_string_stream << "direction TB" << std::endl;
        const std::vector<Turbo::FrameGraph::TSubpass> &subpasses = pass_node_item.renderPass.subpasses;
        uint32_t subpasses_count = subpasses.size();
        for (uint32_t subpass_index = 0; subpass_index < subpasses_count; subpass_index++)
        {
            const Turbo::FrameGraph::TSubpass &subpass = subpasses[subpass_index];
            std::string subpass_name = "Subpass " + std::to_string(subpass_index);
            std::string subpass_key_name = pass_node_key_name + "Subpass" + std::to_string(subpass_index);
            mermaid_string_stream << subpass_key_name << "(\"" << subpass_name << "\")"
                                  << ":::Subpass" << std::endl;

            bool is_have_next = false;
            if ((subpass_index + 1) < subpasses_count)
            {
                is_have_next = true;
            }

            const Turbo::FrameGraph::TSubpass &current_subpass = subpass;
            std::string &current_subpass_key_name = subpass_key_name;
            if (is_have_next)
            {
                const Turbo::FrameGraph::TSubpass &next_subpass = subpasses[subpass_index + 1];
                std::string next_subpass_key_name = pass_node_key_name + "Subpass" + std::to_string(subpass_index + 1);
                mermaid_string_stream << current_subpass_key_name << "-->" << next_subpass_key_name << std::endl;
                link_index++;
            }
        }
        mermaid_string_stream << "end" << std::endl;

        for (uint32_t subpass_index = 0; subpass_index < subpasses_count; subpass_index++)
        {
            const Turbo::FrameGraph::TSubpass &subpass = subpasses[subpass_index];
            std::string subpass_name = "Subpass " + std::to_string(subpass_index);
            std::string subpass_key_name = pass_node_key_name + "Subpass" + std::to_string(subpass_index);

            const std::vector<TResource> &subpass_read_resources = subpass.reads;
            uint32_t subpass_read_resources_count = subpass_read_resources.size();
            for (uint32_t subpass_read_resource_index = 0; subpass_read_resource_index < subpass_read_resources_count; subpass_read_resource_index++)
            {
                const TResource &read_resource = subpass_read_resources[subpass_read_resource_index];
                Turbo::FrameGraph::TResourceNode &resource_node = this->GetResourceNode(read_resource);
                std::string resource_name = resource_node.GetName();
                uint32_t resource_version = resource_node.GetVersion();

                std::string resource_key_name = resource_name + std::to_string(resource_version);
                std::regex space_regex(" ");
                resource_key_name = std::regex_replace(resource_key_name, space_regex, "_");
                mermaid_string_stream << resource_key_name << "(\"" << resource_name << "\")"
                                      << ":::Resource" << std::endl;
                mermaid_string_stream << resource_key_name << "-->" << subpass_key_name << std::endl;
                link_index++;
                link_styles.push_back({link_index, false});
            }

            const std::vector<TResource> &subpass_write_resources = subpass.writes;
            uint32_t subpass_write_resources_count = subpass_write_resources.size();
            for (uint32_t subpass_write_resource_index = 0; subpass_write_resource_index < subpass_write_resources_count; subpass_write_resource_index++)
            {
                const TResource &write_resource = subpass_write_resources[subpass_write_resource_index];
                Turbo::FrameGraph::TResourceNode &resource_node = this->GetResourceNode(write_resource);
                std::string resource_name = resource_node.GetName();
                uint32_t resource_version = resource_node.GetVersion();
                std::string resource_key_name = resource_name + std::to_string(resource_version);
                std::regex space_regex(" ");
                resource_key_name = std::regex_replace(resource_key_name, space_regex, "_");
                mermaid_string_stream << resource_key_name << "(\"" << resource_name << "\")"
                                      << ":::Resource" << std::endl;
                mermaid_string_stream << subpass_key_name << "-->" << resource_key_name << std::endl;
                link_index++;
                link_styles.push_back({link_index, true});
            }
        }
    }

    if (pass_node_cout > 0)
    {
        const TPassNode &start_pass_node_item = (*this->passNodes)[0];
        std::string start_pass_node_key_name = "PassNode" + std::to_string(start_pass_node_item.pass.id);

        const TPassNode &end_pass_node_item = (*this->passNodes)[pass_node_cout - 1];
        std::string end_pass_node_key_name = "PassNode" + std::to_string(end_pass_node_item.pass.id);

        mermaid_string_stream << "Start"
                              << "-.->" << start_pass_node_key_name << std::endl;
        link_index++;

        mermaid_string_stream << end_pass_node_key_name << "-.->"
                              << "End" << std::endl;
        link_index++;
    }

    for (const linkStyle &link_styles_item : link_styles)
    {
        if (link_styles_item.isWrite)
        {
            mermaid_string_stream << "linkStyle " << std::to_string(link_styles_item.index - 1) << " " << write_link_style << std::endl;
        }
        else
        {
            mermaid_string_stream << "linkStyle " << std::to_string(link_styles_item.index - 1) << " " << read_link_style << std::endl;
        }
    }

    return mermaid_string_stream.str();
}

Turbo::FrameGraph::TBlackboard &Turbo::FrameGraph::TFrameGraph::GetBlackboard()
{
    return this->blackboard;
}
