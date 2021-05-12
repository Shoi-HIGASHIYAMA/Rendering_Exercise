#include"light.h"

vec3 sphere_light::calc_norm(const vec3& hit_pos)const {
	return unit_vector(hit_pos - center);
}

bool sphere_light::hit(const ray& r, double t_min, double t_max, double& t)const {
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

bool sphere_light::sample_surface(const vec3& origin, light_record& rec)const {

	//Sample a point with equal probablity
	while (true) {
		double theta = random_double()*pi;
		double phi = random_double()*pi * 2.0;

		vec3 sp = vec3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));

		//If the sampled point can be seen from origin, adopt the point. 
		if (dot(sp, origin - center) > 0.0) {
			rec.pdf = 1.0 / (4.0 * pi*radius*radius / 2.0);
			rec.point = center + sp * radius;
			break;
		}
	}

	return true;
}

double sphere_light::get_light_source_pdf(const vec3& point)const {
	//PDF is the reciprocal of the area of the hemisphere.
	return 1.0 / (4.0 * pi*radius*radius / 2.0);
}