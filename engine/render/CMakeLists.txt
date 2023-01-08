project(TRender)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

aux_source_directory(./src/ SRCS)

set(LINK_PARTH 
PUBLIC /engine
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
target_link_directories(${PROJECT_NAME} PUBLIC ${LINK_PARTH})
target_link_libraries(${PROJECT_NAME} PUBLIC TCore TFrameGraph)

target_include_directories(${PROJECT_NAME}
    PUBLIC ./include
    PUBLIC ../
    )