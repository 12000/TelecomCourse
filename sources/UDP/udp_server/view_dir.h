#ifndef VIEW_DIR
#define VIEW_DIR
#include "view.h"

int view_dir(char *buffer, int buf_size, int sock, struct sockaddr_in si_other, int slen){
    bzero(buffer, buf_size);
    getcwd(buffer, buf_size);
    view(buffer, 5, sock, si_other, slen);
    return 0;
}

#endif // VIEW_DIR

