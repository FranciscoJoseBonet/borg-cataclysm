cmake_minimum_required(VERSION 3.10)
project(BorgCataclysm)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DSFML_STATIC)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(BorgCataclysm src/main.cpp ${SOURCES})

target_include_directories(BorgCataclysm PRIVATE "C:/SFML-3.0.0/include")

add_custom_command(TARGET BorgCataclysm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:BorgCataclysm>/assets
    COMMENT "Copiando carpeta assets al directorio de build..."
)

target_link_directories(BorgCataclysm PRIVATE "C:/SFML-3.0.0/lib")

target_link_libraries(BorgCataclysm
    sfml-window-s
    sfml-graphics-s
    sfml-system-s
    opengl32
    freetype
    gdi32
    winmm
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
