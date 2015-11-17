#ifndef DOWNLOAD
#define DOWNLOAD
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int download(char *buffer, size_t size, int sock){
    int r;
    int p;
    int i = 0;
    int j = 0;
    int k = 0;
    int fileSize = 0;
    char send_buf[size], name[size];

    // прием имени файла
    bzero(buffer, size);
    recv(sock, buffer, size, 0);
    // формирование пути
    bzero(name, size);
    getcwd(name, size);
    strcat(name, "/");
    strcat(name, buffer);
    printf("%s\n", name);

    //прием и формирование размера файла
    bzero(buffer, size);
    recv(sock, buffer, size, 0);
    while(buffer[i] != '\0'){
          i++;
          k++;
     }
     for(j=0; j<k; j++){
         p = pow(10,i-1);
         fileSize+=p*((int)buffer[j]-48);
         i--;
     }
    //printf("size string: %s\n", buffer);
    printf("SIze:%d\n", fileSize);

    //формирование пути с именем файла

   FILE* f;
   f = fopen(name, "wb");
   if(f == NULL){
       printf("File dont open\n");
       return -1;
   }

    /*while(true){
        f = fopen("/home/user/server/server_work/456.txt", "a");
        //f = fopen(name, "a");

        //прием
        r = recv(sock, buffer, size, 0);
        if(r <= 0){
            printf("Recv error\n");
            bzero(send_buf, size);
            sprintf(send_buf, "%s", "Rcv error");
            send(sock, send_buf, size, 0);
            //return false;
            break;
        }
        else{
            bzero(send_buf, size);
            sprintf(send_buf, "%d", r);
            send(sock, send_buf, size, 0);
        }

        //запись
        fwrite(buffer, 1, r, f);
        fclose(f);
    }*/

   while(fileSize > 0){
       if(fileSize >=1024){
           bzero(buffer, size);
           r = recv(sock, buffer, size, 0);
           fwrite(buffer, 1, size, f); // записываем весь буфер

           // отправка клиенту
           bzero(send_buf, size);
           sprintf(send_buf, "%d",r );
           send(sock, send_buf, size, 0);
       }
       else{
           bzero(buffer, size);
           r = recv(sock, buffer, size, 0);
           fwrite(buffer, 1, fileSize, f); //записываем только сколько надо

           // отправка клиенту
           bzero(send_buf, size);
           sprintf(send_buf, "%d",r );
           send(sock, send_buf, size, 0);
       }
       fileSize -=1024;
   }
   fclose(f);
   return 0;
}

#endif // DOWNLOAD

