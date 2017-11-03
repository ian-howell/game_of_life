.PHONY: all clean

CXX = /usr/bin/g++
CXXFLAGS = -g -Wall -w -pedantic-errors -Wextra -Wconversion -std=c++11
LINKER = -lncurses

SOURCES = $(wildcard src/*.cpp)
HEADERS = $(wildcard src/*.h)

OBJECTS = $(SOURCES:%.cpp=%.o)
DEPS := $(OBJECTS:%.o=%.d)

EXECUTABLE = driver

default: $(EXECUTABLE)

-include $(DEPS)

%.o: %.cpp
	@echo "Compiling $<"
	@$(CXX) -c $(CXXFLAGS) -MMD -o $@ $< $(LINKER)

$(EXECUTABLE): $(OBJECTS)
	@echo "Building $@"
	@$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LINKER)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

profile: CXXFLAGS = -g -Wall -w -pedantic-errors -Wextra -Wconversion -std=c++11 -pg
profile: $(EXECUTABLE)

clean:
	-@rm -f core
	-@rm -f $(EXECUTABLE)
	-@rm -f $(OBJECTS)
	-@rm -f $(DEPS)
