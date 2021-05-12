#pragma once

#include"rtweekend.h"
#include"vec3.h"

class MicrofacetDistribution {
private:
	double alpha;
public:
	MicrofacetDistribution(double alpha) :alpha(alpha) {}
	virtual ~MicrofacetDistribution() {}

	//Half Vector Sampling Based on GGX Distribution
	virtual vec3 sample_wm(const vec3& wo, vec3& wi) const {
		const double U1 = random_double();
		const double U2 = random_double();

		double theta = atan2(alpha*U1, 1.0 - U1);
		double phi = 2.0 * pi * U2;

		vec3 wm = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

		if (wm.z < 0.0) {
			wm = -wm;
		}

		//Calculation of Incident Vector
		wi = unit_vector(2.0 * dot(wm, wo)*wm - wo);

		return wm;
	}

	double calc_fr(const vec3& wo, const vec3& wi, const vec3& wm) const {
		return F(wi, wm)*G(wo, wi)*D(wm) / (4.0*wi.z*wo.z);
	}

	double calc_pdf(const vec3& wi, const vec3& wm)const {
		return D(wm)*wm.z / (4.0*dot(wi, wm));
	}

	// Fresnel term
	double F(const vec3& wi, const vec3& wm)const {
		const double F0 = 0.1;
		return F0 + (1 - F0)*pow(1.0 - abs(dot(wi, wm)), 5.0);
	}

	// Smith's masking-shadowing function
	double G(const vec3& wo, const vec3& wi) const {
		return G1(wo)*G1(wi);
	}

	// Smith's masking function
	double G1(const vec3& w) const {
		return 1.0 / (1.0 + lambda(w));
	}
	double lambda(const vec3& w)const {
		return (-1.0 + sqrt(1 + alpha * alpha*(1.0 / (w.z*w.z) - 1))) / 2.0;
	}

	// GGX Distribution
	double D(const vec3& wm)const {
		if (wm.z < 1e-9) {
			return 0.0;
		}

		double thetam = acos(wm.z / wm.length());
		double phim = atan2(wm.y, wm.x);
		double a = alpha * alpha*wm.z;
		double b = pi * pow(cos(thetam), 4.0);
		double c = alpha * alpha + pow(tan(thetam), 2.0);
		double res = a / (b*c*c);

		return res;
	}
};