#pragma once
#ifndef TFRAMEGRAPH_H
#define TFRAMEGRAPH_H
#include <concepts>
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
};

template <typename Data, typename Execute>
class TPassAgency : public TAgency
{
  private:
    Execute execute;
    Data data{};

  public:
    TPassAgency(Execute execute);
    Data &GetData();
};

template <typename Virtualizable>
class TResourceAgency : public TAgency
{
  private:
    uint32_t id;
    typename Virtualizable::Descriptor descriptor;
    Virtualizable *resource;
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
};

class TResourceNode : public TNode
{
  private:
    TResource resource;
    TAgency *agency;
};

class TFrameGraph
{
  private:
    std::vector<TPassNode> *passNodes;
    std::vector<TResourceNode> *resourceNodes;
    std::vector<TAgency> *agencys;

  private:
    TPassNode &CreatePassNode(std::string &name, TAgency *agency);

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
};

template <typename Data, typename Execute>
Turbo::FrameGraph::TPassAgency<Data, Execute>::TPassAgency(Execute execute)
{
    this->execute = execute;
}

template <typename Data, typename Execute>
Data &Turbo::FrameGraph::TPassAgency<Data, Execute>::GetData()
{
    return this->data;
}

template <typename Data, typename Setup, typename Execute>
inline const Data &Turbo::FrameGraph::TFrameGraph::AddPass(const std::string &name, Setup &&setup, Execute &&execute)
{
    TPassAgency<Data, Execute> *pass_agency = new TPassAgency<Data, Execute>(execute);
    TPassNode &pass_node = this->CreatePassNode(name, pass_agency);

    TBuilder builder(*this, pass_node);
    std::invoke(setup, builder, pass_agency->GetData());
    return pass_agency->GetData();
}
} // namespace FrameGraph
} // namespace Turbo

#endif // !TFRAMEGRAPH_H