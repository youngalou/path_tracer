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
////////////////////////////////////////////////////////////////////////////////

#include "path_tracer.h"

namespace path_tracer {

const double inf = 1e9;
const double eps = 1e-4;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
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
	Plane operator=(const Plane *obj)
	{
		this->d = obj->d;
		this->n = obj->n;
		return (*this);
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
	Sphere operator=(const Sphere *obj)
	{
		this->c = obj->c;
		this->r = obj->r;
		return (*this);
	}
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

class Intersection {
public:
	Intersection() { t = inf; object = nullptr; }
	Intersection(double t_, Obj* object_) { t = t_; object = object_; }
	operator bool() { return object != nullptr; }
	double t;
	Obj* object;
};

class Halton {
	double value, inv_base;
public:
	void number(int i, int base) {
		double f = inv_base = 1.0 / base;
		value = 0.0;
		while (i > 0) {
			value += f*(double)(i%base);
			i /= base;
			f *= inv_base;
		}
	}
	void next() {
		double r = 1.0 - value - 0.0000001;
		if (inv_base < r) value += inv_base;
		else {
			double h = inv_base, hh;
			do { hh = h; h *= inv_base; } while (h >= r);
			value += hh + h - 1.0;
		}
	}
	double get() { return value; }
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

t_env	*init_env(void)
{
	t_env	*env = new t_env;
	env->mlx = mlx_init();
	env->win = mlx_new_window(env->mlx, WIN_W, WIN_H, "Path Tracer");
	env->img = mlx_new_image(env->mlx, WIN_W, WIN_H);
	env->pixels = (int*)mlx_get_data_addr(env->img, &env->bpp, &env->sl, &env->endian);
	env->bpp = 4;
	env->sl = 0;
	env->endian = 0;
	return (env);
}


//get world vector from pixel position
Vec camcr(const double x, const double y) {
	double w = WIN_W;
	double h = WIN_H;
	float fovx = PI / 4;
	float fovy = (h / w)*fovx;

	return Vec(((2 * x - w) / w)*tan(fovx),
		((2 * y - h) / h)*tan(fovy),
			   -1.0);
}

//uniform sampling of a hemisphere when hitting diffuse object
Vec hemisphere(double u1, double u2) {
	const double r = sqrt(1.0 - u1*u1);
	const double phi = 2 * PI*u2;
	return Vec(cos(phi)*r, sin(phi)*r, u1);
}

////////////////////////////////////////////////////////////////////////////////

Intersection 	find_intersect(const Ray& ray, Plane *plane, Sphere *sphere)
{
	Intersection closestIntersection;
	double	t;
	for (int i = 0; i < 6; i++)
	{
		t = plane[i].intersect(ray);
		if (t > eps && t < closestIntersection.t)
		{
			closestIntersection.t = t;
			closestIntersection.object = &plane[i];
		}
	}
	for (int i = 0; i < 4; i++)
	{
		t = sphere[i].intersect(ray);
		if (t > eps && t < closestIntersection.t)
		{
			closestIntersection.t = t;
			closestIntersection.object = &sphere[i];
		}
	}
	return (closestIntersection);
}

void trace(Ray &ray, Plane *plane, Sphere *sphere, int depth, Vec& clr, Halton& hal, Halton& hal2)
{
	if (depth >= DEPTH)
		return;

	Intersection intersection = find_intersect(ray, plane, sphere);
	if (!intersection)
		return;

	// Travel the ray to the hit point where the closest object lies and compute the surface normal there.
	Vec hp = ray.o + ray.d * intersection.t;
	Vec N = intersection.object->normal(hp);
	ray.o = hp;

	clr = clr + Vec(intersection.object->emission, intersection.object->emission, intersection.object->emission) * 2;

	if (intersection.object->type == 1) {
		hal.next();
		hal2.next();
		ray.d = (N + hemisphere(hal.get(), hal2.get()));
		double cost = ray.d.dot(N);
		Vec tmp = Vec();
		trace(ray, plane, sphere, depth + 1, tmp, hal, hal2);
		clr.x += cost*(tmp.x*intersection.object->cl.x)*0.1;
		clr.y += cost*(tmp.y*intersection.object->cl.y)*0.1;
		clr.z += cost*(tmp.z*intersection.object->cl.z)*0.1;
	}
	if (intersection.object->type == 2) {
		double cost = ray.d.dot(N);
		ray.d = (ray.d - N*(cost * 2)).norm();
		Vec tmp = Vec(0, 0, 0);
		trace(ray, plane, sphere, depth + 1, tmp, hal, hal2);
		clr = clr + tmp;
	}
}

int		main(void)
{
	t_env	*env = init_env();
	srand(time(NULL));
	Plane 	plane[6];
	Sphere 	sphere[4];

	//type (1=diff, 2=spec, 3=refr)
	plane[0] = new Plane(2.5, Vec(-1, 0, 0)); // Bottom plane
	plane[0].setMat(Vec(6, 6, 6), 0, 1);
	plane[1] = new Plane(5.5, Vec(0, 0, 1)); // Back plane
	plane[1].setMat(Vec(6, 6, 6), 0, 1);
	plane[2] = new Plane(2.75, Vec(0, 1, 0)); // Left plane
	plane[2].setMat(Vec(10, 2, 2), 0, 1);
	plane[3] = new Plane(2.75, Vec(0, -1, 0)); // Right plane
	plane[3].setMat(Vec(2, 10, 2), 0, 1);
	plane[4] = new Plane(3.0, Vec(1, 0, 0)); // Ceiling plane
	plane[4].setMat(Vec(6, 6, 6), 0, 1);
	plane[5] = new Plane(0.5, Vec(0, 0, -1)); // Front plane
	plane[6].setMat(Vec(6, 6, 6), 0, 1);
	sphere[0] = new Sphere(1.05, Vec(1.45, -0.75, -4.4)); // Middle sphere
	sphere[0].setMat(Vec(4, 8, 4), 0, 2);
	sphere[1] = new Sphere(0.5, Vec(2.05, 2.0, -3.7)); // Right sphere
	sphere[1].setMat(Vec(10, 10, 1), 0, 2);
	sphere[2] = new Sphere(0.6, Vec(1.95, -1.75, -3.1)); // Left sphere
	sphere[2].setMat(Vec(4, 4, 12), 0, 1);
	sphere[3] = new Sphere(0.5, Vec(-1.9, 0, -3)); // Light
	sphere[3].setMat(Vec(0, 0, 0), 120, 1);

	Vec **pix = new Vec*[WIN_W];
	for (int i = 0; i < WIN_W; i++) {
		pix[i] = new Vec[WIN_H];
	}

	// correlated Halton-sequence dimensions
	Halton hal, hal2;
	hal.number(0, 2);
	hal2.number(0, 2);

	for (int s = 0; s < SPP; s++)
	{
		for (int y = 0; y < WIN_H; y++)
		{
			for (int x = 0; x < WIN_W; x++)
			{
				Vec clr;
				Ray ray;
				ray.o = (Vec(0, 0, 0));
				Vec cam = camcr(y, x);
				cam.x = cam.x + RND / 700;
				cam.y = cam.y + RND / 700;
				ray.d = (cam - ray.o).norm();
				path_tracer::trace(ray, plane, sphere, 0, clr, hal, hal2);
				pix[y][x].x += clr.x;
				pix[y][x].y += clr.y;
				pix[y][x].z += clr.z;
				env->pixels[x + (y * WIN_W)] = 0xffffff;
			}
		}
	}
	mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
	while (1)
	{}
	return (0);
}
}
