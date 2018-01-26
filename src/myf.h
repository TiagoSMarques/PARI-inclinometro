
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

//#include <cv.h>
//#include <highgui.h>

//para a shared memory
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include <unistd.h> // for close
#include <fcntl.h>  //for open
#include <termios.h>

//para a shared memory
int count_parentG;
int count_childG;
int pid_child;
int parent_pid;

#define SHM_SIZE 51200      //50Mb
