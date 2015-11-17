#ifndef HELP
#define HELP
#include <string.h>

int get_help(){
    printf("ch_dir - сменить текущую директорию на сервере\n");
    printf("mk_dir - создать папку в текущей директории\n");
    //printf("download <имя файла> - загрузить файл с сервера\n");
    printf("upload - загрузить файл на сервер из рабочей директории\n");
    printf("view - просмотр дерева каталогов сервера\n");
    //printf("connect <IP> <номер порта> - подключиться к серверу \n");
    printf("disconnect - отключиться от сервера\n");
    return 0;
}

#endif // HELP

