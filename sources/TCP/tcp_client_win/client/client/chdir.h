#ifndef CHDIR
#define CHDIR
#include <string.h>

int ch_dir(char *buffer, int sock, int size){
	memset(buffer, 0, size);
    strcpy(buffer, "ch_dir\r\n");
    send(sock, buffer, size, 0);

    printf("����: ");
    memset(buffer, 0, size);
    scanf("%s", buffer);
    send(sock, buffer, size, 0);

    //����� ������ ���� ��������� �� ������
    memset(buffer, 0, size);
    recv(sock, buffer, size, 0);
    if(strcmp(buffer, "succes") != 0)
       printf("�������� �������!");

    memset(buffer, 0, size);
    recv(sock, buffer, size, 0);
    printf("������� �������: %s\n", buffer);
    return 0;
}

#endif // CHDIR

