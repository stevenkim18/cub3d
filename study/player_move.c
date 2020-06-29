#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../mlx/mlx.h"

// mlx_hoop 설정 값
#define X_EVENT_KEY_PRESS 2

// 키 넘버
#define ESC 53
#define W   13
#define A   0
#define S   1
#define D   2

// 색 코드
#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define GRAY  0xB3B3B3
#define RED   0xFF0000

#define TILE_SIZE 30    // 정사각형 한변의 길이
#define ROWS 10         // 행(세로의 길이)
#define COLS 15        // 열(가로의 길이)

#define WIDTH (COLS * TILE_SIZE)      // 화면의 가로 길이
#define HEIGHT (ROWS * TILE_SIZE)     // 화면의 세로 길이

typedef struct  s_img
{
    void    *img;
    int     *data;

    int     sl;
    int     bpp;
    int     e;
}               t_img;

typedef struct  s_player
{
    int     x;
    int     y;
    int     r;
}               t_player;

typedef struct  s_game
{
    void        *mlx;
    void        *win;
    t_img       img;

    int         map[ROWS][COLS];

    t_player    *player;
}               t_game;

int main(void)
{
    t_game  game;

    game.mlx = mlx_init();
    game.win = mlx_new_window(game.mlx, WIDTH, HEIGHT, "example");
    game.img


}
