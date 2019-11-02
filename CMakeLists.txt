cmake_minimum_required(VERSION 3.12)
project(DevCon_ticket)

set(CMAKE_CXX_STANDARD 17)

#[[if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    find_package( Boost REQUIRED )
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()]]

add_subdirectory(CLI11)

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_library(qrLib qr/BitBuffer.cpp qr/QrCode.cpp qr/QrSegment.cpp)
add_library(ticket devcon_ticket.cpp)


find_program(MAGICK_CONFIG "Magick++-config")
# Ask about CXX and lib flags/locations
execute_process(COMMAND "${MAGICK_CONFIG}" "--cxxflags" OUTPUT_VARIABLE MAGICK_CXX_FLAGS)
execute_process(COMMAND "${MAGICK_CONFIG}" "--libs" OUTPUT_VARIABLE MAGICK_LD_FLAGS)
# Remove trailing whitespace (CMAKE warns about this)
string(STRIP "${MAGICK_CXX_FLAGS}" MAGICK_CXX_FLAGS)
string(STRIP "${MAGICK_LD_FLAGS}" MAGICK_LD_FLAGS)
# Append all flags to CMAKE_CXX_FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${MAGICK_CXX_FLAGS} ${MAGICK_LD_FLAGS}")


add_executable(DevCon_ticket main.cpp )
target_link_libraries(DevCon_ticket ticket qrLib ${OpenCV_LIBS} CLI11)
