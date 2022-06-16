#pragma once
#ifndef TFRAMEGRAPH_H
#define TFRAMEGRAPH_H
//#include <concepts>
#include <cstdint>
#include <string>
#include <vector>
namespace Turbo
{
namespace FrameGraph
{

typedef struct TNodeHandle
{
    uint32_t id;
} TNodeHandle;

typedef struct TPass : public TNodeHandle
{
    // some extension data for future
} TPass;

typedef struct TResource : public TNodeHandle
{
    // some extension data for future
} TResource;

class TAgency
{
  public:
    // Create()
    // Destroy();
    // operate ()
};

template <typename Data, typename Execute>
class TPassAgency : public TAgency
{
  private:
    Execute execute;
    Data data{};

  public:
    TPassAgency(Execute &execute);
    Data &GetData();
};

template <typename Virtualizable>
class TResourceAgency : public TAgency
{
  private:
    uint32_t id;
    typename Virtualizable::Descriptor descriptor;
    Virtualizable *resource;

  public:
    TResourceAgency(uint32_t id, typename Virtualizable::Descriptor &&descriptor);

    Virtualizable &GetResoureceAgency();
};

class TNode
{
  private:
    std::string name;
    uint32_t refCount;

  public:
    TNode(const std::string &name);

    const std::string &GetName();
};

class TPassNode : public TNode
{
  private:
    TPass pass;
    TAgency *agency;

    std::vector<TResource> creates;
    std::vector<TResource> reads;
    std::vector<TResource> writes;

  public:
    TPassNode(const std::string &name, TPass pass, TAgency *agency);
    ~TPassNode();
    TResource AddCreate(TResource resource);
    TResource AddRead(TResource resource);
    TResource AddWrite(TResource resource);

    bool IsCreate(TResource resource);
    bool IsWrite(TResource resource);
    bool IsRead(TResource resource);
};

class TResourceNode : public TNode
{
  private:
    TResource resource;
    uint32_t agencyID;
    uint32_t version;

  public:
    TResourceNode(const std::string &name, TResource resource, uint32_t resourceID, uint32_t version);
    TResource GetResource();
    uint32_t GetAgencyID();
    uint32_t GetVersion();
};

class TFrameGraph
{
  private:
    std::vector<TPassNode> *passNodes;
    std::vector<TResourceNode> *resourceNodes;
    std::vector<TAgency *> *agencys;

  private:
    TPassNode &CreatePassNode(const std::string &name, TAgency *agency);
    TResourceNode &CreateResourceNode(const std::string &name, uint32_t resourceID);
    TResourceNode &CloneResourceNode(TResource resource);

  public:
    class TBuilder
    {
      private:
        TFrameGraph &frameGraph;
        TPassNode &passNode;

      public:
        TBuilder(TFrameGraph &frameGraph, TPassNode &passNode);

      public:
        template <typename Virtualizable>
        TResource Create(const std::string &name, typename Virtualizable::Descriptor &&descriptor);
        TResource Read(TResource resource);
        TResource Write(TResource resource);
    };

  public:
    TFrameGraph();
    ~TFrameGraph();

    template <typename Data, typename Setup, typename Execute>
    const Data &AddPass(const std::string &name, Setup &&setup, Execute &&execute);

    template <typename Virtualizable>
    TResource Create(const std::string &name, typename Virtualizable::Descriptor &&descriptor);

    bool IsValid(TResource resource);

    template <typename Virtualizable>
    Virtualizable &GetResourceAgency(TResource resource);

    void Compile();
    void Execute();
};

class TResources
{
  private:
    TFrameGraph &frameGraph;
    TPassNode &passNode;

  public:
    TResources(TFrameGraph &frameGraph, TPassNode &passNode);

    template <typename Virtualizable>
    Virtualizable &Get(TResource resource);

    template <typename Virtualizable>
    const typename Virtualizable::Descriptor &getDescriptor(TResource resource);
};

template <typename Data, typename Execute>
inline Turbo::FrameGraph::TPassAgency<Data, Execute>::TPassAgency(Execute &execute) : execute(execute)
{
}

template <typename Data, typename Execute>
inline Data &Turbo::FrameGraph::TPassAgency<Data, Execute>::GetData()
{
    return this->data;
}

template <typename Virtualizable>
inline Turbo::FrameGraph::TResourceAgency<Virtualizable>::TResourceAgency(uint32_t id, typename Virtualizable::Descriptor &&descriptor) : id(id), descriptor(descriptor)
{
}

template <typename Virtualizable>
inline Virtualizable &Turbo::FrameGraph::TResourceAgency<Virtualizable>::GetResoureceAgency()
{
    return *this->resource;
}

template <typename Virtualizable>
inline TResource Turbo::FrameGraph::TFrameGraph::TBuilder::Create(const std::string &name, typename Virtualizable::Descriptor &&descriptor)
{
    TResource resource = this->frameGraph.Create<Virtualizable>(name, std::move(descriptor));
    return this->passNode.AddCreate(resource);
}

template <typename Data, typename Setup, typename Execute>
inline const Data &Turbo::FrameGraph::TFrameGraph::AddPass(const std::string &name, Setup &&setup, Execute &&execute)
{
    TPassAgency<Data, Execute> *pass_agency = new TPassAgency<Data, Execute>(execute);
    TPassNode &pass_node = this->CreatePassNode(name, pass_agency);

    TBuilder builder(*this, pass_node);
    //std::invoke(setup, builder, pass_agency->GetData());
    return pass_agency->GetData();
}

template <typename Virtualizable>
inline TResource Turbo::FrameGraph::TFrameGraph::Create(const std::string &name, typename Virtualizable::Descriptor &&descriptor)
{
    const uint32_t agency_id = static_cast<uint32_t>(this->agencys->size());
    TResourceAgency<Virtualizable> *resource_agency = new TResourceAgency<Virtualizable>(agency_id, std::move(descriptor));
    this->agencys->emplace_back(resource_agency);
    TResourceNode &resource_node = this->CreateResourceNode(name, agency_id);
    return resource_node.GetResource();
}

} // namespace FrameGraph
} // namespace Turbo

#endif // !TFRAMEGRAPH_H