/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_tracer.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 13:12:28 by lyoung            #+#    #+#             */
/*   Updated: 2018/02/12 13:12:30 by lyoung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include "minilibx/mlx.h"

#define WIN_W 500
#define WIN_H 500
#define SPP 10

#define PI 3.1415926536
#define RND (2.0*(double)rand()/RAND_MAX-1.0)
#define RND2 ((double)rand()/RAND_MAX)

typedef struct 	s_env
{
	void		*mlx;
	void		*win;
	void		*img;
	int			*pixels;
	int			bpp;
	int			sl;
	int			endian;
}				t_env;