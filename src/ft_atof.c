/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-thom <ede-thom@42.edu.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 12:54:07 by ede-thom          #+#    #+#             */
/*   Updated: 2020/10/14 21:23:49 by ede-thom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	ft_atof(char *nb)
{
	return (atof(nb));
}

int		name_cmp(char *fixed, char *var)
{
	int len;

	len = ft_max(ft_strlen(var), ft_strlen(fixed));
	return (ft_strncmp(fixed, var, len));
}

int		tab_del_return(char **tab, int ret)
{
	char **tmp;

	tmp = tab;
	while (*tab)
	{
		free(*tab++);
	}
	free(tmp);
	return (ret);
}

int gamma_corrected(int color, double one_over_gamma)
{
	t_fcolor rgb;

	rgb = int_to_fcolor(color);

	rgb.red = pow(rgb.red, one_over_gamma);
	rgb.green = pow(rgb.green, one_over_gamma);
	rgb.blue = pow(rgb.blue, one_over_gamma);

	return (fcolor_to_int(rgb));
}