# 备忘录

```CXX
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    bindings.resize(binding_count);

    for (uint32_t binding_index; binding_index < binding_count; binding_index++)
    {
        TDescriptor *descriptor = this->descriptors[binding_index];
        if (descriptor != nullptr)
        {
            VkDescriptorSetLayoutBinding& vk_descriptor_set_layout_binding = bindings[binding_index];
            vk_descriptor_set_layout_binding.binding = descriptor->GetBinding();
            vk_descriptor_set_layout_binding.descriptorType = descriptor->GetVkDescriptorType();
            vk_descriptor_set_layout_binding.descriptorCount = descriptor->GetCount();
            vk_descriptor_set_layout_binding.stageFlags = this->shader->GetVkShaderStageFlags();
            vk_descriptor_set_layout_binding.pImmutableSamplers = nullptr;
        }
        else
        {
            throw Turbo::Core::TException(TResult::INVALID_PARAMETER);
        }
    }
```

```CXX
    std::vector<int> ints;
    std::vector<int>::iterator int_iterator = ints.begin();
    for (int &int_item : ints)
    {
        ints.erase(int_iterator);
        int_iterator++;
    }
```

## 容器对应 TRefPtr<T> 和 T* 类型的形参问题

```CXX
    void CmdBindVertexBuffers(const std::vector<TRefPtr<TBuffer>> &vertexBuffers = {});
    void CmdBindVertexBuffers(const std::vector<TBuffer *> &vertexBuffers = {});
    //FIX::对于如上 vector<TRefPtr<TBuffer>/TBuffer *> 形参，考虑使用 模板，这样就不需要声明两个函数了，只需要声明一个模板函数即可：
    template<typename T>
    void CmdBindVertexBuffers(const std::vector<T> &vertexBuffers = {});//需要对 T 进行限制
    //NOTE: 该方法编程方便，但可读性较差。
```

事实上 ``std::vector`` 都不是强制要求，只要：

* 传入的 ``容器`` 支持 ``for(:)`` 遍历
* ``容器`` 中存储的项为 ``TReference*`` 或 ``TRefPtr<T>``

``Turbo`` 的本质需求就是将用户传递进来的一串集合转成 ``Vulkan`` 要求的数组。

## 使用 TRefPtr<T>(ptr).Valid() 检查 ptr 的问题

```CXX
void SomeFun(T* ptr)
{
    //FIX::建议使用专门的检查函数，或构造函数
    if(!TRefPtr<T>(ptr).Valid())//该行结束可能会导致 ptr 进行 delete
    {
        return;
    }

    ...//Do valid
}

T* t = new T();
SomeFun(t);//SomeFun 结束之后 t 如果没人使用将会被 delete
```
