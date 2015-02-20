CC=g++
CFLAGS= -Wall
LDFLAGS= 
SOURCES=src/CS100.cpp src/Shell.cpp  
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=rshell

all: $(SOURCES) $(EXECUTABLE) ls cp
debug: LDFLAGS += -g
debug: all
	
	
$(EXECUTABLE): obj/CS100.o obj/Shell.o
	@mkdir -p bin
	$(CC)  $(LDFLAGS) obj/CS100.o obj/Shell.o   -o bin/$@

cp : obj/cp.o
	$(CC)  $(LDFLAGS) obj/cp.o -o bin/$@
	
ls:obj/Lslib.o obj/Ls.o
	$(CC)  $(LDFLAGS) obj/Lslib.o obj/Ls.o -o bin/$@	
	
	
## CP files

obj/cp.o : src/cp.cpp
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@
	
	
## Shell files

	


obj/Shell.o:src/Shell.cpp 
	@mkdir -p obj
	$(CC) -c $(CFLAGS)  $< -o $@
	
obj/SimpleGLibPipe.o:src/SimpleGLibPipe.h
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@
	
obj/CS100.o:src/CS100.cpp 
	@mkdir -p obj
	$(CC) -c $(CFLAGS)  $< -o $@
	
obj/log.o:src/log.cpp 
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@
obj/Tokenizer.o:src/Tokenizer.cpp 
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@	
	
## Ls files
	
obj/Lslib.o:src/Lslib.cpp src/DirUtils.h
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@

obj/Ls.o:src/Ls.cpp 
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@
	
	
obj/DirUtils.o:src/DirUtils.h
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@
	

# make commands
	
install:
	@mkdir -p ~/bin
	mv bin/* ~/bin/
	
uninstall:
	rm ~/bin/ls ~/bin/rshell 


clean:
	rm -rf bin obj
