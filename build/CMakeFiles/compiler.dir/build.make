# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /opt/cmake-3.18.1/bin/cmake

# The command to remove a file.
RM = /opt/cmake-3.18.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/compiler39/compiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/compiler39/compiler/build

# Include any dependencies generated for this target.
include CMakeFiles/compiler.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/compiler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/compiler.dir/flags.make

CMakeFiles/compiler.dir/grammar/CACTBaseListener.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/grammar/CACTBaseListener.cpp.o: ../grammar/CACTBaseListener.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/compiler39/compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/compiler.dir/grammar/CACTBaseListener.cpp.o"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compiler.dir/grammar/CACTBaseListener.cpp.o -c /home/compiler39/compiler/grammar/CACTBaseListener.cpp

CMakeFiles/compiler.dir/grammar/CACTBaseListener.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/grammar/CACTBaseListener.cpp.i"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/compiler39/compiler/grammar/CACTBaseListener.cpp > CMakeFiles/compiler.dir/grammar/CACTBaseListener.cpp.i

CMakeFiles/compiler.dir/grammar/CACTBaseListener.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/grammar/CACTBaseListener.cpp.s"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/compiler39/compiler/grammar/CACTBaseListener.cpp -o CMakeFiles/compiler.dir/grammar/CACTBaseListener.cpp.s

CMakeFiles/compiler.dir/grammar/CACTLexer.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/grammar/CACTLexer.cpp.o: ../grammar/CACTLexer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/compiler39/compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/compiler.dir/grammar/CACTLexer.cpp.o"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compiler.dir/grammar/CACTLexer.cpp.o -c /home/compiler39/compiler/grammar/CACTLexer.cpp

CMakeFiles/compiler.dir/grammar/CACTLexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/grammar/CACTLexer.cpp.i"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/compiler39/compiler/grammar/CACTLexer.cpp > CMakeFiles/compiler.dir/grammar/CACTLexer.cpp.i

CMakeFiles/compiler.dir/grammar/CACTLexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/grammar/CACTLexer.cpp.s"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/compiler39/compiler/grammar/CACTLexer.cpp -o CMakeFiles/compiler.dir/grammar/CACTLexer.cpp.s

CMakeFiles/compiler.dir/grammar/CACTListener.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/grammar/CACTListener.cpp.o: ../grammar/CACTListener.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/compiler39/compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/compiler.dir/grammar/CACTListener.cpp.o"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compiler.dir/grammar/CACTListener.cpp.o -c /home/compiler39/compiler/grammar/CACTListener.cpp

CMakeFiles/compiler.dir/grammar/CACTListener.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/grammar/CACTListener.cpp.i"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/compiler39/compiler/grammar/CACTListener.cpp > CMakeFiles/compiler.dir/grammar/CACTListener.cpp.i

CMakeFiles/compiler.dir/grammar/CACTListener.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/grammar/CACTListener.cpp.s"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/compiler39/compiler/grammar/CACTListener.cpp -o CMakeFiles/compiler.dir/grammar/CACTListener.cpp.s

CMakeFiles/compiler.dir/grammar/CACTParser.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/grammar/CACTParser.cpp.o: ../grammar/CACTParser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/compiler39/compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/compiler.dir/grammar/CACTParser.cpp.o"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compiler.dir/grammar/CACTParser.cpp.o -c /home/compiler39/compiler/grammar/CACTParser.cpp

CMakeFiles/compiler.dir/grammar/CACTParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/grammar/CACTParser.cpp.i"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/compiler39/compiler/grammar/CACTParser.cpp > CMakeFiles/compiler.dir/grammar/CACTParser.cpp.i

CMakeFiles/compiler.dir/grammar/CACTParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/grammar/CACTParser.cpp.s"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/compiler39/compiler/grammar/CACTParser.cpp -o CMakeFiles/compiler.dir/grammar/CACTParser.cpp.s

CMakeFiles/compiler.dir/src/main.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/compiler39/compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/compiler.dir/src/main.cpp.o"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compiler.dir/src/main.cpp.o -c /home/compiler39/compiler/src/main.cpp

CMakeFiles/compiler.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/src/main.cpp.i"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/compiler39/compiler/src/main.cpp > CMakeFiles/compiler.dir/src/main.cpp.i

CMakeFiles/compiler.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/src/main.cpp.s"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/compiler39/compiler/src/main.cpp -o CMakeFiles/compiler.dir/src/main.cpp.s

CMakeFiles/compiler.dir/src/semanticAnalysis.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/src/semanticAnalysis.cpp.o: ../src/semanticAnalysis.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/compiler39/compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/compiler.dir/src/semanticAnalysis.cpp.o"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compiler.dir/src/semanticAnalysis.cpp.o -c /home/compiler39/compiler/src/semanticAnalysis.cpp

CMakeFiles/compiler.dir/src/semanticAnalysis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/src/semanticAnalysis.cpp.i"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/compiler39/compiler/src/semanticAnalysis.cpp > CMakeFiles/compiler.dir/src/semanticAnalysis.cpp.i

CMakeFiles/compiler.dir/src/semanticAnalysis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/src/semanticAnalysis.cpp.s"
	/usr/bin/g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/compiler39/compiler/src/semanticAnalysis.cpp -o CMakeFiles/compiler.dir/src/semanticAnalysis.cpp.s

# Object files for target compiler
compiler_OBJECTS = \
"CMakeFiles/compiler.dir/grammar/CACTBaseListener.cpp.o" \
"CMakeFiles/compiler.dir/grammar/CACTLexer.cpp.o" \
"CMakeFiles/compiler.dir/grammar/CACTListener.cpp.o" \
"CMakeFiles/compiler.dir/grammar/CACTParser.cpp.o" \
"CMakeFiles/compiler.dir/src/main.cpp.o" \
"CMakeFiles/compiler.dir/src/semanticAnalysis.cpp.o"

# External object files for target compiler
compiler_EXTERNAL_OBJECTS =

compiler: CMakeFiles/compiler.dir/grammar/CACTBaseListener.cpp.o
compiler: CMakeFiles/compiler.dir/grammar/CACTLexer.cpp.o
compiler: CMakeFiles/compiler.dir/grammar/CACTListener.cpp.o
compiler: CMakeFiles/compiler.dir/grammar/CACTParser.cpp.o
compiler: CMakeFiles/compiler.dir/src/main.cpp.o
compiler: CMakeFiles/compiler.dir/src/semanticAnalysis.cpp.o
compiler: CMakeFiles/compiler.dir/build.make
compiler: CMakeFiles/compiler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/compiler39/compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable compiler"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/compiler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/compiler.dir/build: compiler

.PHONY : CMakeFiles/compiler.dir/build

CMakeFiles/compiler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/compiler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/compiler.dir/clean

CMakeFiles/compiler.dir/depend:
	cd /home/compiler39/compiler/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/compiler39/compiler /home/compiler39/compiler /home/compiler39/compiler/build /home/compiler39/compiler/build /home/compiler39/compiler/build/CMakeFiles/compiler.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/compiler.dir/depend
