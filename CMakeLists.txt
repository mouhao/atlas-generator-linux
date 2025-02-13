cmake_minimum_required(VERSION 3.7)
project(atlas_generator)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

SET(CMAKE_PREFIX_PATH
        /Users/survivor/Applications/Qt5.9.0/5.9/clang_64/lib/cmake/Qt5Core
        /Users/survivor/Applications/Qt5.9.0/5.9/clang_64/lib/cmake/Qt5Gui)

find_package(Qt5Core)
find_package(Qt5Gui)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        RectangleBinPack/GuillotineBinPack.cpp
        RectangleBinPack/MaxRectsBinPack.cpp
        RectangleBinPack/Rect.cpp
        RectangleBinPack/ShelfBinPack.cpp
        RectangleBinPack/ShelfNextFitBinPack.cpp
        RectangleBinPack/SkylineBinPack.cpp
        CRC32/CRC32.cpp
        AtlasPacker.cc
        Configuration.cc
        main.cc
        Utils.cc
        JsonExport.cpp)

add_executable(atlas_generator ${SOURCE_FILES})
target_link_libraries(atlas_generator
        Qt5::Core
        Qt5::Gui)