# Issue

## demo

- `fixed :` <font color=green>**[ â ]year/month/day**</font>
- `error :` <font color=red>**[ ð­ ]year/month/day**</font>
- `alarm :` <font color=yellow>**[ ! ]year/month/day**</font>
- `develope :` <font color=orange>**[ ð  ] now**</font>

### **ææ¡£ï¼å«å¿åææ¡£**

- èè`Turbo`ä¸­æ¯å¦åç½®å ä¸ª`TCommandBufferPool`ç¨äº`Turbo`å¼æåé¨èªå·±ç¨ï¼å¨`Turbo::Core::TDevice`è¢«åå»ºï¼åé¨çæ`Turbo::Core::TDeviceQueue`ä¹åï¼`Turbo::Core::TDevice`è¢«éæ¯ä¸å¶ç¸å³ç`TCommandBufferPool`åå¶ç¸å³çåé¨èµæºä¹ä¼ä¸å¹¶éæ¯ï¼ãï¼è¯¥åè½åºè¯¥å¨`CommandBuffer`ææåè½å®ç°ç»æä¹åå®ç°ï¼

- <font color=green>**[ â ]2022/5/6**</font> ~~<font color=yellow>**[ ! ]2022/5/6 åºæ¬ææ¸é®é¢åå äºï¼`TFormatInfo` éè¦æä¾å¯¹äº`VkFormatFeatureFlagBits`çæ¯æï¼éè¿è°ç¨`vkGetPhysicalDeviceFormatProperties`è·å¾ã**</font> `Turbo::Core::TCommandBuffer::BeginRenderPass()ä¸­çClearColor` å¯è½æé®é¢ï¼ç­å°æ¯æçªå£æ¸²æååè°ï¼è°è¯èµ·æ¥æ¹ä¾¿ï¼ï¼èä¸ç°å¨åæ­»äºï¼éè¦å¼æ¾å¯¹å¤æ¥å£(æ¯å¦è¯´æ¾å°`TAttachemnt`ä¸­å£°æ)ã~~

- èèå°`TCommandBuffer::BeginRenderPass()`ä¸­ç`ClearColor`å¾é¢è²æ°æ®æ¾å°`TAttachment`ä¸­ï¼è¿æ ·ç¨æ·å°±å¯ä»¥èªå®ä¹`ClearColore`äº

- `Turbo::Core::TCommandBuffer::BeginRenderPass()ä¸­çClearColor` ä¸­æäºå½æ°å½¢åä¼ çæ¯å¼ç¨ï¼ç¹å«æ¯`std::vector<XXX>& xxx`ï¼ï¼è¿å°ä¼å¯¼è´å¾å¤æ²¡æå¿è¦çåå­åå»ºï¼å°è¯å°å¼ç¨æ¹ææéã

- <font color=green>**[ â ]2022/5/1**</font> ~~`TFormatInfo` ç®åç¨èµ·æ¥æ¯è¾éº»ç¦ï¼æä¾ä¸ç§ç®ä¾¿æ¹æ³ï¼æ¯å¦éç¤ºè½¬æ¢ã~~

- `TFormatInfo`å¢å å¯¹äº`vkGetPhysicalDeviceFormatProperties(...)`å`vkGetPhysicalDeviceImageFormatProperties(...)`åè½çå®ç°æ¥å£

- `Turbo::Core::TBuffer`çæé å½æ°éè¦ä¼ å¥`VkBufferCreateFlags`ï¼å¨`Turbo`ä¸­ç¨èµ·æ¥æ¯è¾å¥æªï¼æèä¸åå¤æä¾è¯¥åæ°ï¼ç®åæä¸æ¸æ¥ç¨æ³ï¼æèå°±è¿ä¹æ¾çå°æååè¯´ï¼

- <font color=green>**[ â ]2022/4/28**</font> ~~`Turbo::Core::TBuffer`çæé å½æ°éè¦ä¼ å¥`VkDeviceSize`ï¼å¨`Turbo`ä¸­å¦å¤å®ä¹`TDeviceSize`~~

- <font color=green>**[ â ]2022/4/28**</font> ~~`Turbo::Core::TBuffer`çæé å½æ°éè¦ä¼ å¥`VkBufferUsageFlagBits`ï¼å¨`Turbo`ä¸­ç¨èµ·æ¥æ¯è¾å¥æªï¼æé èªå·±çç¸å³æ°æ®ç»æ~~

- `Turbo::Core::TImage`çæé å½æ°éè¦ä¼ å¥`VkImageCreateFlags`ï¼å¨`Turbo`ä¸­ç¨èµ·æ¥æ¯è¾å¥æªï¼æèä¸åå¤æä¾è¯¥åæ°ï¼ç®åæä¸æ¸æ¥ç¨æ³ï¼æèå°±è¿ä¹æ¾çå°æååè¯´ï¼

- <font color=green>**[ â ]2022/4/28**</font> ~~`Turbo::Core::TImage`çæé å½æ°éè¦ä¼ å¥`VkImageType`ï¼å¨`Turbo`ä¸­ç¨èµ·æ¥æ¯è¾å¥æªï¼æé èªå·±çç¸å³æ°æ®ç»æ~~

- <font color=green>**[ â ]2022/4/28**</font> ~~`Turbo::Core::TImage`çæé å½æ°éè¦ä¼ å¥`VkSampleCountFlagBits`ï¼å¨`Turbo`ä¸­ç¨èµ·æ¥æ¯è¾å¥æªï¼æé èªå·±çç¸å³æ°æ®ç»æ~~

- <font color=green>**[ â ]2022/4/28**</font> ~~`Turbo::Core::TImage`çæé å½æ°éè¦ä¼ å¥`VkImageTiling`ï¼å¨`Turbo`ä¸­ç¨èµ·æ¥æ¯è¾å¥æªï¼æé èªå·±çç¸å³æ°æ®ç»æ~~

- <font color=green>**[ â ]2022/4/28**</font> ~~`Turbo::Core::TImage`çæé å½æ°éè¦ä¼ å¥`VkImageUsageFlags`ï¼å¨`Turbo`ä¸­ç¨èµ·æ¥æ¯è¾å¥æªï¼æé èªå·±çç¸å³æ°æ®ç»æ~~

- <font color=green>**[ â ]2022/4/28**</font> ~~`Turbo::Core::TImage`çæé å½æ°éè¦ä¼ å¥`VkImageLayout`ï¼å¨`Turbo`ä¸­ç¨èµ·æ¥æ¯è¾å¥æªï¼æé èªå·±çç¸å³æ°æ®ç»æ~~

- <font color=green>**[ â ]2022/4/28**</font> ~~`Turbo::Core::TImageView`çæé å½æ°éè¦ä¼ å¥`VkImageViewType`ï¼å¨`Turbo`ä¸­ç¨èµ·æ¥æ¯è¾å¥æªï¼æé èªå·±çç¸å³æ°æ®ç»æ~~

- <font color=green>**[ â ]2022/4/28**</font> ~~`Turbo::Core::TImageView`çæé å½æ°éè¦ä¼ å¥`VkImageAspectFlags`ï¼å¨`Turbo`ä¸­ç¨èµ·æ¥æ¯è¾å¥æªï¼æé èªå·±çç¸å³æ°æ®ç»æ~~

- `Turbo::Core::TShader`ä¸­å¯¹äº`code`çäºè¿å¶æ°æ®æ¯ä¸ç´å­å¨äºåå­çï¼å¯¹äº`Vulkan`æ åæ¥è¯´ï¼è¿é¨åäºè¿å¶æ°æ®å¨`VkShaderModule`åå»ºå®ä¹åæ¯æ²¡ææä¹çï¼ç®åè¯¥é¨åæ°æ®ï¼ä»ä»æ¯ç¨äº`ToString()`æ¶è½¬æ`Shader`å¯è¯»ä»£ç è¾åºï¼æ å¶ä»æç¨æ§ã

- `Turbo::Core::TShader`ä¸­å¯¹äº`VkDescriptor`çæ¯æï¼ç®åè¿ç¼ºå°å¯¹äº`storage image`, `subpass input(input attachemnt)`å`storage buffer`

- <font color=orange>**[ ð  ] now**</font> å®ç° `subpass input(input attachemnt)`ç¸å³ç¹æ§ï¼å®æå»¶è¿æ¸²æç¸å³åè½

- <font color=green>**[ â ]2022/5/14**</font> ~~`Turbo::Core::TShader`ä¸­å¯¹äº`VkDescriptor`çæ¯æï¼å¼å§éé`texture`ï¼`sampler`ã~~

- <font color=green>**[ â ]2022/5/14**</font> ~~`Turbo::Core::TDescriptorSet`ä¸­å¯¹äº`Turbo::Core::TDescriptorSet::BindData()`å½æ°åªæ¯ç¨æ¥æµè¯çï¼åªæ¯æ`VkWriteDescriptor`å`UniformBuffer`ï¼å¶ä»çå¾å®ç°~~

- <font color=green>**[ â ]2022/5/14**</font> ~~`Turbo::Core::TDescriptorSet`ä¸­å¯¹äº`Turbo::Core::TDescriptorSet::BindData()`å½æ°åªæ¯ç¨æ¥æµè¯çï¼åªæ¯æ`VkWriteDescriptor`å`UniformBuffer`ï¼å¶ä»çå¾å®ç°~~

- <font color=green>**[ â ]2022/4/28**</font> ~~`Turbo::Core::TSubpass::Add[...]AttachmentReference`ä¸­å¯¹äº`VkImageLayout`çä¼ å¥å`TImageView`é®é¢ä¸æ ·~~

- **`Note:`** `Turbo::Core::TSubpass`ä¸­ç`std::vector<VkAttachmentReference> ...`ä¿çï¼ç¨èµ·æ¥å¾æ¹ä¾¿

- èè`VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC`å¦ä½éé

- <font color=green>**[ â ]2022/4/29**</font> ~~`Turbo::Core::TAttachment`æé å½æ°ä¸­å¯¹äº`Vulkan`å£°æçæ°æ®ç»æä¾èµè¿å¤ï¼é¡»èªå®ä¹ï¼`VkFormat`,`VkSampleCountFlagBits`,`VkAttachmentLoadOp`,`VkAttachmentStoreOp`ç­ï¼~~

- <font color=green>**[ â ]2022/4/30**</font> ~~å¯¹äº`VkVertexInputBindingDescription`å`VkVertexInputAttributeDescription`èèå¦ä½è®¾è®¡ï¼ä½¿ç¨èµ·æ¥æ¹ä¾¿~~

- <font color=green>**[ â ]2022/5/3**</font> ~~`Turbo::Core::TRenderPass`æé å½æ°ç`TPipelineType type`è½¬ç§»å°`TSubpass`ä¸­~~

- <font color=green>**[ â ]2022/5/2**</font>  ~~`Turbo::Core::TPipeline`éåº¦ä¾èµ`Vulkan`çæ°æ®ç»æ~~

- `Turbo::Core::TPipeline`ç®ååªå®ç°äº`Graphic`ï¼è®¡ç®ç®¡çº¿`Compute`æªå®ç°

- <font color=green>**[ â ]2022/5/1**</font> ~~`Turbo::Core::TPipeline`ç®åæé å½æ°ç´æ¥åå¥çæ¯`Graphic`ç®¡çº¿æ°æ®ï¼è¯¥è®¾è®¡æé®é¢ï¼`Graphic`å¾å½¢ç®¡çº¿åè®¡ç®ç®¡çº¿`Compute`åå»ºåæ°ä¸åï¼ç¸å³åè½ååå»ºåºè¯¥æ¾å°å·ä½çå­ç±»ä¸­ï¼`TPipeline`åªæä¾èåºç±»~~

- <font color=green>**[ â ]2022/5/27**</font> ~~`Turbo::Core::TPipeline`ç®å`...CreatInfo`ä¸å®æ´ï¼æäºæ°æ®æ¯åæ­»çï¼å¦ï¼~~
  - ~~`VkPipelineMultisampleStateCreateInfo`~~
  - ~~`VkPipelineDepthStencilStateCreateInfo`~~
  - ~~`VkPipelineColorBlendAttachmentState`~~
  - ~~`VkPipelineColorBlendStateCreateInfo`~~
  - ~~`VkPipelineDynamicStateCreateInfo`~~

- ç®ååªå®ç°äº`TGraphicsPipeline`ä¸­ææç`VkPipelineShaderStageCreateInfo::pName`æä»£ç`Shader`å¥å£å½æ°å¨æ¯`main`,èèæ¯å¦æä¾èªå®ä¹åæ¥å£(`glslang`å¥½åæä¾ç¸å³å¥å£å½æ°è·ååè½)

- ç®ååªå®ç°äº`TGraphicsPipeline`ä¸­ç`VkDynamicState`ç®åæ¯åæ­»çå°`VK_DYNAMIC_STATE_VIEWPORT`ã`VK_DYNAMIC_STATE_SCISSOR`å`VK_DYNAMIC_STATE_LINE_WIDTH`å å¥äºå¨ææ°ç»ä¸­

- `Turbo::Core::TCommandBuffer`å¾å¤æä»¤æªå®ç°ï¼å¾å®ç°

- `Second Command Buffer`ç®å`Turbo`ä¸æ¯æ

- å°è¯éè½½`TSubpass`ç`operator >>`ç¨äºæææ¸²æé¾ï¼æ¯å¦ï¼(ä¹è®¸æ¾å°FrameGraphå±æ¯ä¸ªä¸éçéæ©)

```CXX
TSubpassChain subpass_chain_1 = subpass1 >> subpass2 >> subpass3;
TSubpassChain subpass_chain_2 = subpass4 >> subpass5 >> subpass6;

TRenderPass render_pass_1(subpass_chain_1);
TRenderPass render_pass_2(subpass_chain_2);
```

- <font color=green>**[ â ]2022/5/4**</font> ~~å®ç°`Barrier`~~

- <font color=green>**[ â ]2022/5/5**</font> ~~å®ç°`TCommandBuffer::FillBuffer()`~~

- ä¹è®¸`TCommandBuffer`å¯¹äº`Barrier`ä¸­ç`TAccess`ä¹è®¸å¯ä»¥ç±åé¨æ ¹æ®`oldLayout`å`newLayout`æ¥ç¡®å®æ°å¼

- éè¦æä¾`TBufferView`

- éè¦æä¾`TDisplay`

- éè¦ä¿®ç¼®`TExtension`ï¼å°è¯åæ`TInstanceExtension`å`TPhysicalDeviceExtension`

- <font color=green>**[ â ]2022/5/16**</font> ~~éè¦æä¾`TFilter`å¯¹åº`VkFilter`ï¼å®å`TCommandBuffer::BlitImage(...)`~~

- éè¦æä¾å¯¹åº`KTX`æä»¶çæ¯æ(éæ ¸å¿)

- éè¦æä¾å¯¹åº`gltf`æä»¶çæ¯æ(éæ ¸å¿)

- <font color=green>**[ â ]2022/5/10**</font> ~~å®ç°ç´¢å¼ç»å¶~~

- <font color=green>**[ â ]2022/5/11**</font> ~~å®ç°çº¹çç¹æ§~~ å¼å¥`KTX`è§è`(KTXå·¥å·åºç®åå¥½åä¸æ¯æMinGWç¼è¯å¨)`

- <font color=green>**[ â ]2022/5/12**</font>~~`Descriptor`è®¾è®¡æéå¤§é»è¾æ¼æ´ï¼éè¦éæ~~

- `TSampler`ç`VkCompareOp compareOp`ç¹æ§ç®ååºå®ä¸º`VK_FALSE`,ç­ç»ä¸è®¾è®¡æ·±åº¦æµè¯é£ä¸é¨ååå±å¼

- å½å`TSurface`åªæ¯æ`Windows`å¹³å°ï¼å¶ä»å¹³å°æªå®ç°ï¼ä½çäºæ¥å£

- ç¬¬ä¸æ¹åº`glm`å¯¹äº`MVP`ç©éµå¼å¸¸çé®é¢

- å½å`DescriptorSet`è®¾è®¡æ»æè§ä¸åçï¼ä¼åä½ï¼åºè¯¥æä¸ªæ± å­

- ä½¿ç¨å è½½å¨æåºçæ¹å¼å è½½`Vulkan`åº

- `TCommandBuffer::CmdPushConstants(...)`ä¸­ç`ShaderStages`è¿ä¸æ­¥ç ç©¶ï¼ç°å¨åçæé®é¢

- æ¢³çä¸ä¸`TExtensionInfo`ç±»ï¼æå¥½ç±»ä¼¼`TFormatInfo`çç±»ææ³

- `TSampler`çåé¡¹å¼æ§æ²¡å¼è¦å`TDevice`å¼å¯ç`feature`éå

- æ´çè®¾è®¡æå³`Vulkan`ä¸­ç`ImageSubresource`

- `TInstance`ç`IsEnabledExtension(string)`å½æ°ä¸­åè¾äºä¸¤éï¼æ²¡æå¿è¦ï¼æä¾ä¸ä¸ªæ°çå¯¹åºå½æ°ï¼`IsEnabledExtension(Turbo::Core::TExtensionType)`

- `TImage`éè¦å¯¹å¤æä¾`TImageLayout GetImageLayout()`æ¥è·åå½åçå¾åå¸å±,æèä¹è®¸åºè¯¥æ¯`TCommandBuffer`ä¸­è·åå½å`TImage`ç`TImageLayout`ï¼å ä¸ºå¾çå¸å±å¨`TCommandBuffer`ä¸­éæåæ¢çï¼åªæå¨`TCommanBuffer`è¢«æ¨éè¿è¡åï¼å¾çå¸å±æç¡®å®ä¸æ¥ï¼æ­¤æ¶å¯ä»¥ç¨äºå·æ°çæ­£`TImage`ä¸­çå¾çå¸å±ï¼`TCommandBuffer`å½ä»¤ä¹é´çå¾çå¸å±åæ¢åªæ¯åæ¢`TImage`çä¸´æ¶å¾çå¸å±

- <font color=green>**[ â ]2022/5/3**</font>  ~~`Subpass Dependence`ç®å`Turbo`æªå®ç°~~

- <font color=green>**[ â ]2022/5/2**</font> ~~å®ç°`Turbo::Core::TCommandBuffer::NextSubpass()`å½æ°~~

- <font color=green>**[ â ]2022/5/2**</font>  ~~`Turbo`ç®ååªæ¯æåéæ ·ï¼å¤éæ ·æªæ£é¼ãå¯¹äºå¤éæ ·æ¯`TSubpassPass`çåè½ï¼å¨`TSubpass`ä¸­æå®å¤éæ ·éä»¶ï¼æ ¹æ®`Vulkan`æ åï¼è§å®`ResolveAttachment`ä¸­çå¤éæ ·éä»¶å¦ææçè¯ï¼æ°éä¸å®ç­äº`ColorAttachment`(è¦ä¸å°±é½æ¯å¤éæ ·ï¼è¦ä¸å°±é½æ¯åéæ ·),`TPipeline`ä¸­ä¹æ`VkPipelineMultisampleStateCreateInfo`~~

- <font color=green>**[ â ]2022/5/8**</font> ~~**(è¯¥åè½è½¬ç§»è³`TSurface`ä¸­)** æä¾`Swapchain`,æäºçªå£åºåå»ºå®çªå£ç´æ¥è¿å`VkSurfaceKHR`,æä»¥`TSwapchain`éè¦å¯¹å¤æä¾ä¸ä¸ªæ¥å£æé å½æ°`TSwapchain(TDevice* device, VkSurfaceKHR)`ç¨äºæ¥æ¶å¤é¨åå»ºå¥½ç`VkSurfaceKHR`~~

- <font color=green>**[ â ]2022/7/23**</font> ~~`TVulkanLoader`ä¸­æä¾ç±»ä¼¼è·å`struct DeviceFunctionTable`ç±»åçå½æ°ï¼ç¨äºè·å`VkDevice`ç¹å®çå½æ°å®ç°çæ¬~~

- `TVulkanLoader`ä¸­`Destroy()`ç®åæªè¢«è°ç¨ï¼åºè¯¥å¨`TInstance`ææå½æ°ä¸­è°ç¨ï¼ç¨äºå¸è½½`Vulkan`å¨æåº

- <font color=green>**[ â ]2022/7/27**</font> ~~`Turbo`ä¸­æªå®å¨è¦ç`TVulkanLoader`è·åçå½æ°è°ç¨~~
  - <font color=green>**[ â ]2022/7/25**</font> ~~`Turbo::Core::TDeviceQueue::Present(...)::vkQueuePresentKHR(...)`æªè¦ç~~
  - <font color=green>**[ â ]2022/7/25**</font> ~~`TSurface`æªè¦ç~~
  - <font color=green>**[ â ]2022/7/27**</font> ~~`TSwapchain`æªè¦ç~~

- <font color=green>**[ â ]2022/7/30**</font> ~~`TCore`ä¸­`CMakeLists.txt`ä¸­æäºåºåºè¯¥æ¯`PRIVATE`~~

- `TCore`ä¸­`CMakeLists.txt`å¯¹äºåºçåå»ºåºè¯¥åæ¶ç°æçè®¾æéæåºçéç½®ï¼è½¬æç¨æ·èªå®ä¹çæéæåºè¿æ¯å¨æåº

- <font color=green>**[ â ]2022/7/30**</font> ~~`Turbo`æ ¸å¿çæç`TCore`åº,ç§å¸¸çåºè¯¥å¨`Visual Studio`ä¸­é¾æ¥æ¶ä¸éè¦åæ¬¡é¾æ¥æ ¸å¿çä¾èµåºï¼ä½æ¯ç®åæµè¯ï¼å¨`Visual Studio`è¿æ¯éè¦é¾æ¥æ¯å¦`vma`,`glslang`ç­æ ¸å¿ä¾èµåºæè½é¾æ¥å°`TCore`åºä¸­å¦åæ¥æªå®ä¹ç¸å³å½æ°å®ç°çéè¯¯ï¼åæ­¥æçæ¯`CMake`ç`target_link_libraries`ä¸­ç`PUBLIC`å`PRIVATE`çåºå«ï¼ä½æ¯æ¹æ`PRIVATE`å¥½åå¹¶ä¸è½è§£å³ä»¥ä¸é®é¢ãå¦æä½¿ç¨`ar.exe`ä¹ç±»çåºåå¹¶å·¥å·éè¦åèªå®ä¹`CMake`æä»¤ï¼`Visual Studio`çåºåå¹¶å·¥å·æ¯å®æ¹ç`lib.exe`ï¼ä½æ¯ï¼ä½æ¯ä½¿ç¨åºåå¹¶å·¥å·åºè¯¥ä¸æ¯æå¥½çè§£å³æ¹æ¡ãç ç©¶ä¸­...~~

- `Turbo`ä¸­ç®åè¿æå¾å¤è·¨å¹³å°çåå®¹æ²¡æå¡«è¡¥ï¼æ­£å¸¸æ²¡æå®ç°çéè¦æåº`throw Turbo::Core::TException(Turbo::Core::TResult::UNIMPLEMENTED,...)`å¼å¸¸,å¸®å©è·¨å¹³å°å¼åæç¤º

- `TVulkanLoader`éè¦éé`Linux`ç³»ç»

- èèæ¯å¦å°å¤é¨å¼å¥ç`VkSurfaceKHR`ä¸­å¨`TSurface`æææ¶é¡ºä¾¿éæ¯ï¼ç®åå¤é¨å¼å¥ç`VkSurfaceKHR`ï¼éè¦å¨å¤é¨èªå·±éæ¯

- `Turbo`å¼æä¸­å¯¹äº`Vulkan`çæ©å±æ¯æä¸å¨ï¼æ°çç`Vulkan`æ©å±æ¯å¦`1.3`çä¸äºæ°æ©å±æ²¡æï¼ææ¶é´æ·»å 
