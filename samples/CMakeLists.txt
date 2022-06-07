project(TSample)

add_executable(FrameGraphSample ./FrameGraphSample.cpp)

target_include_directories(FrameGraphSample
PUBLIC ../engine/framegraph/include
)

target_link_directories(FrameGraphSample
PUBLIC /engine/framegraph/
)

target_link_libraries(FrameGraphSample PUBLIC 
TFrameGraph
)
