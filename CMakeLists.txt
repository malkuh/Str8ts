cmake_minimum_required (VERSION 3.0)

set(PROJ_NAME "Str8ts")

project(Str8ts
    VERSION 1.0
    DESCRIPTION "Str8ts Solver"
    LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

add_executable(solver "src/solver.cpp")

add_library(Str8tsLib "src/Str8ts.cpp" "src/SMap.cpp" "src/STable.cpp" "src/Straight.cpp" "src/LUTs.cpp" "src/runstats.cpp" "src/verbose.cpp" "src/prompt.cpp" "src/helpme.cpp")

find_library(LIBRARY_NCURSES  ncurses)

target_link_libraries(solver PRIVATE Str8tsLib ${LIBRARY_NCURSES})
