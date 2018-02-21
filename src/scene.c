/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazuli <lazuli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/16 16:59:28 by lazuli            #+#    #+#             */
/*   Updated: 2018/02/21 14:56:21 by lazuli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../path_tracer.h"

//get world vector from pixel position
t_vec	get_cam_dir(t_env *env, const float x, const float y)
{
	float	w = WIN_W;
	float	h = WIN_H;

	float	vec_x = ((2.0 * x - w) / w)*tan(env->fov_x);
	float	vec_y = (1 - ((2.0*y)/h)*tan(env->fov_y));
	float	vec_z = 1.0; //.5 / tan(env->fov_x/2);
	return new_vec(vec_x, vec_y, vec_z);
}

//type (1=plane, 2=sphere)
//brdf (1=diff, 2=spec, 3=light)
//plane:	c = normal vector,		r = position (float)-distance from origin on normal axis
//sphere:   c = center (pos vec), 	r = radius
void	define_obj(t_obj *obj, int type, int brdf, double emission, t_clr clr, t_vec c, float r)
{
	obj->type = type;
	obj->brdf = brdf;
	obj->emission.r = emission;
	obj->emission.g = emission;
	obj->emission.b = emission;
	obj->clr = clr;
	obj->c = c;
	obj->r = r;
}

t_obj	*init_obj(void)
{
	t_obj	*obj = malloc(sizeof(t_obj) * 10);
	define_obj(&obj[0], 1, 1, 0, new_clr(.3, .3, .3), new_vec(0, 1, 0), 5.5);				//floor plane
	define_obj(&obj[1], 1, 1, 0, new_clr(.3, .3, .3), new_vec(0, -1, 0), 2.5);				//ceiling plane
	define_obj(&obj[2], 1, 1, 0, new_clr(.3, .3, .3), new_vec(0, 0, 1), 5.5);				//front plane
	define_obj(&obj[3], 1, 1, 0, new_clr(.3, .3, .3), new_vec(0, 0, -1), 5.5);				//back plane
	define_obj(&obj[4], 1, 1, 0, new_clr(.4, .1, .1), new_vec(1, 0, 0), 5.5);				//left plane
	define_obj(&obj[5], 1, 1, 0, new_clr(.1, .1, .4), new_vec(-1, 0, 0), 5.5);				//right plane
	define_obj(&obj[6], 2, 1, 0, new_clr(.1, .4, .4), new_vec(-1.75, -4.5, 3.1), 0.6);		//left sphere
	define_obj(&obj[7], 2, 1, 0, new_clr(.4, .4, .1), new_vec(2.0, -5, 3.7), 0.5);			//right sphere
	define_obj(&obj[8], 2, 2, 0, new_clr(.1, .4, .1), new_vec(-.5, -4, 4.4), 1.05);			//middle sphere
	define_obj(&obj[9], 2, 3, 5.0, new_clr(1, 1, 1), new_vec(0, .5, 3.5), 0.5);			//light source
	return obj;
}