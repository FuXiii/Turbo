add_executable(ReadGlobalWind ./ReadGlobalWind.cpp ${IMGUI_SOURCE_PATH} ${CMAKE_SOURCE_DIR}/gizmo/GlobalWind.cpp)
target_include_directories(ReadGlobalWind PUBLIC ${INCLUDE_PARTH} ${CMAKE_SOURCE_DIR}/gizmo)
target_compile_definitions(ReadGlobalWind PUBLIC TURBO_ASSET_ROOT="${CMAKE_SOURCE_DIR}/asset")