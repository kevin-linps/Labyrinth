cmake_minimum_required(VERSION 3.10)
project(Labyrinth)

add_subdirectory(Labyrinth/Vendor/)

set(      PROJECT_INCLUDE Labyrinth/Headers/)
file(GLOB PROJECT_HEADERS ${PROJECT_INCLUDE}*.h)
file(GLOB PROJECT_SOURCES Labyrinth/Sources/*.cpp)
file(GLOB PROJECT_SHADERS Labyrinth/Shaders/*.comp
                          Labyrinth/Shaders/*.frag
                          Labyrinth/Shaders/*.geom
                          Labyrinth/Shaders/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS})

target_link_libraries(${PROJECT_NAME} glfw glad glm stb tomlplusplus)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
# if(MSVC)
#     target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
# else()
#     target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
# endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Labyrinth/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_SHADERS})