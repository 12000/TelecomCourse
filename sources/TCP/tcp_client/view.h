#ifndef VIEW_H
#define VIEW_H
#include <string.h>

int view(char* buffer, int sock, int buf_size){
    bzero( buffer, buf_size);
    strcpy(buffer, "view\r\n");
    send(sock, buffer, buf_size, 0);
    //int count = 0;
    //int depth;
    char d_f[buf_size];
    while(1){

        // Прием количества пробелов
        bzero(buffer, buf_size);
        recv(sock, buffer, buf_size,0);
        int depth = 0;
        int i = strlen(buffer);
        //printf("strlen=%d       ", i);
        int j = 0;
        for(i; i>0;i--){
            depth += pow(10, i-1)*((int)buffer[j]-48);
            j++;
        }
        // Печать пробелов
       // printf("%d",depth);
        int k=0;
        for(k;k<depth; k++)
            printf(" ");

        // Прием смвола папки/файла
        bzero(d_f, buf_size);
        recv(sock, d_f, buf_size,0);

        // Печать строки
        bzero(buffer, buf_size);
        recv(sock, buffer, buf_size,0);
        if(strcmp(buffer, "end")==0)
            break;
        else if(strcmp(d_f, "dir"))
            printf("%s/\n",buffer);
        else if(strcmp(d_f, "file"))
            printf("%s\n",buffer);
 }
    return 0;
}

#endif // VIEW_H

