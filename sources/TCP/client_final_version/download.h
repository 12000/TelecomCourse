#ifndef DOWNLOAD
#define DOWNLOAD
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define MAXBUF  1024

int download(char *buffer, size_t size, int sock){
    int r;
    int p;
    int i = 0;
    int j = 0;
    int k = 0;
    int fileSize = 0;
	time_t start_time, finish_time;
    char send_buf[MAXBUF], name[MAXBUF], trace[MAXBUF];

	//�������� �������
	memset(buffer, 0, size);
    strcpy(buffer, "download\r\n");
    send(sock, buffer, size, 0);

	// �������� �� ������ ����� �����
    printf("��� �� �������: ");
    memset(name, 0, size);
    scanf("%s",name);
    send(sock, name, size, 0);

	//�������� ������������� �������� �����
	memset(buffer, 0, size);
	recv(sock, buffer, size, 0);
	if(strcmp(buffer, "false") == 0){
		printf("Incorrect name - file dont open!\n");
		return -1;
	}

	// ��� ���������
	printf("���� (���� ���������): ");
    memset(trace, 0, size);
    scanf("%s",trace);
	printf("\n");
	//������������ ���� � ������ �����
	strcat(trace, "\\");
	strcat(trace, name);

	FILE* f;
	f = fopen(trace, "wb");
	if(f == NULL){
	    printf("File dont create\n");
	    return -1;
	}

    //����� � ������������ ������� �����
    memset(buffer, 0, size);
    recv(sock, buffer, size, 0);
    while(buffer[i] != '\0'){
          i++;
          k++;
     }
     for(j=0; j<k; j++){
         p = pow(10.0,i-1);
         fileSize+=p*((int)buffer[j]-48);
         i--;
     }
    //printf("size string: %s\n", buffer);
    printf("SIze:%d\n", fileSize);

   
	// ��������� ����� ������
	start_time = time(NULL);
   while(fileSize > 0){
       if(fileSize >=1024){
           memset(buffer, 0, size);
           r = recv(sock, buffer, size, 0);
           fwrite(buffer, 1, size, f); // ���������� ���� �����

           // ������
           memset(send_buf, 0, size);
           printf("������� %d\n",r );
       }
       else{
           memset(buffer,0, size);
           r = recv(sock, buffer, size, 0);
           fwrite(buffer, 1, fileSize, f); //���������� ������ ������� ����

           // �������� �������
           memset(send_buf,0, size);
           printf("������� %d\n",r );
       }
       fileSize -=1024;
   }
   // ��������� ����� ������e��� ��������
	finish_time = time(NULL);
   fclose(f);
   printf("����� ��������: %f\n", difftime(finish_time, start_time));
   return 0;
}

#endif // DOWNLOAD