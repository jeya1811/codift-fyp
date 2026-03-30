cmake_minimum_required(VERSION 3.10)
project(CodiftPass)

find_package(LLVM 15 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(CodiftPass MODULE
    checkPass.cpp
    injectPass.cpp
)

set_target_properties(CodiftPass PROPERTIES
    CXX_STANDARD          14
    CXX_STANDARD_REQUIRED YES
    LINK_FLAGS            "-Wl,--allow-shlib-undefined"
)
