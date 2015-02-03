### RSHell

## Has two programs made durring compilation
    rshell
    ls
    
#ls
    make ls
    bin/ls

#rshell
    make rshell
    bin/rshell

#install
make 
make install

This installs both the rshell program and the ls
program into the users home directory 

running 'make all' make both programs


# Features
  Takes comments after \# 
  Any strings after the \# sign will be ignored

  
# Bugs
    rshell does not terminate on first exit signal
    rshell accepts &&, || without spaces before and after token
        -It must require at least one space before and after token  