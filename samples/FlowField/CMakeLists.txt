add_executable(ReadGlobalWind ./ReadGlobalWind.cpp ${IMGUI_SOURCE_PATH} ${CMAKE_SOURCE_DIR}/gizmo/GlobalWind.cpp)
target_include_directories(ReadGlobalWind PUBLIC ${INCLUDE_PARTH} ${CMAKE_SOURCE_DIR}/gizmo)
target_compile_definitions(ReadGlobalWind 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="ReadGlobalWind"
)

add_executable(ShowWindByTex ./ShowWindByTex.cpp ${IMGUI_SOURCE_PATH} ${CMAKE_SOURCE_DIR}/gizmo/GlobalWind.cpp)
target_include_directories(ShowWindByTex PUBLIC ${INCLUDE_PARTH} ${CMAKE_SOURCE_DIR}/gizmo)
target_link_libraries(ShowWindByTex PRIVATE ${LIBS})
target_compile_definitions(ShowWindByTex 
PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset"
PUBLIC TURBO_PROJECT_NAME="ShowWindByTex"
)