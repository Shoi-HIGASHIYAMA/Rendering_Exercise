#pragma once

#include"object.h"
#include"light.h"
#include"color.h"

#include<memory>
#include<vector>

using namespace std;

class world{
private:
	vector<shared_ptr<object>> objects;
	vector<shared_ptr<light>> lights;
private:
	bool hit(const ray& r, hit_record& rec)const;
	bool sample_light(vec3 point, light_record& rec)const;
	double calc_light_pdf(const ray& r)const;
public:
	world(){}

	void clear() { objects.clear(), lights.clear(); }
	void add_object(shared_ptr<object> object) { objects.push_back(object); }
	void add_light(shared_ptr<light> object) { lights.push_back(object); }

	vec3 ray_color(const ray& r, vec3 throughput, int depth, const int roulette_depth, const int max_depth)const;
};