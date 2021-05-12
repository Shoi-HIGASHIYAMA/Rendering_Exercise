#include"world.h"

#include<functional>

//Detect ray collision with objects or light sources.
bool world::hit(const ray& r, hit_record& rec)const {
	bool hit_anything = false;
	double t = infinity;

	for (int i = 0; i != objects.size(); i++) {
		if (objects[i]->hit(r, 0.001, t, t)) {
			hit_anything = true;
			rec = hit_record(i, r.at(t), objects[i]->calc_norm(r.at(t)), false);
		}
	}
	for (int i = 0; i != lights.size(); i++) {
		if (lights[i]->hit(r, 0.001, t, t))	{
			hit_anything = true;
			rec = hit_record(i, r.at(t), lights[i]->calc_norm(r.at(t)), true);
		}
	}

	return hit_anything;
}

//Sample a point on surface of a light source.
bool world::sample_light(vec3 point, light_record& rec)const {
	int N = (int)lights.size();

	if (N == 0)return false;

	//Determin a light source with equal probability.
	int idx = (int)(random_double()*N);

	lights[idx]->sample_surface(point, rec);
	rec.pdf /= N;

	return true;
}

//Calculate the direct lighting pdf of the point on the light source surface that collides with the ray r.
double world::calc_light_pdf(const ray& r)const {
	hit_record rec;
	if (!hit(r, rec) || !rec.isLight)return 0.0;
	double tp = lights[rec.id]->get_light_source_pdf(rec.point) / (int)lights.size();
	double lower = abs(dot(rec.normal, unit_vector(r.get_orig() - rec.point)));
	double upper = (r.get_orig() - rec.point).length()*(r.get_orig() - rec.point).length();
	tp = upper / lower * tp;
	return tp;
}

//Ray Tracing
vec3 world::ray_color(const ray& r, vec3 throughput, int depth, const int roulette_depth, const int max_depth)const {
	if (depth == max_depth)	{
		return vec3(0.0, 0.0, 0.0);
	}

	//Russian roulette
	if (roulette_depth <= depth) {
		double continueProbability = min(throughput.length(), 0.9);
		if (continueProbability < random_double()) {
			return vec3(0.0, 0.0, 0.0);
		}
		throughput /= continueProbability;
	}

	//Collision Detection
	hit_record rec;
	if (!hit(r, rec)) {
		if (depth == 0)	{
			return vec3(0.1, 0.1, 0.1);
		}
		else {
			return vec3(0.0, 0.0, 0.0);
		}
	}

	//if r collides with a light source, return its emission.
	if (rec.isLight) {
		return lights[rec.id]->get_emit();
	}

	//Direct Lighting
	auto calc_direct_lighting = [&]() {
		vec3 wo_world = -r.get_dir();
		vec3 wo_local = world_to_local(wo_world, rec.normal, rec.binormal, rec.tangent);

		//Light Source Sampling
		light_record lrec;
		if (!sample_light(rec.point, lrec))	{
			return vec3(0.0, 0.0, 0.0);
		}

		vec3 wi_world = unit_vector(lrec.point - rec.point);
		vec3 wi_local = world_to_local(wi_world, rec.normal, rec.binormal, rec.tangent);

		//Half Vector
		vec3 wm_world = unit_vector(wi_world + wo_world);
		vec3 wm_local = world_to_local(wm_world, rec.normal, rec.binormal, rec.tangent);

		ray next_ray = ray(rec.point, wi_world);

		double fr = objects[rec.id]->get_microfacet()->calc_fr(wo_local, wi_local, wm_local);
		double ps = objects[rec.id]->get_microfacet()->calc_pdf(wi_local, wm_local);
		double pd = calc_light_pdf(next_ray);

		vec3 fd = objects[rec.id]->get_color();
		fd *= fr * wo_local.z;

		if (fd.length() < 1e-9)	{
			return vec3(0.0, 0.0, 0.0);
		}

		vec3 next_throughput = throughput * fd;
		vec3 Ld = ray_color(next_ray, next_throughput, depth + 1, roulette_depth, max_depth);
		double alpha = pd*pd / (pd*pd + ps*ps);

		return Ld * fd / pd * alpha;
	};

	//BRDF
	auto calc_brdf_lighting = [&]()	{
		vec3 wo_world = -r.get_dir();
		vec3 wo_local = world_to_local(wo_world, rec.normal, rec.binormal, rec.tangent);

		//Half Vector Sampling
		vec3 wi_local;
		vec3 wm_local = objects[rec.id]->get_microfacet()->sample_wm(wo_local, wi_local);
		vec3 wi_world = local_to_world(wi_local, rec.normal, rec.binormal, rec.tangent);

		ray next_ray = ray(rec.point, wi_world);

		double fr = objects[rec.id]->get_microfacet()->calc_fr(wo_local, wi_local, wm_local);
		double ps = objects[rec.id]->get_microfacet()->calc_pdf(wi_local, wm_local);
		double pd = calc_light_pdf(next_ray);

		vec3 fs = objects[rec.id]->get_color();
		fs *= fr * wo_local.z;

		if (fs.length() < 1e-9)	{
			return vec3(0.0, 0.0, 0.0);
		}

		vec3 next_throughput = throughput * fs;
		vec3 Ls = ray_color(next_ray, next_throughput, depth + 1, roulette_depth, max_depth);
		double beta = ps*ps / (pd*pd + ps*ps);
		return Ls * fs / ps * beta;
	};

	vec3 L(0.0, 0.0, 0.0);
	L += calc_direct_lighting();
	L += calc_brdf_lighting();

	return L;
}