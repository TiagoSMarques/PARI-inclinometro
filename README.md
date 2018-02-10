# PARI-inclinometro
ProjetoFinal PARI

Modo de instalação:

Fazer:  "git clone https://github.com/TiagoSMarques/PARI-inclinometro" para uma pasta onde se pretende guardar o projeto.
Criar pasta com o nome build (por exemplo) nesse diretório, e dentro dessa pasta executar o comando "cmake ..".

Para compilar o programa basta depois fazer "make" nessa mesma pasta e da mesma maneira para gerar a documentação executar o comando "make doc".

Antes de executar o programa é preciso saber o endereço MAC do dispositivo remoto, e
assim é neceesário compilar o ficheiro testBT.c (gcc -o simplescan testeBT.c -lbluetooth) e
executar o programa "simplescan" e assim determinar os endereços MAC dos dispositivos em redor.

Depois disto é também necessário emparelhar os dois dispositivos. Para isto
utiliza-se o programa RFCOMM disponível nos repositórios da Canonical, entre outros,
e executar o comando "sudo rfcomm connect 1 98:D3:37:00:89:66 1" com o respetivo MAC address.

Posto isto o programa está pronto a ser executado (ficheiro btcom na pasta build).

Tiago Simões Marques nº 73138
