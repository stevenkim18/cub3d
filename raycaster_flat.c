/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_flat.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seunkim <seunkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/27 01:50:15 by seunkim           #+#    #+#             */
/*   Updated: 2020/07/12 22:09:08 by seunkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "./mlx/mlx.h"

#define X_EVENT_KEY_PRESS		2
#define X_EVENT_KEY_EXIT		17 //창 닫기 버튼

#define ESC	53
#define W   13
#define A   0
#define S   1
#define D   2
#define LEFT 123
#define RIGHT 124

#define RED 0xff0000
#define DARKRED 0x6f0000

# define SPEED 0.15
# define TURN 0.05

#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

typedef struct  s_game
{
    void        *mlx;
    void        *win;

    void        *img;
    int		    *data;
	int		    size_l;
	int		    bpp;
	int		    endian;

    int         map[mapWidth][mapHeight];

    double      posX;
    double      posY;
    double      dirX;
    double      dirY;
    double      planeX;
    double      planeY;

    double      time;
    double      oldTime;

    double      cameraX;
    double      rayDirX;
    double      rayDirY;

    int         mapX;
    int         mapY;

    double      sideDistX;
    double      sideDistY;

    double      deltaDistX;
    double      deltaDistY;
    double      perpWallDist;

    int         stepX;
    int         stepY;

    int         hit;
    int         side;

    int         lineHeight;

    int         drawStart;
    int         drawEnd;

    double      oldDirX;
    double      oldPlaneX;
}               t_game;

int draw_map(t_game *game)
{
    int worldMap[mapWidth][mapHeight] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    memcpy(game->map, worldMap, sizeof(int) * mapWidth * mapHeight);
    return (1);
}

int     handlekey(int key, t_game *game)
{
        if (key == ESC)
            exit(0);
        else if (key == W)
        {
            if (game->map[(int)(game->posX + game->dirX * SPEED)][(int)game->posY] == 0)
                game->posX += game->dirX * SPEED;
            if (game->map[(int)game->posX][(int)(game->posY + game->dirY * SPEED)] == 0)
                game->posY += game->dirY * SPEED;
        }
        else if (key == S)
        {
            if (game->map[(int)(game->posX - game->dirX * SPEED)][(int)game->posY] == 0)
                game->posX -= game->dirX * SPEED;
            if (game->map[(int)game->posX][(int)(game->posY - game->dirY * SPEED)] == 0)
                game->posY -= game->dirY * SPEED;
        }
        else if (key == D)
        {
            if (game->map[(int)(game->posX + game->planeX * SPEED)][(int)game->posY] == 0)
                game->posX += game->planeX * SPEED;
            if (game->map[(int)game->posX][(int)(game->posY + game->planeY * SPEED)] == 0)
                game->posY += game->planeY * SPEED;
        }
        else if (key == A)
        {
            if (game->map[(int)(game->posX - game->dirX * SPEED)][(int)game->posY] == 0)
                game->posX -= game->planeX * SPEED;
            if (game->map[(int)game->posX][(int)(game->posY - game->planeY * SPEED)] == 0)
                game->posY -= game->planeY * SPEED;
        }
        else if (key == RIGHT)
        {
            game->oldDirX = game->dirX;
            game->dirX = game->dirX * cos(-TURN) - game->dirY * sin(-TURN);
            game->dirY = game->oldDirX * sin(-TURN) + game->dirY * cos(-TURN);

            game->oldPlaneX = game->planeX;
            game->planeX = game->planeX * cos(-TURN) - game->planeY * sin(-TURN);
            game->planeY = game->oldPlaneX * sin(-TURN) + game->planeY * cos(-TURN);
        }
        else if (key == LEFT)
        {
            game->oldDirX = game->dirX;
            game->dirX = game->dirX * cos(TURN) - game->dirY * sin(TURN);
            game->dirY = game->oldDirX * sin(TURN) + game->dirY * cos(TURN);

            game->oldPlaneX = game->planeX;
            game->planeX = game->planeX * cos(TURN) - game->planeY * sin(TURN);
            game->planeY = game->oldPlaneX * sin(TURN) + game->planeY * cos(TURN);
        }
        printf("pos x=%f y=%f\n", game->posX, game->posY);
        return (1);
}

int 	close(t_game *game)
{
		exit(0);
}

int     ft_draw(t_game *game)
{
    int     x;
    int     y;

    // 방햫키를 누를떄마다 새로 화면의 이미지를 갱신해줘야 함.
    game->img = mlx_new_image(game->mlx, screenWidth, screenHeight);
    game->data = (int *)mlx_get_data_addr(game->img, &game->bpp, &game->size_l, &game->endian);

    while (x < screenWidth)
    {
        // 카메라 평면 -1(화면 왼쪽) 에서 1(화면 오른쪽)
        game->cameraX = 2 * x / (double)screenWidth - 1;
        // 광선의 방향 백터
        game->rayDirX = game->dirX + game->planeX * game->cameraX;
        game->rayDirY = game->dirY + game->planeY * game->cameraX;

        // 현재 어떤 위치에 있는지.. 소수점을 없앰. (floor() 내림 함수 사용해도 됨.)
        game->mapX = (int)game->posX;
        game->mapY = (int)game->posY;

        game->deltaDistX = fabs(1 / game->rayDirX);
        game->deltaDistY = fabs(1 / game->rayDirY);

        game->hit = 0;

        if (game->rayDirX < 0)
        {
            game->stepX = -1;
            game->sideDistX = (game->posX - game->mapX) * game->deltaDistX;
        }
        else
        {
            game->stepX = 1;
            game->sideDistX = (game->mapX + 1.0 - game->posX) * game->deltaDistX;
        }
        if (game->rayDirY < 0)
        {
            game->stepY = -1;
            game->sideDistY = (game->posY - game->mapY) * game->deltaDistY;
        }
        else
        {
            game->stepY = 1;
            game->sideDistY = (game->mapY + 1.0 - game->posY) * game->deltaDistY;
        }

        // DDA
        while (game->hit == 0)
        {
            if (game->sideDistX < game->sideDistY)
            {   
                // x좌표에서 광선과 만나는 점과의 거리들을 더함.
                game->sideDistX += game->deltaDistX;
                game->mapX += game->stepX;
                game->side = 0;
            }
            else
            {
                game->sideDistY += game->deltaDistY;
                game->mapY += game->stepY;
                game->side = 1;
            }
            if (game->map[game->mapX][game->mapY] > 0)
                game->hit = 1;
        }
        
        if (game->side == 0)
            game->perpWallDist = (game->mapX - game->posX + (1 - game->stepX) / 2) / game->rayDirX;
        else
            game->perpWallDist = (game->mapY - game->posY + (! - game->stepY) / 2) / game->rayDirY;
        
        game->lineHeight = (int)(screenHeight / game->perpWallDist);

        game->drawStart = -game->lineHeight / 2 + screenHeight / 2;
        if (game->drawStart < 0)
            game->drawStart = 0;

        game->drawEnd = game->lineHeight / 2 + screenHeight / 2;
        if (game->drawEnd >= screenHeight)
            game->drawEnd = screenHeight - 1;
        
        y = game->drawStart;
        while (y <= game->drawEnd)
        {
            if (game->side == 1)
                game->data[x + y * screenWidth] = DARKRED; // y축은 어둡게
            else
                game->data[x + y * screenWidth] = RED;
            y++;
        }
        //printf("%d - start:%d end:%d\n", x, game->drawStart, game->drawEnd);
        x++;
    }

    // 이미지 창에 보여주기
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
    return (1);
}

int main(void)
{
    t_game      game;
    int         x;
    int         y;

    x = 0;
    game.mlx = mlx_init();
    game.win = mlx_new_window(game.mlx, screenWidth, screenHeight, "example");
    
    draw_map(&game);

    game.posX = 22;     // 현재위치
    game.posY = 12;      
    game.dirX = -1;     // 방향 백터
    game.dirY = 0;
    game.planeX = 0;    // 카메라 평면
    game.planeY = 0.66;
    game.time = 0;
    game.oldTime = 0;

    mlx_loop_hook(game.mlx, &ft_draw, &game);

    // 키보드 입력 처리
    mlx_hook(game.win, X_EVENT_KEY_PRESS, 0, &handlekey, &game);
    mlx_hook(game.win, X_EVENT_KEY_EXIT, 0, &close, &game);
    mlx_loop(game.mlx);
}
