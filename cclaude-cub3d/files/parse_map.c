/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seunkim <seunkim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 21:41:46 by cclaude           #+#    #+#             */
/*   Updated: 2020/06/29 15:32:38 by seunkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// xpm 파일 처리
int		ft_xpm(t_all *s, unsigned int **adr, char *file)
{
	int		fd;
	void	*img;
	int		tab[5];

	// xpm 확장자 확인
	if (ft_namecheck(file, "xpm") == 0)
		return (-1);
	// xpm 파일이 잘 열리는 지 확인 (파일이 안열리면 open 함수가 -1 리턴)
	if ((fd = open(file, O_RDONLY)) == -1)
		return (-1);
	close(fd);
	// tab[0] -> xpm 파일의 가로 길이 / tab[1] -> xpm 파일의 세로 길이
	img = mlx_xpm_file_to_image(s->mlx.ptr, file, &tab[0], &tab[1]);
	// xpm 파일에 가로 세로 길이가 64픽셀 같음.
	if (img == NULL || tab[0] != 64 || tab[1] != 64)
		return (-1);
	// tex.n tex.s tex.w tex.e tex.i에 xpm 이미지의 정보를(색 코드들) 넣어줌.
	*adr = (unsigned int *)mlx_get_data_addr(img, &tab[2], &tab[3], &tab[4]);
	free(img);
	return (0);
}

int		ft_texture(t_all *s, unsigned int **adr, char *line, int *i)
{
	char	*file;
	int		j;
	
	// tex.n, tex.s tex.w tex.e tex.i가 비어 있는지 확인.
	if (*adr != NULL)
		return (-7);	// "Error : Texture path specified twice"
	(*i) += 2;
	ft_spaceskip(line, i);
	j = *i;
	while (line[*i] != ' ' && line[*i] != '\0')
		(*i)++;
	if (!(file = malloc(sizeof(char) * (*i - j + 1))))
		return (-8); 	// "Error : Malloc fail (texture path)"
	*i = j;
	j = 0;
	// 파일 이름 복사
	while (line[*i] != ' ' && line[*i] != '\0')
		file[j++] = line[(*i)++];
	file[j] = '\0';
	j = ft_xpm(s, adr, file);
	free(file);
	return (j == -1 ? -9 : 0); // "Error : Invalid texture image"
}

int		ft_slablen(t_all *s, char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '0' || line[i] == '1' || line[i] == '2')
			count++;
		else if (line[i] == 'N' || line[i] == 'S' || line[i] == 'W')
			count++;
		else if (line[i] == 'E')
			count++;
		i++;
	}
	if (s->map.x != 0 && s->map.x != count)
		return (-1);
	return (count);
}

char	*ft_slab(t_all *s, char *line, int *i)
{
	char	*slab;
	int		j;

	// 맵의 가로 길이 만큼 메모리 할당
	if (!(slab = malloc(sizeof(char) * (ft_slablen(s, line) + 1))))
		return (NULL);
	j = 0;
	// 맵 한줄 파싱 해서 저장
	while (line[*i] != '\0')
	{	
		// 벽, 빈공간, 플레이어 처리
		if ((line[*i] == '0' || line[*i] == '1' || line[*i] == 'N')
			|| (line[*i] == 'E' || line[*i] == 'S' || line[*i] == 'W'))
			slab[j++] = line[*i];
		// 아이템 처리
		else if (line[*i] == '2')
		{
			slab[j++] = line[*i];
			s->map.spr++; // 아이템 개수 증가
		}
		else if (line[*i] != ' ')
		{
			free(slab);
			return (NULL);
		}
		(*i)++;
	}
	slab[j] = '\0';
	return (slab);
}

// 파싱한 맵을 구조체 변수에 저장하는 함수
int		ft_map(t_all *s, char *line, int *i)
{
	char	**tmp;
	int		j;

	s->err.m = 1;
	if (!(tmp = malloc(sizeof(char *) * (s->map.y + 2))))
		return (-11); // "Error : Malloc fail (map table)"
	j = -1;
	while (++j < s->map.y)
		tmp[j] = s->map.tab[j];
	if ((tmp[s->map.y] = ft_slab(s, line, i)) == NULL)
	{
		free(tmp);
		return (-12); // "Error : Invalid map"
	}
	tmp[s->map.y + 1] = NULL;
	if (s->map.y > 0)
		free(s->map.tab);
	s->map.tab = tmp;
	// 한줄 씩 읽어 올때 마다 맵의 세로 사이즈 1씩 증가
	s->map.y++;
	// 맵의 가로 사이즈를 저장!!!!!!!!!!!!
	if ((s->map.x = ft_slablen(s, line)) == -1)
		return (-13); // "Error : Map isn't a rectangle"
	return (0);
}
