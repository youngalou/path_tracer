/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_tracer.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazuli <lazuli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 13:12:28 by lyoung            #+#    #+#             */
/*   Updated: 2018/02/20 17:54:00 by lazuli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "mac-mlx/mlx.h"

#define RND (2.0*(double)rand()/RAND_MAX-1.0)
#define RND2 ((double)rand()/RAND_MAX)

#define WIN_W 1000
#define WIN_H 1000
#define SPP 20
#define DEPTH 5
#define NUM_OBJ 10
#define H_FOV 2*M_PI * 60.0 / 360.0

typedef struct	s_hal
{
	double		value;
	double		inv_base;
}				t_hal;

typedef struct	s_vec
{
	double		x;
	double		y;
	double		z;
}				t_vec;

typedef struct	s_ray
{
	t_vec		o;
	t_vec		d;
}				t_ray;

typedef struct	s_clr
{
	t_vec		mask;
	t_vec		clr;
}				t_clr;

typedef struct	s_obj
{
	int			type;
	int			brdf;
	t_vec		emission;
	t_vec		clr;
	t_vec		c;
	double		r;
}				t_obj;

typedef struct	s_env
{
	void		*mlx;
	void		*win;
	void		*img;
	int			*pixels;
	int			bpp;
	int			sl;
	int			endian;
	t_obj		*obj;
	t_vec		cam_pos;
	double 		fov_x;
	double		fov_y;
}				t_env;

/*
** --------------- main.c --------------
*/

t_obj	*init_obj(void);
t_env	*init_env(void);

/*
** --------------- vector.c --------------
*/

t_vec	new_vec(double x, double y, double z);
t_vec	vec_add(t_vec a, t_vec b);
t_vec	vec_diff(t_vec a, t_vec b);
t_vec	vec_distr_mult(double b, t_vec a);
t_vec	vec_distr_div(double b, t_vec a);
t_vec	vec_mult(t_vec a, t_vec b);
double	vec_length(t_vec vec);
double	vec_dot(t_vec a, t_vec b);
t_vec	vec_norm(t_vec vec);

/*
** --------------- scene.c --------------
*/

t_vec	get_cam_dir(t_env *env, const double x, const double y);
void	define_obj(t_obj *obj, int type, int brdf, double emission, t_vec clr, t_vec c, double r);
t_obj	*init_obj(void);

/*
** --------------- halton.c --------------
*/

t_hal	hal_number(int i, int base);
t_hal	hal_next(t_hal hal);

/*
** --------------- utils.c --------------
*/

void	print_vec(t_vec vec);