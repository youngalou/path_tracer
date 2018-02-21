/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazuli <lazuli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/14 17:09:25 by lyoung            #+#    #+#             */
/*   Updated: 2018/02/20 13:19:46 by lazuli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../path_tracer.h"

t_vec	new_vec(double x, double y, double z)
{
	t_vec	new;
	
	new.x = x;
	new.y = y;
	new.z = z;
	return new;
}

t_vec	vec_add(t_vec a, t_vec b)
{
	t_vec	res;

	res.x = a.x + b.x;
	res.y = a.y + b.y;
	res.z = a.z + b.z;
	return res;
}

t_vec	vec_diff(t_vec a, t_vec b)
{
	t_vec	res;

	res.x = a.x - b.x;
	res.y = a.y - b.y;
	res.z = a.z - b.z;
	return res;
}

t_vec	vec_distr_mult(double b, t_vec a)
{
	t_vec	res;

	res.x = a.x * b;
	res.y = a.y * b;
	res.z = a.z * b;
	return res;
}

t_vec	vec_distr_div(double b, t_vec a)
{
	t_vec	res;

	res.x = a.x / b;
	res.y = a.y / b;
	res.z = a.z / b;
	return res;
}

t_vec	vec_mult(t_vec a, t_vec b)
{
	t_vec	res;

	res.x = a.x * b.x;
	res.y = a.y * b.y;
	res.z = a.z * b.z;
	return res;
}

double	vec_length(t_vec vec)
{
	return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

double	vec_dot(t_vec a, t_vec b)
{
	return (a.x*b.x + a.y*b.y + a.z*b.z);
}

t_vec	vec_norm(t_vec vec)
{
	return vec_distr_mult((1 / vec_length(vec)), vec);
}