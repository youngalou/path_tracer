/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   halton.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazuli <lazuli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/16 16:38:48 by lazuli            #+#    #+#             */
/*   Updated: 2018/02/19 15:21:24 by lazuli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../path_tracer.h"

t_hal	hal_number(int i, int base)
{
	t_hal	hal;

	double f = hal.inv_base = 1.0 / base;
	hal.value = 0.0;
	while (i > 0)
	{
		hal.value += f*(double)(i%base);
		i /= base;
		f *= hal.inv_base;
	}
	return hal;
}

t_hal	hal_next(t_hal hal)
{
	double	r = 1.0 - hal.value - 0.0000001;
	if (hal.inv_base < r)
		hal.value += hal.inv_base;
	else
	{
		double	h = hal.inv_base, hh;
		while (h >= r)
		{
			hh = h;
			h *= hal.inv_base;
		}
		hal.value += hh + h - 1.0;
	}
	return hal;
}