MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

CXX = ../../../raylib/w64devkit/bin/g++
RAYLIB_PATH = ../../../raylib/raylib
CXXFLAGS = -DPLATFORM_DESKTOP -static -std=c++20 -Wall


CXXFLAGS += -ggdb3
# DEBUG = true

INCLUDE_PATHS = -I. -Ilib -IeUGxternal -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external/glfw/include
LDFLAGS = -L. -L$(RAYLIB_RELEASE_PATH) -L$(RAYLIB_PATH)/src -lraylib -lopengl32 -lgdi32 -lwinmm

SRCDIR = src
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
HEADERS = $(wildcard $(SRCDIR)/*.hpp)
OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))

EXECUTABLE = field

export PATH := $(RAYLIB_PATH)\..\w64devkit\bin;$(PATH)

all: $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDE_PATHS) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATHS) -c $< -o $@



clean:
	rm -f $(OBJECTS) $(EXECUTABLE)


.PHONY: all clean
