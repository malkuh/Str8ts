cmake_minimum_required (VERSION 2.8)

set(PROJ_NAME "Str8ts")

project(${PROJ_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

add_executable(solver "src/solver.cpp")
# add_executable(test "src/test.cpp")

add_library(Str8tsLib "src/Str8ts.cpp" "src/SMap.cpp" "src/STable.cpp" "src/Straight.cpp" "src/LUTs.cpp")

target_link_libraries(solver Str8tsLib)
# target_link_libraries(test Str8tsLib)
