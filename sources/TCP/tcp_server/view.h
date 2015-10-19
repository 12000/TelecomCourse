#ifndef VIEW_H
#define VIEW_H

#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAXBUF 1024
void view(char *dir, int depth, int clientfd) {
    // буфер для послыки
    char buffer[MAXBUF];
    DIR *dp;
    struct dirent *entry;
     struct stat statbuf;
     if ((dp = opendir(dir)) == NULL) {
             fprintf(stderr, "cannot open directory: %s\n", dir);
             return;
    }

    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
         lstat(entry->d_name, &statbuf);
         if (S_ISDIR(statbuf.st_mode)) {
             //Находит каталог, но игнорирует . и ..
                if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                         continue;

                 // отправка пробелов
                 bzero( buffer, MAXBUF);
                 sprintf( buffer, "%d", depth);
                 send(clientfd, buffer, MAXBUF, 0);

                 //отправка символа папки/файла
                  bzero( buffer, MAXBUF);
                  strcpy(buffer, "file");
                  send(clientfd, buffer, MAXBUF, 0);

                 //отправка строки
                  bzero( buffer, MAXBUF);
                  strcpy(buffer, entry->d_name);
                  //buffer[strlen(buffer)] = "/";
                  send(clientfd, buffer, MAXBUF, 0);

                int new_depth = depth + 6;
                 // Рекурсивный вызов с новый отступом
                 view(entry->d_name, new_depth, clientfd);
         }
          else {
              // отправка пробелов
              bzero( buffer, MAXBUF);
              sprintf( buffer, "%d", depth);
              printf("%s", buffer);
              send(clientfd, buffer, MAXBUF, 0);
               printf("depth = %d", depth);

               //отправка символа папки/файла
                bzero( buffer, MAXBUF);
                strcpy(buffer, "dir");
                send(clientfd, buffer, MAXBUF, 0);

              //отправка строки
               bzero( buffer, MAXBUF);
               strcpy(buffer, entry->d_name);
               send(clientfd, buffer, MAXBUF, 0);
               printf("%s", buffer);
          }
  }

 chdir("..");
 closedir(dp);

}
#endif // VIEW_H

