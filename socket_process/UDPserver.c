#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main(){
    int udpSocket, nBytes; // дескрипторы сокетов
    char buffer[1024]; // буфeр для приема
    struct sockaddr_in serverAddr, clientAddr; // структура с адресом
    struct sockaddr_storage serverStorage;
    socklen_t addr_size, client_addr_size;

    //cоздаем UDP сокет
    // ненадежный обмен на основе передач датаграмм без установления соеденения
    udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

    //настраиваем его на локалхост
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)); //связываем сокет с адресом
    addr_size = sizeof serverStorage;   //инициализируем размер
    printf("Listening\n");

    //бесконечный цикл
    while(1){
        //пытаемся получить любую UDP-дайтаграму адрес и порт, сохраняем это в serverStorage
        nBytes = recvfrom(udpSocket,buffer,1024,0,(struct sockaddr *)&serverStorage, &addr_size);
        //повышаем регистр
        for(int i=0;i<nBytes-1;i++)
            buffer[i] = toupper(buffer[i]);
        //отправляем сообщение в верхнем регистре клиенту, используя serverStorage в качестве адреса
        sendto(udpSocket,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);
    }
}

