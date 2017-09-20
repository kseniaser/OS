//запуск процессов согласно графу
//подсчитать колличество процессов в графе и вывести

#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

//fd[1] файловый дескриптор с потоком стандартного вывода процесса
//fd[0] файловый дескриптор с потоком стандартного ввода процесса
int fd[2];
int count = 0; //число процессов
int graph[][2] = {
        {0, 1},
        {0, 2},
        {1, 3},
        {1, 4},
        {2, 5},
        {3, 6},
        {3, 7},
        {4, 8},
        {5, 9}
};

//обходит граф, запуская ото всюду процессы
void dfs(int vertex, int graphlength){
    pid_t position=-1; //pid_t целое число со знаком,  предоставляет ID процесса
    for (int i=0; i<graphlength; i++){
        if (graph[i][0] == vertex){
            printf("New process from vertex %i to %i\n",vertex,graph[i][1]);
            position = fork(); //запускаем процесс из каждой вершины
            wait(NULL); //родительский процесс ждет дочерние
            dfs(graph[i][1],graphlength);

        }
    }
    read(fd[0], &count, sizeof(int)); //читаем из трубы
    count++; // увеличиваем число, идущее в ответ
    write(fd[1], &count, sizeof(int)); //пишем в трубу
}

int main()
{
    pipe(fd); //Создадим неименованный однонапрвленный канал
    write(fd[1], &count, sizeof(int));
    dfs(0,sizeof(graph)/(2*sizeof(int))); //запускаем с первой вершины
    read(fd[0], &count, sizeof(int)); //читаем ответ
    printf("Vertices-process count %i\n", count); //выводим ответ
    kill(pipe(fd),SIGTERM); //убиваем оставшиеся процессы
}