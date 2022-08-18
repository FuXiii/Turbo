project(TSample)

function(Add_Sample Sample Srcs Incs Links Libs)
add_executable(${Sample} ${Srcs})
target_include_directories(${Sample} PUBLIC ${Incs})
target_link_directories(${Sample} PUBLIC ${Links})
target_link_libraries(${Sample} PUBLIC ${Libs})
endfunction()

set(INCLUDE_PARTH 
PUBLIC ../engine/framegraph/include 
PUBLIC ../engine/core/include
PUBLIC ../thirdparty/glm/
PUBLIC ../thirdparty/tinygltf/
PUBLIC ../thirdparty/imgui/
PUBLIC ../thirdparty/imgui/backends/
PUBLIC ../thirdparty/KTX-Software/include/
)

set(LINK_PARTH 
PUBLIC /engine/framegraph/ 
PUBLIC /engine/core/
PUBLIC /thirdparty/glfw/src/
PUBLIC /thirdparty/KTX-Software/
)

set(LIBS 
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


