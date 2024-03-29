CC=g++
CFLAGS=-Wall -g3 -o0 -ggdb -D DEBUG
SOURCEDIR=source
SOURCES=$(wildcard $(SOURCEDIR)/*.cpp)
OBJDIR=objects
OBJECTS=$(SOURCES:$(SOURCEDIR)/%.cpp=$(OBJDIR)\\%.o)
EXECUTABLE=calculator.exe

all: $(EXECUTABLE)

clean:
	del $(OBJECTS)
	del $(EXECUTABLE)
	del variables*

mem: $(EXECUTABLE)
	drmemory -ignore_kernel $(EXECUTABLE)

gdb: $(EXECUTABLE)
	gdb $(EXECUTABLE)

run: $(EXECUTABLE)
	$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

$(OBJDIR)\\%.o: $(SOURCEDIR)\%.cpp
	$(CC) $(CFLAGS) $< -c -o $@