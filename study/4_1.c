#include <stdio.h>
#include "../mlx/mlx.h"

int main(void)
{
    void    *mlx;
    void    *win;

    void    *img;

    int     *data;
    int     bpp;
    int     sl;
    int     e;

    int     x;
    int     y;

    // mlx 초기화
    mlx = mlx_init();
    // 창 초기화        가로(width), 세로(height)  
    win = mlx_new_window(mlx, 800, 600, "mlx");
    // 이미지 생성
    img = mlx_new_image(mlx, 400, 300);
    data = (int*)mlx_get_data_addr(img, &bpp, &sl, &e);

    // data에 400*300 = 120000개의 배열이 만들어 짐.
    // 첫번째 줄에 색을 채우려면 0 * 400 + (0 ~ 399) 에 색을 채우면 됨.
    // 마지막 줄에 색을 채우려면 299 * 400 + (0 ~ 399) 에 색을 채우면 됨.
    y = 0;
    while (y < 300)
    {
        x = 0;
        while (x < 400)
        {
            if (x % 2)
                data[y * 400 + x] = 0xFFFFFF;   // 흰색
            else
                data[y * 400 + x] = 0x000000;   // 검은색
        }
        y++;
    }
    // 이미지 창에 출력
    mlx_put_image_to_window(mlx, win, img, 0, 0);
    // 창 고정
    mlx_loop(mlx);
}