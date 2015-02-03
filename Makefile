CC=g++
CFLAGS= -Wall
LDFLAGS=
SOURCES=src/CS100.cpp src/Shell.cpp  
LSSOURCE=src/SmallLS.cpp src/SmallLS.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=rshell

all: $(SOURCES) $(EXECUTABLE) ls
	
$(EXECUTABLE): obj/CS100.o obj/Shell.o 
	@mkdir -p bin 
	$(CC)  $(LDFLAGS) obj/CS100.o obj/Shell.o  -o bin/$@

obj/Shell.o:src/Shell.cpp
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@

obj/CS100.o:src/CS100.cpp 
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@
	
obj/log.o:src/log.cpp 
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@
	
	
ls:obj/Lslib.o obj/Ls.o
	$(CC)  $(LDFLAGS) obj/Lslib.o obj/Ls.o -o bin/$@
	
obj/Lslib.o:src/Lslib.cpp 
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@

obj/Ls.o:src/Ls.cpp 
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@
	
install:
	@mkdir -p ~/bin
	mv bin/* ~/bin/
		
	
clean:
	rm -rf bin obj
