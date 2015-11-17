// ѕример простого TCP клиента
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

  #define PORT 1234
  #define SERVERADDR "192.168.1.44"
#define MAXBUF  1024

  int main()
  {
    setlocale(LC_ALL, "Russian");
	  char buffer[MAXBUF];

    // Ўаг 1 - инициализаци€ библиотеки Winsock
    if (WSAStartup(0x202,(WSADATA *)&buffer[0]))
    {
      printf("WSAStart error %d\n",WSAGetLastError());
	  _getch();
      return -1;
    }

    // Ўаг 2 - создание сокета
    SOCKET sock;
    sock=socket(AF_INET,SOCK_STREAM,0);
    if (sock < 0)
    {
      printf("Socket() error %d\n",WSAGetLastError());
	  _getch();
      return -1;
    }

    // Ўаг 3 - установка соединени€

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

    // адрес сервера получен Ц пытаемс€ установить
    // соединение 
    if (connect(sock,(sockaddr *)&dest_addr,
                sizeof(dest_addr)))
    {
      printf("Connect error %d\n",WSAGetLastError());
	  _getch();
      return -1;
    }

    printf("—оединение с %s успешно установлено\n",SERVERADDR);

    // Ўаг 4 - чтение и передача сообщений
    /*int nsize;
    while((nsize=recv(sock,&buffer[0],
                      sizeof(buffer)-1,0))
                  !=SOCKET_ERROR)
    {
      // ставим завершающий ноль в конце строки 
      buffer[nsize]=0;

      // выводим на экран 
      printf("S=>C:%s",buffer);

      // читаем пользовательский ввод с клавиатуры
      printf("S<=C:"); fgets(&buffer[0],sizeof(buffer)-1,
             stdin);

      // проверка на "quit"
      if (!strcmp(&buffer[0],"quit\n"))
      {
        //  орректный выход
        printf("Exit...");
        closesocket(sock);
        WSACleanup();
		_getch();
        return 0;
      }

      // передаем строку клиента серверу
      send(sock,&buffer[0],nsize,0);
    }*/

    //printf("Recv error %d\n",WSAGetLastError());
	//-----------------------------------------------
	memset(buffer, 0, MAXBUF);
    recv(sock , buffer , MAXBUF , 0); // прием слова
    puts(buffer);
    printf("ƒл€ получени€ справки введите команду help\n");
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
        else if(strcmp(buffer, "mk_dir") == 0)
            mk_dir(buffer, MAXBUF, sock);
        else if(strcmp(buffer, "rm_dir") == 0)
            rm_dir(buffer, MAXBUF, sock);
		else if(strcmp(buffer, "disconnect") == 0){
            send(sock, "disconnect", 10, 0);
			break;
		}
        else
            printf("unknown comand\n");
        printf("ƒл€ получени€ справки введите команду help\n");
    }
	//-----------------------------------------------
	printf("\nExit\n");
    closesocket(sock);
    WSACleanup();
	_getch();
    return 0;
  }