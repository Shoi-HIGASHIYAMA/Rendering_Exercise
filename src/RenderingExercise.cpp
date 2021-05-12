#include"rtweekend.h"
#include"camera.h"
#include"world.h"
#include"color.h"

using namespace std;

int main()
{
	//Properties
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = (int)(image_width / aspect_ratio);
	const int samples_per_pixel = 30;
	const int roulette_depth = 4;
	const int max_depth = 10;
	const double alpha = 0.1;

	//World Construction
	world* m_world = new world();

	m_world->add_light(make_shared<sphere_light>(point3(-1.5, 2, 0.25), 0.025, vec3(30.0, 30.0, 30.0), vec3(0, 0, 0)));
	m_world->add_light(make_shared<sphere_light>(point3(1.5, 2, 0.25), 0.25, vec3(30.0, 30.0, 30.0), vec3(0, 0, 0)));
	m_world->add_object(make_shared<sphere>(point3(0, 0, -100.5), 100, vec3(0.0, 0.0, 0.0), vec3(1, 1, 1), alpha));

	//Camera
	camera cam;

	//Render
	cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; j--) {
		cerr << "\rScanlines remaining: " << j << " " << flush;
		for (int i = 0; i < image_width; i++) {
			color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; s++) {
				double u = (i + random_double()) / (image_width - 1);
				double v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += m_world->ray_color(r, vec3(1.0, 1.0, 1.0), 0, roulette_depth, max_depth);
			}
			write_color(cout, pixel_color, samples_per_pixel);
		}
	}

	cerr << "\nDone.\n";

	delete m_world;

	return 0;
}