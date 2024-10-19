#ifndef _NET_H_
#define _NET_H_

int create_socket(int);

int accept_client(int);

void handle_client(int);

#endif
