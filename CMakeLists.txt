cmake_minimum_required(VERSION 3.16)
project(udap_micro_ui C CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-fpermissive")
set(CMAKE_CXX_FLAGS "-Wno-unknown-pragmas")

# Find SDL2
find_package(SDL2 REQUIRED)

# Collect sources
file(GLOB_RECURSE FONT_SOURCES  ./fonts/*.c)
file(GLOB_RECURSE ICON_SOURCES   ./icons/*.c)
file(GLOB_RECURSE UI_SOURCES    ./src/ui/*.cpp)
file(GLOB_RECURSE HAL_SOURCES   ./platforms/macos/*.cpp)
file(GLOB_RECURSE LVGL_SOURCES  ./lib/lvgl/src/*.c)

add_executable(udap_micro_ui
        ${FONT_SOURCES}
        ${ICON_SOURCES}
        ${UI_SOURCES}
        ${HAL_SOURCES}
        ${LVGL_SOURCES}
)

target_include_directories(udap_micro_ui PRIVATE
        src/hal           # so ui/ code can find hal_*.h
        src/ui           # so ui/ code can find hal_*.h
        lib/lvgl          # lvgl.h
        lib/lvgl/src
        ${CMAKE_SOURCE_DIR}   # for lv_conf.h at root
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(udap_micro_ui PRIVATE ${SDL2_LIBRARIES})

# Tell LVGL where to find lv_conf.h
target_compile_definitions(udap_micro_ui PRIVATE
        LV_CONF_INCLUDE_SIMPLE=1
)