cmake_minimum_required(VERSION 3.14)

project(finances)


set(CMAKE_CXX_STANDARD 23)
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.9.3/lib/cmake/Qt6")

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE PRIVATE_HEADERS src/*.hpp)
qt_wrap_cpp(MOCS_HEADERS ${PRIVATE_HEADERS})

add_executable(${PROJECT_NAME} ${SOURCES} ${PRIVATE_HEADERS} ${MOCS_HEADERS})
target_link_libraries(${PROJECT_NAME} Qt::Core Qt::Widgets)
