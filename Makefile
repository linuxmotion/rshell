CC=g++
CFLAGS= -Wall
LDFLAGS=
SOURCES=src/CS100.cpp src/Shell.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=rshell

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): obj/CS100.o obj/Shell.o 
	@mkdir -p bin
	$(CC)  $(LDFLAGS) obj/CS100.o obj/Shell.o  -o bin/$@

obj/Shell.o:src/Shell.cpp
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@

obj/CS100.o:src/CS100.cpp 
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm -rf bin obj
