/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_tracer.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazuli <lazuli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 13:12:28 by lyoung            #+#    #+#             */
/*   Updated: 2018/02/21 17:56:11 by lazuli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "mac-mlx/mlx.h"

#define RND (2.0*(float)rand()/RAND_MAX-1.0)
#define RND2 ((float)rand()/RAND_MAX)

#define WIN_W 1000
#define WIN_H 1000
#define SPP 5000
#define DEPTH 5
#define NUM_OBJ 10
#define H_FOV 2*M_PI * 60.0 / 360.0

typedef struct	s_vec
{
	float		x;
	float		y;
	float		z;
}				t_vec;

typedef struct	s_ray
{
	t_vec		o;
	t_vec		d;
}				t_ray;

typedef struct	s_clr
{
	double		r;
	double		g;
	double		b;
}				t_clr;

typedef struct	s_obj
{
	int			type;
	int			brdf;
	t_clr		emission;
	t_clr		clr;
	t_vec		c;
	float		r;
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
	float 		fov_x;
	float		fov_y;
	t_clr		pix[WIN_H][WIN_W];
}				t_env;

/*
** --------------- main.c --------------
*/

t_env	*init_env(void);

/*
** --------------- ray_trace.c --------------
*/

float	intersect_plane(t_ray ray, t_obj obj);
float	intersect_sphere(t_ray ray, t_obj obj);
t_vec	hemisphere(const float u1, const float u2);
t_clr	ray_trace(t_env *env, t_ray ray, int depth, t_clr clr);

/*
** --------------- vector.c --------------
*/

extern inline t_vec	new_vec(const float x, const float y, const float z);
extern inline t_vec	vec_norm(const t_vec vec);
extern inline t_vec	vec_add(const t_vec a, const t_vec b);
extern inline t_vec	vec_diff(const t_vec a, const t_vec b);
extern inline t_vec	vec_distr_mult(const float b, const t_vec a);
extern inline t_vec	vec_distr_div(const float b, const t_vec a);
extern inline t_vec	vec_mult(const t_vec a, const t_vec b);
extern inline float	vec_length(const t_vec vec);
extern inline float	vec_dot(const t_vec a, const t_vec b);

/*
** --------------- color.c --------------
*/

extern inline t_clr	new_clr(const double r, const double g, const double b);
extern inline t_clr	clr_mult(const t_clr a, const t_clr b);
extern inline t_clr	clr_add(const t_clr a, const t_clr b);
extern inline t_clr	clr_distr_div(const double b, const t_clr a);
extern inline int		clr_to_int(const t_clr clr);

/*
** --------------- scene.c --------------
*/

t_vec	get_cam_dir(t_env *env, const float x, const float y);
void	define_obj(t_obj *obj, int type, int brdf, double emission, t_clr clr, t_vec c, const float r);
t_obj	*init_obj(void);

/*
** --------------- utils.c --------------
*/

void	print_vec(t_vec vec);