#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

int if_two=0;

SOCKET client_socket[2] = {INVALID_SOCKET,INVALID_SOCKET};
HANDLE thread[2]={0};

void maintain1_to_0() {
    //信息接收数组以及初始化
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    //服务端程序需要在开机时持续执行
    while (1) {
        if (if_two != 0) {
            while (1) {
                //recv函数从连接的套接字或绑定的无连接套接字接收数据，返回值是接收到的字符数
                int iResult = recv(client_socket[1], buf, 1000, 0);
                if (iResult > 0) {//接收到信息了
                    //send函数在连接的套接字上发送数据
                    //printf("recv from black ");
                    int iSendResult = send(client_socket[0], buf, iResult, 0);
                    //printf("send to red\n");
                    if (iSendResult == SOCKET_ERROR) {
                        printf("send failed: %d\n", WSAGetLastError());
                        closesocket(client_socket[0]);
                        WSACleanup();
                        exit(EXIT_FAILURE);
                    }
                } else if (iResult == 0) {
                    printf("Connection closing ...\n");
                    system("pause");
                } else {
                    printf("recv failed: %d\n", WSAGetLastError());
                    closesocket(client_socket[0]);
                    WSACleanup();
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}
void maintain0_to_1() {
    //信息接收数组以及初始化
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    //服务端程序需要在开机时持续执行
    while (1) {
        //recv函数从连接的套接字或绑定的无连接套接字接收数据，返回值是接收到的字符数
        int iResult = recv(client_socket[0], buf, 1000, 0);
        if (iResult > 0) {//接收到信息了
            //send函数在连接的套接字上发送数据
            //printf("recv from red ");
            int iSendResult = send(client_socket[1], buf, iResult, 0);
            //printf("send to black\n");
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(client_socket[1]);
                WSACleanup();
                exit(EXIT_FAILURE);
            }
        } else if (iResult == 0) {
            printf("Connection closing ...\n");
            system("pause");
        } else {
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(client_socket[1]);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
    }
}

void buildConnection(const SOCKET *listen_socket) {
    //red
    //accept函数允许在套接字上尝试传入连接
    client_socket[0] = accept(*listen_socket, NULL, NULL);
    if (client_socket[0] == INVALID_SOCKET) {
        printf("accept() failed: %d\n", WSAGetLastError());
        closesocket(*listen_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    //连接成功建立
    printf("Client(0):connection established!\n");
    //创建在调用进程的虚拟地址空间内执行的线程
    thread[0]=CreateThread(NULL, (SIZE_T) NULL, (LPTHREAD_START_ROUTINE) maintain1_to_0, NULL, 0UL, NULL);

    //client2
    //accept函数允许在套接字上尝试传入连接
    client_socket[1] = accept(*listen_socket, NULL, NULL);
    if (client_socket[1] == INVALID_SOCKET) {
        printf("accept() failed: %d\n", WSAGetLastError());
        closesocket(*listen_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    //连接成功建立
    printf("Client(1):connection established!\n");
    if_two=1;
    //创建在调用进程的虚拟地址空间内执行的线程
    thread[1]=CreateThread(NULL, (SIZE_T) NULL, (LPTHREAD_START_ROUTINE) maintain0_to_1, NULL, 0UL, NULL);

}

int main() {
    //初始化
    //printf("This is the server.\n");
    WSADATA wsaData;
    int iResult;
    //网络服务初始化(Web Server API)，第一个参数为需求的最低socket版本号，示例使用的是2.2版本
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    //如果初始化失败则报错并打印错误信息（iResult），网络编程中需要时刻注意中途可能出现的错误并给予返回信息
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        //自带收尾工作的return 1，返回值为1表示程序未正常结束
        exit(EXIT_FAILURE);
    }

    //为getaddrinfo函数做准备
    struct addrinfo *result, hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;//IPv4地址
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;//TCP协议
    hints.ai_flags = AI_PASSIVE;//服务端被动绑定
    //获取本机ip
    //8080表示端口号，取值范围为0~65535，但有些端口为保留端口，使用前请先查询目标端口是否被占用
    //getaddrinfo函数提供从主机名到地址的独立于协议的转换，结果记录在result中
    iResult = getaddrinfo(NULL, "8080", &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    //建立监听的socket
    SOCKET listen_socket = INVALID_SOCKET;
    //socket函数根据getaddrinfo函数返回的信息创建一个绑定到相应端口的套接字，这个套接字用于监听
    listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listen_socket == INVALID_SOCKET) {
        printf("socket() failed: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    //绑定函数将本地地址与套接字相关联
    iResult = bind(listen_socket, result->ai_addr, (int) result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind() failed: %d\n", WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    //printf("server build up successfully\n");
    //得到本机ip
    struct hostent *phostinfo = gethostbyname("");
    char ip[20] = {0};
    int size = sizeof(ip);
    char *p = inet_ntoa(*((struct in_addr *) (*phostinfo->h_addr_list)));
    strncpy(ip, p, size - 1);
    ip[size - 1] = '\0';
    printf("%s\n", ip);
    //绑定完成后，释放不再使用的内存空间
    freeaddrinfo(result);

    //侦听函数将套接字置于侦听传入连接的状态
    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("listen() failed: %d\n", WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        return EXIT_FAILURE;
    }
    printf("waiting for connection\n");

    //建立连接线程
    buildConnection(&listen_socket);

    WaitForSingleObject(thread[0], INFINITE);
    WaitForSingleObject(thread[1], INFINITE);
    return 0;
}