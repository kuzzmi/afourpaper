# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kuzzmi/Projects/afourpaper/server/imagetools/encoder

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kuzzmi/Projects/afourpaper/server/imagetools/encoder/build

# Include any dependencies generated for this target.
include CMakeFiles/decode.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/decode.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/decode.dir/flags.make

CMakeFiles/decode.dir/decode.cpp.o: CMakeFiles/decode.dir/flags.make
CMakeFiles/decode.dir/decode.cpp.o: ../decode.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kuzzmi/Projects/afourpaper/server/imagetools/encoder/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/decode.dir/decode.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/decode.dir/decode.cpp.o -c /home/kuzzmi/Projects/afourpaper/server/imagetools/encoder/decode.cpp

CMakeFiles/decode.dir/decode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/decode.dir/decode.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/kuzzmi/Projects/afourpaper/server/imagetools/encoder/decode.cpp > CMakeFiles/decode.dir/decode.cpp.i

CMakeFiles/decode.dir/decode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/decode.dir/decode.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/kuzzmi/Projects/afourpaper/server/imagetools/encoder/decode.cpp -o CMakeFiles/decode.dir/decode.cpp.s

CMakeFiles/decode.dir/decode.cpp.o.requires:
.PHONY : CMakeFiles/decode.dir/decode.cpp.o.requires

CMakeFiles/decode.dir/decode.cpp.o.provides: CMakeFiles/decode.dir/decode.cpp.o.requires
	$(MAKE) -f CMakeFiles/decode.dir/build.make CMakeFiles/decode.dir/decode.cpp.o.provides.build
.PHONY : CMakeFiles/decode.dir/decode.cpp.o.provides

CMakeFiles/decode.dir/decode.cpp.o.provides.build: CMakeFiles/decode.dir/decode.cpp.o

# Object files for target decode
decode_OBJECTS = \
"CMakeFiles/decode.dir/decode.cpp.o"

# External object files for target decode
decode_EXTERNAL_OBJECTS =

decode: CMakeFiles/decode.dir/decode.cpp.o
decode: CMakeFiles/decode.dir/build.make
decode: libImageLibrary.a
decode: CMakeFiles/decode.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable decode"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/decode.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/decode.dir/build: decode
.PHONY : CMakeFiles/decode.dir/build

CMakeFiles/decode.dir/requires: CMakeFiles/decode.dir/decode.cpp.o.requires
.PHONY : CMakeFiles/decode.dir/requires

CMakeFiles/decode.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/decode.dir/cmake_clean.cmake
.PHONY : CMakeFiles/decode.dir/clean

CMakeFiles/decode.dir/depend:
	cd /home/kuzzmi/Projects/afourpaper/server/imagetools/encoder/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kuzzmi/Projects/afourpaper/server/imagetools/encoder /home/kuzzmi/Projects/afourpaper/server/imagetools/encoder /home/kuzzmi/Projects/afourpaper/server/imagetools/encoder/build /home/kuzzmi/Projects/afourpaper/server/imagetools/encoder/build /home/kuzzmi/Projects/afourpaper/server/imagetools/encoder/build/CMakeFiles/decode.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/decode.dir/depend

