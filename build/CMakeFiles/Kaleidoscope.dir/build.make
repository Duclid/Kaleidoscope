# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dhp/code/cpp/Kaleidoscope

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dhp/code/cpp/Kaleidoscope/build

# Include any dependencies generated for this target.
include CMakeFiles/Kaleidoscope.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Kaleidoscope.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Kaleidoscope.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Kaleidoscope.dir/flags.make

CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.o: CMakeFiles/Kaleidoscope.dir/flags.make
CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.o: ../kaleidoscope.cpp
CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.o: CMakeFiles/Kaleidoscope.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dhp/code/cpp/Kaleidoscope/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.o -MF CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.o.d -o CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.o -c /home/dhp/code/cpp/Kaleidoscope/kaleidoscope.cpp

CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dhp/code/cpp/Kaleidoscope/kaleidoscope.cpp > CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.i

CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dhp/code/cpp/Kaleidoscope/kaleidoscope.cpp -o CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.s

# Object files for target Kaleidoscope
Kaleidoscope_OBJECTS = \
"CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.o"

# External object files for target Kaleidoscope
Kaleidoscope_EXTERNAL_OBJECTS =

Kaleidoscope: CMakeFiles/Kaleidoscope.dir/kaleidoscope.cpp.o
Kaleidoscope: CMakeFiles/Kaleidoscope.dir/build.make
Kaleidoscope: CMakeFiles/Kaleidoscope.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dhp/code/cpp/Kaleidoscope/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Kaleidoscope"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Kaleidoscope.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Kaleidoscope.dir/build: Kaleidoscope
.PHONY : CMakeFiles/Kaleidoscope.dir/build

CMakeFiles/Kaleidoscope.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Kaleidoscope.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Kaleidoscope.dir/clean

CMakeFiles/Kaleidoscope.dir/depend:
	cd /home/dhp/code/cpp/Kaleidoscope/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dhp/code/cpp/Kaleidoscope /home/dhp/code/cpp/Kaleidoscope /home/dhp/code/cpp/Kaleidoscope/build /home/dhp/code/cpp/Kaleidoscope/build /home/dhp/code/cpp/Kaleidoscope/build/CMakeFiles/Kaleidoscope.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Kaleidoscope.dir/depend
