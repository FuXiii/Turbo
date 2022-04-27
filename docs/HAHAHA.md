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
