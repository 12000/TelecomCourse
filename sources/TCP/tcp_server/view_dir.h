#ifndef VIEW_DIR
#define VIEW_DIR
#include "view.h"

int view_dir(char *buffer, int buf_size, int clientfd){
    bzero(buffer, buf_size);
    getcwd(buffer, buf_size);
    //strcpy(buffer, "/home/user/server/server_work");
    view(buffer, 5, clientfd);
    return 0;
}

#endif // VIEW_DIR

