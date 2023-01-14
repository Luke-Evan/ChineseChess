//
// Created by Estrella on 2023/1/11.
//

#ifndef PROJECT_2_SRC_SERVER_H_
#define PROJECT_2_SRC_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <pthread.h>

void maintainContact(const SOCKET* client_socket);
void buildConnection(const SOCKET* listen_socket);


#endif //PROJECT_2_SRC_SERVER_H_
