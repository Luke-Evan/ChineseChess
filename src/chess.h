
#ifndef PROJECT_2_SRC_CHESS_H_
#define PROJECT_2_SRC_CHESS_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <time.h>
#include <string.h>

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_mixer.h"

#define WIDTH 1500
#define HEIGHT 900
#define board_width 770
#define unit_length 70 //单位长度
#undef main
int rand_seed=114514;


//基础
static SDL_Window *Window = NULL;
static SDL_Renderer *Renderer = NULL;
static SDL_Event main_event;
static SDL_Event play_event;
static SDL_Event rule_event;
//开始界面
static SDL_Surface *background_surf=NULL;
static SDL_Rect background_rect={0,0,WIDTH,HEIGHT};
static SDL_Surface *start_surf[5]={NULL};
static SDL_Color black={0,0,0,0};
static SDL_Color white={255,255,255,255};
static SDL_Rect start_rect[5]={{350,100,850,200},
                        {260,400,330,80},
                        {260,530,330,80},
                        {260,660,330,80},
                        {950,660,330,80}};
//游戏界面
static SDL_Surface *play_surf[5]={NULL};
static SDL_Rect play_rect[5]={{110,225,130,65},//认输
                              {110,375,130,65},//悔棋
                              {110,525,130,65},//吐槽
                              {1170,140,230,65},//天降“骑”兵
                              {1155,650,200,65}};//重新开始
static SDL_Surface *rule_surf=NULL;
static SDL_Surface *backboard_surf=NULL;
static SDL_Surface *back_surf=NULL;
static SDL_Rect back_rect={20,20,100,50};
static SDL_Surface *win_surf[2]={NULL};
static SDL_Rect win_rect={590,300,280,150};
static SDL_Surface *tu_cao_surf[4]={NULL};
static SDL_Rect tu_cao_rect={1140,250,300,300};
static int round=0;
static int if_end=0;
int if_tu_cao=0;
//棋子，棋盘
static SDL_Surface *chess_board_surf=NULL;
static SDL_Rect chess_board_rect={WIDTH/2-board_width/2, 0, board_width, HEIGHT};
static SDL_Surface *piece_surf[14]={NULL};
static SDL_Rect piece_rect[10][9]={0};
//记录棋子位置
int step=0;
int record_step_location[200][10][9]={0};
int init_location[10][9]={
    {-3,-4,-5,-6,-7,-6,-5,-4,-3},
    {0,0,0,0,0,0,0,0,0},
    {0,-2,0,0,0,0,0,-2,0},
    {-1,0,-1,0,-1,0,-1,0,-1},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {1,0,1,0,1,0,1,0,1},
    {0,2,0,0,0,0,0,2,0},
    {0,0,0,0,0,0,0,0,0},
    {3,4,5,6,7,6,5,4,3}
};
int record_location[10][9]={
    {-3,-4,-5,-6,-7,-6,-5,-4,-3},
    {0,0,0,0,0,0,0,0,0},
    {0,-2,0,0,0,0,0,-2,0},
    {-1,0,-1,0,-1,0,-1,0,-1},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {1,0,1,0,1,0,1,0,1},
    {0,2,0,0,0,0,0,2,0},
    {0,0,0,0,0,0,0,0,0},
    {3,4,5,6,7,6,5,4,3}
};
/*
规定数组值的意义
 红方：
 兵=1，炮=2，车=3，马=4，相=5，仕=6，帅=7；
 黑方：
 卒=-1，炮=-2，车=-3，马=-4，象=-5，士=-6，帅=-7；
 没有棋子为0；
*/
//棋子移动
static SDL_Surface *frame_surf=NULL;
int judge_do=0;//判断是否落子
int if_frame=0;
int piece_x1=0,piece_y1=1;//x1,y1表示选子
int piece_x2=0,piece_y2=1;//x2,y2表示落子
int x3=4,y3=0;//黑将
int x4=4,y4=9;//红帅
//字体
static TTF_Font *li_shu=NULL;
static TTF_Font *kai_shu=NULL;
//音乐
static Mix_Music *bgm=NULL;
static Mix_Chunk *chose=NULL;
static Mix_Chunk *do_piece=NULL;
static Mix_Chunk *eat=NULL;
static Mix_Chunk *loss=NULL;
static Mix_Chunk *win=NULL;
static Mix_Chunk *jiang=NULL;

void init_sdl();
void load_res() {
    //字体
    li_shu = TTF_OpenFont("fonts/li_shu.ttf", 400);
    kai_shu = TTF_OpenFont("fonts/kai_shu.ttf", 400);
    start_surf[0] = TTF_RenderUTF8_Blended(li_shu, "中 国 象 棋", black);
    start_surf[1] = TTF_RenderUTF8_Blended(li_shu, "单人模式", black);
    start_surf[2] = TTF_RenderUTF8_Blended(li_shu, "双人模式", black);
    start_surf[3] = TTF_RenderUTF8_Blended(li_shu, "规则说明", black);
    start_surf[4] = TTF_RenderUTF8_Blended(li_shu, "退出游戏", black);
    back_surf = TTF_RenderUTF8_Blended(li_shu, "返回", black);
    play_surf[0] = TTF_RenderUTF8_Blended(li_shu, "认输", white);
    play_surf[1] = TTF_RenderUTF8_Blended(li_shu, "悔棋", white);
    play_surf[2] = TTF_RenderUTF8_Blended(li_shu, "吐槽", white);
    play_surf[3] = TTF_RenderUTF8_Blended(li_shu, "天降“骑”兵", white);
    //图片
    play_surf[4] = IMG_Load("images/restart.png");
    backboard_surf = IMG_Load("images/backboard.png");
    background_surf = IMG_Load("images/background.png");
    chess_board_surf = IMG_Load("images/chess_board.png");
    piece_surf[0] = IMG_Load("images/red-bing.png");
    piece_surf[1] = IMG_Load("images/red-pao.png");
    piece_surf[2] = IMG_Load("images/red-ju.png");
    piece_surf[3] = IMG_Load("images/red-ma.png");
    piece_surf[4] = IMG_Load("images/red-xiang.png");
    piece_surf[5] = IMG_Load("images/red-shi.png");
    piece_surf[6] = IMG_Load("images/red-shuai.png");
    piece_surf[7] = IMG_Load("images/black-zu.png");
    piece_surf[8] = IMG_Load("images/black-pao.png");
    piece_surf[9] = IMG_Load("images/black-ju.png");
    piece_surf[10] = IMG_Load("images/black-ma.png");
    piece_surf[11] = IMG_Load("images/black-xiang.png");
    piece_surf[12] = IMG_Load("images/black-shi.png");
    piece_surf[13] = IMG_Load("images/black-jiang.png");
    frame_surf = IMG_Load("images/frame.png");
    rule_surf = IMG_Load("images/rule.jpg");
    win_surf[0] = IMG_Load("images/black_win.png");
    win_surf[1] = IMG_Load("images/red_win.png");
    tu_cao_surf[0] = IMG_Load("images/tucao0.jpg");
    tu_cao_surf[1] = IMG_Load("images/tucao1.jpg");
    tu_cao_surf[2] = IMG_Load("images/tucao2.jpg");
    tu_cao_surf[3] = IMG_Load("images/tucao3.jpg");
    //音频
    bgm = Mix_LoadMUS("music/bgm.mp3");
    chose = Mix_LoadWAV("music/chose.mp3");
    do_piece = Mix_LoadWAV("music/do_piece.wav");
    eat = Mix_LoadWAV("music/eat.mp3");
    loss = Mix_LoadWAV("music/loss.wav");
    win = Mix_LoadWAV("music/win.wav");
    jiang = Mix_LoadWAV("music/jiang.wav");
    Mix_VolumeMusic(10);
}
//黑方被将
void if_check1() {
    int count = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 3; j <= 5; j++) {
            if (record_location[i][j] == -7) {
                count++;
                x3 = j;
                y3 = i;
            }
        }
    }
    if (count != 0) {
        //马的将军
        if (y3-1>=0){
            if ((record_location[y3 - 1][x3 - 2] == 4 && record_location[y3 - 1][x3 - 1] == 0) ||
                (record_location[y3 - 1][x3 + 2] == 4 && record_location[y3 - 1][x3 + 1] == 0) ||
                (record_location[y3 - 2][x3 - 1] == 4 && record_location[y3 - 1][x3 - 1] == 0) ||
                (record_location[y3 - 2][x3 + 1] == 4 && record_location[y3 - 1][x3 + 1] == 0)) {
                Mix_PlayChannel(-1, jiang, 0);
                //printf("hong ma jiang\n");
                return;
            }
        }
        if ((record_location[y3 + 2][x3 - 1] == 4 && record_location[y3 + 1][x3 - 1] == 0) ||
            (record_location[y3 + 2][x3 + 1] == 4 && record_location[y3 + 1][x3 + 1] == 0) ||
            (record_location[y3 + 1][x3 - 2] == 4 && record_location[y3 + 1][x3 - 1] == 0) ||
            (record_location[y3 + 1][x3 + 2] == 4 && record_location[y3 + 1][x3 + 1] == 0) ) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hong ma jiang\n");
            return;
        }
        //兵将军
        if (record_location[y3 + 1][x3] == 1 || record_location[y3][x3 - 1] == 1 || record_location[y3][x3 + 1] == 1) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hong bing jiang\n");
            return;
        }
        //车的将军
        int che_check = 0;
        for (int i = y3 + 1; i < 10; i++) {//下
            if (record_location[i][x3] != 0) {
                if (record_location[i][x3] == 3) {
                    che_check++;
                    break;
                } else { break; }
            }
        }
        for (int i = y3 - 1; i >= 0; i--) {//上
            if (record_location[i][x3] != 0) {
                if (record_location[i][x3] == 3) {
                    che_check++;
                    break;
                } else { break; }
            }
        }
        for (int i = x3 + 1; i < 9; i++) {//右
            if (record_location[y3][i] != 0) {
                if (record_location[y3][i] == 3) {
                    che_check++;
                    break;
                } else { break; }
            }
        }
        for (int i = x3 - 1; i >= 0; i--) {//左
            if (record_location[y3][i] != 0) {
                if (record_location[y3][i] == 3) {
                    che_check++;
                    break;
                } else { break; }
            }
        }
        if (che_check > 0) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hong che jiang\n");
            return;
        }
        //炮的将军
        int sign = 0;
        count = 0;
        for (int i = y3 + 1; i < 10; i++) {//下
            if (record_location[i][x3] != 0) {
                count++;
            }
            if (count==2){
                if (record_location[i][x3] == 2) {
                    sign = 1;
                }
                break;
            }
        }
        if (sign == 1 && count == 2) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hong pao jiang from below\n");
            return;
        }
        sign = 0;
        count = 0;
        for (int i = y3 - 1; i >= 0; i--) {//上
            if (record_location[i][x3] != 0) {
                count++;
            }
            if (count==2){
                if (record_location[i][x3] == 2) {
                    sign = 1;
                }
                break;
            }
        }
        if (sign == 1 && count == 2) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hong pao jiang from up\n");
            return;
        }
        sign = 0;
        count = 0;
        for (int i = x3 + 1; i < 9; i++) {//右
            if (record_location[y3][i] != 0) {
                count++;
            }
            if (count==2){
                if (record_location[y3][i] == 2) {
                    sign = 1;
                }
                break;
            }
        }
        if (sign == 1 && count == 2) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hong pao jiang from right\n");
            return;
        }
        sign = 0;
        count = 0;
        for (int i = x3 - 1; i >= 0; i--) {//左
            if (record_location[y3][i] != 0) {
                count++;
            }
            if (count==2){
                if (record_location[y3][i] == 2) {
                    sign = 1;
                }
                break;
            }
        }
        if (sign == 1 && count == 2) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hong pao jiang from left\n");
            return;
        }
        //中门对狙
        sign = 0;
        count = 0;
        for (int i = y3 + 1; i < 10; i++) {//左
            if (record_location[i][x3] != 0) {
                count++;
            }
            if (record_location[i][x3] == 7) {
                sign = 1;
                break;
            }
        }
        if (sign == 1 && count == 1) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hong shuai jiang\n");
            return;
        }
    }

}
//红帅被将
void if_check2() {
    int count = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 3; j <= 5; j++) {
            if (record_location[i][j] == 7) {
                count++;
                x4 = j;
                y4 = i;
            }
        }
    }
    if (count != 0) {
        //马的将军
        if (y4+1<10){
            if ((record_location[y4 + 2][x4 - 1] == -4 && record_location[y4 + 1][x4 - 1] == 0) ||
                (record_location[y4 + 2][x4 + 1] == -4 && record_location[y4 + 1][x4 + 1] == 0) ||
                (record_location[y4 + 1][x4 - 2] == -4 && record_location[y4 + 1][x4 - 1] == 0) ||
                (record_location[y4 + 1][x4 + 2] == -4 && record_location[y4 + 1][x4 + 1] == 0) ) {
                Mix_PlayChannel(-1, jiang, 0);
                //printf("hei ma jiang\n");
                return;
            }
        }
        if ((record_location[y4 - 1][x4 - 2] == -4 && record_location[y4 - 1][x4 - 1] == 0) ||
            (record_location[y4 - 1][x4 + 2] == -4 && record_location[y4 - 1][x4 + 1] == 0) ||
            (record_location[y4 - 2][x4 - 1] == -4 && record_location[y4 - 1][x4 - 1] == 0) ||
            (record_location[y4 - 2][x4 + 1] == -4 && record_location[y4 - 1][x4 + 1] == 0)) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hei ma jiang\n");
            return;
        }
        //兵将军
        if (record_location[y4 - 1][x4] == -1 || record_location[y4][x4 - 1] == -1
            || record_location[y4][x4 + 1] == -1) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hei bing jiang\n");
            return;
        }
        //车的将军
        int che_check = 0;
        for (int i = y4 + 1; i < 10; i++) {//下
            if (record_location[i][x4] != 0) {
                if (record_location[i][x4] == -3) {
                    che_check++;
                    break;
                } else { break; }
            }
        }
        for (int i = y4 - 1; i >= 0; i--) {//上
            if (record_location[i][x4] != 0) {
                if (record_location[i][x4] == -3) {
                    che_check++;
                    break;
                } else { break; }
            }
        }
        for (int i = x4 + 1; i < 9; i++) {//右
            if (record_location[y4][i] != 0) {
                if (record_location[y4][i] == -3) {
                    che_check++;
                    break;
                } else { break; }
            }
        }
        for (int i = x4 - 1; i >= 0; i--) {//左
            if (record_location[y4][i] != 0) {
                if (record_location[y4][i] == -3) {
                    che_check++;
                    break;
                } else { break; }
            }
        }
        if (che_check > 0) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hei che jiang\n");
            return;
        }
        //炮的将军
        int sign = 0;
        count = 0;
        for (int i = y4 + 1; i < 10; i++) {//下
            if (record_location[i][x4] != 0) {
                count++;
            }
            if (count==2){
                if (record_location[i][x4] == -2) {
                    sign = 1;
                }
                break;
            }
        }
        if (sign == 1 && count == 2) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hei pao jiang\n");
            return;
        }
        sign = 0;
        count = 0;
        for (int i = y4 - 1; i >= 0; i--) {//上
            if (record_location[i][x4] != 0) {
                count++;
            }
            if (count==2){
                if (record_location[i][x4] == -2) {
                    sign = 1;
                }
                break;
            }
        }
        if (sign == 1 && count == 2) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hei pao jiang\n");
            return;
        }
        sign = 0;
        count = 0;
        for (int i = x4 + 1; i < 9; i++) {//右
            if (record_location[y4][i] != 0) {
                count++;
            }
            if (count==2){
                if (record_location[y4][i] == -2) {
                    sign = 1;
                }
                break;
            }
        }
        if (sign == 1 && count == 2) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hei pao jiang\n");
            return;
        }
        sign = 0;
        count = 0;
        for (int i = x4 - 1; i >= 0; i--) {//左
            if (record_location[y4][i] != 0) {
                count++;
            }
            if (count==2){
                if (record_location[y4][i] == -2) {
                    sign = 1;
                }
                break;
            }
        }
        if (sign == 1 && count == 2) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hei pao jiang\n");
            return;
        }
        //中门对狙
        sign = 0;
        count = 0;
        for (int i = y4 - 1; i >= 0; i--) {
            if (record_location[i][x4] != 0) {
                count++;
            }
            if (record_location[i][x4] == -7) {
                sign = 1;
                break;
            }
        }
        if (sign == 1 && count == 1) {
            Mix_PlayChannel(-1, jiang, 0);
            //printf("hei shuai jiang\n");
            return;
        }
    }
}
void print_start() {
    SDL_RenderClear(Renderer);
    SDL_Texture *background_text = SDL_CreateTextureFromSurface(Renderer, background_surf);
    SDL_RenderCopy(Renderer, background_text, NULL, &background_rect);
    SDL_DestroyTexture(background_text);
    SDL_Texture *start_text[5] = {NULL};
    for (int i = 0; i < 5; i++) {
        start_text[i] = SDL_CreateTextureFromSurface(Renderer, start_surf[i]);
        SDL_RenderCopy(Renderer, start_text[i], NULL, &start_rect[i]);
        SDL_DestroyTexture(start_text[i]);
    }
    SDL_RenderPresent(Renderer);
}

void click_chess(){
    if (play_event.button.x > 400 && play_event.button.x < 1060 && play_event.button.y > 30
        && play_event.button.y < 800) {//在棋盘内部点击
        if (judge_do == 0) {//将要选子
            piece_x1 = (play_event.button.x - 455 + unit_length / 2) / 70;//计算x坐标
            if (play_event.button.y < 605) {//计算y坐标
                piece_y1 = (play_event.button.y - 85 + unit_length / 2) / 70;
            } else {
                piece_y1 = (play_event.button.y - 115 + unit_length / 2) / 70;
            }
            //printf("chose:(%d,%d)\n", piece_x1, piece_y1);
            if (record_location[piece_y1][piece_x1] != 0) {
                if_frame = 1;
                Mix_PlayChannel(-1,chose,0);
            }
        } else {//将要落子
            piece_x2 = (play_event.button.x - 455 + unit_length / 2) / 70;//计算x坐标
            if (play_event.button.y < 605) {//计算y坐标
                piece_y2 = (play_event.button.y - 85 + unit_length / 2) / 70;
            } else {
                piece_y2 = (play_event.button.y - 115 + unit_length / 2) / 70;
            }
            if (play_event.button.button == SDL_BUTTON_RIGHT) {//如果选错了
                judge_do = 0;
                if_frame = 0;
            }
        }
    } else {//棋盘外部点击
        if (judge_do != 0 && play_event.button.button == SDL_BUTTON_RIGHT) {
            judge_do = 0;
            if_frame = 0;
        }
    }
}
void admit_defeat();

void restart() {
    step = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 9; j++) {
            record_location[i][j] = init_location[i][j];
        }
    }
    round = 0;
    judge_do = 0;
    if_frame = 0;
    if_end=0;
    piece_x1 = 0, piece_y1 = 1;//x1,y1表示选子
    piece_x2 = 0, piece_y2 = 1;//x2,y2表示落子
    x3 = 4, y3 = 0;//黑将
    x4 = 4, y4 = 9;//红帅
    if( Mix_PausedMusic() == 1 )
    {
        Mix_ResumeMusic();
    }

}
void zu_to_che() {
    Mix_PlayChannel(-1,do_piece,0);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 9; j++) {
            if (record_location[i][j] == 1) {
                record_location[i][j] = 3;
            }
            if (record_location[i][j] == -1) {
                record_location[i][j] = -3;
            }
        }
    }
    if_check1();
    if_check2();
}

int rule_UI() {
    while (1) {
        SDL_RenderClear(Renderer);
        SDL_Texture *rule_text = SDL_CreateTextureFromSurface(Renderer, rule_surf);
        SDL_RenderCopy(Renderer, rule_text, NULL, &background_rect);
        SDL_DestroyTexture(rule_text);
        SDL_RenderPresent(Renderer);
        while (SDL_PollEvent(&rule_event)) {
            switch (rule_event.type) {
                case SDL_MOUSEBUTTONDOWN:return 0;
                case SDL_QUIT:return 1;
                default:break;
            }
        }
    }
}
void print_play_func() {
    //背景
    SDL_Texture *background_text = SDL_CreateTextureFromSurface(Renderer, background_surf);
    SDL_Texture *chess_board_text = SDL_CreateTextureFromSurface(Renderer, chess_board_surf);
    SDL_RenderCopy(Renderer, background_text, NULL, &background_rect);
    SDL_DestroyTexture(background_text);
    SDL_RenderCopy(Renderer, chess_board_text, NULL, &chess_board_rect);
    SDL_DestroyTexture(chess_board_text);
    //功能
    SDL_Texture *backboard_text = SDL_CreateTextureFromSurface(Renderer, backboard_surf);
    SDL_Texture *back_text = SDL_CreateTextureFromSurface(Renderer, back_surf);
    SDL_RenderCopy(Renderer, back_text, NULL, &back_rect);
    SDL_DestroyTexture(back_text);
    SDL_Texture *play_text[5] = {NULL};
    for (int i = 0; i < 5; i++) {
        play_text[i] = SDL_CreateTextureFromSurface(Renderer, play_surf[i]);
        SDL_RenderCopy(Renderer, backboard_text, NULL, &play_rect[i]);
        SDL_RenderCopy(Renderer, play_text[i], NULL, &play_rect[i]);
        SDL_DestroyTexture(play_text[i]);
    }
    SDL_DestroyTexture(backboard_text);
}
void print_chess() {
    //棋子图片加载
    SDL_Texture *piece_text[14] = {NULL};
    for (int i = 0; i < 14; i++) {
        piece_text[i] = SDL_CreateTextureFromSurface(Renderer, piece_surf[i]);
    }
    //棋子绘制
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 9; j++) {
            switch (record_location[i][j]) {
                case -1:SDL_RenderCopy(Renderer, piece_text[7], NULL, &piece_rect[i][j]);
                    break;
                case -2:SDL_RenderCopy(Renderer, piece_text[8], NULL, &piece_rect[i][j]);
                    break;
                case -3:SDL_RenderCopy(Renderer, piece_text[9], NULL, &piece_rect[i][j]);
                    break;
                case -4:SDL_RenderCopy(Renderer, piece_text[10], NULL, &piece_rect[i][j]);
                    break;
                case -5:SDL_RenderCopy(Renderer, piece_text[11], NULL, &piece_rect[i][j]);
                    break;
                case -6:SDL_RenderCopy(Renderer, piece_text[12], NULL, &piece_rect[i][j]);
                    break;
                case -7:SDL_RenderCopy(Renderer, piece_text[13], NULL, &piece_rect[i][j]);
                    break;
                case 1:SDL_RenderCopy(Renderer, piece_text[0], NULL, &piece_rect[i][j]);
                    break;
                case 2:SDL_RenderCopy(Renderer, piece_text[1], NULL, &piece_rect[i][j]);
                    break;
                case 3:SDL_RenderCopy(Renderer, piece_text[2], NULL, &piece_rect[i][j]);
                    break;
                case 4:SDL_RenderCopy(Renderer, piece_text[3], NULL, &piece_rect[i][j]);
                    break;
                case 5:SDL_RenderCopy(Renderer, piece_text[4], NULL, &piece_rect[i][j]);
                    break;
                case 6:SDL_RenderCopy(Renderer, piece_text[5], NULL, &piece_rect[i][j]);
                    break;
                case 7:SDL_RenderCopy(Renderer, piece_text[6], NULL, &piece_rect[i][j]);
                    break;
                default:break;
            }
        }
    }
    for (int i = 0; i < 14; i++) {
        SDL_DestroyTexture(piece_text[i]);
    }

}
int check_move(int x1, int y1, int x2, int y2) {
    int count = 0;
    if (x1 == x2 && y1 == y2) {//不能原地踏步
        return 0;
    } else {
        switch (record_location[y1][x1]) {
            case 4://红马
                //先限制八个位置
                if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) == 5) {
                    //分上下左右
                    if (y2 == y1 - 2) {//上
                        if (record_location[y1 - 1][x1] == 0 && record_location[y2][x2] <= 0) {
                            return 1;
                        } else { return 0; }
                    } else if (y2 == y1 + 2) {//下
                        if (record_location[y1 + 1][x1] == 0 && record_location[y2][x2] <= 0) {
                            return 1;
                        } else { return 0; }
                    } else if (x2 == x1 - 2) {//左
                        if (record_location[y1][x1 - 1] == 0 && record_location[y2][x2] <= 0) {
                            return 1;
                        } else { return 0; }
                    } else if (x2 == x1 + 2) {//右
                        if (record_location[y1][x1 + 1] == 0 && record_location[y2][x2] <= 0) {
                            return 1;
                        } else { return 0; }
                    }
                }
                return 0;
            case -4:    //黑马
                if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) == 5) { //先限制在八个位置
                    //分上下左右
                    if (y2 == y1 - 2) {//上
                        if (record_location[y1 - 1][x1] == 0 && record_location[y2][x2] >= 0) {
                            return 1;
                        }
                    } else if (y2 == y1 + 2) {//下
                        if (record_location[y1 + 1][x1] == 0 && record_location[y2][x2] >= 0) {
                            return 1;
                        }
                    } else if (x2 == x1 - 2) {//左
                        if (record_location[y1][x1 - 1] == 0 && record_location[y2][x2] >= 0) {
                            return 1;
                        }
                    } else if (x2 == x1 + 2) {//右
                        if (record_location[y1][x1 + 1] == 0 && record_location[y2][x2] >= 0) {
                            return 1;
                        }
                    }
                }
                return 0;
            case 5://红象
                if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) == 8) { //限制在4个位置
                    if (y2 >= 5) {//不越界
                        if (record_location[(y1 + y2) / 2][(x1 + x2) / 2] == 0 && record_location[y2][x2] <= 0) {
                            return 1;
                        }
                    }
                }
                return 0;
            case -5://黑象
                if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) == 8) { //限制在4个位置
                    if (y2 < 5) {//不越界
                        if (record_location[(y1 + y2) / 2][(x1 + x2) / 2] == 0 && record_location[y2][x2] >= 0) {
                            return 1;
                        }
                    }
                }
                return 0;
            case 6://红士
                if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) == 2) { //限制在4个位置
                    if (y2 >= 7 && y2 <= 9 && x2 >= 3 && x2 <= 5) {//九宫格内
                        if (record_location[y2][x2] <= 0) {
                            return 1;
                        }
                    }
                }
                return 0;
            case -6://黑士
                if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) == 2) { //限制在4个位置
                    if (y2 >= 0 && y2 <= 2 && x2 >= 3 && x2 <= 5) {//九宫格内
                        if (record_location[y2][x2] >= 0) {
                            return 1;
                        }
                    }
                }
                return 0;
            case 7://红帅
                if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) == 1) { //限制在4个位置
                    if (y2 >= 7 && y2 <= 9 && x2 >= 3 && x2 <= 5) {//九宫格内
                        if (record_location[y2][x2] <= 0) {
                            return 1;
                        }
                    }
                }
                if (x1 == x2) {
                    if (record_location[y2][x2] == -7) {
                        for (int i = y1 - 1; i > y2; i--) {
                            if (record_location[i][x1] != 0) {
                                count++;
                            }
                        }
                        if (count == 0) {
                            return 1;
                        }
                    }
                }
                return 0;
            case -7://黑将
                if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) == 1) { //限制在4个位置
                    if (y2 >= 0 && y2 <= 2 && x2 >= 3 && x2 <= 5) {//九宫格内
                        if (record_location[y2][x2] >= 0) {
                            return 1;
                        }
                    }
                }
                if (x1 == x2) {
                    if (record_location[y2][x2] == 7) {
                        for (int i = y1 + 1; i < y2; i++) {
                            if (record_location[i][x1] != 0) {
                                count++;
                            }
                        }
                        if (count == 0) {
                            return 1;
                        }
                    }
                }
                return 0;
            case 1://红兵
                if (y1 >= 5) {//没过河
                    if (y2 == y1 - 1 && x1 == x2 && record_location[y2][x2] <= 0) {
                        return 1;
                    } else { return 0; }
                } else {//已过河
                    if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) == 1 && y2 != y1 + 1
                        && record_location[y2][x2] <= 0) {
                        return 1;
                    } else { return 0; }
                }
            case -1://黑卒
                if (y1 < 5) {//没过河
                    if (y2 == y1 + 1 && x1 == x2 && record_location[y2][x2] >= 0) {
                        return 1;
                    } else { return 0; }
                } else {//已过河
                    if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) == 1 && y2 != y1 - 1
                        && record_location[y2][x2] >= 0) {
                        return 1;
                    } else { return 0; }
                }
            case 3://红车
                if (x1 == x2) {//竖着走
                    if (y2 < y1) {//上
                        for (int i = y2 + 1; i < y1; i++) {
                            if (record_location[i][x1] != 0) {
                                count++;
                            }
                        }
                        if (count == 0 && record_location[y2][x2] <= 0) {
                            return 1;
                        } else { return 0; }
                    } else {//下
                        for (int i = y1 + 1; i < y2; i++) {
                            if (record_location[i][x1] != 0) {
                                count++;
                            }
                        }
                        if (count == 0 && record_location[y2][x2] <= 0) {
                            return 1;
                        } else { return 0; }
                    }
                } else if (y1 == y2) {//横着走
                    if (x2 > x1) {//右
                        for (int i = x1 + 1; i < x2; i++) {
                            if (record_location[y1][i] != 0) {
                                count++;
                            }
                        }
                        if (count == 0 && record_location[y2][x2] <= 0) {
                            return 1;
                        } else { return 0; }
                    } else {//左
                        for (int i = x2 + 1; i < x1; i++) {
                            if (record_location[y1][i] != 0) {
                                count++;
                            }
                        }
                        if (count == 0 && record_location[y2][x2] <= 0) {
                            return 1;
                        } else { return 0; }
                    }
                } else { return 0; }
            case -3://黑车
                if (x1 == x2) {//竖着走
                    if (y2 < y1) {//上
                        for (int i = y2 + 1; i < y1; i++) {
                            if (record_location[i][x1] != 0) {
                                count++;
                            }
                        }
                        if (count == 0 && record_location[y2][x2] >= 0) {
                            return 1;
                        } else { return 0; }
                    } else {//下
                        for (int i = y1 + 1; i < y2; i++) {
                            if (record_location[i][x1] != 0) {
                                count++;
                            }
                        }
                        if (count == 0 && record_location[y2][x2] >= 0) {
                            return 1;
                        } else { return 0; }
                    }
                } else if (y1 == y2) {//横着走
                    if (x2 > x1) {//右
                        for (int i = x1 + 1; i < x2; i++) {
                            if (record_location[y1][i] != 0) {
                                count++;
                            }
                        }
                        if (count == 0 && record_location[y2][x2] >= 0) {
                            return 1;
                        } else { return 0; }
                    } else {//左
                        for (int i = x2 + 1; i < x1; i++) {
                            if (record_location[y1][i] != 0) {
                                count++;
                            }
                        }
                        if (count == 0 && record_location[y2][x2] >= 0) {
                            return 1;
                        } else { return 0; }
                    }
                } else { return 0; }
            case 2://红炮
                if (x1 == x2) {//竖着走
                    if (y2 < y1) {//上
                        for (int i = y2 + 1; i < y1; i++) {
                            if (record_location[i][x1] != 0) {
                                count++;
                            }
                        }//  只移动                                    吃子
                        if ((count == 0 && record_location[y2][x2] == 0)
                            || (record_location[y2][x2] < 0 && count == 1)) {
                            return 1;
                        } else {
                            //printf("hong pao move up wrong\n");
                            return 0;
                        }
                    } else {//下
                        for (int i = y1 + 1; i < y2; i++) {
                            if (record_location[i][x1] != 0) {
                                count++;
                            }
                        }//  只移动                                    吃子
                        if ((count == 0 && record_location[y2][x2] == 0)
                            || (record_location[y2][x2] < 0 && count == 1)) {
                            return 1;
                        } else { return 0; }
                    }
                } else if (y1 == y2) {//横着走
                    if (x2 > x1) {//右
                        for (int i = x1 + 1; i < x2; i++) {
                            if (record_location[y1][i] != 0) {
                                count++;
                            }
                        }//  只移动                                    吃子
                        if ((count == 0 && record_location[y2][x2] == 0)
                            || (record_location[y2][x2] < 0 && count == 1)) {
                            return 1;
                        } else { return 0; }
                    } else {//左
                        for (int i = x2 + 1; i < x1; i++) {
                            if (record_location[y1][i] != 0) {
                                count++;
                            }
                        }//  只移动                                    吃子
                        if ((count == 0 && record_location[y2][x2] == 0)
                            || (record_location[y2][x2] < 0 && count == 1)) {
                            return 1;
                        } else { return 0; }
                    }
                } else { return 0; }
            case -2://黑炮
                if (x1 == x2) {//竖着走
                    if (y2 < y1) {//上
                        for (int i = y2 + 1; i < y1; i++) {
                            if (record_location[i][x1] != 0) {
                                count++;
                            }
                        }//  只移动                                    吃子
                        if ((count == 0 && record_location[y2][x2] == 0)
                            || (record_location[y2][x2] > 0 && count == 1)) {
                            return 1;
                        } else { return 0; }
                    } else {//下
                        for (int i = y1 + 1; i < y2; i++) {
                            if (record_location[i][x1] != 0) {
                                count++;
                            }
                        }//  只移动                                    吃子
                        if ((count == 0 && record_location[y2][x2] == 0)
                            || (record_location[y2][x2] > 0 && count == 1)) {
                            return 1;
                        } else { return 0; }
                    }
                } else if (y1 == y2) {//横着走
                    if (x2 > x1) {//右
                        for (int i = x1 + 1; i < x2; i++) {
                            if (record_location[y1][i] != 0) {
                                count++;
                            }
                        }//  只移动                                    吃子
                        if ((count == 0 && record_location[y2][x2] == 0)
                            || (record_location[y2][x2] > 0 && count == 1)) {
                            return 1;
                        } else { return 0; }
                    } else {//左
                        for (int i = x2 + 1; i < x1; i++) {
                            if (record_location[y1][i] != 0) {
                                count++;
                            }
                        }//  只移动                                    吃子
                        if ((count == 0 && record_location[y2][x2] == 0)
                            || (record_location[y2][x2] > 0 && count == 1)) {
                            return 1;
                        } else { return 0; }
                    }
                } else { return 0; }

            default:break;
        }
    }
}
void print_tu_cao();
void print_frame();
void print_all() {
    print_play_func();
    print_tu_cao();
    print_chess();
    print_frame();
}

void change_location();
void single_change_location();
void check_win();
int play_UI_single() {
    Mix_PlayMusic(bgm,-1);
    while (1) {
        SDL_RenderClear(Renderer);
        single_change_location();
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
                        restart();
                        return 0;
                    }
                    //认输
                    if (play_event.button.x >= 110 && play_event.button.x <= 240 && play_event.button.y >= 225
                        && play_event.button.y <= 290) {
                        admit_defeat();
                        break;
                    }
                    //悔棋
                    if (play_event.button.x >= 110 && play_event.button.x <= 240 && play_event.button.y >= 375
                        && play_event.button.y <= 440) {
                        for (int i = 0; i < 10; i++) {
                            for (int j = 0; j < 9; j++) {
                                record_location[i][j] = record_step_location[step][i][j];
                            }
                        }
                        if(round==0){//回合转换
                            round=1;
                        } else{
                            round=0;
                        }
                        break;
                    }
                    //吐槽
                    if (play_event.button.x >= 110 && play_event.button.x <= 240 && play_event.button.y >= 525
                        && play_event.button.y <= 590) {
                        if_tu_cao=1;
                        break;
                    }
                    //天降“骑”兵
                    if (play_event.button.x >= 1170 && play_event.button.x <= 1400 && play_event.button.y >= 140
                        && play_event.button.y <= 205) {
                        zu_to_che();
                        break;
                    }
                    //重新开始
                    if (play_event.button.x >= 1155 && play_event.button.x <= 1355 && play_event.button.y >= 650
                        && play_event.button.y <= 715) {
                        restart();
                        break;
                    }
                    break;
                default:break;
            }
            break;
        }

    }
}
void main_loop();
void free_quit() {
    SDL_FreeSurface(background_surf);
    SDL_FreeSurface(chess_board_surf);
    SDL_FreeSurface(back_surf);
    SDL_FreeSurface(rule_surf);
    SDL_FreeSurface(backboard_surf);

    for (int i = 0; i < 14; i++) {
        SDL_FreeSurface(piece_surf[i]);
    }
    for (int i = 0; i < 5; i++) {
        SDL_FreeSurface(start_surf[i]);
    }
    for (int i = 0; i < 5; i++) {
        SDL_FreeSurface(play_surf[i]);
    }
    for (int i = 0; i < 2; i++) {
        SDL_FreeSurface(win_surf[i]);
    }
    for (int i = 0; i < 4; i++) {
        SDL_FreeSurface(tu_cao_surf[i]);
    }

    TTF_CloseFont(li_shu);
    TTF_CloseFont(kai_shu);

    Mix_HaltMusic();
    Mix_FreeMusic(bgm);
    Mix_FreeChunk(chose);
    Mix_FreeChunk(do_piece);
    Mix_FreeChunk(eat);
    Mix_FreeChunk(loss);
    Mix_FreeChunk(win);
    Mix_FreeChunk(jiang);
    Mix_Quit();

    WSACleanup();

    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();


}


//for socket
int play_UI_contact();
void check_buf();
static char server_ip[20]={0};
SOCKET server_socket;
void send_func(int send_x1,int send_y1,int send_x2,int send_y2,int send_kind);
void recv_thread();
void contact_with_server();
void recv_change_location();
void recv_defeat();
struct contact_buf{
  int x1;
  int y1;
  int x2;
  int y2;
  int func_kind;
};
struct contact_buf *recv_buf;
struct contact_buf send_buf;
int if_recv=0;

int ME,ANOTHER;
int func_kind=0;
int recv_x1=0,recv_y1=0;
int recv_x2=0,recv_y2=0;
void single_change_location(){
    if (judge_do != 0 && check_move(piece_x1, piece_y1, piece_x2, piece_y2) != 0) {
        //printf("drop:(%d,%d)\n", piece_x2, piece_y2);
        step+=1;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                record_step_location[step][i][j] = record_location[i][j];
            }
        }
        int change = record_location[piece_y1][piece_x1];
        record_location[piece_y1][piece_x1] = 0;
        record_location[piece_y2][piece_x2] = change;
        if(round==0){//回合转换
            round=1;
        } else{
            round=0;
        }
        Mix_PlayChannel(-1,do_piece,0);
        if_check1();//检查黑方是否被将
        if_check2();//检查红方是否被将
        if (if_tu_cao == 1) {
            if_tu_cao = 0;
        }
    }
}
void check_win() {
    int count = 0;
    //黑方胜
    for (int i = 0; i < 10; i++) {
        for (int j = 3; j <=5; j++) {
            if (record_location[i][j] == 7) {
                count++;
            }
        }
    }
    if (count == 0&&if_end==0) {
        SDL_Texture *black_win_text = SDL_CreateTextureFromSurface(Renderer, win_surf[0]);
        SDL_RenderCopy(Renderer, black_win_text, NULL, &win_rect);
        SDL_DestroyTexture(black_win_text);
        Mix_PauseMusic();
        if (ME==0){
            Mix_PlayChannel(-1,loss,0);
        }else{
            Mix_PlayChannel(-1,win,0);
        }
        if_end=1;
    }
    count = 0;
    //红方胜
    for (int i = 0; i < 10; i++) {
        for (int j = 3; j <=5; j++) {
            if (record_location[i][j] == -7) {
                count++;
            }
        }
    }
    if (count == 0&&if_end==0) {
        SDL_Texture *red_win_text = SDL_CreateTextureFromSurface(Renderer, win_surf[1]);
        SDL_RenderCopy(Renderer, red_win_text, NULL, &win_rect);
        SDL_DestroyTexture(red_win_text);
        Mix_PauseMusic();
        if (ME==1){
            Mix_PlayChannel(-1,loss,0);
        }else{
            Mix_PlayChannel(-1,win,0);
        }
        if_end=1;
    }
}
void print_tu_cao() {
    if (if_tu_cao != 0) {
        SDL_Texture *tu_cao_text = SDL_CreateTextureFromSurface(Renderer, tu_cao_surf[rand() % 5]);
        SDL_RenderCopy(Renderer, tu_cao_text, NULL, &tu_cao_rect);
        SDL_DestroyTexture(tu_cao_text);
    }
}
void step_back() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 9; j++) {
            record_location[i][j] = record_step_location[step][i][j];
        }
    }
    round = ME;
}
//回合转换
void change_location() {
    if (judge_do != 0 && check_move(piece_x1, piece_y1, piece_x2, piece_y2) != 0) {
        step++;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                record_step_location[step][i][j] = record_location[i][j];
            }
        }
        int change = record_location[piece_y1][piece_x1];
        record_location[piece_y1][piece_x1] = 0;
        record_location[piece_y2][piece_x2] = change;
        send_func(piece_x1,piece_y1,piece_x2,piece_y2,0);
        //printf("send one change:0\n");
        round=ANOTHER;//回合转换
        Mix_PlayChannel(-1,do_piece,0);
        if_check1();//检查黑方是否被将
        if_check2();//检查红方是否被将
        if (if_tu_cao == 1) {
            if_tu_cao = 0;
        }
    }
}
void print_frame() {
    SDL_Texture *frame_text = SDL_CreateTextureFromSurface(Renderer, frame_surf);
    if (judge_do == 0) {//选择时的边框
        if ((round == 0 && record_location[piece_y1][piece_x1] > 0)
            || (round == 1 && record_location[piece_y1][piece_x1] < 0)) {
            if (if_frame != 0 && record_location[piece_y1][piece_x1] != 0) {
                SDL_RenderCopy(Renderer, frame_text, NULL, &piece_rect[piece_y1][piece_x1]);
                judge_do = 1;
            }
        }
    } else {
        if (check_move(piece_x1, piece_y1, piece_x2, piece_y2) != 0) {
            SDL_RenderCopy(Renderer, frame_text, NULL, &piece_rect[piece_y2][piece_x2]);
            judge_do = 0;
        }else{
            SDL_RenderCopy(Renderer, frame_text, NULL, &piece_rect[piece_y1][piece_x1]);
        }
    }
    SDL_DestroyTexture(frame_text);
}
//回合转换
void check_buf() {
    if (if_recv==1){
        switch (func_kind) {
            case 0:recv_change_location();
                round=ME;
                break;
            case 1:break;
            case 2:recv_defeat();
                break;
            case 3:step_back();
                break;
            case 4:if_tu_cao=1;
                break;
            case 5:zu_to_che();
                break;
            case 6:restart();
                break;
            default:break;
        }
        if_recv=0;
    }
}
#endif //PROJECT_2_SRC_CHESS_H_
