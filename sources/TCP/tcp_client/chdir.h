#ifndef CHDIR
#define CHDIR

int ch_dir(char *buffer, int sock, int buf_size){
    bzero(buffer, buf_size);
    strcpy(buffer, "ch_dir\r\n");
    send(sock, buffer, buf_size, 0);

    printf("путь: ");
    bzero(buffer, buf_size);
    scanf("%s", buffer);
    send(sock, buffer, buf_size, 0);

    //bzero(buffer, buf_size);
    //recv(sock, buffer, buf_size, 0);
   // printf("%s\n", buffer);
    return 0;
}

#endif // CHDIR

