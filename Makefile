# Executable name
EXEC = pacoronam
# All .cpp files are assumed to be source files
SRCS = $(wildcard *.cpp)
# Dependency files, automatically generated and put into `build` folder
DEPS = $(patsubst %.cpp,build/%.d,$(SRCS))
# Object files (also in `build` folder)
OBJS = $(DEPS:.d=.o)
# C++ compiler and its compilation flags (can be overriden on the command line)
CXX = clang++
CXXFLAGS = -std=c++14 -O3 -Wall -Wextra -pedantic -g -lsfml-graphics -lsfml-window -lsfml-system
DEPFLAGS = -MT $@ -MMD -MP -MF build/$*.Td
# Other commands
RMDIR = rm -rf

# Default target (it comes first), conventionally called `all`
all:	$(EXEC)

# Removes the build folder, but keeps generated executable if present
clean:
	@echo "CLEAN"
	$(RMDIR) build

# Removes both the build folder and generated executable
distclean:	clean
	@echo "DISTCLEAN"
	$(RM) $(EXEC)

# Executable is assumed to depend on all objects (i.e. on all .o files)
$(EXEC):	$(OBJS)
	@echo "LINK $@"
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to build object files and dependencies
build/%.o:	%.cpp build/%.d | build
	@echo "CXX $@"
	$(CXX) $(DEPFLAGS) $(CXXFLAGS) -c -o $@ $<
	mv $(@:.o=.Td) $(@:.o=.d)
	touch $@

build:
	@echo "MKDIR build"
	mkdir -p build

# For more details about dependency handling, see
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
$(DEPS):

include $(wildcard $(DEPS))

# Phony rules are those that do not correspond to an actual file
.PHONY: all clean distclean

# Only display recipes when doing `make V=1`
$(V).SILENT:
