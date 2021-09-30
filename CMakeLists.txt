cmake_minimum_required(VERSION 3.8)

project(topology_api CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()
add_compile_options(/MDd)
add_subdirectory(googletest)

# API
add_library(${PROJECT_NAME} STATIC
    src/topologyapi.cpp
    src/topology.cpp
    src/component.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    include
)

# Tests
add_executable(${PROJECT_NAME}_tests 
    test/test_topology.cpp
    test/read_write_topology_test.cpp
    test/get_remove_topology_test.cpp
    test/query_topologies_test.cpp
)

target_include_directories(${PROJECT_NAME}_tests PRIVATE
    test/test_topology.hpp
)


target_link_libraries(${PROJECT_NAME}_tests PRIVATE
    gtest_main
    ${PROJECT_NAME}
)

add_test(
    NAME ${PROJECT_NAME}_tests
    COMMAND ${PROJECT_NAME}_tests
)

add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/topologies/top1.json
        $<TARGET_FILE_DIR:${PROJECT_NAME}_tests>/top1.json)

# Interface
add_executable(${PROJECT_NAME}_interface
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}_interface PRIVATE
    ${PROJECT_NAME}
)

set_target_properties(${PROJECT_NAME}_interface PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

add_custom_command(TARGET ${PROJECT_NAME}_interface POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/topologies
        $<TARGET_FILE_DIR:${PROJECT_NAME}_interface>/topologies)

