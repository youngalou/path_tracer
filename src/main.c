/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazuli <lazuli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 11:50:27 by lyoung            #+#    #+#             */
/*   Updated: 2018/02/21 14:36:21 by lazuli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../path_tracer.h"

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
	env->fov_y = ((float)WIN_H / (float)WIN_W) * env->fov_x;
	for (int y = 0; y < WIN_H; y++)
	{
		for (int x = 0; x < WIN_W; x++)
			env->pix[y][x] = new_clr(0, 0, 0);
	}
	srand(time(NULL));
	return env;
}

int		main(void)
{
	t_env	*env = init_env();
	
	for (int s = 1; s <= SPP; s++)
	{
		#pragma omp parallel for schedule(dynamic) collapse(2)
		for (int y = 0; y < WIN_H; y++)
		{
			for (int x = 0; x < WIN_W; x++)
			{
				t_vec	cam_dir = get_cam_dir(env, x, y);
				cam_dir.x = cam_dir.x + RND / 700;
				cam_dir.y = cam_dir.y + RND / 700;
				t_ray	ray;
				ray.o = env->cam_pos;
				ray.d = vec_norm(vec_diff(cam_dir, ray.o));
				env->pix[y][x] = clr_add(env->pix[y][x], ray_trace(env, ray, 0, new_clr(1, 1, 1)));
				env->pixels[x + (y * WIN_W)] = clr_to_int(clr_distr_div(s, env->pix[y][x]));
			}
		}
		mlx_do_sync(env->mlx);
		mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
	}
	mlx_loop(env->mlx);
	return (0);
}
