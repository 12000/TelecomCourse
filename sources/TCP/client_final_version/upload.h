#ifndef UPLOAD
#define UPLOAD
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int upload(char* buffer, int size, int sock){
    char name[1024];
	//int size = ssize;
	//char *f_buf;//[1024];
    long int fileSize;
    long int message_length;

	time_t start_time, finish_time;
    FILE* f;

	//f_buf = (char*) malloc(fbuf_size);

    printf("Путь: ");
    memset(name, 0, 1024);
    scanf("%s", name);

    f = fopen(name, "rb");
    if(f == NULL){
        printf("File dont open\n");
        return -1;
    }

    memset(buffer, 0, size);
    strcpy(buffer, "upload\r\n");
    send(sock, buffer, size, 0);
	//Sleep(1000);
	   

    //отправка размера
    fseek(f, 0, SEEK_END);
    fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);
    printf("Size of file: %d\n", fileSize);
    memset(buffer, 0, size);
    //strcpy(buffer, (char)fileSize);
    sprintf(buffer, "%d", fileSize);
    send(sock, buffer, size, 0);
	//Sleep(1000);


	// отправка на сервер имени файла
    printf("Имя на сервере: ");
    memset(buffer, 0, size);
    scanf("%s",buffer);
    send(sock, buffer, size, 0);
	//Sleep(1000);

	// запомнить время начала
	start_time = time(NULL);
    
	while(fileSize > 0){
        memset(buffer, 0, size);
        if(fileSize >= size){
            message_length = fread(buffer, size, 1, f); // количество

            if(message_length != 0){
                    send(sock, buffer, size, 0);
					Sleep(1);
			}
            memset(buffer, 0, size);
            recv(sock, buffer, size, 0);
            printf("Передача - было принято сервером: %s\n", buffer);
        }
        else{
            message_length = fread(buffer, fileSize, 1, f);

            if(message_length != 0){
                    send(sock, buffer, fileSize, 0);
					Sleep(1);
			}
            memset(buffer, 0, size);
            recv(sock, buffer, size, 0);
            printf("Передача - было принято сервером: %s\n", buffer);
        }
        fileSize -= size;
    }
	
	
	finish_time = time(NULL);
    fclose(f);
	printf("Время передачи: %f\n", difftime(finish_time, start_time));

    return 0;
}

#endif // UPLOAD

