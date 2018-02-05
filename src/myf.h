
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
#include <memory.h>

//para a interface gtk
#include <gtk/gtk.h>
#include <string.h>
GtkBuilder * builderG;

gint ContRead;
gint BtEnd;

int count_parentG;
int count_childG;
int pid_child;
int parent_pid;

#define SHM_SIZE 5120      //5Mb

struct SensInfo {

char *dist1;
char *dist2;
char *dist3;
char *dist4;

char *roll;
char *pitch;

int i;
};

struct SensInfoNum {

float dist1;
float dist2;
float dist3;
float dist4;

float roll;
float pitch;

int i;
int fd;
int BtWrite;
char BtText[20];

};
