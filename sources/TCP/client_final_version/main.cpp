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
#define MAXBUF  1024

  int main()
  {
	char* SERVERADDR;
	SERVERADDR = (char*)malloc(15);
    setlocale(LC_ALL, "Russian");
	char buffer[MAXBUF];
	
	memset(buffer, 0, MAXBUF);
	while(1){
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
	}

    //инициализация библиотеки Winsock
    if (WSAStartup(0x202,(WSADATA *)&buffer[0]))
    {
      printf("WSAStart error %d\n",WSAGetLastError());
	  _getch();
      return -1;
    }

    //создание сокета
    SOCKET sock;
    sock=socket(AF_INET,SOCK_STREAM,0);
    if (sock < 0)
    {
      printf("Socket() error %d\n",WSAGetLastError());
	  _getch();
      return -1;
    }

    //установка соединения
    // заполнение структуры sockaddr_in
    // указание адреса и порта сервера
    sockaddr_in dest_addr;
    dest_addr.sin_family=AF_INET;
    dest_addr.sin_port=htons(PORT);
    HOSTENT *hst;

    // преобразование IP адреса из символьного в
    // сетевой формат
    if (inet_addr(SERVERADDR)!=INADDR_NONE)
      dest_addr.sin_addr.s_addr=inet_addr(SERVERADDR);
    else
      // попытка получить IP адрес по доменному
      // имени сервера
      if (hst=gethostbyname(SERVERADDR))
      // hst->h_addr_list содержит не массив адресов,
      // а массив указателей на адреса
      ((unsigned long *)&dest_addr.sin_addr)[0]=
        ((unsigned long **)hst->h_addr_list)[0][0];
      else 
      {
        printf("Invalid address %s\n",SERVERADDR);
        closesocket(sock);
        WSACleanup();
		_getch();
        return -1;
      }
	printf("123\n");
    if (connect(sock,(sockaddr *)&dest_addr,
                sizeof(dest_addr)))
    {
      printf("Connect error %d\n",WSAGetLastError());
	  _getch();
      return -1;
    }
	//-----------------------------------------------
	memset(buffer, 0, MAXBUF);
    recv(sock , buffer , MAXBUF , 0); // прием слова
    puts(buffer);
    printf("Для получения справки введите команду help\n");
    while(1){
		memset(buffer, 0, MAXBUF);
        printf("Cmd: ");
        scanf("%s" , buffer);
        // команды
        if(strcmp(buffer, "view") == 0)
			view_dir(buffer, sock, MAXBUF);
        else if(strcmp(buffer, "view_dir") == 0)
            view_dir(buffer, sock, MAXBUF);
        else if(strcmp(buffer, "help") == 0)
            get_help();
        else if(strcmp(buffer, "get_dir") == 0)
            get_dir(buffer, sock, MAXBUF);
        else if(strcmp(buffer, "ch_dir") == 0)
            ch_dir(buffer, sock, MAXBUF);
        else if(strcmp(buffer, "upload") == 0)
            upload(buffer, MAXBUF, sock);
		else if(strcmp(buffer, "download") == 0)
            download(buffer, MAXBUF, sock);
        else if(strcmp(buffer, "mk_dir") == 0)
            mk_dir(buffer, MAXBUF, sock);
        else if(strcmp(buffer, "rm_dir") == 0)
            rm_dir(buffer, MAXBUF, sock);
		else if(strcmp(buffer, "echo") == 0)
            echo(buffer, sock, MAXBUF);
		else if(strcmp(buffer, "disconnect") == 0){
            send(sock, "disconnect", 10, 0);
			break;
		}
        else
            printf("unknown comand\n");
        printf("Для получения справки введите команду help\n");
    }
	//-----------------------------------------------
	printf("\nExit\n");
    closesocket(sock);
    WSACleanup();
	_getch();
    return 0;
  }