#pragma once
#ifndef TURBO_FRAMEGRAPH_TFRAMEGRAPH_H
#define TURBO_FRAMEGRAPH_TFRAMEGRAPH_H
#include <assert.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// #include <concepts>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>

namespace Turbo
{
namespace FrameGraph
{
constexpr size_t EXECUTE_MAX_LOAD = 1024;
constexpr uint32_t TURBO_INVALID_ID = std::numeric_limits<uint32_t>::max();
constexpr uint32_t TURBO_INVALID_SUBPASS_INDEX = std::numeric_limits<uint32_t>::max();
using ID = uint32_t;
using TVersion = uint32_t;

struct TNodeHandle
{
    ID id = TURBO_INVALID_ID;
};

struct TPass : public TNodeHandle
{
    // some extension data for future
};

struct TResource : public TNodeHandle
{
    // some extension data for future
    bool isInput = false;
};

class TProxy
{
  public:
    TProxy() = default;
    virtual ~TProxy() = default;

    // Create()
    // Destroy();
    // operate ()
};

class TResources;

class TPassExecutorProxy : public TProxy
{
  public:
    TPassExecutorProxy() = default;
    ~TPassExecutorProxy() = default;
    virtual void Executor(const TResources &resources, void *context) = 0;
};

template <typename TData, typename TExecute>
class TPassProxy : public TPassExecutorProxy
{
  private:
    TData data{};
    TExecute execute;

  public:
    TPassProxy(TExecute &&execute);
    ~TPassProxy() = default;
    TData &GetData();

    virtual void Executor(const TResources &resources, void *context) override;
};

class TVirtualResourceProxy : public TProxy
{
  public:
    friend class TFrameGraph;

  private:
    std::string name;
    TVersion version;

    TPass firstUser;
    TPass lastUser;

  public:
    TVirtualResourceProxy(const std::string &name);

    const std::string &GetName();

    TVersion GetVersion();

    virtual void Create(void *allocator) = 0;
    virtual void Destroy(void *allocator) = 0;
};

template <typename T>
class TResourceProxy : public TVirtualResourceProxy
{
  private:
    uint32_t id;
    typename T::Descriptor descriptor;
    T resource; // FIXME: 此处可能会有问题，用户不一定指定带有默认构造函数的资源类，详见Issue文档

  public:
    TResourceProxy(const std::string &name, uint32_t id, typename T::Descriptor &&descriptor);

    T &GetResource();

    virtual void Create(void *allocator) override;
    virtual void Destroy(void *allocator) override;
};

class TSubpass
{
  public:
    friend class TFrameGraph;

  private:
    std::vector<TResource> writes;
    std::vector<TResource> reads;
    std::vector<TResource> inputs;

  public:
    TSubpass() = default;
    ~TSubpass() = default;

    void Write(TResource resource);
    void Read(TResource resource);
    void Input(TResource resource);

    std::vector<TResource> GetWrites();
    std::vector<TResource> GetReads();
    std::vector<TResource> GetInputs();
};

class TRenderPass
{
  public:
    friend class TFrameGraph;

  private:
    std::vector<Turbo::FrameGraph::TSubpass> subpasses;

  public:
    TRenderPass() = default;
    ~TRenderPass() = default;

    void AddSubpass(const TSubpass &subpass);

    std::vector<Turbo::FrameGraph::TSubpass> GetSubpasses();
};

class TNode
{
  public:
    friend class TFrameGraph;

  private:
    uint32_t refCount = 0;

  public:
    TNode();
};

class TPassNode : public TNode
{
  public:
    friend class TFrameGraph;

  private:
    std::string name;

    TPass pass;
    std::unique_ptr<TPassExecutorProxy> passExecutorProxy;

    std::vector<TResource> creates;
    std::vector<TResource> reads;
    std::vector<TResource> writes;

    std::vector<TVirtualResourceProxy *> devirtualizes;
    std::vector<TVirtualResourceProxy *> destroies;

    TRenderPass renderPass;

    bool sideEffect = false;

  public:
    TPassNode(const std::string &name, TPass pass, std::unique_ptr<TPassExecutorProxy> &&passExecutorProxy);
    TResource AddCreate(TResource resource);
    TResource AddRead(TResource resource);
    TResource AddWrite(TResource resource);

    bool IsCreate(TResource resource);
    bool IsWrite(TResource resource);
    bool IsRead(TResource resource);

    TRenderPass GetRenderPass();

    const std::string &GetName();
};

class TResourceNode : public TNode
{
  public:
    friend class TFrameGraph;

  private:
    TResource resource;
    TVirtualResourceProxy *virtualResourceProxy;
    TVersion version;

    TPass writer;

  public:
    TResourceNode(TResource resource, TVirtualResourceProxy *virtualResourceProxy, uint32_t version);
    TResource GetResource();
    TVersion GetVersion();
    TVirtualResourceProxy *GetResourceProxy();
    const std::string &GetName();
};

class TBlackboard
{
  private:
    std::unordered_map<std::string, TResource> blackboardMap;

  public:
    TBlackboard() = default;
    ~TBlackboard() = default;

    TResource &operator[](std::string const &name)
    {
        if (this->blackboardMap.find(name) != blackboardMap.end())
        {
            return this->blackboardMap[name];
        }

        return this->blackboardMap[name] = TResource{};
    }
};

class TFrameGraph
{
  private:
    std::vector<TPassNode> *passNodes;
    std::vector<TResourceNode> *resourceNodes;
    std::vector<TVirtualResourceProxy *> *resourceProxys;

    TBlackboard blackboard;

  private:
    TPassNode &CreatePassNode(const std::string &name, std::unique_ptr<TPassExecutorProxy> &&passExecutorProxy);
    TResourceNode &CreateResourceNode(TVirtualResourceProxy *virtualResourceProxy);

  public:
    class TBuilder final
    {
      public:
        class TSubpass final
        {
          private:
            TBuilder *builder = nullptr;
            uint32_t index = TURBO_INVALID_SUBPASS_INDEX;

          public:
            TSubpass() = default;
            TSubpass(TBuilder *builder, uint32_t index);
            ~TSubpass() = default;

            TResource Write(TResource resource);
            TResource Read(TResource resource);
            TResource Input(TResource resource);
        };

      private:
        TFrameGraph &frameGraph;
        TPassNode &passNode;

      public:
        TBuilder(TFrameGraph &frameGraph, TPassNode &passNode);

      private:
        TResource Read(TResource resource);
        TResource Write(TResource resource);

      public:
        template <typename T>
        TResource Create(const std::string &name, typename T::Descriptor &&descriptor);

        Turbo::FrameGraph::TFrameGraph::TBuilder::TSubpass CreateSubpass();

        TBuilder &SideEffect();
    };

  public:
    TFrameGraph();
    TFrameGraph(const TFrameGraph &) = delete;
    TFrameGraph(TFrameGraph &&) noexcept = delete;
    ~TFrameGraph();

    TFrameGraph &operator=(const TFrameGraph &) = delete;
    TFrameGraph &operator=(TFrameGraph &&) noexcept = delete;

    template <typename TData, typename TSetup, typename TExecute>
    const TData &AddPass(const std::string &name, TSetup &&setup, TExecute &&execute); // *filament返回的是PassProxy

    template <typename T>
    TResource Create(const std::string &name, typename T::Descriptor &&descriptor); // *filament接收的是descriptor的左值引用，非右值

    bool IsValid(TResource resource);
    bool IsValid(TPass pass);

    TResourceNode &GetResourceNode(TResource resource);

    void Compile();
    void Execute(void *context = nullptr, void *allocator = nullptr);

    std::string ToMermaid();

    TBlackboard &GetBlackboard();
};

class TResources
{
  private:
    TFrameGraph &frameGraph;
    TPassNode &passNode;

  public:
    TResources(TFrameGraph &frameGraph, TPassNode &passNode);

    template <typename T>
    T &Get(TResource resource) const;

    // template <typename Virtualizable>
    // const typename Virtualizable::Descriptor &getDescriptor(TResource resource);
};

template <typename TData, typename TExecute>
inline Turbo::FrameGraph::TPassProxy<TData, TExecute>::TPassProxy(TExecute &&execute) : execute{std::forward<TExecute>(execute) /*or std::move()? in filament*/}
{
}

template <typename TData, typename TExecute>
inline TData &Turbo::FrameGraph::TPassProxy<TData, TExecute>::GetData()
{
    return this->data;
}

template <typename TData, typename TExecute>
inline void Turbo::FrameGraph::TPassProxy<TData, TExecute>::Executor(const TResources &resources, void *context)
{
    this->execute(this->data, resources, context);
}

template <typename T>
inline Turbo::FrameGraph::TResourceProxy<T>::TResourceProxy(const std::string &name, uint32_t id, typename T::Descriptor &&descriptor) : TVirtualResourceProxy(name), id(id), descriptor(descriptor)
{
}

template <typename T>
inline T &Turbo::FrameGraph::TResourceProxy<T>::GetResource()
{
    return this->resource;
}

template <typename T>
inline void Turbo::FrameGraph::TResourceProxy<T>::Create(void *allocator)
{
    this->resource.Create(this->GetName(), this->descriptor, allocator);
}

template <typename T>
inline void Turbo::FrameGraph::TResourceProxy<T>::Destroy(void *allocator)
{
    this->resource.Destroy(allocator);
}

template <typename T>
inline TResource Turbo::FrameGraph::TFrameGraph::TBuilder::Create(const std::string &name, typename T::Descriptor &&descriptor)
{
    TResource resource = this->frameGraph.Create<T>(name, std::forward<typename T::Descriptor>(descriptor));
    return this->passNode.AddCreate(resource);
}

template <typename TData, typename TSetup, typename TExecute>
inline const TData &Turbo::FrameGraph::TFrameGraph::AddPass(const std::string &name, TSetup &&setup, TExecute &&execute)
{
    // static_assert(std::is_invocable<TSetup, TFrameGraph::TBuilder &, TData &>::value, "Invalid Setup");//deepin linux don't support c++ 17
    // static_assert(std::is_invocable<Execute, const TData &, const TResources &, void *>::value, "Invalid Execute");//deepin linux don't support c++ 17
    static_assert(sizeof(TExecute) < EXECUTE_MAX_LOAD, "Execute overload");

    TPassProxy<TData, TExecute> *pass_proxy = new TPassProxy<TData, TExecute>(std::forward<TExecute>(execute));
    TPassNode &pass_node = TFrameGraph::CreatePassNode(name, std::unique_ptr<TPassExecutorProxy>(pass_proxy));

    TBuilder builder(*this, pass_node);
    // std::invoke(setup, builder, pass_proxy->GetData());//deepin linux don't support c++ 17
    setup(builder, pass_proxy->GetData());
    return pass_proxy->GetData();
}

template <typename T>
inline TResource Turbo::FrameGraph::TFrameGraph::Create(const std::string &name, typename T::Descriptor &&descriptor)
{
    const uint32_t resource_proxy_id = static_cast<uint32_t>(this->resourceProxys->size());
    TResourceProxy<T> *resource_proxy = new TResourceProxy<T>(name, resource_proxy_id, std::forward<typename T::Descriptor>(descriptor)); //* filament中name是属于资源代理的，描述是左值引用，资源代理有个优先级参数
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
    this->resourceProxys->emplace_back(resource_proxy);
    TResourceNode &resource_node = TFrameGraph::CreateResourceNode(resource_proxy);
    return resource_node.GetResource();
}

template <typename T>
inline T &Turbo::FrameGraph::TResources::Get(TResource resource) const
{
    assert(this->passNode.IsCreate(resource) || this->passNode.IsRead(resource) || this->passNode.IsWrite(resource));
    TResourceProxy<T> *resource_proxy = dynamic_cast<TResourceProxy<T> *>(this->frameGraph.GetResourceNode(resource).GetResourceProxy());
    return resource_proxy->GetResource();
}

} // namespace FrameGraph
} // namespace Turbo

#endif // !TURBO_FRAMEGRAPH_TFRAMEGRAPH_H