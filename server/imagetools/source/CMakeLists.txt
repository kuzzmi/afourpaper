cmake_minimum_required( VERSION 2.8.11.1 )

project( AFourPaper )
enable_language( CXX )

find_package( OpenCV REQUIRED )

# Add here your C++ files (the headers too)
set ( SOURCES
    "pngToData.cpp"
)

add_executable(
    ImageProcessor
    ${SOURCES}
)

target_link_libraries( ImageProcessor ${OpenCV_LIBS} )

