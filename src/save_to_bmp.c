/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_to_bmp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-thom <ede-thom@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 19:29:35 by ede-thom          #+#    #+#             */
/*   Updated: 2019/12/08 15:22:53 by ede-thom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char	*build_str_name(char *radical, int f_no, int cam_no)
{
	char	*ret;
	char	*tmp;
	int		cur;

	if (!(ret = (char*)malloc(sizeof(char) * (ft_strlen(radical) + (int)(log10(f_no * cam_no + 1) + 2)))))
		clean_exit(1, "Malloc failed lmao");
	ft_memmove(ret, radical, ft_strlen(radical) + 1);
	cur = ft_indexof('.', ret) > -1 ? ft_indexof('.', ret) : ft_strlen(ret);

	ft_memmove(ret + cur, "_", 2);
	cur = ft_strlen(ret);
	
	if (!(tmp = ft_itoa(f_no)))
		clean_exit(1, "Malloc failed lmao");
	ft_memmove(ret + cur, tmp, ft_strlen(tmp) + 1);
	cur = ft_strlen(ret);
	free(tmp);

	ft_memmove(ret + cur, "_cam", 5);
	cur = ft_strlen(ret);

	if (!(tmp = ft_itoa(cam_no)))
		clean_exit(1, "Malloc failed lmao");
	ft_memmove(ret + cur, tmp, ft_strlen(tmp) + 1);
	cur = ft_strlen(ret);
	free(tmp);

	ft_memmove(ret + cur, ".bmp", 5);
	cur = ft_strlen(ret);
	ret[cur] = '\0';
	return (ret);
}

int		find_suffix(t_scene scene)
{
	int		s;
	int		fd;
	char	*tmp;

	s = -1;
	while (++s < 500)
	{
		tmp = build_str_name(scene.scene_name, s, 0);
		if ((fd = open(tmp, O_RDONLY)) == -1)
		{
			free(tmp);
			return (s);
		}
		free(tmp);
		close(fd);
	}
	clean_exit(1, "Failed to find a name for the save file (500 tries)");
	return (-1);
}

int		save_to_bmp(t_scene scene)
{
	int suffix;
	int cam_no;
	int fd;
	char *tmp;

	suffix = find_suffix(scene);
	cam_no = 0;
	while (cam_no < scene.camera_count)
	{
		tmp = build_str_name(scene.scene_name, suffix, cam_no);
		if ((fd = open(tmp, O_CREAT)) == -1)
			clean_exit(1, "Failed to generate file");
		printf("Created %s", tmp);
		free(tmp);
		close(fd);
		cam_no++;
	}
	
	return (0);
}
