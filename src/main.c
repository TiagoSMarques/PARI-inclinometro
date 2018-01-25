//Programa principal
//
#include "myf.h"
#include "prototypes.h"

int main (int main(int argc, char const *argv[])) {

    int fd,oldS;
    char port[30]="/dev/rfcomm1";

    fd=OpenPort(port, "\0");
    oldS=ChangeBaudRate(fd,B9600);

    //printf("fd: %d \n",fd);

    if (fd == -1) { printf("Error. Couldn't open port\n"); exit(1); };
    ReadPortUntilChar(fd);
    close(fd);
}
