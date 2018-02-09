/**
 * @file testeBT.c
 * @brief Função para fazer um scan dos aparelho bluetooth nas proximidades
 *e imprimir no terminal os respetivos endereços MAC e o nome comum associado ao aparelho.

 *Código baseado no exemplo mostrado em  https://people.csail.mit.edu/albert/bluez-intro/c404.html
 * Mais informações sobre o projeto em: https://github.com/TiagoSMarques/PARI-inclinometro
 *
 * @author Tiago Marques, marques.tiago19@ua.pt
 * @internal
     Created 28-Jan-2018
 * Revisions 06-Feb-2018
 *  =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main(int argc, char **argv)
{
    //inicialização de variáveis
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = { 0 };          //alocar espaço para MAC adress do aparelho remoto
    char name[248] = { 0 };         //alocar memoria para nome do aparelho

    //identificar o adaptador de bluetooth local
    //no caso deste pc só ha um adaptador por isso NULL significa primero a se encontrado
    dev_id = hci_get_route(NULL);
    //depois de identificado abrir uma socket de cominicação nesse device
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }


    len  = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;  //limpa a cache de devices encontrados anteriormente
    //alocar recursos para a estrutura de dados que irá guardar as informações da procura
    //de dispositivos efetuada (inquiry_info)
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info)); //lista de devices detetados

    //efetua a procura (durante 1.28*len segundos e pode encontrar no max max_resp devices)
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");

    for (i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr); //transforma o endereço bdaddr_t numa string
        memset(name, 0, sizeof(name));
        //se tiver nome imprime o endereço e o nome
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name),
            name, 0) < 0)
        strcpy(name, "[unknown]");
        printf("%s  %s\n", addr, name);
    }

    free( ii );         //liberta a memoria alocada
    close( sock );      //fecha a socket de comunicação
    return 0;
}
