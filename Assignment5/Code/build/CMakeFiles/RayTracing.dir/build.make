# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jbgong/Downloads/games101/Assignment5/Code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jbgong/Downloads/games101/Assignment5/Code/build

# Include any dependencies generated for this target.
include CMakeFiles/RayTracing.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RayTracing.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RayTracing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RayTracing.dir/flags.make

CMakeFiles/RayTracing.dir/main.cpp.o: CMakeFiles/RayTracing.dir/flags.make
CMakeFiles/RayTracing.dir/main.cpp.o: ../main.cpp
CMakeFiles/RayTracing.dir/main.cpp.o: CMakeFiles/RayTracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jbgong/Downloads/games101/Assignment5/Code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RayTracing.dir/main.cpp.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracing.dir/main.cpp.o -MF CMakeFiles/RayTracing.dir/main.cpp.o.d -o CMakeFiles/RayTracing.dir/main.cpp.o -c /Users/jbgong/Downloads/games101/Assignment5/Code/main.cpp

CMakeFiles/RayTracing.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracing.dir/main.cpp.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jbgong/Downloads/games101/Assignment5/Code/main.cpp > CMakeFiles/RayTracing.dir/main.cpp.i

CMakeFiles/RayTracing.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracing.dir/main.cpp.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jbgong/Downloads/games101/Assignment5/Code/main.cpp -o CMakeFiles/RayTracing.dir/main.cpp.s

CMakeFiles/RayTracing.dir/Scene.cpp.o: CMakeFiles/RayTracing.dir/flags.make
CMakeFiles/RayTracing.dir/Scene.cpp.o: ../Scene.cpp
CMakeFiles/RayTracing.dir/Scene.cpp.o: CMakeFiles/RayTracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jbgong/Downloads/games101/Assignment5/Code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RayTracing.dir/Scene.cpp.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracing.dir/Scene.cpp.o -MF CMakeFiles/RayTracing.dir/Scene.cpp.o.d -o CMakeFiles/RayTracing.dir/Scene.cpp.o -c /Users/jbgong/Downloads/games101/Assignment5/Code/Scene.cpp

CMakeFiles/RayTracing.dir/Scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracing.dir/Scene.cpp.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jbgong/Downloads/games101/Assignment5/Code/Scene.cpp > CMakeFiles/RayTracing.dir/Scene.cpp.i

CMakeFiles/RayTracing.dir/Scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracing.dir/Scene.cpp.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jbgong/Downloads/games101/Assignment5/Code/Scene.cpp -o CMakeFiles/RayTracing.dir/Scene.cpp.s

CMakeFiles/RayTracing.dir/Renderer.cpp.o: CMakeFiles/RayTracing.dir/flags.make
CMakeFiles/RayTracing.dir/Renderer.cpp.o: ../Renderer.cpp
CMakeFiles/RayTracing.dir/Renderer.cpp.o: CMakeFiles/RayTracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jbgong/Downloads/games101/Assignment5/Code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/RayTracing.dir/Renderer.cpp.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracing.dir/Renderer.cpp.o -MF CMakeFiles/RayTracing.dir/Renderer.cpp.o.d -o CMakeFiles/RayTracing.dir/Renderer.cpp.o -c /Users/jbgong/Downloads/games101/Assignment5/Code/Renderer.cpp

CMakeFiles/RayTracing.dir/Renderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracing.dir/Renderer.cpp.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jbgong/Downloads/games101/Assignment5/Code/Renderer.cpp > CMakeFiles/RayTracing.dir/Renderer.cpp.i

CMakeFiles/RayTracing.dir/Renderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracing.dir/Renderer.cpp.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jbgong/Downloads/games101/Assignment5/Code/Renderer.cpp -o CMakeFiles/RayTracing.dir/Renderer.cpp.s

# Object files for target RayTracing
RayTracing_OBJECTS = \
"CMakeFiles/RayTracing.dir/main.cpp.o" \
"CMakeFiles/RayTracing.dir/Scene.cpp.o" \
"CMakeFiles/RayTracing.dir/Renderer.cpp.o"

# External object files for target RayTracing
RayTracing_EXTERNAL_OBJECTS =

RayTracing: CMakeFiles/RayTracing.dir/main.cpp.o
RayTracing: CMakeFiles/RayTracing.dir/Scene.cpp.o
RayTracing: CMakeFiles/RayTracing.dir/Renderer.cpp.o
RayTracing: CMakeFiles/RayTracing.dir/build.make
RayTracing: CMakeFiles/RayTracing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jbgong/Downloads/games101/Assignment5/Code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable RayTracing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RayTracing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RayTracing.dir/build: RayTracing
.PHONY : CMakeFiles/RayTracing.dir/build

CMakeFiles/RayTracing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RayTracing.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RayTracing.dir/clean

CMakeFiles/RayTracing.dir/depend:
	cd /Users/jbgong/Downloads/games101/Assignment5/Code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jbgong/Downloads/games101/Assignment5/Code /Users/jbgong/Downloads/games101/Assignment5/Code /Users/jbgong/Downloads/games101/Assignment5/Code/build /Users/jbgong/Downloads/games101/Assignment5/Code/build /Users/jbgong/Downloads/games101/Assignment5/Code/build/CMakeFiles/RayTracing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RayTracing.dir/depend

