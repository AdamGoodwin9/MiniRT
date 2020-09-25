/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cyl_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-thom <ede-thom@42.edu.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 18:01:17 by dhorvill          #+#    #+#             */
/*   Updated: 2020/09/24 01:40:11 by ede-thom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char*		check_cyl_args(t_parse_args parsed)
{
	float args[MAX_PARSE_FIGURE_ARGUMENTS];

	ft_memcpy(args, parsed.args, parsed.size * sizeof(float));
	if (parsed.size < 11)
		return ("Not enough arguments for a cylinder.");
    if (args[3] > 1  || args[4] > 1  || args[5] > 1 ||
		args[3] < -1 || args[4] < -1 || args[5] < -1)
		return ("Cylinder orientation vector values not in range (-1, 1).");
	if (args[6] < 0)
		return ("Cylinder diameter is negative");
    if (args[7] < 0)
		return ("Cylinder height is negative");
	if (args[8] > 255 || args[9] > 255 || args[10] > 255 ||
		args[8] < 0   || args[9] < 0   || args[10] < 0)
		return ("Invalid RGB color values for cylinder (must be values between 0 and 255 per color).");
	return (NULL);
}

t_cyl	create_cyl(t_parse_args parsed)
{
	char *msg;
	t_cyl cyl;

	msg = check_cyl_args(parsed);
	if (msg != NULL)
		clean_exit(1, msg);
	cyl = (t_cyl)create_hcyl(parsed);
	cyl.intersection = cyl_intersection;
	cyl.eclipses = cyl_eclipses_light;
	cyl.get_normal_at = get_cyl_normal_vector;
	return (cyl);
}

t_point		cyl_intersection(t_cyl cyl, t_vect ray, t_point start)
{
    t_circle bottom;
    t_circle top;
    t_point cylI;
    t_point bottomI;
    t_point topI;

    bottom = cyl;
    top = cyl;
    top.center = add(cyl.center, scale(cyl.normal, cyl.length));
    bottom.normal = scale(cyl.normal, -1);
	cylI = hcyl_intersection(cyl, ray, start);
    bottomI = circle_intersection(bottom, ray, start);
    topI = circle_intersection(top, ray, start);
	if (distance(cylI, start) > distance((distance(bottomI, start) < distance(topI, start) ? bottomI : topI), start))
    {
        return (distance(bottomI, start) < distance(topI, start) ? bottomI : topI);
    }
	return (cylI);
}

t_vect  get_cyl_normal_vector(t_vect inter, t_figure cyl, t_point start)
{
    t_circle bottom;
    t_circle top;
    t_point cylI;
    t_point bottomI;
    t_point topI;
	t_figure part_hit;

    bottom = cyl;
    top = cyl;
    top.center = add(cyl.center, scale(cyl.normal, cyl.length));
    bottom.normal = scale(cyl.normal, 1);
	top.normal = scale(cyl.normal, 1);
	cyl.get_normal_at = get_hcyl_normal_vector;
	top.get_normal_at = get_circle_normal_vector;
	bottom.get_normal_at = get_circle_normal_vector;
	cylI = hcyl_intersection(cyl, vector(start, inter), start);
    bottomI = circle_intersection(bottom, vector(start, inter), start);
    topI = circle_intersection(top, vector(start, inter), start);
    if (distance(cylI, start) > distance((distance(bottomI, start) < distance(topI, start) ? bottomI : topI), start))
        part_hit = distance(bottomI, start) < distance(topI, start) ? bottom : top;
    else
		part_hit = cyl;
	return (part_hit.get_normal_at(inter, part_hit, start));
	
}

int		cyl_eclipses_light(t_point inter, t_cyl cyl, t_point spot)
{
	t_vect	inter_to_spot;
	t_vect	inter_to_cyl;
	t_vect	cyl_inter;

	if (cyl.is_reflective)
		return (0);
	inter_to_spot = substract(spot, inter);
	cyl_inter = cyl_intersection(cyl, normalize(inter_to_spot), inter);
	inter_to_cyl = substract(cyl_inter, inter);
	if (norm(inter_to_spot) < norm(inter_to_cyl))
		return(0);
	return (1);
}
