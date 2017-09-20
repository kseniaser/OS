//три потока для сложения 2х матриц
//при нажатии на "х" программа заканчивается

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

static int first[9];
static int second[9];
static int result[9];
char p;
static char cmd = 'c'; // сигнал переключения потоков
static pthread_mutex_t mutex; //создаем семафор

//функция создания матриц
void *crt(void *p) {
    //бесконечный цикл
    while (1) {
        if (cmd == 'c') {
            pthread_mutex_lock(&mutex); //заходим в семафор и закрываем доступ
            //создаем две матрицы
            for (int i = 0; i < 9; ++i) {
                first[i] = (int) (random() % 9 - 0);
                second[i] = (int) (random() % 9 - 0);
            }
            cmd = 's'; //в штатном режиме переходим на следующий шаг
            pthread_mutex_unlock(&mutex); //после работы открываем доступ и покидаем
            sleep(1);
        }
    }
}

//функция суммирования
void *sum(void *p) {
    while (1) {
        if (cmd == 's') {
            pthread_mutex_lock(&mutex);
            //записываем суммы в результирующую матрицуу
            for (int i = 0; i < 9; ++i) {
                result[i] = first[i] + second[i];
            }
            cmd = 'p'; //в штатном режиме переходим на следующий шаг
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }
    }
}

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

//функция для печати матриц
void *prt(void *p) {
    while (1) {
        if (cmd == 'p') {
            pthread_mutex_lock(&mutex);
            printing(first);
            printf("+");
            printing(second);
            printf("=");
            printing(result);
            printf("\n\n");
            cmd = 'c'; //в штатном режиме переходим на следующий шаг
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }
    }
}

int main() {
    pthread_mutex_init(&mutex, NULL);

    //создаем три потока без ничего
    pthread_t thread_1;
    pthread_t thread_2;
    pthread_t thread_3;

    //даем потокам задания, запускаем без аргументов
    pthread_create(&thread_1, NULL, &crt, NULL);
    pthread_create(&thread_2, NULL, &sum, NULL);
    pthread_create(&thread_3, NULL, &prt, NULL);

    //обработка команды отключения
    scanf("%c", &p);
    if (p == 'x') {
        return 0;
    }

    //ждем пока закончится поток 1, вечно
    pthread_join(thread_1, NULL);
    pthread_mutex_destroy(&mutex);
}