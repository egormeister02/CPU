# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/egor/Projects/CPU

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/egor/Projects/CPU/build

# Include any dependencies generated for this target.
include CMakeFiles/cpu.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cpu.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cpu.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cpu.dir/flags.make

CMakeFiles/cpu.dir/cpu/CPU_func.cpp.o: CMakeFiles/cpu.dir/flags.make
CMakeFiles/cpu.dir/cpu/CPU_func.cpp.o: ../cpu/CPU_func.cpp
CMakeFiles/cpu.dir/cpu/CPU_func.cpp.o: CMakeFiles/cpu.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/egor/Projects/CPU/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cpu.dir/cpu/CPU_func.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cpu.dir/cpu/CPU_func.cpp.o -MF CMakeFiles/cpu.dir/cpu/CPU_func.cpp.o.d -o CMakeFiles/cpu.dir/cpu/CPU_func.cpp.o -c /home/egor/Projects/CPU/cpu/CPU_func.cpp

CMakeFiles/cpu.dir/cpu/CPU_func.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpu.dir/cpu/CPU_func.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/egor/Projects/CPU/cpu/CPU_func.cpp > CMakeFiles/cpu.dir/cpu/CPU_func.cpp.i

CMakeFiles/cpu.dir/cpu/CPU_func.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpu.dir/cpu/CPU_func.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/egor/Projects/CPU/cpu/CPU_func.cpp -o CMakeFiles/cpu.dir/cpu/CPU_func.cpp.s

CMakeFiles/cpu.dir/cpu/CPU_main.cpp.o: CMakeFiles/cpu.dir/flags.make
CMakeFiles/cpu.dir/cpu/CPU_main.cpp.o: ../cpu/CPU_main.cpp
CMakeFiles/cpu.dir/cpu/CPU_main.cpp.o: CMakeFiles/cpu.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/egor/Projects/CPU/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/cpu.dir/cpu/CPU_main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cpu.dir/cpu/CPU_main.cpp.o -MF CMakeFiles/cpu.dir/cpu/CPU_main.cpp.o.d -o CMakeFiles/cpu.dir/cpu/CPU_main.cpp.o -c /home/egor/Projects/CPU/cpu/CPU_main.cpp

CMakeFiles/cpu.dir/cpu/CPU_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpu.dir/cpu/CPU_main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/egor/Projects/CPU/cpu/CPU_main.cpp > CMakeFiles/cpu.dir/cpu/CPU_main.cpp.i

CMakeFiles/cpu.dir/cpu/CPU_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpu.dir/cpu/CPU_main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/egor/Projects/CPU/cpu/CPU_main.cpp -o CMakeFiles/cpu.dir/cpu/CPU_main.cpp.s

# Object files for target cpu
cpu_OBJECTS = \
"CMakeFiles/cpu.dir/cpu/CPU_func.cpp.o" \
"CMakeFiles/cpu.dir/cpu/CPU_main.cpp.o"

# External object files for target cpu
cpu_EXTERNAL_OBJECTS =

../bin/cpu: CMakeFiles/cpu.dir/cpu/CPU_func.cpp.o
../bin/cpu: CMakeFiles/cpu.dir/cpu/CPU_main.cpp.o
../bin/cpu: CMakeFiles/cpu.dir/build.make
../bin/cpu: libMyStack.a
../bin/cpu: CMakeFiles/cpu.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/egor/Projects/CPU/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../bin/cpu"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cpu.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cpu.dir/build: ../bin/cpu
.PHONY : CMakeFiles/cpu.dir/build

CMakeFiles/cpu.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cpu.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cpu.dir/clean

CMakeFiles/cpu.dir/depend:
	cd /home/egor/Projects/CPU/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/egor/Projects/CPU /home/egor/Projects/CPU /home/egor/Projects/CPU/build /home/egor/Projects/CPU/build /home/egor/Projects/CPU/build/CMakeFiles/cpu.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cpu.dir/depend
