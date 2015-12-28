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
    long int fileSize = 0;
    char send_buf[size], name[size];

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
    printf("SIze:%d\n", fileSize);


    // прием имени файла
    bzero(buffer, size);
    recv(sock, buffer, size, 0);
    // формирование пути
    bzero(name, size);
    getcwd(name, size);
    strcat(name, "/");
    strcat(name, buffer);
    printf("%s\n", name);

    //формирование пути с именем файла

   FILE* f;
   f = fopen(name, "wb");
   if(f == NULL){
       printf("File dont open\n");
       return -1;
   }

    while(fileSize > 0){
       if(fileSize >=size){
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
       fileSize -=size;
   }

  /*while(fileSize > 0){
          if(fileSize >=fbuf_size){
              bzero(f_buf, fbuf_size);
              r = recv(sock, f_buf, fbuf_size, 0);
              fwrite(f_buf, 1, size, f); // записываем весь буфер

              // отправка клиенту
              bzero(send_buf, size);
              sprintf(send_buf, "%d",r );
              send(sock, send_buf, size, 0);
          }
          else{
              bzero(f_buf, fbuf_size);
              r = recv(sock, f_buf, fbuf_size, 0);
              fwrite(f_buf, 1, fileSize, f); //записываем только сколько надо

              // отправка клиенту
              bzero(send_buf, size);
              sprintf(send_buf, "%d",r );
              send(sock, send_buf, size, 0);
          }
          fileSize -=fbuf_size;
          printf("now fileSize: %d\n", fileSize);
          printf("now r: %d\n", r);
          sleep(2);
      }*/
   printf("succes!\n");
   fclose(f);
   //free(f_buf);
   return 0;
}

#endif // DOWNLOAD

