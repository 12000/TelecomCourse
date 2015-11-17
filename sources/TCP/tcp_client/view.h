#ifndef VIEW_H
#define VIEW_H
#include <string.h>
#include <math.h>

int view(char* buffer, int sock, int buf_size){
     printf("Рабочий каталог: server_work/\n");
    bzero( buffer, buf_size);
    strcpy(buffer, "view\r\n");
    send(sock, buffer, buf_size, 0);

    while(1){
        int depth;
        int i;
        int j;
        int k;
        bzero(buffer, buf_size);
        recv(sock, buffer, buf_size,0);
        if(strcmp(buffer, "0|d|end")==0){
            printf("end\n");
            break;
        }
        else{
            i = 0;
            j = 0;
            k = 0;
            int p=0;
           depth = 0;
           // подсчет количества знаков числа
           while(buffer[i] != '|'){
                 i++;
                 k++;
            }
            for(j=0; j<k; j++){
                p = pow(10,i-1);
                depth+=p*((int)buffer[j]-48);
                i--;
            }
            //---------------------------------------------------
            k++; // знак '|'
            if(buffer[k] == 'f')
                p = 0;
            else
                p = 1;
            //----------------------------------------------------
            k++; // знак '|'
            k++;
            // печать пробелов
            for(i=0; i<depth; i++)
                printf(" ");
            // печать названия
            while(buffer[k] != '\0'){
                printf("%c", buffer[k]);
                k++;
            }
            // печать символа папки и перевод строки
            if(p == 1)
                printf("/\n");
            else
                printf("\n");
        }
 }
    return 0;
}

#endif // VIEW_H

