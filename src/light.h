#pragma once

#include"hittable.h"

class light :public hittable {
public:
	light(const vec3& emit, const vec3& color) :hittable(emit, color) {}
	virtual ~light() {}
	virtual bool sample_surface(const vec3& point, light_record& rec)const = 0;
	virtual double get_light_source_pdf(const vec3& point)const = 0;
};

class sphere_light :public light {
private:
	point3 center;
	double radius;
public:
	sphere_light() :light(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0)) {}
	sphere_light(const point3& cen, double r, const vec3& emit, const vec3& color) :center(cen), radius(r), light(emit, color) {}

	virtual vec3 calc_norm(const vec3& hit_pos)const override;
	virtual bool hit(const ray& r, double t_min, double t_max, double& t)const override;
	virtual bool sample_surface(const vec3& point, light_record& rec)const override;
	virtual double get_light_source_pdf(const vec3& point)const;
};