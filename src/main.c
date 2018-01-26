//Programa principal
//
#include "myf.h"
#include "prototypes.h"

int main (int main(int argc, char const *argv[])) {

    int fd,oldS,ret=0;
    int childpid;
    char port[30]="/dev/rfcomm1";
    char text[20]="22";

    fd=OpenPort(port, "\0");
    oldS=ChangeBaudRate(fd,B9600);
    if (fd == -1) { printf("Error. Couldn't open port\n"); exit(1); };

    childpid=fork();

    if (childpid == 0 ) {
        printf("this is the child \n");
        while (ret !=15 ) {
        fgets(text, sizeof(text), stdin);
        ret=WriteToBT(fd, text);
        }
        //return 0;
    }

    else {
        printf("this is the parent\n");

        sleep(2); //required to make flush work, for some reason
        tcflush(fd,TCIOFLUSH);  //limpar o buffer de comunicações antigas

            ReadPortUntilChar(fd);
    }


    printf("Comunication ended!\n");
    close(fd);
    return 0;
}
