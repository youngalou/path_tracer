/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazuli <lazuli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/14 17:09:25 by lyoung            #+#    #+#             */
/*   Updated: 2018/02/21 14:40:47 by lazuli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../path_tracer.h"

inline t_vec	new_vec(const float x, const float y, const float z)
{
	t_vec	new;
	
	new.x = x;
	new.y = y;
	new.z = z;
	return new;
}

inline t_vec	vec_add(const t_vec a, const t_vec b)
{
	t_vec	res;

	res.x = a.x + b.x;
	res.y = a.y + b.y;
	res.z = a.z + b.z;
	return res;
}

inline t_vec	vec_diff(const t_vec a, const t_vec b)
{
	t_vec	res;

	res.x = a.x - b.x;
	res.y = a.y - b.y;
	res.z = a.z - b.z;
	return res;
}

inline t_vec	vec_distr_mult(const float b, const t_vec a)
{
	t_vec	res;

	res.x = a.x * b;
	res.y = a.y * b;
	res.z = a.z * b;
	return res;
}

inline t_vec	vec_distr_div(const float b, const t_vec a)
{
	t_vec	res;

	res.x = a.x / b;
	res.y = a.y / b;
	res.z = a.z / b;
	return res;
}

inline t_vec	vec_mult(const t_vec a, const t_vec b)
{
	t_vec	res;

	res.x = a.x * b.x;
	res.y = a.y * b.y;
	res.z = a.z * b.z;
	return res;
}

inline float	vec_length(const t_vec vec)
{
	return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

inline float	vec_dot(const t_vec a, const t_vec b)
{
	return (a.x*b.x + a.y*b.y + a.z*b.z);
}

inline t_vec	vec_norm(const t_vec vec)
{
	return vec_distr_mult((1 / vec_length(vec)), vec);
}