#pragma once
#ifndef TFRAMEGRAPH_H
#define TFRAMEGRAPH_H
//#include <concepts>
#include <cstdint>
#include <string>
#include <vector>

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

namespace Turbo
{
namespace FrameGraph
{
//<Test>
template <class T>
concept Integral = std::is_integral<T>::value;

template <class T>
concept SignedIntegral = Integral<T> && std::is_signed<T>::value;

template <class T>
concept UnsignedIntegral = Integral<T> && std::is_unsigned<T>::value;

template <typename T>
concept Virtualizable = requires(T t)
{
    requires std::is_default_constructible_v<T> and std::is_move_constructible_v<T>;

    typename T::Desc;
    {
        t.create(typename T::Desc{}, (void *)nullptr)
        } -> std::same_as<void>;
    {
        t.destroy(typename T::Desc{}, (void *)nullptr)
        } -> std::same_as<void>;
};
//</Test>

constexpr size_t EXECUTE_MAX_LOAD = 1024;
using ID = uint32_t;

struct TNodeHandle
{
    ID id;
};

struct TPass : public TNodeHandle
{
    // some extension data for future
};

struct TResource : public TNodeHandle
{
    // some extension data for future
};

class TAgency
{
  public:
    TAgency() = default;
    virtual ~TAgency() = default;

    // Create()
    // Destroy();
    // operate ()
};

template <typename Data, typename Execute>
class TPassAgency : public TAgency
{
  private:
    Data data{};
    Execute execute;

  public:
    TPassAgency(Execute &&execute);
    inline ~TPassAgency()
    {
        printf("~TPassAgency\n");
    }
    Data &GetData();

    // void operator()(TResources& resources,void* context);//waite for implements
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
    std::unique_ptr<TAgency> agency;

    std::vector<TResource> creates;
    std::vector<TResource> reads;
    std::vector<TResource> writes;

  public:
    TPassNode(const std::string &name, TPass pass, std::unique_ptr<TAgency> &&agency);
    TResource AddCreate(TResource resource);
    TResource AddRead(TResource resource);
    TResource AddWrite(TResource resource);

    bool IsCreate(TResource resource);
    bool IsWrite(TResource resource);
    bool IsRead(TResource resource);
};

using TAgencyID = uint32_t;
using TVersion = uint32_t;

class TResourceNode : public TNode
{
  private:
    TResource resource;
    TAgencyID agencyID;
    TVersion version;

  public:
    TResourceNode(const std::string &name, TResource resource, uint32_t resourceID, uint32_t version);
    TResource GetResource();
    TAgencyID GetAgencyID();
    TVersion GetVersion();
};

class TFrameGraph
{
  private:
    std::vector<TPassNode> *passNodes;
    std::vector<TResourceNode> *resourceNodes;
    std::vector<TAgency *> *agencys;

  private:
    TPassNode &CreatePassNode(const std::string &name, std::unique_ptr<TAgency> &&agency);
    TResourceNode &CreateResourceNode(const std::string &name, uint32_t resourceID);
    TResourceNode &CloneResourceNode(TResource resource);

  public:
    class TBuilder final
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
    TFrameGraph(const TFrameGraph &) = delete;
    TFrameGraph(TFrameGraph &&) noexcept = delete;
    ~TFrameGraph();

    TFrameGraph &operator=(const TFrameGraph &) = delete;
    TFrameGraph &operator=(TFrameGraph &&) noexcept = delete;

    template <typename Data, typename Setup, typename Execute>
    const Data &AddPass(const std::string &name, Setup &&setup, Execute &&execute); // *filament返回的是PassAgency

    template <typename Virtualizable>
    TResource Create(const std::string &name, typename Virtualizable::Descriptor &&descriptor); // *filament接收的是descriptor的左值引用，非右值

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
inline Turbo::FrameGraph::TPassAgency<Data, Execute>::TPassAgency(Execute &&execute) : execute{std::forward<Execute>(execute) /*or std::move()? in filament*/}
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
    static_assert(std::is_invocable<Setup, TFrameGraph::TBuilder &, Data &>::value, "Invalid Setup");
    static_assert(std::is_invocable<Execute, const Data &, TResources &, void *>::value, "Invalid Execute");
    static_assert(sizeof(Execute) < EXECUTE_MAX_LOAD, "Execute overload");

    TPassAgency<Data, Execute> *pass_agency = new TPassAgency<Data, Execute>(std::forward<Execute>(execute));
    TPassNode &pass_node = this->CreatePassNode(name, std::unique_ptr<TAgency>(pass_agency));

    TBuilder builder(*this, pass_node);
    std::invoke(setup, builder, pass_agency->GetData());
    return pass_agency->GetData();
}

template <typename Virtualizable>
inline TResource Turbo::FrameGraph::TFrameGraph::Create(const std::string &name, typename Virtualizable::Descriptor &&descriptor)
{
    const uint32_t agency_id = static_cast<uint32_t>(this->agencys->size());
    TResourceAgency<Virtualizable> *resource_agency = new TResourceAgency<Virtualizable>(agency_id, std::move(descriptor)); //* filament中name是属于资源代理的，描述是左值引用，资源代理有个优先级参数
                                                                                                                            //* filament中资源代理继承结构：VirtualResource -> ResourceEntryBase -> ResourceEntry
                                                                                                                            //* VirtualResource: 1.PassNode* first;第一次使用该资源的Pass，将会实例化资源
                                                                                                                            //                   2.PassNode* last;最后一个使用该资源的Pass，将会销毁该资源
                                                                                                                            //* ResourceEntryBase: 1.char* name;名称
                                                                                                                            //*                    2.uint16_t id;标识号
                                                                                                                            //*                    3.bool imported;是否为外部引入
                                                                                                                            //*                    4.uint8_t priority;优先级
                                                                                                                            //*                    5.uint8_t version;资源版本，由builder更新，概念与当前架构相同
                                                                                                                            //*                    6.uint32_t refs; ;资源索引数（读取当前资源数），compile()阶段计算
                                                                                                                            //*                    7.uint32_t refs; ;资源索引数（读取当前资源数），compile()阶段计算
                                                                                                                            //*                    8.bool discardStart = true;未知
                                                                                                                            //*                    9.bool discardEnd = false;未知
                                                                                                                            //* ResourceEntry：1.T resource;容器，用户指定的资源
    this->agencys->emplace_back(resource_agency);
    TResourceNode &resource_node = this->CreateResourceNode(name, agency_id);
    return resource_node.GetResource();
}

} // namespace FrameGraph
} // namespace Turbo

#endif // !TFRAMEGRAPH_H