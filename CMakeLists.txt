cmake_minimum_required(VERSION 3.16)
project(SegmentTreeViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Encontrar Qt5
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Archivos fuente
set(SOURCES
        main.cpp
)

# Headers
set(HEADERS
        segment_tree.h
        treewidget.h
        mainwindow.h
)

# Crear ejecutable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Linkear con Qt
target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)