//копируем данные с одного файла в другой
//имя файлов не длиннее 100 символов

#include <stdio.h>
#include <stdlib.h>

int main()
{
    char ch, from_file[100], to_file[100];
    FILE *from, *to; //ссылки на 2 файла
    printf("Enter name of file to copy\n");
    gets(from_file); //считываем в from_file
    from = fopen(from_file, "r"); //открываем файл для чтения

    //случай если нет такого файла
    if( from == NULL )
    {
        printf("No such file\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter name of target file\n");
    gets(to_file);//считываем в to_file
    to = fopen(to_file, "w");//открываем файл для записи

    //случай если нет такого файла
    if( to == NULL )
    {
        fclose(from);
        printf("No such file\n");
        exit(EXIT_FAILURE);
    }

    //пока не конец файла записываем в to файл
    while( ( ch = fgetc(from) ) != EOF )
        fputc(ch, to);

    printf("File copied successfully\n");

    //закрываем файлы
    fclose(from);
    fclose(to);

    return 0;
}