/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-thom <ede-thom@42.edu.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 20:48:16 by ede-thom          #+#    #+#             */
/*   Updated: 2020/10/12 20:49:00 by ede-thom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_H
# define UTILITY_H

# include "rt_objs.h"

typedef struct	s_parse_args
{
	char	name[MAX_FIGURE_NAME_LENGTH];
	float	args[MAX_PARSE_FIGURE_ARGUMENTS + 5];
	int		size;
}				t_parse_args;

int				name_cmp(char *fixed, char *var);
float			ft_atof(char *nb);

#endif
