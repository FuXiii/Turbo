add_executable(ReadGlobalWind ./ReadGlobalWind.cpp ${IMGUI_SOURCE_PATH} ${CMAKE_SOURCE_DIR}/samples/utils/GlobalWind.cpp)
target_include_directories(ReadGlobalWind PUBLIC ${INCLUDE_PARTH} ${CMAKE_SOURCE_DIR}/samples/utils)
target_compile_definitions(ReadGlobalWind 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="ReadGlobalWind"
)

add_executable(ShowWindByTex ./ShowWindByTex.cpp ${IMGUI_SOURCE_PATH} ${CMAKE_SOURCE_DIR}/samples/utils/GlobalWind.cpp ${CMAKE_SOURCE_DIR}/samples/utils/ReadFile.cpp)
target_include_directories(ShowWindByTex PUBLIC ${INCLUDE_PARTH} ${CMAKE_SOURCE_DIR}/samples/utils)
target_link_libraries(ShowWindByTex PRIVATE ${LIBS})
target_compile_definitions(ShowWindByTex 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="ShowWindByTex"
)

add_sample(ShowWindByPatch ./ShowWindByPatch.cpp ${CMAKE_SOURCE_DIR}/samples/utils/GlobalWind.cpp)
add_sample(ShowWindByInstancedPoints ./ShowWindByInstancedPoints.cpp ${CMAKE_SOURCE_DIR}/samples/utils/GlobalWind.cpp)
add_sample(ShowWIndByInstancedLines ./ShowWIndByInstancedLines.cpp ${CMAKE_SOURCE_DIR}/samples/utils/GlobalWind.cpp)