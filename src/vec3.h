#pragma once

#include<cmath>
#include<iostream>
#include"rtweekend.h"

using namespace std;

class vec3 {
public:
	double x,y,z;
public:
	vec3() :x(0.0), y(0.0), z(0.0) {}
	vec3(double x, double y, double z) :x(x), y(y), z(z) {}

	vec3 operator-()const { return vec3(-x, -y, -z); }

	vec3& operator+=(const vec3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	vec3& operator*=(const double t){
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}

	vec3& operator/=(const double t) {
		x /= t;
		y /= t;
		z /= t;
		return *this;
	}

	double length()const {
		return sqrt(length_squared());
	}

	double length_squared()const {
		return x * x + y * y + z * z;
	}

	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
};

using point3 = vec3;
using color = vec3;

inline ostream& operator<<(ostream& out, const vec3& v) {
	return out << v.x << " " << v.y << " " << v.z;
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline vec3 operator*(double t, const vec3& v) {
	return vec3(t*v.x, t*v.y, t*v.z);
}

inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
	return (1.0 / t)*v;
}

inline double dot(const vec3& u, const vec3& v) {
	return
		u.x * v.x +
		u.y * v.y +
		u.z * v.z;
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return
		vec3(
			u.y * v.z - u.z * v.y,
			u.z * v.x - u.x * v.z,
			u.x * v.y - u.y * v.x
		);
}

inline vec3 unit_vector(const vec3& v) {
	return v / v.length();
}

//world coordinate -> local coordinate
inline vec3 world_to_local(const vec3& v, const vec3& normal, const vec3& binormal, const vec3& tangent) {
	return unit_vector(vec3(dot(v, binormal), dot(v, tangent), dot(v, normal)));
}

//local coordinate -> world coordinate
inline vec3 local_to_world(const vec3& v, const vec3& normal, const vec3& binormal, const vec3& tangent) {
	return unit_vector(vec3(
		binormal.x * v.x + tangent.x * v.y + normal.x * v.z,
		binormal.y * v.x + tangent.y * v.y + normal.y * v.z,
		binormal.z * v.x + tangent.z * v.y + normal.z * v.z));
}

vec3 random_in_unit_sphere();
vec3 random_unit_vector();
vec3 random_in_hemisphere(const vec3& normal);