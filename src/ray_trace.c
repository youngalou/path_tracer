/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_trace.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazuli <lazuli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 13:33:32 by lazuli            #+#    #+#             */
/*   Updated: 2018/02/21 14:53:21 by lazuli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../path_tracer.h"

const float	g_inf = 1e9;
const float	g_eps = 1e-4;

float	intersect_plane(t_ray ray, t_obj obj)
{
	float	d0 = vec_dot(obj.c, ray.d);
	if (d0 != 0)
	{
		float	t = (-1 * ((vec_dot(obj.c, ray.o) + obj.r) / d0));
		return	(t > g_eps) ? t : 0;
	}
	else
		return 0;
}

float	intersect_sphere(t_ray ray, t_obj obj)
{
	t_vec	tmp = vec_diff(ray.o, obj.c);
	float	b = vec_dot(vec_distr_mult(2, tmp), ray.d);
	float	c = vec_dot(tmp, tmp) - (obj.r * obj.r);
	float	disc = (b * b) - (4 * c);
	if (disc < 0)
		return 0;
	disc = sqrt(disc);
	float sol1 = -b + disc;
	float sol2 = -b - disc;
	return (sol2 > g_eps) ? sol2 / 2 : ((sol1 > g_eps) ? sol1 / 2 : 0);
}

t_vec	hemisphere(float u1, float u2)
{
	const float r = sqrt(1.0 - u1*u1);
	const float phi = 2 * M_PI*u2;
	return new_vec(cos(phi)*r, sin(phi)*r, u1);
}

t_clr	ray_trace(t_env *env, t_ray ray, int depth, t_clr clr)
{
	float	dist = g_inf;
	float	t;
	t_obj	obj;
	if (depth >= DEPTH)
		return new_clr(.25, .25, .25);
	//intersection routine
	for (int i = 0; i < NUM_OBJ; i++)
	{
		t = ((env->obj[i].type == 1) ? intersect_plane(ray, env->obj[i]) : intersect_sphere(ray, env->obj[i]));
		if (t > g_eps && t < dist)
		{
			dist = t;
			obj = env->obj[i];
		}
	}
	ray.o = vec_distr_mult(dist, vec_add(ray.o, ray.d));
	t_vec	N = (obj.type == 1) ? obj.c : vec_norm(vec_diff(ray.o, obj.c));
	N = vec_dot(N, ray.d) > 0.0f ? vec_distr_mult(-1.0f, N) : N;

	t_clr	tmp = new_clr(1, 1, 1);
	if (obj.brdf == 1) //diffuse brdf
	{
		ray.d = vec_add(N, hemisphere(RND2, RND2));
		float	cost = vec_dot(ray.d, N);
		tmp = ray_trace(env, ray, depth + 1, tmp);
		clr.r = cost*(tmp.r*obj.clr.r);
		clr.g = cost*(tmp.g*obj.clr.g);
		clr.b = cost*(tmp.b*obj.clr.b);
		return clr;
	}
	if (obj.brdf == 2) //specular brdf
	{
		float	cost = vec_dot(ray.d, N);
		ray.d = vec_norm(vec_diff(ray.d, vec_distr_mult(cost*2, N)));
		tmp = ray_trace(env, ray, depth + 1, tmp);
		clr = clr_mult(clr, tmp);
		return clr;
	}
	if (obj.brdf == 3)
		return obj.emission;
	return clr;
}