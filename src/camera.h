#pragma once

#include"vec3.h"
#include"ray.h"

class camera {
private:
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
public:
	camera() {
		double aspect_ratio = 16.0 / 9.0;
		double viewport_height = 2.0;
		double viewport_width = aspect_ratio * viewport_height;
		double focal_length = 1.0;

		origin = point3(0, 0, 0);
		horizontal = vec3(viewport_width, 0.0, 0.0);
		vertical = vec3(0.0, 0.0, viewport_height);
		lower_left_corner = origin - horizontal / 2 - vertical / 2 + vec3(0.0, focal_length, 0.0);
	}

	ray get_ray(double u, double v)const {
		return ray(origin, unit_vector(lower_left_corner + u * horizontal + v * vertical - origin));
	}
};