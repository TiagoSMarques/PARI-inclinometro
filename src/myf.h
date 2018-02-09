
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <math.h>
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
int BtEnd;

int count_parentG;
int count_childG;
int pid_child;
int parent_pid;

#define SHM_SIZE 5120      //5Mb
/**@brief  Estrutura com as strings de dados relevantes*/
struct SensInfo {

/**@param  string com o valor da distancia do sensor 1*/
char *dist1;
/**@param  string com o valor da distancia do sensor 2*/
char *dist2;
/**@param  string com o valor da distancia do sensor 3*/
char *dist3;
/**@param  string com o valor da distancia do sensor 4*/
char *dist4;
  /**@param  string com o valor do ângulo de roll*/
char *roll;
 /**@param  string com o valor do ângulo de pitch*/
char *pitch;

/**@param inteiro que conta o número de ciclos decorridos*/
int i;
};

/**@brief  Estrutura com os dados dos sensores na forma de floats, e informações
adicionais relativas a ordens de envio de dados e a respetiva mensagem*/
struct SensInfoNum {

/**@param  float com o valor da distancia do sensor 1*/
float dist1;
/**@param  float com o valor da distancia do sensor 2*/
float dist2;
/**@param  float com o valor da distancia do sensor 3*/
float dist3;
/**@param  float com o valor da distancia do sensor 4*/
float dist4;
/**@param  float com o valor do ângulo de roll*/
float roll;
 /**@param float com o valor do ângulo de pitch*/
float pitch;

/**@param inteiro que conta o número de ciclos decorridos*/
int i;
/**@param inteiro com o valor da porta serie que foi aberta*/
int fd;
/**@param inteiro que representa a intenção de enviar mensagem pela porta serie*/
int BtWrite;
/**@param  string que se deseja enviar*/
char BtText[20];
};
