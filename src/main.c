/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazuli <lazuli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 11:50:27 by lyoung            #+#    #+#             */
/*   Updated: 2018/02/20 18:19:56 by lazuli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../path_tracer.h"

const double	g_inf = 1e9;
const double	g_eps = 1e-4;

int		clr_to_int(t_vec clr)
{
	int	r, g, b;
	
	r = (clr.x < 1) ? clr.x * 255 : 255;
	g = (clr.y < 1) ? clr.y * 255 : 255;
	b = (clr.z < 1) ? clr.z * 255 : 255;
	return (r << 16) + (g << 8) + b;
}

t_env	*init_env(void)
{
	t_env	*env = malloc(sizeof(t_env));
	env->mlx = mlx_init();
	env->win = mlx_new_window(env->mlx, WIN_W, WIN_H, "Path Tracer");
	env->img = mlx_new_image(env->mlx, WIN_W, WIN_H);
	env->pixels = (int*)mlx_get_data_addr(env->img, &env->bpp, &env->sl, &env->endian);
	env->bpp = 4;
	env->sl = 0;
	env->endian = 0;
	env->obj = init_obj();
	env->cam_pos = new_vec(0, 0, 0);
	env->fov_x = H_FOV;
	env->fov_y = ((double)WIN_H / (double)WIN_W) * env->fov_x;
	srand(time(NULL));
	return env;
}

double	intersect_plane(t_ray ray, t_obj obj)
{
	double	d0 = vec_dot(obj.c, ray.d);
	if (d0 != 0)
	{
		double	t = (-1 * ((vec_dot(obj.c, ray.o) + obj.r) / d0));
		return	(t > g_eps) ? t : 0;
	}
	else
		return 0;
}

double	intersect_sphere(t_ray ray, t_obj obj)
{
	t_vec	tmp = vec_diff(ray.o, obj.c);
	double	b = vec_dot(vec_distr_mult(2, tmp), ray.d);
	double	c = vec_dot(tmp, tmp) - (obj.r * obj.r);
	double	disc = (b * b) - (4 * c);
	if (disc < 0)
		return 0;
	double sol1 = -b + disc;
	double sol2 = -b - disc;
	disc = sqrt(disc);
	return (sol2 > g_eps) ? sol2 / 2 : ((sol1 > g_eps) ? sol1 / 2 : 0);
}

t_vec	hemisphere(double u1, double u2)
{
	const double r = sqrt(1.0 - u1*u1);
	const double phi = 2 * M_PI*u2;
	return new_vec(cos(phi)*r, sin(phi)*r, u1);
}

t_vec	trace(t_env *env, t_ray ray, int depth, t_vec clr)
{
	double	dist = g_inf;
	double	t;
	t_obj	obj;
	if (depth >= DEPTH)
		return new_vec(.5, .5, .5);
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
	t_vec	hp = vec_distr_mult(dist, vec_add(ray.o, ray.d));
	t_vec	N = (obj.type == 1) ? obj.c : vec_norm(vec_diff(hp, obj.c));
	ray.o = hp;

	t_vec	tmp = new_vec(1, 1, 1);
	if (obj.brdf == 1) //diffuse brdf
	{
		ray.d = vec_add(N, hemisphere(RND2, RND2));
		double	cost = vec_dot(ray.d, N);
		tmp = trace(env, ray, depth + 1, tmp);
		clr.x = cost*(tmp.x*obj.clr.x);
		clr.y = cost*(tmp.y*obj.clr.y);
		clr.z = cost*(tmp.z*obj.clr.z);
		return clr;
	}
	if (obj.brdf == 2) //specular brdf
	{
		double	cost = vec_dot(ray.d, N);
		ray.d = vec_norm(vec_diff(ray.d, vec_distr_mult(cost*2, N)));
		tmp = trace(env, ray, depth + 1, tmp);
		clr = vec_mult(clr, tmp);
		return clr;
	}
	if (obj.brdf == 3)
		return obj.emission;
	return clr;
}

int		main(void)
{
	t_env	*env = init_env();
	
	for (int y = 0; y < WIN_H; y++)
	{
		for (int x = 0; x < WIN_W; x++)
		{
			t_vec	clr = new_vec(0, 0, 0);
			for (int s = 0; s < SPP; s++)
			{
				t_vec	cam_dir = get_cam_dir(env, x, y);
				cam_dir.x = cam_dir.x + RND / 700;
				cam_dir.y = cam_dir.y + RND / 700;
				t_ray	ray;
				ray.o = env->cam_pos;
				ray.d = vec_norm(vec_diff(cam_dir, ray.o));
				clr = vec_add(clr, trace(env, ray, 0, new_vec(1, 1, 1)));
			}
			clr = vec_distr_div(SPP, clr);
			int color = clr_to_int(clr);
			env->pixels[x + (y * WIN_W)] = color;
		}
	}
	mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
	mlx_loop(env->mlx);
	return (0);
}
