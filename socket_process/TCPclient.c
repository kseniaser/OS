//сперва запускаем клиента затем сервер
//после окончания сеанса консоли закрыть

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
    int clientSocket,nBytes; // дескрипторы сокетов
    char buffer[1024]; // буфeр для приема
    struct sockaddr_in serverAddr; // структура с адресом

    //cоздаем сокет Домен сокета, тип сокета,протокол передачи данных
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;//соотв интернет домену(семейство адрессов)
    //htons целое короткое число из порядка байт,принятого на компьютере, в сетевой порядок байт
    //port tc 7891 исполльзует протокол управения передачей данных который является одним из основных в сетях TCP/IP
    serverAddr.sin_port = htons(7891);//номер порта на узле сети
    //локал хост 127.0.0.1 //обращаемся по сети сами к себе
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//адресс узла сети
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero); //заполняем нулями
    connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof serverAddr); //конектимся к серверу

    //бесконечный цикл работы
    while(1){
        printf("Type a sentence to send to server:\n");
        fgets(buffer,1024,stdin); //читаем ввод
        printf("You typed: %s",buffer); //наглядно выводим  его
        nBytes = strlen(buffer) + 1; //количество передаваемых байтов выделяем
        send(clientSocket,buffer,nBytes,0); //отправляем в сокет
        recv(clientSocket, buffer, 1024, 0); //читаем с обновленного сокета
        printf("Received from server: %s\n\n",buffer); //выводим, обработанный сервером, текст
    }
}
