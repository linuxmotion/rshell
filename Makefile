CC=g++
CFLAGS= -Wall 
LDFLAGS=
#SOURCES=src/CS100.cpp src/Shell.cpp  
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=rshell

all: $(SOURCES) $(EXECUTABLE) ls cp
debug: LDFLAGS += -g
debug: all
	
	
$(EXECUTABLE): obj/CS100.o obj/Shell.o obj/cd.o obj/log.o obj/tokenizer.o obj/DirUtils.o
	@mkdir -p bin
	$(CC)  $(LDFLAGS) obj/CS100.o obj/Shell.o  obj/cd.o obj/log.o  -o bin/$@


cp : obj/cp.o 
	$(CC)  $(LDFLAGS) obj/cp.o -o bin/$@
	
ls:obj/Lslib.o obj/Ls.o 
	$(CC)  $(LDFLAGS) obj/Lslib.o obj/Ls.o  -o bin/$@	
	
	
## CP files

obj/cp.o : src/cp.cpp
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@
	
	
## Shell files



obj/Shell.o: src/Shell.cpp  src/DirUtils.h src/log.cpp src/Tokenizer.h src/Cd.cpp
	@mkdir -p obj
	$(CC) -c $(CFLAGS)  $<  -o $@
		
obj/CS100.o: src/CS100.cpp src/Shell.h
	@mkdir -p obj
	$(CC) -c $(CFLAGS)  $< -o $@
	
obj/tokenizer.o: src/Tokenizer.h
	@mkdir -p obj
	$(CC) -c $(CFLAGS)  $< -o $@	
	
## Ls files
	
obj/Lslib.o:src/Lslib.cpp src/DirUtils.h
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@

obj/Ls.o:src/Ls.cpp src/Lslib.cpp
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@
	
	
obj/DirUtils.o:src/DirUtils.h
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@
	
## CD files

obj/cd.o:src/Cd.cpp 
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@
		
## Loggers
	
obj/log.o: src/log.cpp
	@mkdir -p obj
	$(CC) -c $(CFLAGS)  $< -o $@

# make commands
	
install:
	@mkdir -p ~/bin
	mv bin/* ~/bin/
	
uninstall:
	rm ~/bin/ls ~/bin/rshell 


clean:
	rm -rf bin obj
