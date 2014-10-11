cmake_minimum_required( VERSION 2.8 )

set (CMAKE_INSTALL_PREFIX .. CACHE PATH "")

project( AFourPaper )
enable_language( CXX )

# Add here your C++ files (the headers too)
#set ( SOURCES
#    "pngToData.cpp"
#)

add_library(
    ImageLibrary
    STATIC
    "Image.h"
    "Image.cpp"
    "lodepng.h"
    "lodepng.cpp"
)

add_executable(
    encode
    "encode.cpp"
)
target_link_libraries(encode ImageLibrary)

add_executable(
    decode
    "decode.cpp"
)
target_link_libraries(decode ImageLibrary)

install(TARGETS encode decode RUNTIME DESTINATION bin)
