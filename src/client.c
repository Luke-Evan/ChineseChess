#include "client.h"
void inputController(){
    request_queue = malloc(sizeof(node));
    request_queue->next = NULL;
    while(printf("please input the string of request NO.%d\n", ++cnt)){
        node* request = malloc(sizeof(node));
        request->request_id = cnt;
        memset(request->buf, 0, sizeof(request->buf));
        request->next = NULL;
        scanf("%s", request->buf);
        getLastElement(request_queue)->next = request;
    }
    return;
}
void maintainContact(){
    while(1){
        if(!isEmpty(request_queue)){
            node *head = getHead(request_queue);
            int iResult = send(server_socket, head->buf, (int )strlen(head->buf), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(server_socket);
                WSACleanup();
                exit(EXIT_FAILURE);
            }
            node* ans = malloc(sizeof(node));
            ans->request_id = head->request_id;
            memset(ans->buf, 0, sizeof(ans->buf));
            ans->next = NULL;
            iResult = recv(server_socket, ans->buf, 1000, 0);
            printf("request NO.%d has been answered: %s\n", ans->request_id, ans->buf);
            removeHead(request_queue);
        }
        Sleep(100);
    }
}
int main() {
    printf("This is the client.\n");
    WSADATA wsadata;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        exit(EXIT_FAILURE);
    }

    struct addrinfo *result, *ptr, hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;//未知
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    //127.0.0.1是特殊地址，指向当前主机，也可写作localhost，端口与服务器端口相对应
    iResult = getaddrinfo("127.0.0.1", "8080", &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    server_socket = INVALID_SOCKET;
    ptr = result;
    server_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (server_socket == INVALID_SOCKET) {
        printf("socket() failed: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    //connect函数建立与指定套接字的连接
    if (connect(server_socket, ptr->ai_addr, (int)ptr->ai_addrlen) ==SOCKET_ERROR) {
        printf("connect() failed.\n");
        system("pause");
        freeaddrinfo(result);
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(result);

    printf("connection established!\n");

    //建立服务器通信线程
    CreateThread(NULL, (SIZE_T)NULL, (LPTHREAD_START_ROUTINE)maintainContact, NULL, 0UL, NULL);
    //建立客户端输入输出进程
    inputController();

    return 0;
}