#ifndef view_dir_DIR_H
#define view_dir_DIR_H
#include <string.h>
#include <math.h>

int view_dir(char* buffer, int sock, int size){
    memset(buffer, 0, size);
    strcpy(buffer, "view_dir\r\n");
    send(sock, buffer, size, 0);
    while(1){
        int depth;
        int i;
        int j;
        int k;
        memset(buffer, 0, size);
        recv(sock, buffer, size,0);
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
           // ������� ���������� ������ �����
           while(buffer[i] != '|'){
                 i++;
                 k++;
            }
            for(j=0; j<k; j++){
                p = pow(10.0,i-1);
                depth+=p*((int)buffer[j]-48);
                i--;
            }
            //---------------------------------------------------
            k++; // ���� '|'
            if(buffer[k] == 'f')
                p = 0;
            else
                p = 1;
            //----------------------------------------------------
            k++; // ���� '|'
            k++;
            // ������ ��������
            for(i=0; i<depth; i++)
                printf(" ");
            // ������ ��������
            while(buffer[k] != '\0'){
                printf("%c", buffer[k]);
                k++;
            }
            // ������ ������� ����� � ������� ������
            if(p == 1)
                printf("/\n");
            else
                printf("\n");
        }
 }
    return 0;
}

#endif // view_dir_dir_DIR

