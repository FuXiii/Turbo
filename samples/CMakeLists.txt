project(TSample)

function(Add_Sample Sample Srcs Incs Links Libs)
add_executable(${Sample} ${Srcs})
target_include_directories(${Sample} PUBLIC ${Incs})
target_link_directories(${Sample} PUBLIC ${Links})
target_link_libraries(${Sample} PUBLIC ${Libs})
endfunction()

set(INCLUDE_PARTH 
PUBLIC ../engine/render 
PUBLIC ../engine/framegraph
PUBLIC ../engine/core
PUBLIC ../thirdparty/glm/
PUBLIC ../thirdparty/tinygltf/
PUBLIC ../thirdparty/imgui/
PUBLIC ../thirdparty/imgui/backends/
PUBLIC ../thirdparty/KTX-Software/include/
)

set(LINK_PARTH 
PUBLIC /engine/render/ 
PUBLIC /engine/framegraph/ 
PUBLIC /engine/core/
PUBLIC /thirdparty/glfw/src/
PUBLIC /thirdparty/KTX-Software/
)

set(LIBS 
TRender
TCore 
TFrameGraph
glfw
ktx
)

set(IMGUI_SOURCE_PATH 
../thirdparty/imgui/imgui_demo.cpp
../thirdparty/imgui/imgui_draw.cpp
../thirdparty/imgui/imgui_tables.cpp
../thirdparty/imgui/imgui_widgets.cpp
../thirdparty/imgui/imgui.cpp
)

add_executable(FrameGraphSample ./FrameGraphSample.cpp)
target_include_directories(FrameGraphSample PUBLIC ${INCLUDE_PARTH})
target_link_directories(FrameGraphSample PUBLIC ${LINK_PARTH})
target_link_libraries(FrameGraphSample PUBLIC ${LIBS})

add_executable(HelloTriangle ./HelloTriangle.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(HelloTriangle PUBLIC ${INCLUDE_PARTH})
target_link_directories(HelloTriangle PUBLIC ${LINK_PARTH})
target_link_libraries(HelloTriangle PUBLIC ${LIBS})

add_executable(PureHelloTriangle ./PureHelloTriangle.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PureHelloTriangle PUBLIC ${INCLUDE_PARTH})
target_link_directories(PureHelloTriangle PUBLIC ${LINK_PARTH})
target_link_libraries(PureHelloTriangle PUBLIC ${LIBS})

add_executable(PureCombinedImageSampler ./PureCombinedImageSampler.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PureCombinedImageSampler PUBLIC ${INCLUDE_PARTH})
target_link_directories(PureCombinedImageSampler PUBLIC ${LINK_PARTH})
target_link_libraries(PureCombinedImageSampler PUBLIC ${LIBS})

add_executable(PureSeparateImageSampler ./PureSeparateImageSampler.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PureSeparateImageSampler PUBLIC ${INCLUDE_PARTH})
target_link_directories(PureSeparateImageSampler PUBLIC ${LINK_PARTH})
target_link_libraries(PureSeparateImageSampler PUBLIC ${LIBS})

add_executable(PureIndexDraw ./PureIndexDraw.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PureIndexDraw PUBLIC ${INCLUDE_PARTH})
target_link_directories(PureIndexDraw PUBLIC ${LINK_PARTH})
target_link_libraries(PureIndexDraw PUBLIC ${LIBS})

add_executable(TurboTest ./TurboTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(TurboTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(TurboTest PUBLIC ${LINK_PARTH})
target_link_libraries(TurboTest PUBLIC ${LIBS})

add_executable(PBRTest ./PBRTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PBRTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(PBRTest PUBLIC ${LINK_PARTH})
target_link_libraries(PBRTest PUBLIC ${LIBS})

add_executable(SecondaryCommandBufferTest ./SecondaryCommandBufferTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(SecondaryCommandBufferTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(SecondaryCommandBufferTest PUBLIC ${LINK_PARTH})
target_link_libraries(SecondaryCommandBufferTest PUBLIC ${LIBS})

add_executable(MultiDrawTest ./MultiDrawTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(MultiDrawTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(MultiDrawTest PUBLIC ${LINK_PARTH})
target_link_libraries(MultiDrawTest PUBLIC ${LIBS})

add_executable(SubpassTest ./SubpassTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(SubpassTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(SubpassTest PUBLIC ${LINK_PARTH})
target_link_libraries(SubpassTest PUBLIC ${LIBS})

add_executable(LineAndLineWidth ./LineAndLineWidth.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(LineAndLineWidth PUBLIC ${INCLUDE_PARTH})
target_link_directories(LineAndLineWidth PUBLIC ${LINK_PARTH})
target_link_libraries(LineAndLineWidth PUBLIC ${LIBS})

add_executable(InstancedDrawTest ./InstancedDrawTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(InstancedDrawTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(InstancedDrawTest PUBLIC ${LINK_PARTH})
target_link_libraries(InstancedDrawTest PUBLIC ${LIBS})

add_executable(CppStandardTest ./CppStandardTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(CppStandardTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(CppStandardTest PUBLIC ${LINK_PARTH})
target_link_libraries(CppStandardTest PUBLIC ${LIBS})

add_executable(PerlinWorleyNoiseTest ./PerlinWorleyNoiseTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PerlinWorleyNoiseTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(PerlinWorleyNoiseTest PUBLIC ${LINK_PARTH})
target_link_libraries(PerlinWorleyNoiseTest PUBLIC ${LIBS})

add_executable(CineShaderLava ./CineShaderLava.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(CineShaderLava PUBLIC ${INCLUDE_PARTH})
target_link_directories(CineShaderLava PUBLIC ${LINK_PARTH})
target_link_libraries(CineShaderLava PUBLIC ${LIBS})

add_executable(Octagrams ./Octagrams.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(Octagrams PUBLIC ${INCLUDE_PARTH})
target_link_directories(Octagrams PUBLIC ${LINK_PARTH})
target_link_libraries(Octagrams PUBLIC ${LIBS})

add_executable(ProteanClouds ./ProteanClouds.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(ProteanClouds PUBLIC ${INCLUDE_PARTH})
target_link_directories(ProteanClouds PUBLIC ${LINK_PARTH})
target_link_libraries(ProteanClouds PUBLIC ${LIBS})

add_executable(ComputePipelineTest ./ComputePipelineTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(ComputePipelineTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(ComputePipelineTest PUBLIC ${LINK_PARTH})
target_link_libraries(ComputePipelineTest PUBLIC ${LIBS})

add_executable(Auroras ./Auroras.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(Auroras PUBLIC ${INCLUDE_PARTH})
target_link_directories(Auroras PUBLIC ${LINK_PARTH})
target_link_libraries(Auroras PUBLIC ${LIBS})

add_executable(ComputerPipelineGeneratePerlinWorley ./ComputerPipelineGeneratePerlinWorley.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(ComputerPipelineGeneratePerlinWorley PUBLIC ${INCLUDE_PARTH})
target_link_directories(ComputerPipelineGeneratePerlinWorley PUBLIC ${LINK_PARTH})
target_link_libraries(ComputerPipelineGeneratePerlinWorley PUBLIC ${LIBS})

add_executable(ComputerPipelineGenerateWorley ./ComputerPipelineGenerateWorley.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(ComputerPipelineGenerateWorley PUBLIC ${INCLUDE_PARTH})
target_link_directories(ComputerPipelineGenerateWorley PUBLIC ${LINK_PARTH})
target_link_libraries(ComputerPipelineGenerateWorley PUBLIC ${LIBS})

add_executable(IspGlassball11Mix ./IspGlassball11Mix.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(IspGlassball11Mix PUBLIC ${INCLUDE_PARTH})
target_link_directories(IspGlassball11Mix PUBLIC ${LINK_PARTH})
target_link_libraries(IspGlassball11Mix PUBLIC ${LIBS})

add_executable(VolumetricCloud ./VolumetricCloud.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VolumetricCloud PUBLIC ${INCLUDE_PARTH})
target_link_directories(VolumetricCloud PUBLIC ${LINK_PARTH})
target_link_libraries(VolumetricCloud PUBLIC ${LIBS})

add_executable(RayMarchingTest ./RayMarchingTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(RayMarchingTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(RayMarchingTest PUBLIC ${LINK_PARTH})
target_link_libraries(RayMarchingTest PUBLIC ${LIBS})

add_executable(RayMarchingBoundingBoxTest ./RayMarchingBoundingBoxTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(RayMarchingBoundingBoxTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(RayMarchingBoundingBoxTest PUBLIC ${LINK_PARTH})
target_link_libraries(RayMarchingBoundingBoxTest PUBLIC ${LIBS})

add_executable(RayMarchingPerlinWorleyNoise ./RayMarchingPerlinWorleyNoise.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(RayMarchingPerlinWorleyNoise PUBLIC ${INCLUDE_PARTH})
target_link_directories(RayMarchingPerlinWorleyNoise PUBLIC ${LINK_PARTH})
target_link_libraries(RayMarchingPerlinWorleyNoise PUBLIC ${LIBS})

add_executable(RenderDesignTest ./RenderDesignTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(RenderDesignTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(RenderDesignTest PUBLIC ${LINK_PARTH})
target_link_libraries(RenderDesignTest PUBLIC ${LIBS})

add_executable(VulkanTest ./VulkanTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanTest PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanTest PUBLIC ${LIBS})

add_executable(PushConstantTest ./PushConstantTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PushConstantTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(PushConstantTest PUBLIC ${LINK_PARTH})
target_link_libraries(PushConstantTest PUBLIC ${LIBS})

add_executable(RenderAndFrameGraph ./RenderAndFrameGraph.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(RenderAndFrameGraph PUBLIC ${INCLUDE_PARTH})
target_link_directories(RenderAndFrameGraph PUBLIC ${LINK_PARTH})
target_link_libraries(RenderAndFrameGraph PUBLIC ${LIBS})

add_executable(ClearColorTest ./ClearColorTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(ClearColorTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(ClearColorTest PUBLIC ${LINK_PARTH})
target_link_libraries(ClearColorTest PUBLIC ${LIBS})

add_executable(VulkanAllocatorTest ./VulkanAllocatorTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanAllocatorTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanAllocatorTest PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanAllocatorTest PUBLIC ${LIBS})

add_executable(ShaderIncludeTest ./ShaderIncludeTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(ShaderIncludeTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(ShaderIncludeTest PUBLIC ${LINK_PARTH})
target_link_libraries(ShaderIncludeTest PUBLIC ${LIBS})

add_executable(GreenFieldDiorama ./GreenFieldDiorama.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(GreenFieldDiorama PUBLIC ${INCLUDE_PARTH})
target_link_directories(GreenFieldDiorama PUBLIC ${LINK_PARTH})
target_link_libraries(GreenFieldDiorama PUBLIC ${LIBS})

add_executable(PureHelloTriangleAndFrameGraph ./PureHelloTriangleAndFrameGraph.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PureHelloTriangleAndFrameGraph PUBLIC ${INCLUDE_PARTH})
target_link_directories(PureHelloTriangleAndFrameGraph PUBLIC ${LINK_PARTH})
target_link_libraries(PureHelloTriangleAndFrameGraph PUBLIC ${LIBS})

add_executable(VulkanDynamicRenderingTest ./VulkanDynamicRenderingTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanDynamicRenderingTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanDynamicRenderingTest PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanDynamicRenderingTest PUBLIC ${LIBS})

add_executable(DynamicRenderingTest ./DynamicRenderingTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(DynamicRenderingTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(DynamicRenderingTest PUBLIC ${LINK_PARTH})
target_link_libraries(DynamicRenderingTest PUBLIC ${LIBS})

add_executable(NormalTexture ./NormalTexture.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(NormalTexture PUBLIC ${INCLUDE_PARTH})
target_link_directories(NormalTexture PUBLIC ${LINK_PARTH})
target_link_libraries(NormalTexture PUBLIC ${LIBS})

add_executable(BRDF ./BRDF.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(BRDF PUBLIC ${INCLUDE_PARTH})
target_link_directories(BRDF PUBLIC ${LINK_PARTH})
target_link_libraries(BRDF PUBLIC ${LIBS})

add_executable(CPPMultithreading ./CPPMultithreading.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(CPPMultithreading PUBLIC ${INCLUDE_PARTH})
target_link_directories(CPPMultithreading PUBLIC ${LINK_PARTH})
target_link_libraries(CPPMultithreading PUBLIC ${LIBS})

add_executable(TessellationTest ./TessellationTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(TessellationTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(TessellationTest PUBLIC ${LINK_PARTH})
target_link_libraries(TessellationTest PUBLIC ${LIBS})

add_executable(GeometryShaderTest ./GeometryShaderTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(GeometryShaderTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(GeometryShaderTest PUBLIC ${LINK_PARTH})
target_link_libraries(GeometryShaderTest PUBLIC ${LIBS})