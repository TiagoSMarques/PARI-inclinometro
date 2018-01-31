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
        // s_id=GtkMain();
        int n,shm_id;
        //float d1;
        struct SensInfoNum *FromShm;   //ponteiro generico para servir de link para a shared memory
        shm_id = GetSharedMem();
        if(shm_id == 1) return -1;

        //attach memory segment to get a pointer to it
        FromShm =(struct SensInfoNum *) shmat(shm_id, (void *) 0,0);
        if (FromShm == (struct SensInfoNum *) (-1)) {perror("shmat");exit(1);}

        FromShm->i=0;
        sleep(3);
        // data agora aponta para a área partilhada
        while(FromShm->i != 20){
            printf("Main:   %f %f %f %f %f %f   %d\n", FromShm->dist1, FromShm->dist2, FromShm->dist3, FromShm->dist4, FromShm->roll, FromShm->pitch, FromShm->i);
            sleep(1);
        }

        if (shmdt(FromShm)==1){perror("shmt");exit(1);}
        printf("terminado filho\n");
        //Allow elimination of shared memory
        if(s_id > 0) shmctl(s_id, IPC_RMID, NULL);
    }

    return 0;
}
