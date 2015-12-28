#ifndef VIEW_H
#define VIEW_H

#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAXBUF 1024
void view(char *dir, int depth, int clientfd, struct sockaddr_in si_other, int slen) {

    //chdir("/home/user/server/server_work");
    // буфер для посылки
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

                 bzero( buffer, MAXBUF);
                 sprintf( buffer, "%d", depth);
                 strcat(buffer, "|");
                 strcat(buffer, "d");
                 strcat(buffer, "|");
                 strcat(buffer, entry->d_name);
                 //send(clientfd, buffer, MAXBUF, 0);
                 sendto(clientfd, buffer, MAXBUF, 0, (struct sockaddr*) &si_other, slen);

                int new_depth = depth + 6;
                 // Рекурсивный вызов с новый отступом
                 view(entry->d_name, new_depth, clientfd, si_other, slen);
         }
          else {
             bzero( buffer, MAXBUF);
             sprintf( buffer, "%d", depth);
             strcat(buffer, "|");
             strcat(buffer, "f");
             strcat(buffer, "|");
             strcat(buffer, entry->d_name);
             //send(clientfd, buffer, MAXBUF, 0);
             sendto(clientfd, buffer, MAXBUF, 0, (struct sockaddr*) &si_other, slen);
          }
  }
 chdir("..");
 closedir(dp);
 //chdir("/home/user/server/server_work");
}
#endif // VIEW_H

