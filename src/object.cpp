#include"object.h"

vec3 sphere::calc_norm(const vec3& hit_pos)const {
	return unit_vector(hit_pos - center);
}

bool sphere::hit(const ray& r, double t_min, double t_max, double& t)const {

	vec3 oc = r.origin() - center;
	double a = r.direction().length_squared();
	double half_b = dot(oc, r.direction());
	double c = oc.length_squared() - radius * radius;
	double discriminant = half_b * half_b - a * c;

	if (discriminant < 0)return false;

	double sqrtd = sqrt(discriminant);

	double root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root) {
			return false;
		}
	}

	t = root;

	return true;
}