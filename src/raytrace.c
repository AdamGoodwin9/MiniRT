/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytrace.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-thom <ede-thom@42.edu.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 19:15:30 by dhorvill          #+#    #+#             */
/*   Updated: 2020/06/16 17:12:38 by ede-thom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vect	**turn_ray_table(t_vect **ray_table, t_scene s,
			t_vect ref, float theta)
{
	int		i;
	int		j;

	i = -1;
	while (++i < s.resolution.y)
	{
		j = -1;
		while (++j < s.resolution.x)
		{
			ray_table[i][j] = apply_rotation(ray_table[i][j], ref, theta);
		}
	}
	return (ray_table);
}

t_vect	**finish_tracer(t_scene s, t_point start,
						t_vect step, t_vect **ray_table)
{
	int		i;
	int		j;
	float	theta;
	t_vect	rot_axis;

	i = -1;
	while (++i < s.resolution.y)
	{
		if (!(ray_table[i] = (t_vect*)malloc(sizeof(t_vect) * s.resolution.x)))
			clean_exit(1, "Malloc Failed");
		j = -1;
		while (++j < s.resolution.x)
		{
			ray_table[i][j].x = start.x + step.x * (float)j;
			ray_table[i][j].y = start.y + step.y * (float)i;
			ray_table[i][j].z = 1;
			ray_table[i][j] = normalize(ray_table[i][j]);
		}
	}
	i = s.active_camera;
	theta = angle(new_vect(0, 0, 1), s.camera_list[i].orientation);
	rot_axis = cross(new_vect(0, 0, 1), s.camera_list[i].orientation);
	if (norm(rot_axis) < EPSILON)
		rot_axis = new_vect(0, 1, 0);
	return (turn_ray_table(ray_table, s, rot_axis, theta));
}

t_vect	**init_tracer(t_scene s)
{
	t_vect	**ray_table;
	t_point	start;
	t_point end;
	t_vect	step;
	int		i;

	if (!(ray_table = (t_vect**)malloc(sizeof(t_vect*) * s.resolution.y)))
		clean_exit(1, "Malloc failed");
	i = s.active_camera;
	start = s.camera_list[i].orientation;
	start.x = -sin(s.camera_list[i].fov / 2);
	start.y = sin(s.camera_list[i].fov / 2 * (s.resolution.y / s.resolution.x));
	start.z = 1;
	end.x = sin(s.camera_list[i].fov / 2);
	end.y = -sin(s.camera_list[i].fov / 2 * (s.resolution.y / s.resolution.x));
	end.z = 1;
	step.x = ((end.x - start.x) / (float)s.resolution.x);
	step.y = ((end.y - start.y) / (float)s.resolution.y);
	return (finish_tracer(s, start, step, ray_table));
}

int		color_shade(float intensity, t_figure figure, int reflective_color)
{
	t_color base;
	t_color	reflective_rgb;

	base = color_intensity(figure.color, intensity);
	reflective_rgb = int_to_rgb(0);
	if (figure.is_reflective > 0)
		reflective_rgb = int_to_rgb(reflective_color);
	if (figure.is_reflective == 0)
		return (rgb_to_int(base));
	return (weighted_average(base, reflective_rgb, figure.is_reflective));
}

float	get_lum_intensity(t_figure figure, t_point inter,
							t_point spotlight, t_point start)
{
	t_point	normal;
	t_point	ray_to_light;
	float	result;

	normal = figure.get_normal_at(inter, figure, start);
	ray_to_light = vector(inter, spotlight);
	if ((result = dot(normal, ray_to_light)) > 0)
		return (result);
	return (0);
}

int		trace_ray(t_vect ray, t_scene s, t_point start,
					int prev_index, int ignore)
{
	int			i;
	int			index;
	float		lum_intensity;
	float		closest_distance;
	float		distance;
	int			reflective_color;
	static int	current_recursion_depth;
	t_point		intersection;
	t_point		closest_intersection;
	t_vect		reflected_dir;
	t_vect		modified_start;

	current_recursion_depth = 0;
	if (++current_recursion_depth > MAX_RECURSION_DEPTH)
	{
		current_recursion_depth--;
		return (0);
	}
	closest_distance = RENDER_DISTANCE;
	i = -1;
	while (++i < s.figure_count)
	{
		if (i == prev_index && ignore)
			continue;
		intersection = s.figure_list[i].intersection(s.figure_list[i], ray, start);
		if ((distance = norm(true_vect(start, intersection))) < closest_distance && distance > MIN_RENDER_DIST)
		{
			index = i;
			closest_distance = distance;
			closest_intersection = intersection;
		}
	}
	if (closest_distance < RENDER_DISTANCE)
	{
		if (s.figure_list[index].is_reflective > 0)
		{
			reflected_dir = get_reflective_vector(s.figure_list[index], closest_intersection, ray, start);
			modified_start = add(closest_intersection, scale(reflected_dir, EPSILON));
			reflective_color = trace_ray(reflected_dir, s, modified_start, index, 0);
		}
		lum_intensity = get_lum_intensity(s.figure_list[index], closest_intersection, s.spotlight, start);
		lum_intensity = (1 - s.amb_light_ratio) * lum_intensity + s.amb_light_ratio;
		i = -1;
		while (++i < s.figure_count)
		{
			if (i == index)
				continue;
			if (figure_eclipses_light(closest_intersection, s.figure_list[i], s.spotlight))
			{
				lum_intensity = s.amb_light_ratio;
				break ;
			}
		}
		current_recursion_depth--;
		return (filter_color(color_shade(lum_intensity, s.figure_list[index], reflective_color), s.adj_light_color));
	}
	current_recursion_depth--;
	return (rgb_to_int(new_color(s.amb_light_color.x, s.amb_light_color.y, s.amb_light_color.z)));
}
