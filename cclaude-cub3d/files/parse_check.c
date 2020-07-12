/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seunkim <seunkim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/02 17:16:06 by cclaude           #+#    #+#             */
/*   Updated: 2020/06/29 15:21:21 by seunkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// 문자열 순서대로 비교해서 맞는지 확인(save = "--save"가 들어와서 "arg" 와 같은지 확인)
int		ft_savecheck(char *arg, char *save)
{
	int	i;

	i = 0;
	while (arg[i] == save[i])
	{
		if (arg[i] == '\0' && save[i] == '\0')
			return (1);
		i++;
	}
	return (0);
}

// 파일 이름 확인하는 함수(확장자가 cub, xpm 인지 확인)
int		ft_namecheck(char *arg, char *ext)
{
	int	i;

	i = 0;
	//파일 문자열을 맨 끝까지 보내고
	while (arg[i] != '\0')
		i++;
	// 끝에서 부터 ".cub"인지 확인
	if ((i > 4 && arg[i - 1] == ext[2] && arg[i - 2] == ext[1])
		&& (arg[i - 3] == ext[0] && arg[i - 4] == '.'))
		return (1);
	return (0);
}

// 맵 체크
int		ft_mapcheck(t_all *s)
{
	int		i;
	int		j;	

	i = 0;
	j = 0;
	while (i < s->map.y)
	{
		j = 0;
		while (j < s->map.x)
		{
			// 맵이 벽으로 둘려 쌓여 있는지 확인
			if (s->map.tab[i][j] != '1' && i == 0)
				return (-1);
			else if (s->map.tab[i][j] != '1' && i == s->map.y - 1)
				return (-1);
			else if (s->map.tab[i][j] != '1' && j == 0)
				return (-1);
			else if (s->map.tab[i][j] != '1' && j == s->map.x - 1)
				return (-1);
			j++;
		}
		i++;
	}
	return (1);
}

int		ft_parcheck(t_all *s)
{
	if (s->win.x <= 0 || s->win.y <= 0)
		return (ft_strerror(-14));		// 해상도 애러
	else if ((s->tex.n == NULL || s->tex.s == NULL || s->tex.e == NULL)
			|| (s->tex.w == NULL || s->tex.i == NULL))
		return (ft_strerror(-15)); 		// 벽 이미지 불러오기 에러
	else if (s->tex.c == NONE || s->tex.f == NONE)
		return (ft_strerror(-16));		// 색 애러
	else if (s->err.p == 0)
		return (ft_strerror(-17));		// 시작 점 애러
	else if (s->err.p > 1)
		return (ft_strerror(-18));		// 시작 점이 여러개라서 애러(1개여야 함.)
	else if (ft_mapcheck(s) == -1)
		return (ft_strerror(-19));		// 맵이 벽으로 둘려 쌓여 있지 않음.
	return (1);
}
