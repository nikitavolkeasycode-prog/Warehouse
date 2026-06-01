cmake_minimum_required(VERSION 3.10)

project(WarehouseApp
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/Product.cpp
    src/Warehouse.cpp
    src/FileIO.cpp
    src/Menu.cpp
)

# Header files
set(HEADERS
    src/Product.h
    src/Warehouse.h
    src/FileIO.h
    src/Menu.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})