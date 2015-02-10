#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>

// must use valgrind and cppcheck and pass without bugs
using namespace std;


#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

// extern "C" int gettimeofday(timeval *tp, void *tzp);
// extern "C" int getrusage(int who, struct rusage *rusag);

class Timer
{
public:
    int start();
    int elapsedWallclockTime (double &);
    int elapsedUserTime (double &);
    int elapsedSystemTime (double &);
    int elapsedTime (double &wc, double &us, double &system);
    
private:
    rusage old_us_time;
    rusage new_us_time;
    timeval old_wc_time;
    timeval new_wc_time;
};


int
Timer::start()
{
    if (gettimeofday (&this->old_wc_time, 0) == -1
        || getrusage (RUSAGE_SELF, &this->old_us_time) == -1)
        return -1;
    else
        return 0;
}

int
Timer::elapsedWallclockTime (double &wc)
{
    if (gettimeofday (&this->new_wc_time, 0) == -1)
        return -1;
    wc = (this->new_wc_time.tv_sec - this->old_wc_time.tv_sec)
    + (this->new_wc_time.tv_usec - this->old_wc_time.tv_usec) / 1000000.0;
    return 0;
}

int
Timer::elapsedUserTime (double &ut)
{
    if (getrusage (RUSAGE_SELF, &this->new_us_time) == -1)
        return -1;
    
    ut = (this->new_us_time.ru_utime.tv_sec - this->old_us_time.ru_utime.tv_sec)
    + ((this->new_us_time.ru_utime.tv_usec
        - this->old_us_time.ru_utime.tv_usec) / 1000000.0);
    return 0;
}

int
Timer::elapsedSystemTime (double &st)
{
    if (getrusage (RUSAGE_SELF, &this->new_us_time) == -1)
        return -1;
    
    st = (this->new_us_time.ru_stime.tv_sec - this->old_us_time.ru_stime.tv_sec)
    + ((this->new_us_time.ru_stime.tv_usec
        - this->old_us_time.ru_stime.tv_usec) / 1000000.0);
    return 0;
}

int
Timer::elapsedTime (double &wallclock, double &user_time, double &system_time)
{
    if (this->elapsedWallclockTime (wallclock) == -1)
        return -1;
    else
    {
        if (getrusage (RUSAGE_SELF, &this->new_us_time) == -1)
            return -1;
        user_time = (this->new_us_time.ru_utime.tv_sec
                     - this->old_us_time.ru_utime.tv_sec)
        + ((this->new_us_time.ru_utime.tv_usec
            - this->old_us_time.ru_utime.tv_usec) / 1000000.0);
        
        system_time = (this->new_us_time.ru_stime.tv_sec
                       - this->old_us_time.ru_stime.tv_sec)
        + ((this->new_us_time.ru_stime.tv_usec
            - this->old_us_time.ru_stime.tv_usec) / 1000000.0);
        
        return 0;
    }
}



void cp(const char *srcpath,const char *dstpath,int tflag){
    Timer t;
    double eTime;
    DIR *dir;
    struct dirent *dstdir;
    char srcname[1000];
    //std::istream::get and std::ostream::put
    char ch;
    //read and write
    char buffer[BUFSIZ];
    char chbuff[1];
    long bytes_read;
    long bytes_write;
    
    
    char temp[1000];
    char temppath[1000];
    struct stat s;
    char _srcpath[256];
    strcpy(_srcpath, srcpath);
    printf("%s\n",_srcpath);
    char *p = strrchr(_srcpath, '/');
    if(p == NULL){
        printf("pointer empty");
        p = _srcpath;
    }
    else{
       p++;
    }
    if(stat(srcpath,&s)==-1)
    {
        perror("error in stat:");
        return;
    }
    
    strcpy(srcname,p);
    
    
    
    if(NULL==(dir=opendir(dstpath)))
    {
        perror("error in opendir");
        return;
    }
    if(NULL==(dstdir=readdir(dir)))
    {
        perror("error in readdir");
    }
    while ((dstdir=readdir(dir))!=NULL)
    {
        if(dstdir->d_name[0]=='.')
            continue;
        if (0 == strcmp(dstdir->d_name,srcname)){
            cout<<"ERROR:Exist file"<<endl;
            exit(0);
        }
    }
    strcpy(temp,(char*)"/");
    strcat(temp,srcname);
    strcpy(temppath,dstpath);
    strcat(temppath,temp);
    
    //  c++ style open file
    ofstream outfile (temppath,ios::binary);
    ifstream infile (srcpath,ios::binary);
    
    //read and write
    int srcfp;
    if (-1==(srcfp=open(srcpath,O_RDONLY))){
        perror("error in open src file");
    }
    int dstfp;
    if (-1==(dstfp=open(temppath,O_WRONLY|O_CREAT))){
        perror("error in open dst file");
    }
    
    
    
    if (tflag == 0){
        
        
        // read and write with buff
        if(srcfp)
        {
            while(BUFSIZ-1<(bytes_read=(read(srcfp,buffer,BUFSIZ)))){
                if (bytes_read == -1){
                    perror("error in read");
                    break;
                }
                else if (bytes_read > 0){
                    while (-1!=(bytes_write=(write(dstfp,buffer,BUFSIZ)))){
                        if(bytes_write == -1){
                            perror("error in write");
                        }
                        if(bytes_read == bytes_write)
                            break;
                    }
                }
            }
            bytes_write=write(dstfp,buffer,bytes_read);
            if(bytes_write == -1){
                perror("error in write");
            }
        }
    }
    else if (tflag == 1 ){
        //time
        long seconds = time((time_t*)NULL);
        struct tm *clk;
        clk=localtime(&seconds);
        switch (clk->tm_mon+1)
        {
            case 1 :cout<<"Jan";break;
            case 2 :cout<<"Feb";break;
            case 3 :cout<<"Mar";break;
            case 4 :cout<<"Apr";break;
            case 5 :cout<<"May";break;
            case 6 :cout<<"Jun";break;
            case 7 :cout<<"Jul";break;
            case 8 :cout<<"Aug";break;
            case 9 :cout<<"Sep";break;
            case 10:cout<<"Oct";break;
            case 11:cout<<"Nov";break;
            case 12:cout<<"Dec";break;
        }
        cout<<clk->tm_mday<<" ";
        cout<<clk->tm_hour<<":"<<clk->tm_min<<" "<<endl;

        
        
        
        //user
        struct passwd *pwd=NULL;
        if (NULL==getpwuid(getuid()))
        {
            perror("error in getpwuid");
        }
        else
            pwd = getpwuid(getuid());
        cout<<"User :"<<pwd->pw_name<<endl;
        
        
        // std::istream::get and std::ostream::put
        if (-1==(srcfp=open(srcpath,O_RDONLY))){
            perror("error in open src file");
        }
        if (-1==(dstfp=open(temppath,O_WRONLY|O_CREAT))){
            perror("error in open dst file");
        }
        t.start();
        while(infile.get(ch)){
            outfile.put(ch);
        }
        t.elapsedWallclockTime(eTime);
        cout <<"Wallclock time"<< eTime << endl;
        t.elapsedUserTime(eTime);
        cout <<"user time"<< eTime << endl;
        t.elapsedSystemTime(eTime);
        cout <<"System time"<< eTime << endl<<endl;
        remove( temppath );

        // read and write with buff
        t.start();
        if(srcfp)
        {
            while(BUFSIZ-1<(bytes_read=(read(srcfp,buffer,BUFSIZ)))){
                if (bytes_read == -1){
                    perror("error in read");
                    break;
                }
                else if (bytes_read > 0){
                    while (-1!=(bytes_write=(write(dstfp,buffer,BUFSIZ)))){
                        if(bytes_write == -1){
                            perror("error in write");
                        }
                        if(bytes_read == bytes_write)
                            break;
                    }
                }
            }
            bytes_write = write(dstfp,buffer,bytes_read);
            if(bytes_write == -1){
                perror("error in write");
            }
        }
        t.elapsedWallclockTime(eTime);
        cout <<"Wallclock time"<< eTime << endl;
        t.elapsedUserTime(eTime);
        cout <<"user time"<< eTime << endl;
        t.elapsedSystemTime(eTime);
        cout <<"System time"<< eTime << endl<<endl;
        remove( temppath );

        
        //read and write in char
        t.start();
        if (-1==(srcfp=open(srcpath,O_RDONLY))){
            perror("error in open src file");
        }
        if (-1==(dstfp=open(temppath,O_WRONLY|O_CREAT))){
            perror("error in open dst file");
        }
        if(srcfp)
        {
            while(0!=(bytes_read=read(srcfp,chbuff,1))){
                if (bytes_read == -1){
                    perror("error in read");
                    break;
                }
                bytes_read=write(dstfp,chbuff,1);
                if(bytes_write == -1){
                    perror("error in write");
                }
            }
        }
        
        t.elapsedWallclockTime(eTime);
        cout <<"Wallclock time"<< eTime << endl;
        t.elapsedUserTime(eTime);
        cout <<"user time"<< eTime << endl;
        t.elapsedSystemTime(eTime);
        cout <<"System time"<< eTime << endl<<endl;
    }
    if (close(srcfp)==-1){
        perror(" error in close src");
    }
    if (close(dstfp)==-1){
        perror(" error in close dst");
    }
    return;
}

int main(int argc,const char *argv[]){
    //const char *argv[3]={"/Users/Elvis/Documents/UCR\ /CS\ 100/lab/lab5-cp/cp.cpp","/Users/Elvis/Desktop/cp","-t"};
    //int argc=3;
    /* lasting time*/
 
    int tflag=0;
    if (argc < 3){
        cout<<"not enough input arguments"<<endl;
        cout<<"cp: illegal option -- 2"<<endl;
        cout<<"usage: cp [filename1] [filedir2] [-r]"<<endl;
        return -1;
    }
    else if (argc == 4){
        if (argv[3][0]=='-' && strlen(argv[3])<3){
            if (strstr(argv[3],"t")!=NULL)
                tflag=1;
            else{
                cout<<"cp: illegal option -- 2"<<endl;
                cout<<"usage: cp [filename1] [filedir2] [-r]"<<endl;
                return -1;
            }
            cp(argv[1],argv[2],tflag);
        }
    }
    else if (argc==3)
        cp(argv[1],argv[2],tflag);
    return 0;
    
}
