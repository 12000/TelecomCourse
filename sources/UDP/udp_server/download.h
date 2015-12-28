#ifndef DOWNLOAD
#define DOWNLOAD
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int download(char *buffer, size_t size, int sock, struct sockaddr_in si_other, int slen){
    int r;
    int p;
    int i = 0;
    int j = 0;
    int k = 0;
    long int fileSize = 0;
    char send_buf[size], name[size];

    //прием и формирование размера файла
    bzero(buffer, size);
    //recv(sock, buffer, size, 0);
    recvfrom(sock, buffer, size, 0, (struct sockaddr *) &si_other, &slen);
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
    //recv(sock, buffer, size, 0);
    recvfrom(sock, buffer, size, 0, (struct sockaddr *) &si_other, &slen);

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
          // r = recv(sock, buffer, size, 0);
           r = recvfrom(sock, buffer, size, 0, (struct sockaddr *) &si_other, &slen);
           fwrite(buffer, 1, size, f); // записываем весь буфер

           // отправка клиенту
           bzero(send_buf, size);
           sprintf(send_buf, "%d",r );
           //send(sock, send_buf, size, 0);
           sendto(sock, send_buf, size, 0, (struct sockaddr*) &si_other, slen);
           sleep(1);
       }
       else{
           bzero(buffer, size);
          // r = recv(sock, buffer, size, 0);
           r = recvfrom(sock, buffer, MAXBUF, 0, (struct sockaddr *) &si_other, &slen);
           fwrite(buffer, 1, fileSize, f); //записываем только сколько надо

           // отправка клиенту
           bzero(send_buf, size);
           sprintf(send_buf, "%d",r );
           //send(sock, send_buf, size, 0);
           sendto(sock, send_buf, size, 0, (struct sockaddr*) &si_other, slen);
           sleep(1);
       }
       fileSize -=size;
   }

   printf("succes!\n");
   fclose(f);
   //free(f_buf);
   return 0;
}

#endif // DOWNLOAD
