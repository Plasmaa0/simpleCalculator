CC=g++
C_RELEASE_FLAGS=-o0
C_DEBUG_FLAGS=-Wall -g3 -o0 -ggdb -DDEBUG
SOURCES=$(wildcard *.cpp)
OBJDIR=objects
OBJECTS=$(SOURCES:%.cpp=$(OBJDIR)\\%.o)
EXECUTABLE=build.exe

all: release

clean:
	del $(OBJECTS)
	del $(EXECUTABLE)

mem: debug
	drmemory -ignore_kernel $(EXECUTABLE)

gdb: debug
	gdb $(EXECUTABLE)

debug: $(OBJECTS)
	$(CC) $(C_DEBUG_FLAGS) $(OBJECTS) -o $(EXECUTABLE)

release: $(OBJECTS)
	$(CC) $(C_RELEASE_FLAGS) $(OBJECTS) -o $(EXECUTABLE)

$(OBJDIR)\\%.o: %.cpp
	$(CC) $< -c -o $@