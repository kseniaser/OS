//три процесса для сложения 2х матриц
//два процесса работают с матрицами, а оставшийся с символами
//при нажатии на "х" программа заканчивается

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

int *first;
int *second;
char *cmd;
int *result;

//функция, печатающая матрицу
void printing(int *ar) {
    for (int i = 0; i < 9; ++i) {
        //переносим строку если номер элемента кратен количеству столбцов
        if (i % 3 == 0) {
            printf("\n");
        }
        printf("%i ", ar[i]);
    }
    printf("\n");
}

int main() {

    //shmget создает новый сегмент памяти (с ключом 0,соотв размером,доступ для сервера)
    int shmem1 = shmget(0, 9 * sizeof(int), IPC_CREAT | 0666);
    int shmem2 = shmget(0, 9 * sizeof(int), IPC_CREAT | 0666);
    int shmem3 = shmget(0, 9 * sizeof(int), IPC_CREAT | 0666);
    int shmem4 = shmget(0, sizeof(char), IPC_CREAT | 0666);

    //shmat - включение области разделяемой памяти в адрессное пространство текущего процесса
    //(дескриптор,ОС сама размещает рп в аптп, чтение и запись над сегментом)
    first = (int *) shmat(shmem1, 0, 0);
    second = (int *) shmat(shmem2, 0, 0);
    result = (int *) shmat(shmem3, 0, 0);
    cmd = (char *) shmat(shmem4, 0, 0);

    //сначала все процессы неработают
    pid_t crt = -1;
    pid_t sum = -1;
    pid_t prt = -1;

    crt = fork(); //запускаем процесс создания
    //если запущен create, запускаем sum
    if (crt > 0) {
        sum = fork();
        //если запущен sum, запускаем print
        if (sum > 0){
            prt = fork();
        }
    }
    *cmd = 'c'; //индекатор запуска create

    //бесконечный цикл
    while (1) {
        sleep(1);

        //обработка сигнала отключения
        if (crt > 0 && sum > 0 && prt > 0) {
            char p;
            scanf("%c", &p);
            if (p == 'x') {
                *cmd = 'x';
                return 0;
            }
        }

        //процесс create
        if (crt == 0) {
            if (*cmd == 'c') {
                for (int i = 0; i < 9; ++i) {
                    first[i] = (int) (random() % 9 - 0);
                    second[i] = (int) (random() % 9 - 0);
                }
                *cmd = 's'; //в штатном режиме переходим на следующий шаг
            }
        }

        //процесс sum
        if (sum == 0) {
            if (*cmd == 's') {
                for (int i = 0; i < 9; ++i) {
                    result[i]= first[i] + second[i];
                }
                *cmd = 'p'; //в штатном режиме переходим на следующий шаг
            }
        }

        //процесс print
        if (prt == 0) {
            if (*cmd == 'p') {
                printing(first);
                printf("+");
                printing(second);
                printf("=");
                printing(result);
                printf("\n\n");
                *cmd = 'c'; //в штатном режиме передаем права create и повторяем
            }
        }
    }
}


