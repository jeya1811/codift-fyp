cmake_minimum_required(VERSION 3.13)
project(Count)

find_package(LLVM 15.0 REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(CountFunc MODULE
    passes/countFunc.cpp
)
add_library(CodiftCheckPass MODULE
    passes/codiftCheckPass.cpp
)

target_link_libraries(CodiftCheckPass
    ${LLVM_LIBRARIES}
)

set_target_properties(CodiftCheckPass PROPERTIES
    PREFIX ""
    COMPILE_FLAGS "-fno-rtti"
)