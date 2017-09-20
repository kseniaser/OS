//запуск потоков согласно графу
//подсчитать колличество запущенных потоков и вывести число

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <memory.h>
#include <fcntl.h>
#include <pthread.h>

int graph[][2] = {
        {0, 1},
        {0, 2},
        {1, 3},
        {1, 4},
        {2, 5},
        {4, 6},
        {4, 7},
        {5, 8},
        {6, 9}
};

//аргументы для потоков из дфс
struct dfs_struct {
    int* count;
    int vertex;
    int length;
};

//так же, как  и в ProcessCounter, только вместо процессов потоки
void* dfs(void *arguments){
    struct dfs_struct *args = arguments;
    int k = 0,threads = 0;
    int vertex = args -> vertex;
    for (int i=0; i < args->length; i++){
        if (graph[i][0] == vertex){
            threads++;
        }
    }

    //идентификатор потока
    pthread_t newthread[threads];
    pthread_t* pnt = &newthread[0];

    //рекурсивно передаем потоку задачу запустить потоки дальше
    for (int i=0;i < args->length; i++){
        if (graph[i][0] == vertex){
            (*(args->count))++;
            printf("New thread from %i to %i\n",args->vertex,graph[i][1]);

            //создаем поток по идентификатору pnt+k и функции потока dfs
            //и передаем потоку указатель на данные (void*) (args+graph[i][1]-vertex)
            pthread_create(pnt+k, NULL, dfs, (void*) (args + graph[i][1] - vertex));

            pthread_join(*(pnt+k),NULL); //подключаемся и ждем завершения потока
            k++;
        }
    }

    pthread_exit(NULL); //выходим из потока, чтобы остался в конце 1
}


int main() {

    //выделяем общую память, для работы с графом
    int shmid = shmget(0, sizeof(int), IPC_CREAT | 0666);
    int* count = (int*) shmat(shmid, NULL, 0);
    int length=sizeof(graph)/(2*sizeof(int));
    shmid = shmget(0, (length+2)*sizeof(struct dfs_struct), IPC_CREAT | 0666);
    struct dfs_struct* arguments = (struct dfs_struct*) shmat(shmid,NULL,0);
    pthread_t zerothread;
    *count = 1;

    //cоздаем аргументы для каждого потока
    for(int i=0;i<length+1;i++){
        (arguments+i)->count = count;
        (arguments+i)->vertex = i;
        (arguments+i)->length = length;
    }

    pthread_create(&zerothread, NULL, dfs, (void*) arguments);
    pthread_join(zerothread,NULL); //когда останется нулевой потока, то напечатаем ответ
    printf("Count = %i\n",*count);
    pthread_exit(NULL);
}

