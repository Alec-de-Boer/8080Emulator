cmake_minimum_required(VERSION 3.24)
project(8080Emulator)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lmingw32")


include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(8080Emulator src/main.cpp src/State8080.cpp src/Emulator.cpp src/CPU.cpp src/CpuFunctions.cpp)

target_link_libraries(8080Emulator mingw32 SDL2main SDL2)



#[[
cmake_minimum_required(VERSION 3.24)
project(8080Emulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(8080Emulator main.cpp)

]]

