// Пример простого TCP клиента
#define WIN32_LEAN_AND_MEAN  
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <locale.h> 
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "view.h"
#include "help.h"
#include "get_dir.h"
#include "view_dir.h"
#include "chdir.h"
#include "upload.h"
#include "rm_dir.h"
#include "mk_dir.h"
#include "echo.h"
#include "download.h"

#define PORT 1234
//#define SERVERADDR "192.168.1.43"
//#define SERVERADDR "10.1.99.25"
#define SERVERADDR "10.1.99.119"
#define MAXBUF  1024

  int main()
  {
	//char* SERVERADDR;
	//SERVERADDR = (char*)malloc(15);
    setlocale(LC_ALL, "Russian");
	char buffer[MAXBUF];
	
	memset(buffer, 0, MAXBUF);
	/*while(1){
		printf("Cmd: ");
		scanf("%s", buffer);
		if(strcmp("connect", buffer) == 0){
			printf("\nip: ");
			scanf("%s", SERVERADDR);
			break;
		}
		else{
			printf("Waiting to connect command!\n");
			continue;
		}
	}*/

    // Шаг 1 - иницилизация библиотеки Winsocks
    if (WSAStartup(0x202,(WSADATA *)&buffer[0]))
    {
      printf("WSAStartup error: %d\n",
             WSAGetLastError());
      return -1;
    }

    // Шаг 2 - открытие сокета
    SOCKET sock=socket(AF_INET, SOCK_DGRAM, 0);
    if (sock==INVALID_SOCKET)
    {
      printf("socket() error: %d\n",WSAGetLastError());
      WSACleanup();
      return -1;
    }

    // Шаг 3 - обмен сообщений с сервером
    HOSTENT *hst;
    sockaddr_in dest_addr;
	int slen = sizeof(dest_addr);

    dest_addr.sin_family=AF_INET;
    dest_addr.sin_port=htons(PORT);

    // определение IP-адреса узла
    if (inet_addr(SERVERADDR))
      dest_addr.sin_addr.s_addr=inet_addr(SERVERADDR);
    else
      if (hst=gethostbyname(SERVERADDR))
        dest_addr.sin_addr.s_addr=((unsigned long **)
              hst->h_addr_list)[0][0];
    else
      {
        printf("Unknown host: %d\n",WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return -1;
      }
	//----------------------------------------------------
	 //первая отправка
	  printf("Идет первая отправка...\n");
	  sendto(sock, "123", 3, 0, (struct sockaddr*) &dest_addr, slen);
	  //прием порта
	  memset(buffer, 0, MAXBUF);
	  int r;
	  if( (r=recvfrom(sock, buffer, MAXBUF, 0, (struct sockaddr *) &dest_addr, &slen)) != -1 ){
		printf("Порт принят!\n");
	  }
	//закрытие старого сокета
	closesocket(sock);
    WSACleanup();
	printf("Закрыт старый сокет\n");

	int newport = atoi(buffer);
	printf("Установлен новый порт\n");

	WSADATA wsa2;
	if (WSAStartup(MAKEWORD(2, 2), &wsa2) != 0) {
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in new_si_other;
	int sockfd, new_slen = sizeof(new_si_other);

	//новый сокет
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
		exit(EXIT_FAILURE);
	}
	else
		printf("Создан новый сокет\n");

	//установка параметров
	memset((char *)&new_si_other, 0, new_slen);
	new_si_other.sin_family = AF_INET;
	new_si_other.sin_port = htons(newport);
	new_si_other.sin_addr.S_un.S_addr = inet_addr(SERVERADDR);
	printf("Установлены параметры\n");

	//новая отправка
	printf("Новая отправка\n");
	if((r=sendto(sockfd, "345", 3, 0, (struct sockaddr *) &new_si_other, new_slen)) != -1)
		printf("Успешная новая отправка\n");
	else
		printf("Новая отправка не удалась\n");

	// ожилание приема сообщение о работе диспетчера
	memset(buffer, 0, MAXBUF);
	if ((r=recvfrom(sockfd, buffer, MAXBUF, 0, (struct sockaddr *) &new_si_other, &new_slen)) != -1){
		printf("Диспетчер работает\n");
		printf("%s", buffer);
	}
	else
		printf("Диспетчер не работает\n");
	printf("%s", buffer);
	//----------------------------------------------------
    printf("Для получения справки введите команду help\n");
    while(1){
		memset(buffer, 0, MAXBUF);
        printf("Cmd: ");
        scanf("%s" , buffer);
        // команды
        if(strcmp(buffer, "view") == 0){
			view(buffer, sockfd, MAXBUF, new_si_other, new_slen);
		}
        else if(strcmp(buffer, "view_dir") == 0)
            view_dir(buffer, sockfd, MAXBUF, new_si_other, new_slen);
        else if(strcmp(buffer, "help") == 0)
            get_help();
        else if(strcmp(buffer, "get_dir") == 0)
            get_dir(buffer, sockfd, MAXBUF, new_si_other, new_slen);
        else if(strcmp(buffer, "ch_dir") == 0)
            ch_dir(buffer, sockfd, MAXBUF, dest_addr, new_slen);
        else if(strcmp(buffer, "upload") == 0)
            upload(buffer, MAXBUF, sockfd, new_si_other, new_slen);
		else if(strcmp(buffer, "download") == 0)
            download(buffer, MAXBUF, sockfd, new_si_other, new_slen);
        else if(strcmp(buffer, "mk_dir") == 0)
            mk_dir(buffer, MAXBUF, sockfd, new_si_other, new_slen);
        else if(strcmp(buffer, "rm_dir") == 0)
            rm_dir(buffer, MAXBUF, sockfd, new_si_other, new_slen);
		else if(strcmp(buffer, "echo") == 0)
            echo(buffer, sockfd, MAXBUF, new_si_other, new_slen);
		else if(strcmp(buffer, "disconnect") == 0){
            sendto(sockfd, "disconnect", 10, 0, (struct sockaddr*) &new_si_other, new_slen);
			break;
		}
        else
            printf("unknown comand\n");
        printf("Для получения справки введите команду help\n");
    }
	//-----------------------------------------------
	printf("\nExit\n");
    closesocket(sockfd);
    WSACleanup();
	_getch();
    return 0;
  }