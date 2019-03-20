cmake_minimum_required(VERSION 2.8)
project(a4-ece650)

if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
    add_subdirectory(minisat)
else()
    message (FATAL_ERROR "MiniSat not found. Use `git clone https://github.com/agurfinkel/minisat` to check it out.")
endif()


## ECE650 Specific configuration start here
## Project specific configurations go here
find_package (Threads)

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")

# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/minisat)
add_executable(prj-ece650 apece650.cpp datatype.h datatype.cpp IJK.cpp IJK.h PlainInterpreter.cpp PlainInterpreter.h EIterator.cpp EIterator.h Solver.cpp Solver.h)

# create the main executable
# link MiniSAT libraries
target_link_libraries(prj-ece650 minisat-lib-static)
target_link_libraries(prj-ece650 ${CMAKE_THREAD_LIBS_INIT})


#add_executable(a2-ece650 a2ece650.cpp datatype.h datatype.cpp IJK.cpp IJK.h PlainInterpreter.cpp PlainInterpreter.h EIterator.cpp EIterator.h Solver.cpp Solver.h)
