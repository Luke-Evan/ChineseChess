#include "chess.h"

int main() {
    ME=0;
    ANOTHER=1;
    init_sdl();
    load_res();
    main_loop();
    free_quit();
    return 0;
}
void init_sdl() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_AUDIO);
    SDL_EventState(SDL_MOUSEMOTION,IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONUP,IGNORE);
    SDL_EventState(SDL_LOCALECHANGED,IGNORE);
    SDL_StopTextInput();
    TTF_Init();
    Window = SDL_CreateWindow("chess_red",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WIDTH,
                              HEIGHT,
                              SDL_WINDOW_SHOWN);//创建窗口
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);//创建渲染器
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    //行棋记录初始化
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 9; j++) {
            record_step_location[step][i][j] = record_location[i][j];
        }
    }
    //棋盘位置初始化
    for (int i = 0; i < 10; i++) {
        if (i < 5) {
            for (int j = 0; j < 9; j++) {
                piece_rect[i][j] =
                    (SDL_Rect) {455 - unit_length / 2 + unit_length * j, 85 - unit_length / 2 + i * unit_length,
                                unit_length, unit_length};
            }
        } else {
            for (int j = 0; j < 9; j++) {
                piece_rect[i][j] =
                    (SDL_Rect) {455 - unit_length / 2 + unit_length * j, 115 - unit_length / 2 + i * unit_length,
                                unit_length, unit_length};
            }
        }
    }
}

void main_loop() {
    //printf("before main_event\n");//测试
    while (1) {
        print_start();
        while (SDL_PollEvent(&main_event)) {
            switch (main_event.type) {
                case SDL_QUIT:return;
                case SDL_MOUSEBUTTONDOWN:
                    //printf("(%d,%d)\n", main_event.button.x, main_event.button.y);
                    if (main_event.button.x >= 950 && main_event.button.x <= 1280 && main_event.button.y >= 660
                        && main_event.button.y <= 740) {//点击退出游戏
                        return;
                    }
                    if (main_event.button.x >= 260 && main_event.button.x <= 590 && main_event.button.y >= 400
                        && main_event.button.y <= 480) {//点击单人游戏
                        if (play_UI_single()) {
                            //printf("after play_UI_contact\n");
                            return;
                        }
                    }
                    if (main_event.button.x >= 260 && main_event.button.x <= 590 && main_event.button.y >= 530
                        && main_event.button.y <= 610) {//点击双人游戏
                        contact_with_server();
                        //建立服务器通信线程
                        CreateThread(NULL, (SIZE_T) NULL, (LPTHREAD_START_ROUTINE) recv_thread, NULL, 0UL, NULL);
                        play_UI_contact();
                    }
                    if (main_event.button.x >= 260 && main_event.button.x <= 590 && main_event.button.y >= 660
                        && main_event.button.y <= 740) {//点击规则
                        if (rule_UI()) {
                            return;
                        }
                    }
                    break;
                default:break;
            }
        }
    }

}
int play_UI_contact() {
    Mix_PlayMusic(bgm,-1);
    round=ME;
    while (1) {
        SDL_RenderClear(Renderer);
        check_buf();
        change_location();
        print_all();
        check_win();
        SDL_RenderPresent(Renderer);
        srand(rand_seed);
        rand_seed++;
        while (SDL_WaitEvent(&play_event)) {
            switch (play_event.type) {
                case SDL_QUIT:return 1;
                case SDL_MOUSEBUTTONDOWN://printf("(%d,%d)\n", play_event.button.x, play_event.button.y);
                    //落子
                    click_chess();
                    //返回
                    if (play_event.button.x >= 20 && play_event.button.x <= 120 && play_event.button.y >= 20
                        && play_event.button.y <= 70) {
                        func_kind = 1;
                        send_func(0,0,0,0,1);
                        //printf("send one change:1\n");
                        return 0;
                    }
                    //认输
                    if (play_event.button.x >= 110 && play_event.button.x <= 240 && play_event.button.y >= 225
                        && play_event.button.y <= 290) {
                        func_kind = 2;
                        admit_defeat();
                        send_func(0,0,0,0,2);
                        //printf("send one change:2\n");
                        break;
                    }
                    //悔棋
                    if (play_event.button.x >= 110 && play_event.button.x <= 240 && play_event.button.y >= 375
                        && play_event.button.y <= 440) {
                        step_back();
                        func_kind = 3;
                        send_func(0,0,0,0,3);
                        //printf("send one change:3\n");
                        break;
                    }
                    //吐槽
                    if (play_event.button.x >= 110 && play_event.button.x <= 240 && play_event.button.y >= 525
                        && play_event.button.y <= 590) {
                        func_kind = 4;
                        send_func(0,0,0,0,4);
                        //printf("send one change:4\n");
                        break;
                    }
                    //天降“骑”兵
                    if (play_event.button.x >= 1170 && play_event.button.x <= 1400 && play_event.button.y >= 140
                        && play_event.button.y <= 205) {
                        zu_to_che();
                        func_kind = 5;
                        send_func(0,0,0,0,5);
                        //printf("send one change:5\n");
                        break;
                    }
                    //重新开始
                    if (play_event.button.x >= 1155 && play_event.button.x <= 1355 && play_event.button.y >= 650
                        && play_event.button.y <= 715) {
                        restart();
                        func_kind = 6;
                        send_func(0,0,0,0,6);
                        //printf("send one change:6\n");
                        break;
                    }
                    break;
                default:break;
            }
            break;
        }

    }
}
void recv_thread() {
    char buf[1024];
    int i_result;
    while (1) {
        i_result=recv(server_socket, buf, 1000, 0);
        if (i_result>0){
            recv_buf = (struct contact_buf *) buf;
            if_recv=1;
            func_kind=recv_buf->func_kind;
            recv_x1=recv_buf->x1;
            recv_y1=recv_buf->y1;
            recv_x2=recv_buf->x2;
            recv_y2=recv_buf->y2;
            //printf("received one change:%d\n",func_kind);
            Sleep(100);
        }else if (i_result==0){
            printf("Connection closing ...\n");
            system("pause");
        } else{
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(server_socket);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

    }
}
void contact_with_server() {
    printf("This is the  red client.\n");
    printf("Please input the IP address of the server:\n");
    scanf("%s", server_ip);
    WSADATA wsadata;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        exit(EXIT_FAILURE);
    }

    struct addrinfo *result, *ptr, hints;//设置服务器端基本信息
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;//未知
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    //127.0.0.1是特殊地址，指向当前主机，也可写作localhost，端口与服务器端口相对应
    iResult = getaddrinfo(server_ip, "8080", &hints, &result);
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
    if (connect(server_socket, ptr->ai_addr, (int) ptr->ai_addrlen) == SOCKET_ERROR) {
        printf("connect() failed.\n");
        system("pause");
        freeaddrinfo(result);
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(result);

    printf("connection established!\n");
}

void recv_change_location(){
    step++;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 9; j++) {
            record_step_location[step][i][j] = record_location[i][j];
        }
    }
    int change = record_location[recv_y1][recv_x1];
    record_location[recv_y1][recv_x1] = 0;
    record_location[recv_y2][recv_x2] = change;
    Mix_PlayChannel(-1,do_piece,0);
    if_check1();//检查黑方是否被将
    if_check2();//检查红方是否被将
}
void recv_defeat(){
    Mix_PlayChannel(-1,win,0);
    for (int i = 0; i < 10; i++) {
        for (int j = 3; j < 5; j++) {
            if (record_location[i][j] == -7) {
                record_location[i][j] = 0;
                break;
            }
        }
    }
}

void send_func(int send_x1,int send_y1,int send_x2,int send_y2,int send_kind){
    send_buf.x1=send_x1;
    send_buf.y1=send_y1;
    send_buf.x2=send_x2;
    send_buf.y2=send_y2;
    send_buf.func_kind=send_kind;
    send(server_socket,(char*)&send_buf,1000,0);
}
void admit_defeat(){
        for (int i = 0; i < 10; i++) {
            for (int j = 3; j < 5; j++) {
                if (record_location[i][j] == 7) {
                    record_location[i][j] = 0;
                    break;
                }
            }
        }
    Mix_PlayChannel(-1,loss,0);
}