#pragma once

#include"vec3.h"

class ray {
private:
	point3 orig;
	vec3 dir;
public:
	ray() {}
	ray(const point3& orig, const vec3& dir) :orig(orig), dir(dir) {}

	point3 origin()const { return orig; }
	vec3 direction()const { return dir; }

	point3 at(double t) const {
		return orig + t * dir;
	}
	point3 get_orig()const {
		return orig;
	}
	vec3 get_dir()const {
		return dir;
	}
};