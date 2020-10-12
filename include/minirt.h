/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-thom <ede-thom@42.edu.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 20:17:20 by ede-thom          #+#    #+#             */
/*   Updated: 2020/10/12 20:31:05 by ede-thom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINIRT_H
# define MINIRT_H

# include <math.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# include "libft.h"
# include "mlx.h"
# include "rt_vectors.h"
# include "rt_colors.h"
#include "mlx_stuff.h"


t_sphere	create_sphere(t_parse_args parsed);
t_plane		create_plane(t_parse_args parsed);
t_hcyl		create_hcyl(t_parse_args parsed);
t_cyl		create_cyl(t_parse_args parsed);
t_circle	create_circle(t_parse_args parsed);
t_square	create_square(t_parse_args parsed);
t_triangle	create_triangle(t_parse_args parsed);

t_point		sphere_intersection(t_sphere sphere, t_vect ray, t_point start);
t_point		plane_intersection(t_plane plane, t_vect ray, t_point start);
t_point		hcyl_intersection(t_hcyl hcyl, t_vect ray, t_point start);
t_point		cyl_intersection(t_cyl cyl, t_vect ray, t_point start);
t_point		circle_intersection(t_circle circle, t_vect ray, t_point start);
t_point		square_intersection(t_square square, t_vect ray, t_point start);
t_point		triangle_intersection(t_triangle triangle, t_vect ray, t_point start);

t_vect 		get_sphere_normal_vector(t_vect inter, t_figure sphere, t_point start);
t_vect		get_plane_normal_vector(t_vect inter, t_figure plane, t_point start);
t_vect		get_hcyl_normal_vector(t_vect inter, t_figure hcyl, t_point start);
t_vect		get_cyl_normal_vector(t_vect inter, t_figure cyl, t_point start);
t_vect		get_circle_normal_vector(t_vect inter, t_figure circle, t_point start);
t_vect		get_square_normal_vector(t_vect inter, t_figure square, t_point start);
t_vect		get_triangle_normal_vector(t_vect inter, t_figure triangle, t_point start);

int			sphere_eclipses_light(t_point intersection, t_sphere, t_point spot);
int			plane_eclipses_light(t_point intersection, t_plane plane, t_point light);
int			hcyl_eclipses_light(t_point intersection, t_hcyl hcyl, t_point spot);
int			cyl_eclipses_light(t_point intersection, t_cyl cyl, t_point spot);
int			circle_eclipses_light(t_point intersection, t_circle circle, t_point spot);
int			square_eclipses_light(t_point intersection, t_square square, t_point spot);
int			triangle_eclipses_light(t_point intersection, t_triangle triangle, t_point spot);


void		render_frame(t_scene scene);
t_vect		**init_tracer(t_scene scene);
t_vect		scale(t_vect v, float scalar);
int			figure_eclipses_light(t_vect inter, t_figure shape, t_vect light);

t_vect		get_reflective_vector(t_figure figure, t_point inter, t_vect incident, t_point start);

int			name_cmp(char *fixed, char *var);
float		ft_atof(char *nb);

void		clean_exit(int status, char *msg);
t_scene		parse_scene(char *scene_file_path, t_drawable *drawable_list);
void		add_drawable(t_drawable **drawables, char *name, t_figure (*create_func)(t_parse_args parsed));
int			trace_ray(t_vect ray, t_scene scene, t_point start, int	prev_index, int ignore);

int			save_to_bmp(t_scene scene);


#endif
