/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_tracer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 11:50:27 by lyoung            #+#    #+#             */
/*   Updated: 2018/02/12 12:09:33 by lyoung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

////////////////////////////////////////////////////////////////////////////////

#include "path_tracer.h"

////////////////////////////////////////////////////////////////////////////////

struct Vec {
	double x, y, z;
	Vec(double x0 = 0, double y0 = 0, double z0 = 0) { x = x0; y = y0; z = z0; }
	Vec operator+(const Vec &b) const { return Vec(x + b.x, y + b.y, z + b.z); }
	Vec operator-(const Vec &b) const { return Vec(x - b.x, y - b.y, z - b.z); }
	Vec operator*(double b) const { return Vec(x*b, y*b, z*b); }
	Vec operator/(double b) const { return Vec(x / b, y / b, z / b); }
	Vec mult(const Vec &b) const { return Vec(x*b.x, y*b.y, z*b.z); }
	Vec& norm() { return *this = *this * (1 / sqrt(x*x + y*y + z*z)); } // WHY MULT BY RECIPRICOL AS OPPOSED TO DIVIDE?
	double length() { return sqrt(x*x + y*y + z*z); }
	double dot(const Vec &b) const { return x*b.x + y*b.y + z*b.z; }
	Vec operator%(Vec &b) { return Vec(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x); }
};

struct Ray {
	Vec o, d;
	Ray(Vec o0 = 0, Vec d0 = 0) { o = o0, d = d0.norm(); }
};

////////////////////////////////////////////////////////////////////////////////

class Obj {
public:
	Vec cl;
	double emission;
	int type;
	void setMat(Vec cl_ = 0, double emission_ = 0, int type_ = 0) { cl = cl_; emission = emission_; type = type_; }
	virtual double intersect(const Ray&) const = 0;
	virtual Vec normal(const Vec&) const = 0;
};

class Plane : public Obj {
public:
	Vec n;
	double d;
	Plane(double d_ = 0, Vec n_ = 0) {
		d = d_;
		n = n_;
	}
	double intersect(const Ray& ray) const {
		double d0 = n.dot(ray.d);
		if (d0 != 0) {
			double t = -1 * (((n.dot(ray.o)) + d) / d0);
			return (t > eps) ? t : 0;
		} else return 0;
	}
	Vec normal(const Vec& p0) const { return n; }
};

class Sphere : public Obj {
public:
	Vec c;
	double r;

	Sphere(double r_ = 0, Vec c_ = 0) { c = c_; r = r_; }
	double intersect(const Ray& ray) const {
		double b = ((ray.o - c) * 2).dot(ray.d);
		double c_ = (ray.o - c).dot((ray.o - c)) - (r*r);
		double disc = b*b - 4 * c_;
		if (disc < 0) return 0;
		else disc = sqrt(disc);
		double sol1 = -b + disc;
		double sol2 = -b - disc;
		return (sol2 > eps) ? sol2 / 2 : ((sol1 > eps) ? sol1 / 2 : 0);
	}

	Vec normal(const Vec& p0) const {
		return (p0 - c).norm();
	}
};

////////////////////////////////////////////////////////////////////////////////

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
}

Vec camcr(const double x, const double y) {
	double w = width;
	double h = height;
	float fovx = PI / 4;
	float fovy = (h / w)*fovx;

	return Vec(((2 * x - w) / w)*tan(fovx),
		((2 * y - h) / h)*tan(fovy),
			   -1.0);
}

int		main(void)
{
	t_env	*env = init_env();
	for (int s = 0; s < SPP; s++)
	{
		for (int y = 0; y < WIN_H; y++)
		{
			for (int x = 0; x < WIN_W; x++)
			{
				Vec c;
				Ray ray;
				ray.o = (Vec(0, 0, 0));
				Vec cam = camcr(i, j);
				cam.x = cam.x + RND / 700;
				cam.y = cam.y + RND / 700;
				ray.d = (cam - ray.o).norm();
			}
		}
	}
}
