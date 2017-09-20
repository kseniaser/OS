#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main(){
    int welcomeSocket, newSocket, clientLen, nBytes; // дескрипторы сокетов
    char buffer[1024]; // буфeр для приема
    struct sockaddr_in serverAddr; // структура с адресом
    struct sockaddr_storage serverStorage; //информация о входящем соединении
    socklen_t addr_size;

    //cоздаем сокет IP версии 4, надежный двустороний обмен потоками байтов
    // (использование протокола транспортного уровня TCP, 0 - номер протокола в рамках семейства
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET; //address family - Internet
    serverAddr.sin_port = htons(7891);//номер порта на узле сети
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //локал хост 127.0.0.1
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)); //связывает сокет с адрессом

    //можем принимать 5 соединений одновременно
    if(listen(welcomeSocket,5)==0) {
        printf("Listening\n");
    } else {
        printf("Error\n");
    }

    addr_size = sizeof serverStorage;

    //чтобы продолжить принимать запросы от клиентов создаем вечный цикл
    while(1){
        newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
        //создаем для каждого запроса дочерний процесс
        if(!fork()){
            nBytes = 1;
            //пока дочерний процесс жив
            while(nBytes!=0){
                //получаем сообщение из сокета
                nBytes = recv(newSocket,buffer,1024,0);
                for (int i=0;i<nBytes-1;i++){
                    buffer[i] = toupper(buffer[i]); //повышаем регистр
                }
                send(newSocket,buffer,nBytes,0); //передаем данные в сокет
            }
            close(newSocket); //закрываем сокет
            exit(0);
        }
        //если родительский процесс, то закрываем и идем дальше ждать запроса
        else{
            close(newSocket);
        }
    }
}

