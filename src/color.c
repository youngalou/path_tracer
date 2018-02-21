/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazuli <lazuli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 11:48:29 by lazuli            #+#    #+#             */
/*   Updated: 2018/02/21 14:40:46 by lazuli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../path_tracer.h"

inline t_clr	new_clr(const double r, const double g, const double b)
{
	t_clr	new;
	
	new.r = r;
	new.g = g;
	new.b = b;
	return new;
}

inline t_clr	clr_mult(const t_clr a, const t_clr b)
{
	t_clr	res;

	res.r = a.r * b.r;
	res.g = a.g * b.g;
	res.b = a.b * b.b;
	return res;
}

inline t_clr	clr_add(const t_clr a, const t_clr b)
{
	t_clr	res;

	res.r = a.r + b.r;
	res.g = a.g + b.g;
	res.b = a.b + b.b;
	return res;
}

inline t_clr	clr_distr_div(const double b, const t_clr a)
{
	t_clr	res;

	res.r = a.r / b;
	res.g = a.g / b;
	res.b = a.b / b;
	return res;
}

inline int		clr_to_int(const t_clr clr)
{
	int	r, g, b;
	
	r = (clr.r < 1) ? clr.r * 255 : 255;
	g = (clr.g < 1) ? clr.g * 255 : 255;
	b = (clr.b < 1) ? clr.b * 255 : 255;
	return (r << 16) + (g << 8) + b;
}