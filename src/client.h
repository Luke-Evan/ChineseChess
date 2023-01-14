
#ifndef PROJECT_2_SRC_CLIENT_H_
#define PROJECT_2_SRC_CLIENT_H_

#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
typedef struct NODE{
  int request_id;
  char buf[1024];
  struct NODE* next;
}node;
node *request_queue;
SOCKET server_socket;
int cnt;
node *getLastElement(node *ptr){
    node *p = ptr;
    while(p->next)p = p->next;
    return p;
}
node *getHead(node *ptr){
    return ptr->next;
}
void removeHead(node *ptr){
    ptr->next = ptr->next->next;
}
bool isEmpty(node *ptr){
    return ptr->next == NULL;
}

void inputController();
void maintainContact();

#endif //PROJECT_2_SRC_CLIENT_H_
