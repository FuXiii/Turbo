project(TSample)

set(INCLUDE_PARTH 
#PUBLIC ${CMAKE_SOURCE_DIR}/engine/render 
#PUBLIC ${CMAKE_SOURCE_DIR}/engine/framegraph
PUBLIC ${CMAKE_SOURCE_DIR}/engine/core
PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/glm/
PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/tinygltf/
PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/imgui/
PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/imgui/backends/
PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/KTX-Software/include/
)

set(LINK_PARTH 
#PUBLIC /engine/render/ 
#PUBLIC /engine/framegraph/ 
PUBLIC ${CMAKE_SOURCE_DIR}/engine/core/
PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/glfw/src/
PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/KTX-Software/
)

set(LIBS 
#TRender
TCore 
#TFrameGraph
glfw
ktx
)

set(IMGUI_SOURCE_PATH 
${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_demo.cpp
${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_draw.cpp
${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_tables.cpp
${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_widgets.cpp
${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui.cpp
)

function(add_sample name srcs)
    add_executable(${name} 
        ${srcs} 
        ${IMGUI_SOURCE_PATH} 
        ${CMAKE_SOURCE_DIR}/samples/utils/ReadFile.cpp
        ${CMAKE_SOURCE_DIR}/samples/utils/ImGuiPass.cpp
        ${CMAKE_SOURCE_DIR}/samples/utils/InfiniteCoordinateAxisPass.cpp
        ${CMAKE_SOURCE_DIR}/samples/utils/Camera.cpp
        ${ARGN}
    )
    target_include_directories(${name} PUBLIC ${INCLUDE_PARTH} ${CMAKE_SOURCE_DIR}/samples/utils)
    target_link_directories(${name} PUBLIC ${LINK_PARTH})
    target_link_libraries(${name} PUBLIC ${LIBS})
    target_compile_definitions(${name} 
        PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
        PUBLIC TURBO_PROJECT_NAME="${name}"
    )
endfunction()

#add_executable(FrameGraphSample ./FrameGraphSample.cpp)
#target_include_directories(FrameGraphSample PUBLIC ${INCLUDE_PARTH})
#target_link_directories(FrameGraphSample PUBLIC ${LINK_PARTH})
#target_link_libraries(FrameGraphSample PUBLIC ${LIBS})
#target_compile_definitions(FrameGraphSample PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset")

add_executable(InstancedDrawTest ./InstancedDrawTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(InstancedDrawTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(InstancedDrawTest PUBLIC ${LINK_PARTH})
target_link_libraries(InstancedDrawTest PUBLIC ${LIBS})
target_compile_definitions(InstancedDrawTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="InstancedDrawTest"
)

#add_executable(HelloTriangle ./HelloTriangle.cpp ${IMGUI_SOURCE_PATH})
#target_include_directories(HelloTriangle PUBLIC ${INCLUDE_PARTH})
#target_link_directories(HelloTriangle PUBLIC ${LINK_PARTH})
#target_link_libraries(HelloTriangle PUBLIC ${LIBS})
#target_compile_definitions(HelloTriangle 
#PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
#PUBLIC TURBO_PROJECT_NAME="HelloTriangle"
#)

add_sample(HelloTriangle HelloTriangle.cpp)

add_executable(PureHelloTriangle ./PureHelloTriangle.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PureHelloTriangle PUBLIC ${INCLUDE_PARTH})
target_link_directories(PureHelloTriangle PUBLIC ${LINK_PARTH})
target_link_libraries(PureHelloTriangle PUBLIC ${LIBS})
target_compile_definitions(PureHelloTriangle 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="PureHelloTriangle"
)

add_executable(PureCombinedImageSampler ./PureCombinedImageSampler.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PureCombinedImageSampler PUBLIC ${INCLUDE_PARTH})
target_link_directories(PureCombinedImageSampler PUBLIC ${LINK_PARTH})
target_link_libraries(PureCombinedImageSampler PUBLIC ${LIBS})
target_compile_definitions(PureCombinedImageSampler 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="PureCombinedImageSampler"
)

add_executable(PureSeparateImageSampler ./PureSeparateImageSampler.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PureSeparateImageSampler PUBLIC ${INCLUDE_PARTH})
target_link_directories(PureSeparateImageSampler PUBLIC ${LINK_PARTH})
target_link_libraries(PureSeparateImageSampler PUBLIC ${LIBS})
target_compile_definitions(PureSeparateImageSampler 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="PureSeparateImageSampler"
)

add_executable(PureIndexDraw ./PureIndexDraw.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PureIndexDraw PUBLIC ${INCLUDE_PARTH})
target_link_directories(PureIndexDraw PUBLIC ${LINK_PARTH})
target_link_libraries(PureIndexDraw PUBLIC ${LIBS})
target_compile_definitions(PureIndexDraw 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="PureIndexDraw"
)

add_executable(TurboTest ./TurboTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(TurboTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(TurboTest PUBLIC ${LINK_PARTH})
target_link_libraries(TurboTest PUBLIC ${LIBS})
target_compile_definitions(TurboTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="TurboTest"
)

add_executable(PBRTest ./PBRTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PBRTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(PBRTest PUBLIC ${LINK_PARTH})
target_link_libraries(PBRTest PUBLIC ${LIBS})
target_compile_definitions(PBRTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="PBRTest"
)

add_executable(SecondaryCommandBufferTest ./SecondaryCommandBufferTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(SecondaryCommandBufferTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(SecondaryCommandBufferTest PUBLIC ${LINK_PARTH})
target_link_libraries(SecondaryCommandBufferTest PUBLIC ${LIBS})
target_compile_definitions(SecondaryCommandBufferTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="SecondaryCommandBufferTest"
)

add_executable(MultiDrawTest ./MultiDrawTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(MultiDrawTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(MultiDrawTest PUBLIC ${LINK_PARTH})
target_link_libraries(MultiDrawTest PUBLIC ${LIBS})
target_compile_definitions(MultiDrawTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="MultiDrawTest"
)

add_executable(SubpassTest ./SubpassTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(SubpassTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(SubpassTest PUBLIC ${LINK_PARTH})
target_link_libraries(SubpassTest PUBLIC ${LIBS})
target_compile_definitions(SubpassTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="SubpassTest"
)

add_executable(LineAndLineWidth ./LineAndLineWidth.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(LineAndLineWidth PUBLIC ${INCLUDE_PARTH})
target_link_directories(LineAndLineWidth PUBLIC ${LINK_PARTH})
target_link_libraries(LineAndLineWidth PUBLIC ${LIBS})
target_compile_definitions(LineAndLineWidth 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="LineAndLineWidth"
)

add_executable(CppStandardTest ./CppStandardTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(CppStandardTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(CppStandardTest PUBLIC ${LINK_PARTH})
target_link_libraries(CppStandardTest PUBLIC ${LIBS})
target_compile_definitions(CppStandardTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="CppStandardTest"
)

add_executable(PerlinWorleyNoiseTest ./PerlinWorleyNoiseTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PerlinWorleyNoiseTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(PerlinWorleyNoiseTest PUBLIC ${LINK_PARTH})
target_link_libraries(PerlinWorleyNoiseTest PUBLIC ${LIBS})
target_compile_definitions(PerlinWorleyNoiseTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="PerlinWorleyNoiseTest"
)

add_executable(CineShaderLava ./CineShaderLava.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(CineShaderLava PUBLIC ${INCLUDE_PARTH})
target_link_directories(CineShaderLava PUBLIC ${LINK_PARTH})
target_link_libraries(CineShaderLava PUBLIC ${LIBS})
target_compile_definitions(CineShaderLava 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="CineShaderLava"
)

add_executable(Octagrams ./Octagrams.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(Octagrams PUBLIC ${INCLUDE_PARTH})
target_link_directories(Octagrams PUBLIC ${LINK_PARTH})
target_link_libraries(Octagrams PUBLIC ${LIBS})
target_compile_definitions(Octagrams 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="Octagrams"
)

add_executable(ProteanClouds ./ProteanClouds.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(ProteanClouds PUBLIC ${INCLUDE_PARTH})
target_link_directories(ProteanClouds PUBLIC ${LINK_PARTH})
target_link_libraries(ProteanClouds PUBLIC ${LIBS})
target_compile_definitions(ProteanClouds 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="ProteanClouds"
)

add_executable(ComputePipelineTest ./ComputePipelineTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(ComputePipelineTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(ComputePipelineTest PUBLIC ${LINK_PARTH})
target_link_libraries(ComputePipelineTest PUBLIC ${LIBS})
target_compile_definitions(ComputePipelineTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="ComputePipelineTest"
)

add_executable(Auroras ./Auroras.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(Auroras PUBLIC ${INCLUDE_PARTH})
target_link_directories(Auroras PUBLIC ${LINK_PARTH})
target_link_libraries(Auroras PUBLIC ${LIBS})
target_compile_definitions(Auroras 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="Auroras"
)

add_executable(ComputerPipelineGeneratePerlinWorley ./ComputerPipelineGeneratePerlinWorley.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(ComputerPipelineGeneratePerlinWorley PUBLIC ${INCLUDE_PARTH})
target_link_directories(ComputerPipelineGeneratePerlinWorley PUBLIC ${LINK_PARTH})
target_link_libraries(ComputerPipelineGeneratePerlinWorley PUBLIC ${LIBS})
target_compile_definitions(ComputerPipelineGeneratePerlinWorley 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="ComputerPipelineGeneratePerlinWorley"
)

add_executable(ComputerPipelineGenerateWorley ./ComputerPipelineGenerateWorley.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(ComputerPipelineGenerateWorley PUBLIC ${INCLUDE_PARTH})
target_link_directories(ComputerPipelineGenerateWorley PUBLIC ${LINK_PARTH})
target_link_libraries(ComputerPipelineGenerateWorley PUBLIC ${LIBS})
target_compile_definitions(ComputerPipelineGenerateWorley 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="ComputerPipelineGenerateWorley"
)

add_executable(IspGlassball11Mix ./IspGlassball11Mix.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(IspGlassball11Mix PUBLIC ${INCLUDE_PARTH})
target_link_directories(IspGlassball11Mix PUBLIC ${LINK_PARTH})
target_link_libraries(IspGlassball11Mix PUBLIC ${LIBS})
target_compile_definitions(IspGlassball11Mix 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="IspGlassball11Mix"
)

add_executable(VolumetricCloud ./VolumetricCloud.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VolumetricCloud PUBLIC ${INCLUDE_PARTH})
target_link_directories(VolumetricCloud PUBLIC ${LINK_PARTH})
target_link_libraries(VolumetricCloud PUBLIC ${LIBS})
target_compile_definitions(VolumetricCloud 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VolumetricCloud"
)

add_executable(RayMarchingTest ./RayMarchingTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(RayMarchingTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(RayMarchingTest PUBLIC ${LINK_PARTH})
target_link_libraries(RayMarchingTest PUBLIC ${LIBS})
target_compile_definitions(RayMarchingTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="RayMarchingTest"
)

add_executable(RayMarchingBoundingBoxTest ./RayMarchingBoundingBoxTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(RayMarchingBoundingBoxTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(RayMarchingBoundingBoxTest PUBLIC ${LINK_PARTH})
target_link_libraries(RayMarchingBoundingBoxTest PUBLIC ${LIBS})
target_compile_definitions(RayMarchingBoundingBoxTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="RayMarchingBoundingBoxTest"
)

add_executable(RayMarchingPerlinWorleyNoise ./RayMarchingPerlinWorleyNoise.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(RayMarchingPerlinWorleyNoise PUBLIC ${INCLUDE_PARTH})
target_link_directories(RayMarchingPerlinWorleyNoise PUBLIC ${LINK_PARTH})
target_link_libraries(RayMarchingPerlinWorleyNoise PUBLIC ${LIBS})
target_compile_definitions(RayMarchingPerlinWorleyNoise 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="RayMarchingPerlinWorleyNoise"
)

#add_executable(RenderDesignTest ./RenderDesignTest.cpp ${IMGUI_SOURCE_PATH})
#target_include_directories(RenderDesignTest PUBLIC ${INCLUDE_PARTH})
#target_link_directories(RenderDesignTest PUBLIC ${LINK_PARTH})
#target_link_libraries(RenderDesignTest PUBLIC ${LIBS})

add_executable(VulkanTest ./VulkanTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanTest PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanTest PUBLIC ${LIBS})
target_compile_definitions(VulkanTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanTest"
)

add_executable(PushConstantTest ./PushConstantTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(PushConstantTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(PushConstantTest PUBLIC ${LINK_PARTH})
target_link_libraries(PushConstantTest PUBLIC ${LIBS})
target_compile_definitions(PushConstantTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="PushConstantTest"
)

#add_executable(RenderAndFrameGraph ./RenderAndFrameGraph.cpp ${IMGUI_SOURCE_PATH})
#target_include_directories(RenderAndFrameGraph PUBLIC ${INCLUDE_PARTH})
#target_link_directories(RenderAndFrameGraph PUBLIC ${LINK_PARTH})
#target_link_libraries(RenderAndFrameGraph PUBLIC ${LIBS})

add_executable(ClearColorTest ./ClearColorTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(ClearColorTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(ClearColorTest PUBLIC ${LINK_PARTH})
target_link_libraries(ClearColorTest PUBLIC ${LIBS})
target_compile_definitions(ClearColorTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="ClearColorTest"
)

add_executable(VulkanAllocatorTest ./VulkanAllocatorTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanAllocatorTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanAllocatorTest PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanAllocatorTest PUBLIC ${LIBS})
target_compile_definitions(VulkanAllocatorTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanAllocatorTest"
)

add_executable(ShaderIncludeTest ./ShaderIncludeTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(ShaderIncludeTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(ShaderIncludeTest PUBLIC ${LINK_PARTH})
target_link_libraries(ShaderIncludeTest PUBLIC ${LIBS})
target_compile_definitions(ShaderIncludeTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="ShaderIncludeTest"
)

add_executable(GreenFieldDiorama ./GreenFieldDiorama.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(GreenFieldDiorama PUBLIC ${INCLUDE_PARTH})
target_link_directories(GreenFieldDiorama PUBLIC ${LINK_PARTH})
target_link_libraries(GreenFieldDiorama PUBLIC ${LIBS})
target_compile_definitions(GreenFieldDiorama 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="GreenFieldDiorama"
)

#add_executable(PureHelloTriangleAndFrameGraph ./PureHelloTriangleAndFrameGraph.cpp ${IMGUI_SOURCE_PATH})
#target_include_directories(PureHelloTriangleAndFrameGraph PUBLIC ${INCLUDE_PARTH})
#target_link_directories(PureHelloTriangleAndFrameGraph PUBLIC ${LINK_PARTH})
#target_link_libraries(PureHelloTriangleAndFrameGraph PUBLIC ${LIBS})

add_executable(VulkanDynamicRenderingTest ./VulkanDynamicRenderingTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanDynamicRenderingTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanDynamicRenderingTest PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanDynamicRenderingTest PUBLIC ${LIBS})
target_compile_definitions(VulkanDynamicRenderingTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanDynamicRenderingTest"
)

add_executable(DynamicRenderingTest ./DynamicRenderingTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(DynamicRenderingTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(DynamicRenderingTest PUBLIC ${LINK_PARTH})
target_link_libraries(DynamicRenderingTest PUBLIC ${LIBS})
target_compile_definitions(DynamicRenderingTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="DynamicRenderingTest"
)

add_executable(NormalTexture ./NormalTexture.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(NormalTexture PUBLIC ${INCLUDE_PARTH})
target_link_directories(NormalTexture PUBLIC ${LINK_PARTH})
target_link_libraries(NormalTexture PUBLIC ${LIBS})
target_compile_definitions(NormalTexture 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="NormalTexture"
)

add_executable(BRDF ./BRDF.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(BRDF PUBLIC ${INCLUDE_PARTH})
target_link_directories(BRDF PUBLIC ${LINK_PARTH})
target_link_libraries(BRDF PUBLIC ${LIBS})
target_compile_definitions(BRDF 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="BRDF"
)

add_executable(CPPMultithreading ./CPPMultithreading.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(CPPMultithreading PUBLIC ${INCLUDE_PARTH})
target_link_directories(CPPMultithreading PUBLIC ${LINK_PARTH})
target_link_libraries(CPPMultithreading PUBLIC ${LIBS})
target_compile_definitions(CPPMultithreading 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="CPPMultithreading"
)

add_executable(TessellationTest ./TessellationTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(TessellationTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(TessellationTest PUBLIC ${LINK_PARTH})
target_link_libraries(TessellationTest PUBLIC ${LIBS})
target_compile_definitions(TessellationTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="TessellationTest"
)

add_executable(GeometryShaderTest ./GeometryShaderTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(GeometryShaderTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(GeometryShaderTest PUBLIC ${LINK_PARTH})
target_link_libraries(GeometryShaderTest PUBLIC ${LIBS})
target_compile_definitions(GeometryShaderTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="GeometryShaderTest"
)

add_executable(TessellationAndGeometry ./TessellationAndGeometry.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(TessellationAndGeometry PUBLIC ${INCLUDE_PARTH})
target_link_directories(TessellationAndGeometry PUBLIC ${LINK_PARTH})
target_link_libraries(TessellationAndGeometry PUBLIC ${LIBS})
target_compile_definitions(TessellationAndGeometry 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="TessellationAndGeometry"
)

add_executable(MeshShaderTest ./MeshShaderTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(MeshShaderTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(MeshShaderTest PUBLIC ${LINK_PARTH})
target_link_libraries(MeshShaderTest PUBLIC ${LIBS})
target_compile_definitions(MeshShaderTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="MeshShaderTest"
)

add_executable(SpecializationConstantsTest ./SpecializationConstantsTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(SpecializationConstantsTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(SpecializationConstantsTest PUBLIC ${LINK_PARTH})
target_link_libraries(SpecializationConstantsTest PUBLIC ${LIBS})
target_compile_definitions(SpecializationConstantsTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="SpecializationConstantsTest"
)

add_executable(VulkanKHRRayTracingTest ./VulkanKHRRayTracingTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingTest PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingTest PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingTest"
)

add_executable(VulkanKHRRayTracingTestForLighting ./VulkanKHRRayTracingTestForLighting.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingTestForLighting PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingTestForLighting PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingTestForLighting PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingTestForLighting 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingTestForLighting"
)

add_executable(VulkanKHRRayTracingTestForLightingShadow ./VulkanKHRRayTracingTestForLightingShadow.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingTestForLightingShadow PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingTestForLightingShadow PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingTestForLightingShadow PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingTestForLightingShadow 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingTestForLightingShadow"
)

add_executable(VulkanKHRRayTracingTestForLightingShadowWithTexture ./VulkanKHRRayTracingTestForLightingShadowWithTexture.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingTestForLightingShadowWithTexture PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingTestForLightingShadowWithTexture PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingTestForLightingShadowWithTexture PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingTestForLightingShadowWithTexture 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingTestForLightingShadowWithTexture"
)

add_executable(VulkanKHRRayTracingJitterCamera ./VulkanKHRRayTracingJitterCamera.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingJitterCamera PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingJitterCamera PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingJitterCamera PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingJitterCamera 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingJitterCamera"
)

add_executable(VulkanKHRRayTracingAnyHitShader ./VulkanKHRRayTracingAnyHitShader.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingAnyHitShader PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingAnyHitShader PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingAnyHitShader PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingAnyHitShader 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingAnyHitShader"
)

add_executable(VulkanKHRRayTracingTestForInstances ./VulkanKHRRayTracingTestForInstances.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingTestForInstances PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingTestForInstances PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingTestForInstances PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingTestForInstances 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingTestForInstances"
)

add_executable(VulkanKHRRayTracingTestForReflections ./VulkanKHRRayTracingTestForReflections.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingTestForReflections PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingTestForReflections PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingTestForReflections PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingTestForReflections 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingTestForReflections"
)

add_executable(VulkanKHRRayTracingTestForReflections_WithoutLimited ./VulkanKHRRayTracingTestForReflections_WithoutLimited.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingTestForReflections_WithoutLimited PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingTestForReflections_WithoutLimited PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingTestForReflections_WithoutLimited PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingTestForReflections_WithoutLimited 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingTestForReflections_WithoutLimited"
)

add_executable(VulkanKHRRayTracingTestForMultiClosestHits ./VulkanKHRRayTracingTestForMultiClosestHits.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingTestForMultiClosestHits PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingTestForMultiClosestHits PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingTestForMultiClosestHits PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingTestForMultiClosestHits 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingTestForMultiClosestHits"
)

add_executable(VulkanKHRRayTracingTestForAnimationTLAS ./VulkanKHRRayTracingTestForAnimationTLAS.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingTestForAnimationTLAS PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingTestForAnimationTLAS PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingTestForAnimationTLAS PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingTestForAnimationTLAS 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingTestForAnimationTLAS"
)

add_executable(VulkanKHRRayTracingTestForAnimationBLAS ./VulkanKHRRayTracingTestForAnimationBLAS.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingTestForAnimationBLAS PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingTestForAnimationBLAS PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingTestForAnimationBLAS PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingTestForAnimationBLAS 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingTestForAnimationBLAS"
)

add_executable(VulkanKHRRayTracingTestForIntersectionShader ./VulkanKHRRayTracingTestForIntersectionShader.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingTestForIntersectionShader PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingTestForIntersectionShader PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingTestForIntersectionShader PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingTestForIntersectionShader 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingTestForIntersectionShader"
)

add_executable(VulkanKHRRayTracingTestForCallableShader ./VulkanKHRRayTracingTestForCallableShader.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingTestForCallableShader PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingTestForCallableShader PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingTestForCallableShader PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingTestForCallableShader 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingTestForCallableShader"
)

add_executable(VulkanRayQueryTest ./VulkanRayQueryTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanRayQueryTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanRayQueryTest PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanRayQueryTest PUBLIC ${LIBS})
target_compile_definitions(VulkanRayQueryTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanRayQueryTest"
)

add_executable(VulkanKHRRayTracingTestForGLTF ./VulkanKHRRayTracingTestForGLTF.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VulkanKHRRayTracingTestForGLTF PUBLIC ${INCLUDE_PARTH})
target_link_directories(VulkanKHRRayTracingTestForGLTF PUBLIC ${LINK_PARTH})
target_link_libraries(VulkanKHRRayTracingTestForGLTF PUBLIC ${LIBS})
target_compile_definitions(VulkanKHRRayTracingTestForGLTF 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VulkanKHRRayTracingTestForGLTF"
)

add_executable(ReferencedTest ./ReferencedTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(ReferencedTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(ReferencedTest PUBLIC ${LINK_PARTH})
target_link_libraries(ReferencedTest PUBLIC ${LIBS})
target_compile_definitions(ReferencedTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="ReferencedTest"
)

add_executable(PointCloud ./PointCloud.cpp ${IMGUI_SOURCE_PATH} ../thirdparty/ply/plyfile.cpp)
target_include_directories(PointCloud PUBLIC ${INCLUDE_PARTH} ../thirdparty/ply)
target_link_directories(PointCloud PUBLIC ${LINK_PARTH})
target_link_libraries(PointCloud PUBLIC ${LIBS})
target_compile_definitions(PointCloud 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="PointCloud"
)

add_executable(VolumeTest ./VolumeTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VolumeTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(VolumeTest PUBLIC ${LINK_PARTH})
target_link_libraries(VolumeTest PUBLIC ${LIBS})
target_compile_definitions(VolumeTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VolumeTest"
)

add_executable(VkFormatTest ./VkFormatTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(VkFormatTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(VkFormatTest PUBLIC ${LINK_PARTH})
target_link_libraries(VkFormatTest PUBLIC ${LIBS})
target_compile_definitions(VkFormatTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="VkFormatTest"
)

add_executable(SwapchainTest ./SwapchainTest.cpp ${IMGUI_SOURCE_PATH})
target_include_directories(SwapchainTest PUBLIC ${INCLUDE_PARTH})
target_link_directories(SwapchainTest PUBLIC ${LINK_PARTH})
target_link_libraries(SwapchainTest PUBLIC ${LIBS})
target_compile_definitions(SwapchainTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="SwapchainTest"
)

add_sample(ImGuiTest ImGuiTest.cpp)

add_executable(GlslangTest ./GlslangTest.cpp ${IMGUI_SOURCE_PATH} ${CMAKE_SOURCE_DIR}/samples/utils/ReadFile.cpp)
target_include_directories(GlslangTest PRIVATE ${CMAKE_SOURCE_DIR}/engine/core/thirdparty/glslang ${CMAKE_SOURCE_DIR}/samples/utils)
#target_link_directories(GlslangTest PUBLIC ${LINK_PARTH})
target_link_libraries(GlslangTest 
PRIVATE glslang
PRIVATE glslang-default-resource-limits
)
target_compile_definitions(GlslangTest 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="GlslangTest"
)

if(
EXISTS "${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/spirv_reflect.h" AND 
EXISTS "${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/spirv_reflect.c" AND 
EXISTS "${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/include/spirv/unified1/spirv.h" AND
EXISTS "${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/examples/common.h" AND
EXISTS "${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/examples/common.cpp" AND
EXISTS "${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/common/output_stream.h" AND
EXISTS "${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/common/output_stream.cpp" AND
EXISTS "${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/examples/sample.hlsl"
)
    set(SPIRV_REFLECT_SRCS 
    ${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/spirv_reflect.c
    ${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/examples/common.cpp
    ${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/common/output_stream.cpp
    )
    set(SPIRV_REFLECT_HEADERS
    ${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/spirv_reflect.h
    ${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/examples/common.h
    ${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/common/output_stream.h
    )
    set(SPIRV_REFLECT_INCLUDE_PATH
    ${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect
    ${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/include
    ${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect/examples
    )
    
    add_executable(SpirvReflectTest ./SpirvReflectTest.cpp ${SPIRV_REFLECT_SRCS} ${SPIRV_REFLECT_HEADERS} ${CMAKE_SOURCE_DIR}/samples/utils/ReadFile.cpp)
    target_include_directories(SpirvReflectTest PRIVATE ${SPIRV_REFLECT_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/engine/core/thirdparty/glslang ${CMAKE_SOURCE_DIR}/samples/utils)
    target_link_libraries(SpirvReflectTest 
    PRIVATE glslang
    PRIVATE glslang-default-resource-limits
    )
    target_compile_definitions(SpirvReflectTest
    PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
    PUBLIC TURBO_SPIRV_REFLECT_ROOT="${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect"
    PUBLIC TURBO_PROJECT_NAME="SpirvReflectTest"
    SPIRV_REFLECT_USE_SYSTEM_SPIRV_H
    )
endif()

add_sample(ImGuiPassTest ImGuiPassTest.cpp)
add_sample(InfiniteCoordinateAxis InfiniteCoordinateAxis.cpp)
add_sample(InfiniteCoordinateAxisPassTest InfiniteCoordinateAxisPassTest.cpp)

add_subdirectory(./FlowField)
add_subdirectory(./test)

