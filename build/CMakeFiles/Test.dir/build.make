# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.27.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.27.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/dhp/Dev/cpp/Kaleidoscope

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/dhp/Dev/cpp/Kaleidoscope/build

# Include any dependencies generated for this target.
include CMakeFiles/Test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Test.dir/flags.make

CMakeFiles/Test.dir/test.cpp.o: CMakeFiles/Test.dir/flags.make
CMakeFiles/Test.dir/test.cpp.o: /Users/dhp/Dev/cpp/Kaleidoscope/test.cpp
CMakeFiles/Test.dir/test.cpp.o: CMakeFiles/Test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dhp/Dev/cpp/Kaleidoscope/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Test.dir/test.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Test.dir/test.cpp.o -MF CMakeFiles/Test.dir/test.cpp.o.d -o CMakeFiles/Test.dir/test.cpp.o -c /Users/dhp/Dev/cpp/Kaleidoscope/test.cpp

CMakeFiles/Test.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Test.dir/test.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dhp/Dev/cpp/Kaleidoscope/test.cpp > CMakeFiles/Test.dir/test.cpp.i

CMakeFiles/Test.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Test.dir/test.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dhp/Dev/cpp/Kaleidoscope/test.cpp -o CMakeFiles/Test.dir/test.cpp.s

CMakeFiles/Test.dir/parser.cpp.o: CMakeFiles/Test.dir/flags.make
CMakeFiles/Test.dir/parser.cpp.o: /Users/dhp/Dev/cpp/Kaleidoscope/parser.cpp
CMakeFiles/Test.dir/parser.cpp.o: CMakeFiles/Test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dhp/Dev/cpp/Kaleidoscope/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Test.dir/parser.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Test.dir/parser.cpp.o -MF CMakeFiles/Test.dir/parser.cpp.o.d -o CMakeFiles/Test.dir/parser.cpp.o -c /Users/dhp/Dev/cpp/Kaleidoscope/parser.cpp

CMakeFiles/Test.dir/parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Test.dir/parser.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dhp/Dev/cpp/Kaleidoscope/parser.cpp > CMakeFiles/Test.dir/parser.cpp.i

CMakeFiles/Test.dir/parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Test.dir/parser.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dhp/Dev/cpp/Kaleidoscope/parser.cpp -o CMakeFiles/Test.dir/parser.cpp.s

CMakeFiles/Test.dir/lex.cpp.o: CMakeFiles/Test.dir/flags.make
CMakeFiles/Test.dir/lex.cpp.o: /Users/dhp/Dev/cpp/Kaleidoscope/lex.cpp
CMakeFiles/Test.dir/lex.cpp.o: CMakeFiles/Test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dhp/Dev/cpp/Kaleidoscope/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Test.dir/lex.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Test.dir/lex.cpp.o -MF CMakeFiles/Test.dir/lex.cpp.o.d -o CMakeFiles/Test.dir/lex.cpp.o -c /Users/dhp/Dev/cpp/Kaleidoscope/lex.cpp

CMakeFiles/Test.dir/lex.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Test.dir/lex.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dhp/Dev/cpp/Kaleidoscope/lex.cpp > CMakeFiles/Test.dir/lex.cpp.i

CMakeFiles/Test.dir/lex.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Test.dir/lex.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dhp/Dev/cpp/Kaleidoscope/lex.cpp -o CMakeFiles/Test.dir/lex.cpp.s

# Object files for target Test
Test_OBJECTS = \
"CMakeFiles/Test.dir/test.cpp.o" \
"CMakeFiles/Test.dir/parser.cpp.o" \
"CMakeFiles/Test.dir/lex.cpp.o"

# External object files for target Test
Test_EXTERNAL_OBJECTS =

Test: CMakeFiles/Test.dir/test.cpp.o
Test: CMakeFiles/Test.dir/parser.cpp.o
Test: CMakeFiles/Test.dir/lex.cpp.o
Test: CMakeFiles/Test.dir/build.make
Test: CMakeFiles/Test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/dhp/Dev/cpp/Kaleidoscope/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable Test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Test.dir/build: Test
.PHONY : CMakeFiles/Test.dir/build

CMakeFiles/Test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Test.dir/clean

CMakeFiles/Test.dir/depend:
	cd /Users/dhp/Dev/cpp/Kaleidoscope/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/dhp/Dev/cpp/Kaleidoscope /Users/dhp/Dev/cpp/Kaleidoscope /Users/dhp/Dev/cpp/Kaleidoscope/build /Users/dhp/Dev/cpp/Kaleidoscope/build /Users/dhp/Dev/cpp/Kaleidoscope/build/CMakeFiles/Test.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Test.dir/depend

