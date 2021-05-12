#pragma once

#include"hittable.h"
#include"microfacet.h"

class object : public hittable {
private:
	MicrofacetDistribution* microfacet;
public:
	object(const vec3& emit, const vec3& color, double alpha) :hittable(emit, color) {
		microfacet = new MicrofacetDistribution(alpha);
	}

	virtual ~object() { delete microfacet; }

	const MicrofacetDistribution* const get_microfacet()const { return microfacet; }
};

class sphere :public object {
private:
	point3 center;
	double radius;
public:
	sphere(const point3& center, double radius, const vec3& emit, const vec3& color, double alpha) :center(center), radius(radius), object(emit, color, alpha) {}
	~sphere() {}

	virtual vec3 calc_norm(const vec3& hit_pos)const override;
	virtual bool hit(const ray& r, double t_min, double t_max, double& t)const override;
};