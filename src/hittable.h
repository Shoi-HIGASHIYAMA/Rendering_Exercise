#pragma once

#include"ray.h"

struct hit_record {
	int id;
	vec3 point, normal, binormal, tangent;
	bool isLight;
	hit_record(int id = -1, vec3 point = vec3(0.0, 0.0, 0.0), vec3 normal = vec3(0.0, 0.0, 0.0), bool isLight = false) : id(id), point(point), normal(normal), isLight(isLight)
	{
		// Generate local coordinate axes.
		if (abs(normal.y) < abs(normal.x)) {
			tangent = unit_vector(vec3(normal.z, 0.0, -normal.x));
		}
		else {
			tangent = unit_vector(vec3(0.0, normal.z, -normal.y) / normal.length());
		}
		binormal = unit_vector(cross(tangent, normal));
	}
};

struct light_record {
	vec3 point;
	double pdf;
	light_record(vec3 point, double pdf) :point(point), pdf(pdf) {}
	light_record() :light_record(vec3(0.0, 0.0, 0.0), 0.0) {}
};

class hittable {
private:
	vec3 emit;
	vec3 color;
public:
	hittable(vec3 emit, vec3 color) :emit(emit), color(color) {}
	virtual ~hittable() {}
	virtual vec3 calc_norm(const vec3& hit_pos)const  = 0;
	virtual bool hit(const ray& r, double t_min, double t_max, double& t)const = 0;
	vec3 get_emit()const { return emit; }
	vec3 get_color()const { return color; }
};