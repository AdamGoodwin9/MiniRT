/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-thom <ede-thom@42.edu.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 12:54:07 by ede-thom          #+#    #+#             */
/*   Updated: 2020/09/04 08:54:48 by ede-thom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

float	ft_atof(char *nb)
{
	float	i;
	float	d;
	float	mult;
	int		neg;

	i = 0.0;
	d = 0.0;
	mult = 0.1;
	neg = 1;
	if (*nb == '-')
	{
		neg = -1;
		nb++;
	}
	while (*nb && *nb != '.')
		i = i * 10 + (*(nb++) - '0');
	if (*nb == '.')
		nb++;
	while (*nb)
	{
		d += (*(nb++) - '0') * mult;
		mult *= 0.1;
	}
	return ((i + d) * neg);
}
