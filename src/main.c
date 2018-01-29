//Programa principal
//
#include "myf.h"
#include "prototypes.h"

int main (int main(int argc, char const *argv[])) {

    int pid_trans;
    int pid_main;
    int s_id;

    pid_main=fork();
    if(pid_main == -1) { printf("Could not fork(). Exiting\n"); return -1; }

    if (pid_main!=0){       //parte da transmissão de dados

        int fd,oldS,ret=0;

        char port[30]="/dev/rfcomm1";
        char text[20]="";

        fd=OpenPort(port, "");
        oldS=ChangeBaudRate(fd,B9600);
        if (fd == -1) { printf("Error. Couldn't open port\n"); exit(1); };

        pid_trans=fork();
        if(pid_trans == -1) { printf("Could not fork(). Exiting\n"); return -1; }

        if (pid_trans == 0 ) {
            //printf("this is the child \n");
            while (ret != 15 ) {
            fgets(text, sizeof(text), stdin);
            ret=WriteToBT(fd, text);
            }
        }
        else {
            //printf("this is the parent\n");
            sleep(2); //required to make flush work, for some reason
            tcflush(fd,TCIOFLUSH);  //limpar o buffer de comunicações antigas
            ReadPortUntilChar(fd);
        }

        printf("Comunication ended!\n");
        close(fd);
    }
    else {              //parte da aplicação gtk
        printf("Filho principal\n");
        int term=0;
        s_id=GtkMain();
        while(term==0){
            sleep(10);
            term=1;
        }
        printf("terminado filho\n");
        //Allow elimination of shared memory
        if(s_id > 0) shmctl(s_id, IPC_RMID, NULL);
    }

    return 0;
}
