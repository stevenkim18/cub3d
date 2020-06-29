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

// 이미지 구조체
typedef struct s_img
{
    void    *img;
    int     *data;

    int     sl;
    int     bpp;
    int     e;
}               t_img;

// 플레이어 구조체
typedef struct s_player
{
    int     x;      // 플레이어 시작 x좌표
    int     y;      // 플레이어 시작 y좌표
    int     r;      // 플레이어 원 반지름
}              t_player;

// 게임 구조체
typedef struct  s_game
{
    void        *mlx;
    void        *win;
    t_img       img;

    int         map[ROWS][COLS];

    t_player    *player;
}               t_game;

// 키를 처리 하는 함수(여기서는 esc키만 처리)
int handlekeys(int keycode, t_game *game)
{   
    if (keycode == ESC)
        exit(0);
    else if (keycode == W)
    {
        game->player->y--;
        printf("W y = %d\n", game->player->y);
    }
    else if (keycode == S)
        game->player->y++;
    else if (keycode == A)
        game->player->x--;
    else if (keycode == D)
        game->player->x++;   
    return (0);
}

// 정사각형 행렬 만들기
void    draw_square_martix(int *data)
{
    int     x;
    int     y;

     // 정사각형들 그리기(ROWS * COLS 정사각형 행렬)
    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            if (x % TILE_SIZE)
            {
                if ((y % TILE_SIZE) == 0)
                    data[x + y * WIDTH] = GRAY;     // 가로 선
                //else
                //    data[x + y * WIDTH] = WHITE;     // 나머지 흰색으로 채우기
            }
            else
                data[x + y * WIDTH] = GRAY;     // 세로 선
            x++;
        }
        y++;
    }
}

void    fill_map(t_game *game)
{
    int tmp[ROWS][COLS] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};
    memcpy(game->map, tmp, sizeof(int) * ROWS * COLS);
}

// 각 정사각형 안은 색으로 채우는 함수
void    fill_square(int *data, int row, int col, int wall)
{   
    int     x;
    int     y;

    y = row * TILE_SIZE;
    while (y < (row + 1) * TILE_SIZE)
    {
        x = col * TILE_SIZE;
        while (x < (col + 1) * TILE_SIZE)
        {   
            if (wall)
                data[x + y * WIDTH] = BLACK;
            else
                data[x + y * WIDTH] = WHITE;
            x++;
        }
        y++;
    }
}

// 맵에 있는 요소들도 벽과 빈공간 그리는 함수
void    draw_map(t_game *game)
{
    int     x;
    int     y;

    y = 0;
    while (y < ROWS)
    {
        x = 0;
        while (x < COLS)
        {
            fill_square(game->img.data, y, x, game->map[y][x]);
            x++;
        }
        y++;
    }
}

// 빨간 원 그리기
void    draw_circle(t_game *game)
{
    int     x;
    int     y;
    int     r;
    int     diff_x;
    int     diff_y;

    r = game->player->r;
    y = game->player->y - r;
    while (y <= game->player->y + r)
    {
        x = game->player->x - r;
        while (x <= game->player->y + r)
        {
            diff_x = x - game->player->y;
            diff_y = y - game->player->y;
            if (diff_x * diff_x + diff_y * diff_y <= r * r)
                game->img.data[x + y * WIDTH] = RED;
            x++;
        }
        y++;
    }
}

// 플레이어 그리기
void    draw_player(t_game *game)
{
    // 플레이어 위치 초기화 (화면 정 가운데)
    game->player->x = WIDTH / 2;   
    game->player->y = HEIGHT / 2;
    game->player->r = 5;

    draw_circle(game);
}

//  창을 새로 그리기
int    update_window(t_game *game)
{
    // 지도에 요소 채우기
    draw_map(game);
    
    // 구분선 그리기
    draw_square_martix(game->img.data);

    // 플레이어 그리기
    draw_player(game);

    // 이미지 화면에 보이기
    mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
    return (0);
}

int main(void)
{
    t_game  *game;

    game = (t_game*)malloc(sizeof(t_game));

    // mlx, win, img 초기화 및 기본 설정
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "draw square matrix");
    game->img.img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->img.data = (int *)mlx_get_data_addr(game->img.img, &game->img.bpp, &game->img.sl, &game->img.e);
    game->player = (t_player*)malloc(sizeof(t_player));

    fill_map(game);

    // 키 처리 하기 위한 이벤트 처리
    mlx_hook(game->win, X_EVENT_KEY_PRESS, 0, &handlekeys, game);

    mlx_loop_hook(game->mlx, &update_window, game);

    mlx_loop(game->mlx);
}