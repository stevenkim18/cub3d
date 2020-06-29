/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen_col.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seunkim <seunkim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 19:37:22 by cclaude           #+#    #+#             */
/*   Updated: 2020/06/29 14:57:47 by seunkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// 벽 이미지의 픽셀들을 리턴해주는 함수
unsigned int	ft_pixel(t_all *s, double i)
{
	int	index;

	if (floor(s->hit.y) == s->hit.y)
	{
		index = 64 * floor(64 * i) + 64 * (s->hit.x - floor(s->hit.x));
		if (s->ray.w == 1)
			return (s->tex.s[index]);	// 남
		else if (s->ray.w == 0)
			return (s->tex.n[index]);	// 북
	}
	else if (floor(s->hit.x) == s->hit.x)
	{
		index = 64 * floor(64 * i) + 64 * (s->hit.y - floor(s->hit.y));
		if (s->ray.v == 1)
			return (s->tex.e[index]);	// 동
		else if (s->ray.v == 0)
			return (s->tex.w[index]);	// 서
	}
	return (BLACK);
}

void			ft_column(t_all *s, int size)
{
	unsigned int	color;
	int				start;
	int				count;

	start = s->win.x * (s->win.y - size) / 2;
	if (size > s->win.y)
		count = (size - s->win.y) / 2;
	else
		count = 0;
	color = s->tex.c;
	while (s->ray.i < s->win.x * s->win.y)
	{
		if (s->ray.i >= start && count < size)
		{
			color = ft_pixel(s, (double)count / size);
			count++;
		}
		else if (count == size)
			color = s->tex.f;
		s->img.adr[s->ray.i] = mlx_get_color_value(s->mlx.ptr, color);
		s->ray.i += s->win.x;
	}
	s->ray.i -= s->win.x * s->win.y;
}

int				ft_size(t_all *s)
{
	double	correc;
	double	fisheye;

	// fabs : float 절대값
	fisheye = fabs((double)s->ray.i / (s->win.x / 2) - 1);
	fisheye *= 28 * M_PI / 180;
	correc = (double)s->hit.d * cos(fisheye);
	// round 반올림
	return (round(s->win.y / correc));
}

void			ft_stock(t_all *s)
{
	// index에 저장하기
	s->stk[s->ray.i].x = s->ray.x;
	s->stk[s->ray.i].y = s->ray.y;
	s->stk[s->ray.i].d = s->hit.d;
}
